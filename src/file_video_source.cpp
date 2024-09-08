#include "file_video_source.h"

using namespace ZOOMVIDEOSDK;

void FileVideoSource::SendRawVideo(char* frameBuffer, int width, int height, int frameLen, int rotation)
{
	// int width = 1280;
	// int height = 720;
	// int frameLen = height / 2 * 3 * width;

	if (!video_sender_)
		return;

	video_sender_->sendVideoFrame(frameBuffer, width, height, frameLen, rotation);
}

void FileVideoSource::onInitialize(IZoomVideoSDKVideoSender *sender, IVideoSDKVector<VideoSourceCapability> *support_cap_list, VideoSourceCapability &suggest_cap)
{
	video_sender_ = sender;
	printf("file_video_source init\n");

	// int width = 1280;
	// int height = 720;
	// FILE *yuv = fopen("320x180.yuv", "r");
	// int frameLen = height / 2 * 3 * width;
	// // char frameBuffer[frameLen];
	// fread(frameBuffer_, 1, frameLen, yuv);
	// fclose(yuv);
	// // frameBuffer_ = frameBuffer;
}
void FileVideoSource::onPropertyChange(IVideoSDKVector<VideoSourceCapability> *support_cap_list, VideoSourceCapability suggest_cap) {}
void FileVideoSource::onStartSend()
{
	printf("file_video_source start send\n");
}
void FileVideoSource::onStopSend()
{
	printf("file_video_source stop send\n");
}
void FileVideoSource::onUninitialized()
{
	video_sender_ = nullptr;
	printf("file_video_source uninit\n");
}