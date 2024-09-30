
// Zoom Video SDK
#include "helpers/zoom_video_sdk_video_source_helper_interface.h"
#include "hikvision_camera.h"

using namespace ZOOMVIDEOSDK;

class HikvisionVideoSource : public IZoomVideoSDKVideoSource 
{
	IZoomVideoSDKVideoSender* sender_;
	HikvisionCamera* camera_;

public:
	HikvisionVideoSource(HikvisionCamera* camera);
	~HikvisionVideoSource(){};

	/// \brief Callback for video source  prepare.
	/// \param sender See \link IZoomVideoSDKVideoSender \endlink.
	/// \param support_cap_list  See \link IVideoSDKVector \endlink, see \link VideoSourceCapability \endlink.
	/// \param suggest_cap  See \link VideoSourceCapability \endlink.
	virtual	void onInitialize(IZoomVideoSDKVideoSender* sender, IVideoSDKVector<VideoSourceCapability >* support_cap_list, VideoSourceCapability& suggest_cap);

	/// \brief Callback for video size or fps changed.
	/// \param support_cap_list See \link IVideoSDKVector \endlink, see \link VideoSourceCapability \endlink.
	/// \param suggest_cap  See \link VideoSourceCapability \endlink.
	virtual void onPropertyChange(IVideoSDKVector<VideoSourceCapability >* support_cap_list, VideoSourceCapability suggest_cap);
	
	/// \brief Callback for video source can start send raw data.
	virtual void onStartSend();
	
	/// \brief Callback for video source  stop send raw data.
	virtual void onStopSend();
	
	/// \brief Callback for video source uninitialized.
	virtual void onUninitialized();
};