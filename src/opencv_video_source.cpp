#include "opencv_video_source.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <thread>

using namespace cv;
using namespace std;

using namespace ZOOMVIDEOSDK;

int video_play_flag = -1;

void sendVideoToVideoSource(IZoomVideoSDKVideoSender *video_sender, string video_source, int width, int height, int fps)
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
			cout << getBuildInformation() << endl;
			video_play_flag = 0;
			break;
		}
		else
		{
        	// Get the FPS of the video
        	int video_file_fps = cap.get(CAP_PROP_FPS);
        	cout << "Video File's FPS: " << video_file_fps << endl;
			
			//--- GRAB AND WRITE LOOP		
			cout << "Start play " << endl;
			while (video_play_flag > 0)
			{
				// wait for a new frame from camera and store it into 'frame'
				cap.read(frame);
				// check if we succeeded
				if (frame.empty())
				{
					cerr << "Video file finished\n";
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
				std::this_thread::sleep_for(std::chrono::milliseconds(22/video_file_fps));
			}
			cap.release();
		}
	}
	video_play_flag = -1; // stop video play when video_sender is null.
}

void OpenCVVideoSource::onInitialize(IZoomVideoSDKVideoSender *sender, IVideoSDKVector<VideoSourceCapability> *support_cap_list, VideoSourceCapability &suggest_cap)
{
	sender_ = sender;
	width_ = suggest_cap.width;
	height_ = suggest_cap.height;
	fps_ = suggest_cap.frame;

	printf("Suggest width:%d, height:%d\n", suggest_cap.width, suggest_cap.height);
	printf("Suggest fps:%d\n", suggest_cap.frame);
}

void OpenCVVideoSource::onPropertyChange(IVideoSDKVector<VideoSourceCapability> *support_cap_list, VideoSourceCapability suggest_cap) {}

void OpenCVVideoSource::onStartSend()
{
	// ******************* Use OpenCV to send video *******************************
	video_play_flag = 1;
	sendVideoToVideoSource(sender_, "Big_Buck_Bunny_1080_10s_1MB.mp4", width_, height_, fps_);
	// ***************************************************************************
}

void OpenCVVideoSource::onStopSend()
{
	video_play_flag = -1;
}

void OpenCVVideoSource::onUninitialized() {}