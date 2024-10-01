
// Zoom Video SDK
#include "helpers/zoom_video_sdk_video_source_helper_interface.h"

using namespace ZOOMVIDEOSDK;

class OpenCVVideoSource : public IZoomVideoSDKVideoSource 
{
	IZoomVideoSDKVideoSender* sender_;
	int width_;
	int height_;
	int fps_;

public:
	virtual	void onInitialize(IZoomVideoSDKVideoSender* sender, IVideoSDKVector<VideoSourceCapability >* support_cap_list, VideoSourceCapability& suggest_cap);

	virtual void onPropertyChange(IVideoSDKVector<VideoSourceCapability >* support_cap_list, VideoSourceCapability suggest_cap);
	
	/// \brief Callback for video source can start send raw data.
	virtual void onStartSend();
	
	/// \brief Callback for video source  stop send raw data.
	virtual void onStopSend();
	
	/// \brief Callback for video source uninitialized.
	virtual void onUninitialized();
};