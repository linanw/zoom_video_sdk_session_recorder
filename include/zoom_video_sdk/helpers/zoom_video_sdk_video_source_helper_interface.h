/*!
* \file zoom_video_sdk_video_source_helper_interface.h
* \brief video source helper
*
*/

#ifndef _ZOOM_VIDEO_SDK_VIDEO_SOURCE_HELPER_INTERFACE_H_
#define _ZOOM_VIDEO_SDK_VIDEO_SOURCE_HELPER_INTERFACE_H_
#include "zoom_video_sdk_def.h"
#include "zoom_video_sdk_vector_interface.h"

BEGIN_ZOOM_VIDEO_SDK_NAMESPACE
/// \brief Video source preprocessor sink interface.
///
class IZoomVideoSDKVideoSourcePreProcessor 
{
public:
	virtual ~IZoomVideoSDKVideoSourcePreProcessor(){}

	/// \brief Callback on device capture video frame.
	/// \param rawData See \link YUVProcessDataI420 \endlink.
	virtual void onPreProcessRawData(YUVProcessDataI420* rawData) = 0;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Video raw data sender interface.
///
class IZoomVideoSDKVideoSender 
{
public:
	virtual ~IZoomVideoSDKVideoSender(){}

	/// \brief Send one frame data.
	/// \param frameBuffer FrameBuffer YUVI420 buffer.
	/// \param width Frame width.
	/// \param height Frame height.
	/// \param frameLength Buffer length.
	/// \param rotation Buffer rotation.
	virtual void sendVideoFrame(char* frameBuffer, int width, int height, int frameLength, int rotation, FrameDataFormat format = FrameDataFormat_I420_FULL) = 0;
};

enum VideoSourceDataMode
{
	VideoSourceDataMode_None = 0,    ///<For general usage scenarios.
	VideoSourceDataMode_Horizontal,  ///<Special usage scenarios, each frame of video is organized around two images.
	VideoSourceDataMode_Vertical     ///<Special usage scenarios, each frame of video is two images organized up and down together.
};

/*! \struct VideoSourceCapability
	\brief Video source capability information.
*/
struct VideoSourceCapability
{
	unsigned int width;
	unsigned int height;
	unsigned int frame;
	VideoSourceDataMode data_mode;

	VideoSourceCapability()
	{
		Reset();
	}

	VideoSourceCapability(unsigned int w, unsigned int h, unsigned int f)
	{
		width = w;
		height = h;
		frame = f;
		data_mode = VideoSourceDataMode_None;
	}

	VideoSourceCapability& operator=(const VideoSourceCapability& ins)
	{
		width = ins.width;
		height = ins.height;
		frame = ins.frame;
		data_mode = ins.data_mode;
		return *this;
	}

	VideoSourceCapability(const VideoSourceCapability& ins)
	{
		*this = ins;  
	}

	void Reset()
	{
		width = 0;
		height = 0;
		frame = 0;
		data_mode = VideoSourceDataMode_None;
	}
};

/// \brief Video source sink interface.
///
class IZoomVideoSDKVideoSource 
{
public:
	virtual ~IZoomVideoSDKVideoSource(){}

	/// \brief Callback for video source  prepare.
	/// \param sender See \link IZoomVideoSDKVideoSender \endlink.
	/// \param support_cap_list  See \link IVideoSDKVector \endlink, see \link VideoSourceCapability \endlink.
	/// \param suggest_cap  See \link VideoSourceCapability \endlink.
	virtual	void onInitialize(IZoomVideoSDKVideoSender* sender, IVideoSDKVector<VideoSourceCapability >* support_cap_list, VideoSourceCapability& suggest_cap) = 0;

	/// \brief Callback for video size or fps changed.
	/// \param support_cap_list See \link IVideoSDKVector \endlink, see \link VideoSourceCapability \endlink.
	/// \param suggest_cap  See \link VideoSourceCapability \endlink.
	virtual void onPropertyChange(IVideoSDKVector<VideoSourceCapability >* support_cap_list, VideoSourceCapability suggest_cap) = 0;
	
	/// \brief Callback for video source can start send raw data.
	virtual void onStartSend() = 0;
	
	/// \brief Callback for video source  stop send raw data.
	virtual void onStopSend() = 0;
	
	/// \brief Callback for video source uninitialized.
	virtual void onUninitialized() = 0;
};
END_ZOOM_VIDEO_SDK_NAMESPACE
#endif