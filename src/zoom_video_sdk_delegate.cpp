#include "zoom_video_sdk_delegate.h"

ZoomVideoSDKDelegate::ZoomVideoSDKDelegate(GMainLoop *loop, IZoomVideoSDK *video_sdk_obj)
{
    _loop = loop;
    _video_sdk_obj = video_sdk_obj;
}

void ZoomVideoSDKDelegate::onSessionJoin()
{
    printf("Joined session successfully\n");
};

void ZoomVideoSDKDelegate::onSessionLeave()
{
    g_main_loop_quit(_loop);
    printf("Already left session.\n");
};

void ZoomVideoSDKDelegate::onError(ZoomVideoSDKErrors errorCode, int detailErrorCode)
{
    printf("Zoom Video SDK, errorCode : %d  detailErrorCode: %d\n", errorCode, detailErrorCode);
};
