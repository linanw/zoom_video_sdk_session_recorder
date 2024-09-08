/*!
* \file zoom_video_sdk_password_handler_interface.h
* \brief password handler interface.
*
*/

#ifndef _ZOOM_VIDEO_SDK_PASSWORD_HANDLER_INTERFACE_H_
#define _ZOOM_VIDEO_SDK_PASSWORD_HANDLER_INTERFACE_H_
#include "zoom_video_sdk_def.h"

BEGIN_ZOOM_VIDEO_SDK_NAMESPACE
/// \brief Pasword handle interface.
///
class IZoomVideoSDKPasswordHandler
{
public:
	virtual ~IZoomVideoSDKPasswordHandler(){}

    /// \brief Input session password.
    /// \param password The session password.
    /// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
    ///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
    virtual ZoomVideoSDKErrors inputSessionPassword(const zchar_t* password) = 0;
    
	/// \brief Cancel input password and leave session.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
    virtual ZoomVideoSDKErrors leaveSessionIgnorePassword() = 0;
};
END_ZOOM_VIDEO_SDK_NAMESPACE
#endif