/*!
* \file zoom_video_sdk_api.h
* \brief zoom video sdk apis.
*
*/

#ifndef _ZOOM_VIDEO_SDK_API_H_
#define _ZOOM_VIDEO_SDK_API_H_
#include "zoom_video_sdk_def.h"

BEGIN_ZOOM_VIDEO_SDK_NAMESPACE
extern "C"
{
	class IZoomVideoSDK;

	/// \brief Returns zoom video sdk obj.
	/// \return If the function succeeds, the return value is zoom video sdk obj pointer.
	///Otherwise NULL.
	ZOOM_VIDEO_SDK_EXPORT IZoomVideoSDK* CreateZoomVideoSDKObj();
	
	/// \brief Destroy zoom video sdk obj.
	ZOOM_VIDEO_SDK_EXPORT void DestroyZoomVideoSDKObj();
}
END_ZOOM_VIDEO_SDK_NAMESPACE
#endif