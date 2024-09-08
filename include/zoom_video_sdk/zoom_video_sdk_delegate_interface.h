/*!
* \file zoom_video_sdk_delegate_interface.h
* \brief ZOOM Video SDK callback handler.
*
*/

#ifndef _ZOOM_VIDEO_SDK_DELEGATE_INTERFACE_H_
#define _ZOOM_VIDEO_SDK_DELEGATE_INTERFACE_H_
#include "zoom_video_sdk_def.h"
#include "zoom_video_sdk_vector_interface.h"
#include "helpers/zoom_video_sdk_phone_helper_interface.h"
#include "helpers/zoom_video_sdk_recording_helper_interface.h"
#include "helpers/zoom_video_sdk_audio_setting_interface.h"
#include "helpers/zoom_video_sdk_user_helper_interface.h"
#include "helpers/zoom_video_sdk_video_helper_interface.h"
#include "helpers/zoom_video_sdk_audio_helper_interface.h"
#include "helpers/zoom_video_sdk_share_helper_interface.h"
#include "helpers/zoom_video_sdk_livestream_helper_interface.h"
#include "helpers/zoom_video_sdk_livetranscription_helper_interface.h"
#include "helpers/zoom_video_sdk_chat_helper_interface.h"
#include "helpers/zoom_video_sdk_network_connection_helper_interface.h"
#include "helpers/zoom_video_sdk_crc_helper_interface.h"
BEGIN_ZOOM_VIDEO_SDK_NAMESPACE
class IZoomVideoSDKChatMessage;
class IZoomVideoSDKPasswordHandler;
class IZoomVideoSDKRecordingConsentHandler;
/// \brief A listener class that groups together the callbacks related to a session.
///
class IZoomVideoSDKDelegate
{
public:
	/// \brief Invoked when the current user joins the session.
    virtual void onSessionJoin() = 0;
    
	/// \brief Invoked when the current user leaves the session.
	/// \deprecated This interface is marked as deprecated.
    virtual void onSessionLeave() = 0;
    
	/// \brief Invoked when the current user leaves the session with reason.
	/// \param eReason For more details, see \link ZoomVideoSDKSessionLeaveReason \endlink.
    virtual void onSessionLeave(ZoomVideoSDKSessionLeaveReason eReason) = 0;
    
	/// \brief Invoked when errors occur.
	/// \param errorCode For more details, see \link ZoomVideoSDKErrors \endlink.
	/// \param detailErrorCode Detailed errorCode.
    virtual void onError(ZoomVideoSDKErrors errorCode, int detailErrorCode) = 0;
	
	/// \brief Invoked when a user joins the session.
	/// \param pUserHelper User helper utility, see \link IZoomVideoSDKUserHelper \endlink.
	/// \param userList List of users who have just joined the session.
	virtual void onUserJoin(IZoomVideoSDKUserHelper* pUserHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList) = 0;
	
	/// \brief Invoked when a user leaves the session
	/// \param pUserHelper User helper utility, see \link IZoomVideoSDKUserHelper \endlink.
	/// \param userList List of users who have just left the session.
	virtual void onUserLeave(IZoomVideoSDKUserHelper* pUserHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList) = 0;
	
	/// \brief Invoked when a user makes changes to their video, such as starting or stopping their video.
	/// \param pVideoHelper The pointer to video helper object, see \link IZoomVideoSDKVideoHelper \endlink.
	/// \param userList The pointer to user object list.
	virtual void onUserVideoStatusChanged(IZoomVideoSDKVideoHelper* pVideoHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList) = 0;
	
	/// \brief Invoked when a user makes changes to their audio, such as muting or unmuting their audio.
	/// \param pAudioHelper The pointer to audio helper object, see \link IZoomVideoSDKAudioHelper \endlink.
	/// \param userList The pointer to user object list.
	virtual void onUserAudioStatusChanged(IZoomVideoSDKAudioHelper* pAudioHelper, IVideoSDKVector<IZoomVideoSDKUser*>* userList) = 0;
	
	/// \brief Invoked when a user makes changes to their sharing status, such as starting screen sharing, starting view sharing, or stopping sharing.
	/// \param pShareHelper The pointer to share helper object, see \link IZoomVideoSDKShareHelper \endlink.
	/// \param pUser The pointer to user object.
	/// \param status The share status of the user.
	/// \param type The share type of the user.
	virtual void onUserShareStatusChanged(IZoomVideoSDKShareHelper* pShareHelper, IZoomVideoSDKUser* pUser, ZoomVideoSDKShareStatus status, ZoomVideoSDKShareType type) = 0;

	/// \brief Invoked when a user consents to individual recording.
	/// \param pUser The pointer to user object.
	virtual void onUserRecordingConsent(IZoomVideoSDKUser* pUser) = 0;
	
	/// \brief Invoked when a user makes changes to their live stream status.
	/// \param pLiveStreamHelper The pointer to live stream helper object, see \link IZoomVideoSDKLiveStreamHelper \endlink.
	/// \param status The current status of live stream.
    virtual void onLiveStreamStatusChanged(IZoomVideoSDKLiveStreamHelper* pLiveStreamHelper, ZoomVideoSDKLiveStreamStatus status) = 0;
	
	/// \brief Invoked when receiving a chat message.
	/// \param pChatHelper The pointer to chat helper object, see \link IZoomVideoSDKChatHelper \endlink.
	/// \param messageItem The pointer to message object
    virtual void onChatNewMessageNotify(IZoomVideoSDKChatHelper* pChatHelper, IZoomVideoSDKChatMessage* messageItem) = 0;
	
	/// \brief Invoked when the session host changes.
	/// \param pUserHelper The pointer to user helper object, see \link IZoomVideoSDKUserHelper \endlink.
	/// \param pUser The pointer to user object.
	virtual void onUserHostChanged(IZoomVideoSDKUserHelper* pUserHelper, IZoomVideoSDKUser* pUser) = 0;
	
	/// \brief Invoked when the active audio changes.
	/// \param pAudioHelper Audio helper utility, see \link IZoomVideoSDKAudioHelper \endlink.
	/// \param list Active audio list.
	virtual void onUserActiveAudioChanged(IZoomVideoSDKAudioHelper* pAudioHelper, IVideoSDKVector<IZoomVideoSDKUser*>* list) = 0;
	
	/// \brief Invoked when the session requires a password to join.
	/// \param handler The pointer to password handler object, see \link IZoomVideoSDKPasswordHandler \endlink.
    virtual void onSessionNeedPassword(IZoomVideoSDKPasswordHandler* handler) = 0;
	
	/// \brief Invoked when the provided session password is wrong or invalid.
	/// \param handler The pointer to password handler object, see \link IZoomVideoSDKPasswordHandler \endlink.
    virtual void onSessionPasswordWrong(IZoomVideoSDKPasswordHandler* handler) = 0;
	
	/// \brief Invoked when mixed (all users) audio raw data received 
	/// \param data_ The pointer to audio raw data, see \link AudioRawData \endlink.
	virtual void onMixedAudioRawDataReceived(AudioRawData* data_) = 0;

	/// \brief Invoked when individual user's audio raw data received
	/// \param data_ Raw audio data, see \link AudioRawData \endlink.
	/// \param pUser The pointer to user object associated with the raw audio data, see \link IZoomVideoSDKUser \endlink.
	virtual void onOneWayAudioRawDataReceived(AudioRawData* data_, IZoomVideoSDKUser* pUser) = 0;

	/// \brief Invoked when receiving shared raw audio data.
	/// \param data_ Raw audio data, see \link AudioRawData \endlink. 
	virtual void onSharedAudioRawDataReceived(AudioRawData* data_) = 0;
	
	/// \brief Invoked when the manager of the session changes.
	/// \param pUser The pointer to user object, see \link IZoomVideoSDKUser \endlink.
	virtual void onUserManagerChanged(IZoomVideoSDKUser* pUser) = 0;
	
	/// \brief Triggered when user name changed.
	/// \param pUser The pointer to user object, see \link IZoomVideoSDKUser \endlink.
	virtual void onUserNameChanged(IZoomVideoSDKUser* pUser) = 0;

	/// \brief Callback for when the current user is granted camera control access.
	/// Once the current user sends the camera control request, this callback will be triggered with the result of the request.
	/// \param pUser The pointer to the user who received the request, see \link IZoomVideoSDKUser \endlink.
	/// \param isApproved The result of the camera control request
	virtual void onCameraControlRequestResult(IZoomVideoSDKUser* pUser, bool isApproved) = 0;

	/// \brief Callback interface for when the current user has received a camera control request.
	/// This will be triggered when another user requests control of the current user’s camera.
	/// \param pUser The pointer to the user who sent the request, see \link IZoomVideoSDKUser \endlink.
	/// \param requestType The request type, see \link ZoomVideoSDKCameraControlRequestType \endlink.
	/// \param pCameraControlRequestHandler The pointer to the helper instance of the camera controller, see \link IZoomVideoSDKCameraControlRequestHandler \endlink.
	virtual void onCameraControlRequestReceived(IZoomVideoSDKUser* pUser, 
		ZoomVideoSDKCameraControlRequestType requestType, 
		IZoomVideoSDKCameraControlRequestHandler* pCameraControlRequestHandler) = 0;
	
	/// \brief Invoked when receiving a message, data, or a command from the command channel. 
	///        Once the command channel is active, this callback is triggered each time a message is received.
	/// \param pSender The user who sent the command, see \link IZoomVideoSDKUser \endlink.
	/// \param strCmd Received command.
	virtual void onCommandReceived(IZoomVideoSDKUser* sender, const zchar_t* strCmd) = 0;

	/// \brief Invoked when the command channel is ready to be used.
	///        When the SDK attempts to establish a connection for the command channel when joining a session,
	///        this callback is triggered once the connection attempt for the command channel is completed.
	/// \param isSuccess True: success, command channel is ready to be used.
	///        False: Failure, command channel was unable to connect.
	virtual void onCommandChannelConnectResult(bool isSuccess) = 0;
	
	/// \brief Invoked when the invite by phone status changes to any other valid status such as Calling, Ringing, Success, or Failed.
	/// \param status Invite by phone status, see see \link PhoneStatus \endlink.
	/// \param reason IInvite by phone failed reason, see see \link PhoneFailedReason \endlink.
	virtual void onInviteByPhoneStatus(PhoneStatus status, PhoneFailedReason reason) = 0;

    /// \brief Invoked when the callout user successfully joins the meeting.
    /// \param pUser The invited user who joined the meeting.
    /// \param phoneNumber The phone number involved in the callout.
	virtual void onCalloutJoinSuccess(IZoomVideoSDKUser* pUser, const zchar_t* phoneNumber) = 0;

	/// \brief Invoked when cloud recording status has paused, stopped, resumed, or otherwise changed.
	/// \param status Cloud recording status defined in \link RecordingStatus \endlink.
	/// \param pHandler When the cloud recording starts, this object is used to let the user choose whether to accept or not.
	virtual void onCloudRecordingStatus(RecordingStatus status, IZoomVideoSDKRecordingConsentHandler* pHandler) = 0;

	/// \brief Invoked when a host requests you to unmute yourself.
	virtual void onHostAskUnmute() = 0;

	/// \brief Invoked when someone in a given session enables or disables multi-camera. All participants in the session receive this callback.
	/// \param status Refer to \link ZoomVideoSDKMultiCameraStreamStatus \endlink. 
	/// \param pUser The user who enabled multi-camera, refer to \link IZoomVideoSDKUser \endlink. 
	/// \param pVideoPipe The data pipe for the multi-camera, refer to \link IZoomVideoSDKRawDataPipe \endlink. 
	virtual void onMultiCameraStreamStatusChanged(ZoomVideoSDKMultiCameraStreamStatus status, IZoomVideoSDKUser* pUser, IZoomVideoSDKRawDataPipe* pVideoPipe) = 0;

	/// \brief Notify the current mic or speaker volume when testing.
	/// \param MicVolume Specify the volume of the mic.
	/// \param SpeakerVolume Specify the volume of the speaker.
	virtual void onMicSpeakerVolumeChanged(unsigned int micVolume, unsigned int speakerVolume) = 0;

	/// \brief Notify that either mic device or speaker device status changed.
	/// \param type Refer to \link ZoomVideoSDKAudioDeviceType \endlink. 
	/// \param status Refer to \link ZoomVideoSDKDeviceStatus \endlink. 
	virtual void onAudioDeviceStatusChanged(ZoomVideoSDKAudioDeviceType type, ZoomVideoSDKAudioDeviceStatus status) = 0;

	/// \brief Notify the mic status when testing.
	/// \param status The mic status. For more details, see \link ZoomVideoSDK_TESTMIC_STATUS \endlink enum.
	/// \remarks The enuCanPlay status indicates that the SDK has recorded the microphone sound for the longest time (6 seconds).customers can decide for themselves whether to stop test mic or play microphone sound.
	virtual void onTestMicStatusChanged(ZoomVideoSDK_TESTMIC_STATUS status) = 0;

	/// \brief Notify the user that a mic/speaker device is selected when testing. Then the SDK will close the mic/speaker testing. The user shall restart the test manually if he still wants to test.
	virtual void onSelectedAudioDeviceChanged() = 0;
    
    /// \brief Notify that the camera list has changed.
    virtual void onCameraListChanged() = 0;

	/// \brief Invoked when live transcription status changes.
	/// \param status The live transcription status. For more details, see \link ZoomVideoSDKLiveTranscriptionStatus \endlink.
	virtual void onLiveTranscriptionStatus(ZoomVideoSDKLiveTranscriptionStatus status) = 0;

	/// \brief Invoked when a live transcription message is received.
	/// \param ltMsg The live transcription message content. 
	/// \param pUser The user who speak the message, see \link IZoomVideoSDKUser \endlink. 
	/// \param type The live transcription operation type. For more details, see \link ZoomVideoSDKLiveTranscriptionOperationType \endlink.
	/// \deprecated This interface will be marked as deprecated, then it will be instead by onLiveTranscriptionMsgInfoReceived, please stop using it.
	virtual void onLiveTranscriptionMsgReceived(const zchar_t* ltMsg, IZoomVideoSDKUser* pUser, ZoomVideoSDKLiveTranscriptionOperationType type) = 0;

	/// \brief Original language message received callback.
	/// \param messageInfo The spoken language message, see \link ILiveTranscriptionMessageInfo \endlink.	
	virtual void onOriginalLanguageMsgReceived(ILiveTranscriptionMessageInfo* messageInfo) = 0;

	/// \brief Invoked when a live transcription message is received.	
	/// \param messageInfo The live transcription message, see \link ILiveTranscriptionMessageInfo \endlink. 	
	virtual void onLiveTranscriptionMsgInfoReceived(ILiveTranscriptionMessageInfo* messageInfo) = 0;

	/// \brief Invoked when a live translation error occurs.
	/// \param speakingLanguage The spoken message language. 
	/// \param transcriptLanguageId The message language you want to translate.
	virtual void onLiveTranscriptionMsgError(ILiveTranscriptionLanguage* spokenLanguage, ILiveTranscriptionLanguage* transcriptLanguage) = 0;

	/// \brief Invoked when a user deletes a chat message.
	/// \param pChatHelper Chat helper utility, see \link IZoomVideoSDKChatHelper \endlink.
	/// \param MsgID The deleted message's ID.
	/// \param deleteBy Indicates by whom the message was deleted.
	virtual void onChatMsgDeleteNotification(IZoomVideoSDKChatHelper* pChatHelper, const zchar_t* msgID, ZoomVideoSDKChatMessageDeleteType deleteBy) = 0;

	/// \brief Callback event of the chat privilege of participant has changed.
	/// \param pChatHelper The pointer to chat helper object, see \link IZoomVideoSDKChatHelper \endlink.
	/// \param privilege The new chat privilege. For more details, see \link ZoomVideoSDKChatPrivilegeType \endlink enum.
	virtual void onChatPrivilegeChanged(IZoomVideoSDKChatHelper* pChatHelper, ZoomVideoSDKChatPrivilegeType privilege) = 0;

	/// \brief Invoked when send file status make change
	/// \brief param file The pointer to sendfile object, see \link IZoomVideoSDKSendFile \endlink.
	/// \brief param status The stauts of file transfer.
	virtual void onSendFileStatus(IZoomVideoSDKSendFile* file, const FileTransferStatus& status) = 0;

	/// \brief Invoked when receive file status make change
	/// \brief param file The pointer to receive file object, see \link IZoomVideoSDKReceiveFile \endlink.
	/// \brief param status The status of file transfer.
	virtual void onReceiveFileStatus(IZoomVideoSDKReceiveFile* file, const FileTransferStatus& status) = 0;

	/// \brief Notification callback of completing the proxy detection.
	virtual void onProxyDetectComplete() = 0;
	/// \brief The callback will be triggered if the proxy requests to input the username and password.
	/// \remarks Use the handler to configure the related information. For more details, see \link IZoomVideoSDKProxySettingHandler \endlink. 
	/// \brief param handler The pointer to the IZoomVideoSDKProxySettingHandler. For more details, see \link IZoomVideoSDKProxySettingHandler \endlink.
	virtual void onProxySettingNotification(IZoomVideoSDKProxySettingHandler* handler) = 0;

	/// \brief The callback will be triggered when the SSL verified fail.
	/// \remarks Use the handler to check the SSL related information. For more details, see \link IZoomVideoSDKSSLCertificateInfo \endlink.
	///The handler will be destroyed once the function calls end.
	virtual void onSSLCertVerifiedFailNotification(IZoomVideoSDKSSLCertificateInfo* info) = 0;

	/// \brief Callback event of the user's video network quality changes.
	/// \param status video network quality. For more details, see \link ZoomVideoSDKNetworkStatus \endlink enum.
	/// \param pUser The pointer to a user object, see \link IZoomVideoSDKUser \endlink. 	
	virtual void onUserVideoNetworkStatusChanged(ZoomVideoSDKNetworkStatus status, IZoomVideoSDKUser* pUser) = 0;

	/// \brief Callback event of the call CRC device's status.
	/// \param status The call status.. For more details, see \link ZoomVideoSDKCRCCallStatus \endlink enum.
	virtual void onCallCRCDeviceStatusChanged(ZoomVideoSDKCRCCallStatus status) = 0;

#if !defined __linux
	/// \brief Callback event for the subscribed user's video failure reason.
	/// \param fail_reason The user's video subscribe failure reason. For more details, see \link ZoomVideoSDKSubscribeFailReason \endlink enum.
	/// \param pUser The pointer to a user object, see \link IZoomVideoSDKUser \endlink.  
	/// \param handle The window handle that failed to subscribe.
	virtual void onVideoCanvasSubscribeFail(ZoomVideoSDKSubscribeFailReason fail_reason, IZoomVideoSDKUser* pUser, void* handle) = 0;

	/// \brief Callback event for the subscribed user's share view failure reason.
	/// \param fail_reason The user's share view subscribe failure reason. For more details, see \link ZoomVideoSDKSubscribeFailReason \endlink enum.
	/// \param pUser The pointer to a user object, see \link IZoomVideoSDKUser \endlink.  
	/// \param handle The window handle that failed to subscribe.
	virtual void onShareCanvasSubscribeFail(ZoomVideoSDKSubscribeFailReason fail_reason, IZoomVideoSDKUser* pUser, void* handle) = 0;
#endif
    
	/// \brief Callback for the annotation helper clean up.
	/// \param helper The clean up object, see \link IZoomVideoSDKAnnotationHelper \endlink enum.
	/// \After this callback, SDK will release the ZoomVideoSDKAnnotationHelper as well.	
	virtual void onAnnotationHelperCleanUp(IZoomVideoSDKAnnotationHelper* helper) = 0;

	/// \brief Callback for the annotation privilege change.
	/// \enable true means the user is able to annotate,false means the user is not able to annotate.
	/// \param pUser The pointer to a user object, see \link IZoomVideoSDKUser \endlink.  
	virtual void onAnnotationPrivilegeChange(IZoomVideoSDKUser* pUser, bool enable) = 0;
	
	/// \brief Callback for the annotation helper activated. Notify that annotation windows have been created.	
	/// \param handle The window handle that annotating.
	virtual void onAnnotationHelperActived(void* handle) = 0;

	/// \brief Callback event of video alpha channel mode changes.
	/// \param isAlphaModeOn True means that alpha channel mode is on, otherwise off.
	virtual void onVideoAlphaChannelStatusChanged(bool isAlphaModeOn) = 0;
};
END_ZOOM_VIDEO_SDK_NAMESPACE
#endif
