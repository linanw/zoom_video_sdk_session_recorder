/*!
* \file zoom_video_sdk_cmd_channel_interface.h
* \brief cmd channel
*
*/

#ifndef _ZOOM_VIDEO_SDK_CMD_CHANNEL_INTERFACE_H_
#define _ZOOM_VIDEO_SDK_CMD_CHANNEL_INTERFACE_H_
#include "zoom_video_sdk_def.h"

BEGIN_ZOOM_VIDEO_SDK_NAMESPACE

class IZoomVideoSDKUser;
/// \brief The command channel 
///        allows users to send commands or data (such as plain text or a binary encoded into string) to other users in the same session.
class IZoomVideoSDKCmdChannel
{
public:
    virtual ~IZoomVideoSDKCmdChannel() {}

	/// \brief Send custom commands or data to other users in the current session. Limit: up to 60 custom commands per second.
    /// \param receiver The user who receives the command, if receiver is set to NULL, the command is broadcast to all users. see \link IZoomVideoSDKUser \endlink.
    /// \param strCmd The custom commands or data.
	/// \return If the command sends successfully, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise the send command request fails. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
    virtual ZoomVideoSDKErrors sendCommand(IZoomVideoSDKUser* receiver, const zchar_t* strCmd) = 0;
};
END_ZOOM_VIDEO_SDK_NAMESPACE
#endif