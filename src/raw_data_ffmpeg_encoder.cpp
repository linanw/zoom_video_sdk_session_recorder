
// #include <opencv2/imgproc.hpp>
// #include <opencv2/imgcodecs.hpp>

#include "raw_data_ffmpeg_encoder.h"

using namespace ZOOMVIDEOSDK;

const AVPixelFormat pix_fmts[] = {AV_PIX_FMT_YUV420P, AV_PIX_FMT_NONE};
std::vector<RawDataFFMPEGEncoder *> RawDataFFMPEGEncoder::list_;
int RawDataFFMPEGEncoder::instance_count = 0;

RawDataFFMPEGEncoder::RawDataFFMPEGEncoder(IZoomVideoSDKUser *user, FileVideoSource* rawVideoOutput)
{
	instance_id_ = instance_count++;
	user_ = user;
	user_->GetVideoPipe()->subscribe(ZoomVideoSDKResolution_720P, this);
	list_.push_back(this);

	rawVideoOutput_ = rawVideoOutput;
}

RawDataFFMPEGEncoder::~RawDataFFMPEGEncoder()
{
	// finish ffmpeg encoding
	log(L"********** [%d] Finishing encoding, user: %s, %dx%d.\n", instance_id_, user_->getUserName(), in_width, in_height);
	if (is_ffmpeg_encoding_on)
	{
		ffmpeg_stop();
		is_ffmpeg_encoding_on = 0;
	}
	user_->GetVideoPipe()->unSubscribe(this);
	log(L"********** [%d] UnSubscribe, user: %s.\n", instance_id_, user_->getUserName());
	instance_count--;
	user_ = nullptr;
}

RawDataFFMPEGEncoder *RawDataFFMPEGEncoder::find_instance(IZoomVideoSDKUser *user)
{
	for (auto iter = list_.begin(); iter != list_.end(); iter++)
	{
		RawDataFFMPEGEncoder *item = *iter;
		if (item->user_ == user)
		{
			return item;
		}
	}
	return nullptr;
}

void RawDataFFMPEGEncoder::stop_encoding_for(IZoomVideoSDKUser *user)
{
	RawDataFFMPEGEncoder *encoder = RawDataFFMPEGEncoder::find_instance(user);
	if (encoder)
	{
		encoder->~RawDataFFMPEGEncoder();
	}
}
int j = 0;

void RawDataFFMPEGEncoder::onRawDataFrameReceived(YUVRawDataI420 *data)
{
	const zchar_t *userName = user_->getUserName();
	const zchar_t *userID = user_->getUserID();

	const int width = data->GetStreamWidth();
	const int height = data->GetStreamHeight();
	const int bufLen = data->GetBufferLen();
	const int rotation = data->GetRotation();
	const int sourceID = data->GetSourceID();

	// output a copy
	if(rawVideoOutput_){
		rawVideoOutput_->SendRawVideo(data->GetBuffer(), width, height, bufLen, rotation);
	}

	if ((sourceID != current_sourceID) && (sourceID == 0 || strlen(userID) > 0) // to skip frames when sourceID comes in but userID is not ready, otherwise create another sepreate file for this moment.
	)
	{
		log(L"********** [%d] Start encoding, user: %s, %dx%d, sourceID: %d.\n", instance_id_, user_->getUserName(), width, height, sourceID);
		if (is_ffmpeg_encoding_on)
		{
			ffmpeg_stop();
		};
		current_sourceID = sourceID;
		in_width = width;
		in_height = height;
		ffmpeg_start(userName, userID, sourceID);
		is_ffmpeg_encoding_on = 1;
	}
	else
	{
		if (is_ffmpeg_encoding_on == 1 && (width != in_width || height != in_height))
		{
			is_ffmpeg_encoding_on = 0;
			log(L"********** [%d] Update scale, user: %s, %dx%d -> %dx%d sourceID: %d.\n", instance_id_, user_->getUserName(), in_width, in_height, width, height, sourceID);
			in_width = width;
			in_height = height;
			// the video source reslution changed, update the ffmpeg filter for scale.
			ffmpeg_filter_init();

			is_ffmpeg_encoding_on = 1;
		}
	}
	if (is_ffmpeg_encoding_on)
	{
		ffmpeg_filter(
			reinterpret_cast<unsigned char *>(data->GetYBuffer()),
			reinterpret_cast<unsigned char *>(data->GetUBuffer()),
			reinterpret_cast<unsigned char *>(data->GetVBuffer()));
		ffmpeg_encode();
	}
}

void RawDataFFMPEGEncoder::onRawDataStatusChanged(RawDataStatus status)
{
	log(L"********** [%d] onRawDataStatusChanged, user: %s, %d.\n", instance_id_, user_->getUserName(), status);
	if (status == RawData_On)
	{
	}
	else
	{
	}
}

void RawDataFFMPEGEncoder::err_msg(int code)
{
	char errbuf[100];
	av_strerror(code, errbuf, 100);
	printf("%s\n", errbuf);
}

void RawDataFFMPEGEncoder::log(const wchar_t *format, ...)
{
	va_list args;
	va_start(args, format);
	wprintf(format, args);
	// wchar_t msgbuf[500];
	// vswprintf(msgbuf, format,
	// 	args);
	va_end(args);
	// OutputDebugStringW(msgbuf);
}

int RawDataFFMPEGEncoder::ffmpeg_start(const char *userName, const char *userID, int sourceID)
{
	int ret = 0;

	// timestamp
	// _ftime(&start_tstruct);
	start_time = steady_clock::now();

	// init files
	if (strlen(userID) == 0)
		userID = "0";
	char fileName[100];
	sprintf(fileName, "%s_%d_%s_%dx%d_to_%dx%d", userID, sourceID, userName, in_width, in_height, out_width, out_height);
	char yuvFileName[120];
	sprintf(yuvFileName, "../recording/%80s.yuv", fileName);
	if (isOutputYUV)
	{
		fp_yuv = fopen(yuvFileName, "wb + ");
		if (fp_yuv == NULL)
		{
			printf("Error open output file.\n");
			return -1;
		}
	}

	char outFileName[110];
	sprintf(outFileName, "%s.mkv", fileName);
	sprintf(fn_out, "../recording/%s", outFileName);

	// ffmpeg init
	// init filters
	avfilter_register_all();

	ffmpeg_filter_init();

	// init encoder
	av_register_all();
	pFormatCtx = avformat_alloc_context();

	// Method1: Guess Format
	fmt = av_guess_format(NULL, fn_out, NULL);
	pFormatCtx->oformat = fmt;
	// Method 2.
	// avformat_alloc_output_context2(&pFormatCtx, NULL, NULL, out_file);
	// fmt = pFormatCtx->oformat;

	// Open output file
	if (avio_open(&pFormatCtx->pb, fn_out, AVIO_FLAG_READ_WRITE) < 0)
	{
		printf("Failed to open output file! \n");
		return -1;
	}

	// init streams & codec
	video_st = avformat_new_stream(pFormatCtx, 0);
	// video_st->time_base.num = 1;
	// video_st->time_base.den = 25;
	if (video_st == NULL)
	{
		return -1;
	}
	// Param that must set
	pCodecCtx = video_st->codec;
	// pCodecCtx->codec_id =AV_CODEC_ID_HEVC;
	pCodecCtx->codec_id = fmt->video_codec;
	pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
	pCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
	pCodecCtx->bit_rate = 400000;
	pCodecCtx->gop_size = 250;
	pCodecCtx->time_base.num = 1;
	pCodecCtx->time_base.den = 25;
	// H264
	// pCodecCtx->me_range = 16;
	// pCodecCtx->max_qdiff = 4;
	// pCodecCtx->qcompress = 0.6;
	pCodecCtx->qmin = 10;
	pCodecCtx->qmax = 51;
	// Optional Param
	pCodecCtx->max_b_frames = 3;

	// Show some Information
	av_dump_format(pFormatCtx, 0, fn_out, 1);

	// prepare encoder
	pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
	if (!pCodec)
	{
		printf("Can not find encoder! \n");
		return -1;
	}

	pCodecCtx->width = out_width;
	pCodecCtx->height = out_height;

	AVDictionary *param = 0;
	// H.264
	if (pCodecCtx->codec_id == AV_CODEC_ID_H264)
	{
		av_dict_set(&param, "preset", "slow", 0);
		av_dict_set(&param, "tune", "zerolatency", 0);
		// av_dict_set(&param, "profile", "main", 0);
	}
	if (avcodec_open2(pCodecCtx, pCodec, &param) < 0)
	{
		printf("Failed to open encoder! \n");
		return -1;
	}

	// Write File Header
	if ((ret = avformat_write_header(pFormatCtx, NULL)) < 0)
	{
		printf("Failed to write header! \n");
		return ret;
	}
	return ret;
}

int RawDataFFMPEGEncoder::ffmpeg_filter_init()
{
	int ret;

	buffersrc = avfilter_get_by_name("buffer");
	buffersink = avfilter_get_by_name("buffersink");
	outputs = avfilter_inout_alloc();
	inputs = avfilter_inout_alloc();
	buffersink_params = av_buffersink_params_alloc();

	filter_graph = avfilter_graph_alloc();
	///* buffer video source: the decoded frames from the decoder will be inserted here. */
	char args[512];
	snprintf(args, sizeof(args),
			 "video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",
			 in_width, in_height, AV_PIX_FMT_YUV420P,
			 1, 25, 1, 1);

	if (avfilter_graph_create_filter(&buffersrc_ctx, buffersrc, "in", args, NULL, filter_graph) < 0)
	{
		printf("Cannot create buffer source\n");
		return -1;
	}

	/* buffer video sink: to terminate the filter chain. */
	buffersink_params->pixel_fmts = pix_fmts;
	if ((ret = avfilter_graph_create_filter(&buffersink_ctx, buffersink, "out", NULL, buffersink_params, filter_graph)) < 0)
	{
		printf("Cannot create buffer sink, code: %d, ", ret);
		err_msg(ret);
		return ret;
	}

	/* Endpoints for the filter graph. */
	outputs = avfilter_inout_alloc();
	inputs = avfilter_inout_alloc();
	outputs->name = av_strdup("in");
	outputs->filter_ctx = buffersrc_ctx;
	outputs->pad_idx = 0;
	outputs->next = NULL;

	inputs->name = av_strdup("out");
	inputs->filter_ctx = buffersink_ctx;
	inputs->pad_idx = 0;
	inputs->next = NULL;

	char filter_descr[50];
	sprintf(filter_descr, "scale=w=%d:h=%d", out_width, out_height);

	if (avfilter_graph_parse_ptr(filter_graph, filter_descr,
								 &inputs, &outputs, NULL) < 0)
		return -2;

	if (avfilter_graph_config(filter_graph, NULL) < 0)
		return -3;

	frame_in = av_frame_alloc();
	frame_buffer_in = (unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P, in_width,
																		  in_height, 1));
	av_image_fill_arrays(frame_in->data, frame_in->linesize, frame_buffer_in,
						 AV_PIX_FMT_YUV420P, in_width, in_height, 1);

	frame_out = av_frame_alloc();
	frame_buffer_out = (unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P, out_width, out_height, 1));
	av_image_fill_arrays(frame_out->data, frame_out->linesize, frame_buffer_out,
						 AV_PIX_FMT_YUV420P, out_width, out_height, 1);

	frame_in->format = AV_PIX_FMT_YUV420P;
	frame_in->width = in_width;
	frame_in->height = in_height;

	return ret;
}

static bool doOnce = true;

int RawDataFFMPEGEncoder::ffmpeg_filter(uint8_t *Y, uint8_t *U, uint8_t *V)
{
	// input Y,U,V
	frame_in->data[0] = Y;
	frame_in->data[1] = U;
	frame_in->data[2] = V;

	// output Y,U,V
	if (isOutputYUV && frame_in->format == AV_PIX_FMT_YUV420P && doOnce)
	{
		for (int i = 0; i < frame_in->height; i++)
		{
			fwrite(frame_in->data[0] + frame_in->linesize[0] * i, 1, frame_in->width, fp_yuv);
		}
		for (int i = 0; i < frame_in->height / 2; i++)
		{
			fwrite(frame_in->data[1] + frame_in->linesize[1] * i, 1, frame_in->width / 2, fp_yuv);
		}
		for (int i = 0; i < frame_in->height / 2; i++)
		{
			fwrite(frame_in->data[2] + frame_in->linesize[2] * i, 1, frame_in->width / 2, fp_yuv);
		}
		doOnce = false;
	}

	// apply filter
	if (av_buffersrc_add_frame(buffersrc_ctx, frame_in) < 0)
	{
		printf("Error while add frame.\n");
		return -1;
	}
	// wchar_t msgbuf[200];
	// swprintf(msgbuf, L"===================filter===========  w=%d, h=%d, userName=%s, userId=%s, sourceId=%d \n",
	//	in_width, in_height, user_->getUserName(), user_->getUserID(), current_sourceID);
	// OutputDebugStringW(msgbuf);

	/* pull filtered pictures from the filtergraph */
	if ((av_buffersink_get_frame(buffersink_ctx, frame_out)) < 0)
		return -1;

	return 0;
}

int RawDataFFMPEGEncoder::ffmpeg_encode()
{
	// timestamp
	steady_clock::time_point current_time = steady_clock::now();
	frame_out->pts = duration_cast<std::chrono::milliseconds>(current_time - start_time).count() 
		* (video_st->time_base.den) / (video_st->time_base.num * 1000);

	// prepare packet
	av_init_packet(&pkt);

	int ret;
	int got_picture = 0;
	if ((ret = avcodec_encode_video2(pCodecCtx, &pkt, frame_out, &got_picture)) < 0)
	{
		printf("Failed to encode, code: %d, ", ret);
		err_msg(ret);
		return -1;
	}
	if (got_picture == 1)
	{
		// printf("Succeed to encode frame: %5d\tsize:%5d\n", framecnt, pkt.size);
		framecnt++;
		pkt.stream_index = video_st->index;
		av_write_frame(pFormatCtx, &pkt);
		av_packet_unref(&pkt);
	}

	av_frame_unref(frame_out);
	printf(".");

	return 0;
}

int RawDataFFMPEGEncoder::ffmpeg_stop()
{

	// Flush Encoder
	if ((RawDataFFMPEGEncoder::ffmpeg_flush(pFormatCtx, 0)) < 0)
	{
		printf("Flushing encoder failed\n");
		return -1;
	}

	// Write file trailer
	av_write_trailer(pFormatCtx);

	// Clean
	if (video_st)
	{
		avcodec_close(video_st->codec);
		av_free(frame_out);
	}
	avio_close(pFormatCtx->pb);
	avformat_free_context(pFormatCtx);

	if (isOutputYUV)
	{
		fclose(fp_yuv);
	}

	av_free(buffersink_params);

	av_frame_free(&frame_in);
	////av_frame_free(&frame_out);
	avfilter_graph_free(&filter_graph);
	return 0;
}

int RawDataFFMPEGEncoder::ffmpeg_flush(AVFormatContext *fmt_ctx, unsigned int stream_index)
{
	int ret;
	int got_frame;
	AVPacket enc_pkt;
	if (!(fmt_ctx->streams[stream_index]->codec->codec->capabilities &
		  CODEC_CAP_DELAY))
		return 0;
	while (1)
	{
		enc_pkt.data = NULL;
		enc_pkt.size = 0;
		av_init_packet(&enc_pkt);
		ret = avcodec_encode_video2(fmt_ctx->streams[stream_index]->codec, &enc_pkt,
									NULL, &got_frame);
		av_frame_free(NULL);
		if (ret < 0)
			break;
		if (!got_frame)
		{
			ret = 0;
			break;
		}
		printf("Flush Encoder: Succeed to encode 1 frame!\tsize:%5d\n", enc_pkt.size);
		/* mux encoded frame */
		ret = av_write_frame(fmt_ctx, &enc_pkt);
		if (ret < 0)
			break;
	}
	return ret;
}
