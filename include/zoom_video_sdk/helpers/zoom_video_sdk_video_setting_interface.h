/*!
* \file zoom_video_sdk_video_setting_helper_interface.h
* \brief video helper
*
*/

#ifndef _ZOOM_VIDEO_SDK_VIDEO_SETTING_INTERFACE_H_
#define _ZOOM_VIDEO_SDK_VIDEO_SETTING_INTERFACE_H_
#include "zoom_video_sdk_def.h"
#include "zoom_video_sdk_vector_interface.h"

BEGIN_ZOOM_VIDEO_SDK_NAMESPACE

class IZoomVideoSDKVideoSettingHelper
{
public:	
	/// \brief Call this method to enable or disable the temporal de-noise of video.
	/// \param bEnable True to enable the temporal de-noise of video or false to disable it.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors enableTemporalDeNoise(bool bEnable) = 0;

	/// \brief Determine whether the temporal de-noise of video is enabled.
	/// \param [out] bEnable True if the temporal de-noise of video is enabled, otherwise false.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors isTemporalDeNoiseEnabled(bool& bEnable) = 0;
};
END_ZOOM_VIDEO_SDK_NAMESPACE
#endif