#include "helpers/zoom_video_sdk_video_source_helper_interface.h"

using namespace ZOOMVIDEOSDK;

class FileVideoSource : public IZoomVideoSDKVideoSource
{
protected:
    IZoomVideoSDKVideoSender* video_sender_;
    //char frameBuffer_[720/2*3*1280];
    virtual void onInitialize(IZoomVideoSDKVideoSender *sender, IVideoSDKVector<VideoSourceCapability> *support_cap_list, VideoSourceCapability &suggest_cap);
    virtual void onPropertyChange(IVideoSDKVector<VideoSourceCapability> *support_cap_list, VideoSourceCapability suggest_cap);
    virtual void onStartSend();
    virtual void onStopSend();
    virtual void onUninitialized();
public:
    void SendRawVideo(char* frameBuffer, int width, int height, int frameLen, int rotation);
};