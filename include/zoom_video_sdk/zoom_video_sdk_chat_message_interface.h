/*!
* \file zoom_video_sdk_chat_message_interface.h
* \brief chat message interface.
*
*/

#ifndef _ZOOM_VIDEO_SDK_CHAT_MESSAGE_INTERFACE_H_
#define _ZOOM_VIDEO_SDK_CHAT_MESSAGE_INTERFACE_H_
#include "zoom_video_sdk_def.h"

BEGIN_ZOOM_VIDEO_SDK_NAMESPACE
class IZoomVideoSDKUser;
/// \brief Chat message interface.
///
class IZoomVideoSDKChatMessage
{
public:
	virtual ~IZoomVideoSDKChatMessage(){}

	/// \brief Get the message ID of the current message.
    /// \return If the function succeeds, the return value is the message ID of the current message.
    ///Otherwise failed, the return value is the string of length zero(0)
	virtual const zchar_t* getMessageID() = 0;

	/// \brief Get the send user object of the message.
    /// \return If the function succeeds, the return value is send user object, Otherwise NULL.
    virtual IZoomVideoSDKUser* getSendUser() = 0;
    
	/// \brief Get the receive user object of the message.
	/// \return If the function succeeds, the return value is receive user object, Otherwise NULL.
    virtual IZoomVideoSDKUser* getReceiveUser() = 0; 
    
	/// \brief Get the content of the message.
    /// \return The content of the message.
    virtual const zchar_t* getContent() = 0;
    
	/// \brief Get the time stamp of the message.
    /// \return The time stamp of the message.
    virtual time_t getTimeStamp() = 0;
    
	/// \brief Determine if the message is send to all.
    /// \return True if the message is send to all. Otherwise returns false.
    virtual bool isChatToAll() = 0;
    
	/// \brief Determine if the message is send by yourself.
	/// \return True if the message is send by yourself. Otherwise returns false.
    virtual bool isSelfSend() = 0;
};
END_ZOOM_VIDEO_SDK_NAMESPACE
#endif