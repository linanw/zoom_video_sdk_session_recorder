#include "hikvision_video_source.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/mem.h>
}

#include "HCNetSDK.h"
#include <thread>
#include <chrono>

#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

using namespace cv;
using namespace std;

using namespace ZOOMVIDEOSDK;

#define BUFFER_SIZE (4096 * 10240) // Define a buffer size for incoming PS data

uint8_t input_buffer[BUFFER_SIZE]; // Circular buffer for storing PS data
bool hadHeader_ = false;		   // Flag to check if header has been received
bool bufFull = false;

size_t buffer_data_size = 0;	// How much data is currently in the buffer
size_t buffer_read_offset = 0;	// Current read position in the buffer
size_t buffer_write_offset = 0; // Current write position in the buffer

// Custom read function for AVIOContext
int read_packet(void *opaque, uint8_t *buf, int buf_size)
{
	if (buffer_data_size == 0)
	{
		return AVERROR_EOF; // No data in buffer, return EOF
	}

	int read_size = FFMIN(buf_size, buffer_data_size); // Calculate how much data to read
	int remaining_buffer = BUFFER_SIZE - buffer_read_offset;

	// If remaining buffer is enough, read directly
	if (remaining_buffer >= read_size)
	{
		memcpy(buf, input_buffer + buffer_read_offset, read_size);
		buffer_read_offset = (buffer_read_offset + read_size) % BUFFER_SIZE;
	}
	else
	{
		// If the buffer wraps around, split the read
		memcpy(buf, input_buffer + buffer_read_offset, remaining_buffer);
		memcpy(buf + remaining_buffer, input_buffer, read_size - remaining_buffer);
		buffer_read_offset = read_size - remaining_buffer;
	}

	buffer_data_size -= read_size; // Decrease the buffer size by the read amount
	return read_size;
}

// Real-time callback to fill the buffer (from Hikvision's SDK)
void MyRealDataCallback(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, void *pUser)
{
	// This will be called continuously with PS data chunks
	if (!bufFull)
	{
		if (dwDataType == NET_DVR_SYSHEAD)
		{
			hadHeader_ = true;
			printf("h");
		}
		if (hadHeader_)
		{
			size_t free_space = BUFFER_SIZE - buffer_data_size;
			if (free_space >= dwBufSize)
			{
				int remaining_buffer = BUFFER_SIZE - buffer_write_offset;
				// If there is enough space in one go, copy directly
				if (remaining_buffer >= dwBufSize)
				{
					memcpy(input_buffer + buffer_write_offset, pBuffer, dwBufSize);
					buffer_write_offset = (buffer_write_offset + dwBufSize) % BUFFER_SIZE;
				}
				else
				{
					// Otherwise, wrap the data around the circular buffer
					memcpy(input_buffer + buffer_write_offset, pBuffer, remaining_buffer);
					memcpy(input_buffer, pBuffer + remaining_buffer, dwBufSize - remaining_buffer);
					buffer_write_offset = dwBufSize - remaining_buffer;
				}
				buffer_data_size += dwBufSize;
				printf("+");
			}
			else
			{
				printf("~");
				bufFull = true;
			}
		}
	}
}

int avframe_to_yuvi420(AVFrame *frame, uint8_t *yuvi420_buffer, int width, int height)
{
	// if (frame->format != AV_PIX_FMT_YUV420P)
	// {
	// 	fprintf(stderr, "The AVFrame is not in YUV420P format\n");
	// 	return -1;
	// }

	int y_size = width * height;
	int uv_size = (width / 2) * (height / 2);

	// Fill Y plane (Y data is directly copied)
	uint8_t *y_plane = frame->data[0]; // Y data from AVFrame
	memcpy(yuvi420_buffer, y_plane, y_size);

	// Fill interleaved UV plane
	uint8_t *u_plane = frame->data[1]; // U data from AVFrame
	uint8_t *v_plane = frame->data[2]; // V data from AVFrame
	uint8_t *uv_interleaved = yuvi420_buffer + y_size;

	// Interleave U and V
	for (int i = 0; i < uv_size; i++)
	{
		uv_interleaved[2 * i] = u_plane[i];		// U value
		uv_interleaved[2 * i + 1] = v_plane[i]; // V value
	}

	return 0;
}

int video_play_flag = -1;
int width = 640;
int height = 480;

void sendVideoToVideoSource(IZoomVideoSDKVideoSender *video_sender, string video_source)
{
	char *frameBuffer;
	int frameLen = height / 2 * 3 * width;
	frameBuffer = (char *)malloc(frameLen);

	// execute in a thread.
	while (video_play_flag > 0 && video_sender)
	{
		Mat frame;
		VideoCapture cap;
		cap.open(video_source);
		if (!cap.isOpened())
		{
			cerr << "ERROR! Unable to open camera\n";
			video_play_flag = 0;
			break;
		}
		else
		{
			//--- GRAB AND WRITE LOOP
			cout << "Start grabbing" << endl;
			while (video_play_flag > 0)
			{
				// wait for a new frame from camera and store it into 'frame'
				cap.read(frame);
				// check if we succeeded
				if (frame.empty())
				{
					cerr << "ERROR! blank frame grabbed\n";
					break;
				}
				Mat resizedFrame;
				resize(frame, resizedFrame, Size(width, height), 0, 0, INTER_LINEAR);

				// covert Mat to YUV buffer
				Mat yuv;
				cv::cvtColor(resizedFrame, yuv, COLOR_BGRA2YUV_I420);
				char *p;
				for (int i = 0; i < height / 2 * 3; ++i)
				{
					p = yuv.ptr<char>(i);
					for (int j = 0; j < width; ++j)
					{
						frameBuffer[i * width + j] = p[j];
					}
				}
				video_sender->sendVideoFrame(frameBuffer, width, height, frameLen, 0);
				// sleep for 44ms
				std::this_thread::sleep_for(std::chrono::milliseconds(44));
			}
			cap.release();
		}
	}
	video_play_flag = -1; // stop video play when video_sender is null.
}

int decode_ps_to_yuv420p(IZoomVideoSDKVideoSender *sender_)
{
	// 准备输出文件
	FILE *output_file = fopen("out.yuv", "wb");
	if (!output_file)
	{
		fprintf(stderr, "无法打开输出文件 %s\n", "out.yuv");
		return -1;
	}
	// av_register_all(); // Deprecated in newer versions of FFmpeg

	// Setup AVFormatContext with a custom AVIOContext
	AVFormatContext *format_context = avformat_alloc_context(); // linanw diff

	uint8_t *avio_buffer = (uint8_t *)av_malloc(BUFFER_SIZE);
	AVIOContext *avio_ctx = avio_alloc_context(avio_buffer, BUFFER_SIZE, 0, nullptr, read_packet, nullptr, nullptr);
	format_context->pb = avio_ctx;

	if (avformat_open_input(&format_context, nullptr, nullptr, nullptr) < 0)
	{
		fprintf(stderr, "Could not open input stream\n");
		return -1;
	}

	if (avformat_find_stream_info(format_context, nullptr) < 0)
	{
		fprintf(stderr, "Could not find stream info\n");
		return -1;
	}

	// Find video stream
	int video_stream_index = -1;
	for (unsigned int i = 0; i < format_context->nb_streams; i++)
	{
		if (format_context->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			video_stream_index = i;
			printf("Found video stream, index: %d\n", video_stream_index);
			break;
		}
	}

	if (video_stream_index == -1)
	{
		fprintf(stderr, "Could not find video stream\n");
		return -1;
	}

	// Find decoder and open codec
	AVCodecParameters *codec_params = format_context->streams[video_stream_index]->codecpar;
	printf("Codec parameters:\n");
	printf("  Codec ID: %d\n", codec_params->codec_id);
	printf("  Width: %d\n", codec_params->width);
	printf("  Height: %d\n", codec_params->height);
	printf("  Bitrate: %ld\n", codec_params->bit_rate);
	const AVCodec *codec = avcodec_find_decoder(codec_params->codec_id);
	AVCodecContext *codec_context = avcodec_alloc_context3(codec);
	avcodec_parameters_to_context(codec_context, codec_params);
	avcodec_open2(codec_context, codec, nullptr);

	// Prepare for YUV420p conversion
	SwsContext *sws_ctx = sws_getContext(codec_context->width, codec_context->height, codec_context->pix_fmt, codec_context->width, codec_context->height, AV_PIX_FMT_YUV420P, SWS_BILINEAR, nullptr, nullptr, nullptr);
	AVFrame *frame = av_frame_alloc();
	AVFrame *yuv_frame = av_frame_alloc();
	uint8_t *yuv_buffer = (uint8_t *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P, codec_context->width, codec_context->height, 1));
	av_image_fill_arrays(yuv_frame->data, yuv_frame->linesize, yuv_buffer, AV_PIX_FMT_YUV420P, codec_context->width, codec_context->height, 1);

	AVPacket packet;
	printf("Start to process packet.\n");
	// int i = 100;
	while (1)
	{
		if (buffer_data_size > 0)
		{
			if (av_read_frame(format_context, &packet) >= 0)
			{
				printf("-");
				if (packet.stream_index == video_stream_index)
				{
					avcodec_send_packet(codec_context, &packet);
					while (avcodec_receive_frame(codec_context, frame) >= 0)
					{
						printf(".");
						sws_scale(sws_ctx, frame->data, frame->linesize, 0, codec_context->height, yuv_frame->data, yuv_frame->linesize);

						// Copy YUV data to the provided yuv_buffer
						int y_size = codec_context->width * codec_context->height;
						int u_size = y_size / 4;
						int v_size = u_size;

						int sendBufferSize = codec_context->width * codec_context->height * 1.5;
						char* yuvi420_buffer;
						yuvi420_buffer = (char*)malloc(sendBufferSize);

						avframe_to_yuvi420(yuv_frame, (uint8_t *)yuvi420_buffer, codec_context->width, codec_context->height);

						fwrite(yuvi420_buffer, 1, sendBufferSize, output_file);
						sender_->sendVideoFrame(yuvi420_buffer, codec_context->width, codec_context->height, sendBufferSize, 0);
					}
				}
				av_packet_unref(&packet);
			}
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
	fclose(output_file);
	av_frame_free(&frame);
	av_frame_free(&yuv_frame);
	av_free(yuv_buffer);
	sws_freeContext(sws_ctx);
	avcodec_free_context(&codec_context);
	avformat_free_context(format_context);
	// av_free(avio_buffer);

	return 0;
}

HikvisionVideoSource::HikvisionVideoSource(HikvisionCamera *camera)
{
	camera_ = camera;
};

/// \brief Callback for video source  prepare.
/// \param sender See \link IZoomVideoSDKVideoSender \endlink.
/// \param support_cap_list  See \link IVideoSDKVector \endlink, see \link VideoSourceCapability \endlink.
/// \param suggest_cap  See \link VideoSourceCapability \endlink.
void HikvisionVideoSource::onInitialize(IZoomVideoSDKVideoSender *sender, IVideoSDKVector<VideoSourceCapability> *support_cap_list, VideoSourceCapability &suggest_cap)
{
	sender_ = sender;
	cout << suggest_cap.width << " " << suggest_cap.height << endl;
	cout << suggest_cap.frame << endl;
	int n = support_cap_list->GetCount();
	for (int i = 0; i < n; i++)
	{
		VideoSourceCapability cap;
		cap = support_cap_list->GetItem(i);
		cout << cap.width << " " << cap.height << endl;
		cout << cap.frame << endl;
	}
}

/// \brief Callback for video size or fps changed.
/// \param support_cap_list See \link IVideoSDKVector \endlink, see \link VideoSourceCapability \endlink.
/// \param suggest_cap  See \link VideoSourceCapability \endlink.
void HikvisionVideoSource::onPropertyChange(IVideoSDKVector<VideoSourceCapability> *support_cap_list, VideoSourceCapability suggest_cap)
{
}

/// \brief Callback for video source can start send raw data.
void HikvisionVideoSource::onStartSend()
{
	// // **************** Use ffmpeg and hikey camera to send video ****************
	// printf("buffer_data_size: %d, buffer_write_offset: %d\n", buffer_data_size, buffer_write_offset);
	// camera_->GetStream(MyRealDataCallback);
	// while (buffer_data_size < 1024 * 1024)
	// {
	// 	std::this_thread::sleep_for(std::chrono::seconds(1));
	// }
	// decode_ps_to_yuv420p(sender_);
	// // ***************************************************************************

	// ******************* Use OpenCV to send video *******************************
	video_play_flag = 1;
	// sendVideoToVideoSource(sender_, "rtsp://zoom:stream-rtsp@192.168.80.171");
	sendVideoToVideoSource(sender_, "Big_Buck_Bunny_1080_10s_1MB.mp4");
	// ***************************************************************************
}

/// \brief Callback for video source  stop send raw data.
void HikvisionVideoSource::onStopSend()
{
	video_play_flag = -1;
}

/// \brief Callback for video source uninitialized.
void HikvisionVideoSource::onUninitialized()
{
}