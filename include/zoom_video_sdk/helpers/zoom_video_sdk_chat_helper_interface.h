/*!
* \file zoom_video_sdk_chat_helper_interface.h
* \brief chat helper
*
*/

#ifndef _ZOOM_VIDEO_SDK_CHAT_HELPER_INTERFACE_H_
#define _ZOOM_VIDEO_SDK_CHAT_HELPER_INTERFACE_H_
#include "zoom_video_sdk_def.h"
#include "zoom_video_sdk_vector_interface.h"

BEGIN_ZOOM_VIDEO_SDK_NAMESPACE
/// \brief The chat message delete type are sent in the \link onChatMsgDeleteNotification  \endlink callback.
///
typedef enum {
	ZoomVideoSDKChatDelete_None = 0,  ///<None.
	ZoomVideoSDKChatDelete_BySelf,      ///<Indicates that the message was deleted by myself.
    ZoomVideoSDKChatDelete_ByHost,      ///<Indicates that the message was deleted by the session host.
    ZoomVideoSDKChatDelete_ByDlp,       ///<Indicates that the message was deleted by Data Loss Prevention (dlp). This happens when the message goes against the host organization's compliance policies.
}ZoomVideoSDKChatMessageDeleteType;

/// \brief The chat privilege type are sent in the \link onChatPrivilegeChanged  \endlink callback.
///
typedef enum {
    ZoomVideoSDKChatPrivilege_Unknown = 0,             ///<Unknown type
    ZoomVideoSDKChatPrivilege_Publicly_And_Privately,  ///<allow participant to chat with everyone
    ZoomVideoSDKChatPrivilege_No_One,                  ///<allow participant to chat with no one
    ZoomVideoSDKChatPrivilege_Publicly,                ///<allow participant to chat with the host, co-host, and everyone.
}ZoomVideoSDKChatPrivilegeType;



class IZoomVideoSDKUser;
/// \brief Chat helper interface.
///
class IZoomVideoSDKChatHelper
{
public:
	/// \brief Determine whether chat is disabled.
    /// \return True if chat is disabled, otherwise false.
    virtual bool isChatDisabled() = 0;
    
	/// \brief Determine whether private chat is disabled.
    /// \return True if private chat is disabled, otherwise false.
    virtual bool isPrivateChatDisabled() = 0;
    
	/// \brief Send a chat message to a specific user.
    /// \param pUser The receiver.
    /// \param msgContent The message content.
    /// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
    ///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
    virtual ZoomVideoSDKErrors sendChatToUser(IZoomVideoSDKUser* pUser, const zchar_t* msgContent) = 0;
    
	/// \brief Send a chat message to all users.
    /// \param msgContent The message content.
    /// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
    ///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
    virtual ZoomVideoSDKErrors sendChatToAll(const zchar_t* msgContent) = 0;

	/// \brief Determine if a specific message can be deleted.
	/// \param msgID The message id.
	/// \return True if the message can be deleted, otherwise False.
	virtual bool canChatMessageBeDeleted(const zchar_t* msgID) = 0;

	/// \brief delete a specific chat message from the Zoom server. This does not delete the message in your user interface.	
	/// \param msgID The message id.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors deleteChatMessage(const zchar_t* msgID) = 0;

    /// \brief Set participant Chat Privilege when in session. Only session host/manager can run the function.
    /// \param privilege The chat privilege of the participant.
    /// \return If the function succeeds, it will return ZoomVideoSDKErrors_Success, otherwise failed.
    ///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors changeChatPrivilege(ZoomVideoSDKChatPrivilegeType privilege) = 0;

    /// \brief Get participant Chat Privilege when in session.
    /// \return The result of participant chat priviledge.
	virtual ZoomVideoSDKChatPrivilegeType getChatPrivilege() = 0;
};
END_ZOOM_VIDEO_SDK_NAMESPACE
#endif