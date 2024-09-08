/*!
* \file zoom_video_sdk_video_setting_helper_interface.h
* \brief video helper
*
*/

#ifndef _ZOOM_VIDEO_SDK_SHARE_SETTING_INTERFACE_H_
#define _ZOOM_VIDEO_SDK_SHARE_SETTING_INTERFACE_H_
#include "zoom_video_sdk_def.h"

BEGIN_ZOOM_VIDEO_SDK_NAMESPACE

enum ZoomVideoSDKScreenCaptureMode
{
    //Screen capture mode is automatically.
    ZoomVideoSDKScreenCaptureMode_Auto,
    //Screen capture mode is legacy operating systems.
    ZoomVideoSDKScreenCaptureMode_Legacy,
    //Screen capture mode is capture with window filtering.
    ZoomVideoSDKScreenCaptureMode_Filtering,
    //Screen capture mode is advanced share with window filtering.
    ZoomVideoSDKScreenCaptureMode_ADA_Filtering,
    //Screen capture mode is advanced share without window filtering.
    ZoomVideoSDKScreenCaptureMode_ADA_Without_Filtering,
    //Screen capture mode is secure share with window filtering.
    ZoomVideoSDKScreenCaptureMode_Secure_Filtering,
};

class IZoomVideoSDKShareSettingHelper
{
public:
    /// \brief Set screen capture mode.
    /// \param captureMode The mode to be set, \link ZoomVideoSDKScreenCaptureMode \endlink.
    /// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
    ///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
    virtual ZoomVideoSDKErrors setScreenCaptureMode(ZoomVideoSDKScreenCaptureMode captureMode) = 0;

    /// \brief Get the screen capture mode.
    /// \param [out] captureMode The screen capture mode, \link ZoomVideoSDKScreenCaptureMode \endlink.
    /// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
    ///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
    virtual ZoomVideoSDKErrors getScreenCaptureMode(ZoomVideoSDKScreenCaptureMode& captureMode) = 0;

    /// \brief Set the visibility of the green border when sharing the screen.
    /// \param bEnable True indicates to display the green border. False hide.
    /// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
    ///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
    virtual ZoomVideoSDKErrors enableGreenBorder(bool bEnable) = 0;

    /// \brief Determine if the green border is enabled when user shares the screen.
    /// \return True indicates enabled. False not.
    virtual bool isGreenBorderEnabled() = 0;


};
END_ZOOM_VIDEO_SDK_NAMESPACE
#endif