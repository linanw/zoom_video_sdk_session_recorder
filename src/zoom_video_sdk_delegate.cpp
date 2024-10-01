#include "zoom_video_sdk_delegate.h"

#include "zoom_video_sdk_chat_message_interface.h"
// #include "raw_data_ffmpeg_encoder.h"
#include <glib.h>

ZoomVideoSDKDelegate::ZoomVideoSDKDelegate(GMainLoop *loop, IZoomVideoSDK *video_sdk_obj)
{
    _loop = loop;
    _video_sdk_obj = video_sdk_obj;
}

/// \brief Triggered when user enter the session.
void ZoomVideoSDKDelegate::onSessionJoin()
{
    printf("Joined session successfully\n");
};

/// \brief Triggered when session leaveSession
void ZoomVideoSDKDelegate::onSessionLeave()
{
    g_main_loop_quit(_loop);
    printf("Already left session.\n");
};

/// \brief Triggered when session error.
/// \param errorCode for more details, see \link ZoomVideoSDKErrors \endlink.
/// \param detailErrorCode Detailed errorCode.
void ZoomVideoSDKDelegate::onError(ZoomVideoSDKErrors errorCode, int detailErrorCode)
{
    printf("join session errorCode : %d  detailErrorCode: %d\n", errorCode, detailErrorCode);
};

/// \brief Triggered when myself or other user join session.
/// \param pUserHelper is the pointer to user helper object, see \link IZoomVideoSDKUserHelper \endlink.
/// \param userList is the pointer to user object list.
void ZoomVideoSDKDelegate::onUserJoin(IZoomVideoSDKUserHelper *pUserHelper, IVideoSDKVector<IZoomVideoSDKUser *> *userList)
{
    if (userList)
    {
        int count = userList->GetCount();
        for (int index = 0; index < count; index++)
        {
            IZoomVideoSDKUser *user = userList->GetItem(index);
            if (user)
            {
                // RawDataFFMPEGEncoder *encoder = new RawDataFFMPEGEncoder(user);
            }
        }
    }
};

/// \brief Triggered when other users leave session.
/// \param pUserHelper is the pointer to user helper object, see \link IZoomVideoSDKUserHelper \endlink.
/// \param userList is the pointer to user object list.
void ZoomVideoSDKDelegate::onUserLeave(IZoomVideoSDKUserHelper *pUserHelper, IVideoSDKVector<IZoomVideoSDKUser *> *userList)
{
    if (userList)
    {
        int count = userList->GetCount();
        for (int index = 0; index < count; index++)
        {
            IZoomVideoSDKUser *user = userList->GetItem(index);
            if (user)
            {
                // RawDataFFMPEGEncoder::stop_encoding_for(user);
            }
        }
    }
};

/// \brief Triggered when user video status changed.
/// \param pVideoHelper is the pointer to video helper object, see \link IZoomVideoSDKVideoHelper \endlink.
/// \param userList is the pointer to user object list.
void ZoomVideoSDKDelegate::onUserVideoStatusChanged(IZoomVideoSDKVideoHelper *pVideoHelper,
                                                    IVideoSDKVector<IZoomVideoSDKUser *> *userList) {};

/// \brief Triggered when user audio status changed.
/// \param pAudioHelper is the pointer to audio helper object, see \link IZoomVideoSDKAudioHelper \endlink.
/// \param userList is the pointer to user object list.
void ZoomVideoSDKDelegate::onUserAudioStatusChanged(IZoomVideoSDKAudioHelper *pAudioHelper,
                                                    IVideoSDKVector<IZoomVideoSDKUser *> *userList) {};

/// \brief Triggered when user Share status changed.
/// \param pShareHelper is the pointer to share helper object, see \link IZoomVideoSDKShareHelper \endlink.
/// \param pUser is the pointer to user object.
/// \param status is the share status of the user.
void ZoomVideoSDKDelegate::onUserShareStatusChanged(IZoomVideoSDKShareHelper *pShareHelper,
                                                    IZoomVideoSDKUser *pUser,
                                                    ZoomVideoSDKShareStatus status,
                                                    ZoomVideoSDKShareType type) {};

/// \brief Triggered when user live stream status changed
/// \param pLiveStreamHelper is the pointer to live stream helper object, see \link IZoomVideoSDKLiveStreamHelper
/// \endlink. \param status is the current status of live stream.
void ZoomVideoSDKDelegate::onLiveStreamStatusChanged(IZoomVideoSDKLiveStreamHelper *pLiveStreamHelper,
                                                     ZoomVideoSDKLiveStreamStatus status) {

};

/// \brief Triggered when chat message received.
/// \param pChatHelper is the pointer to chat helper object, see \link IZoomVideoSDKChatHelper \endlink.
/// \param messageItem is the pointer to message object
void ZoomVideoSDKDelegate::onChatNewMessageNotify(IZoomVideoSDKChatHelper *pChatHelper, IZoomVideoSDKChatMessage *messageItem) {};

/// \brief Triggered when host changed.
/// \param pUserHelper is the pointer to user helper object, see \link IZoomVideoSDKUserHelper \endlink.
/// \param pUser is the pointer to user object.
void ZoomVideoSDKDelegate::onUserHostChanged(IZoomVideoSDKUserHelper *pUserHelper, IZoomVideoSDKUser *pUser) {};

/// \brief Triggered when active audio user changed.
/// \param pAudioHelper is the pointer to audio helper object, see \link IZoomVideoSDKAudioHelper \endlink.
/// \param list is the pointer to user object list.
void ZoomVideoSDKDelegate::onUserActiveAudioChanged(IZoomVideoSDKAudioHelper *pAudioHelper,
                                                    IVideoSDKVector<IZoomVideoSDKUser *> *list) {};

/// \brief Triggered when session needs password.
/// \param handler is the pointer to password handler object, see \link IZoomVideoSDKPasswordHandler \endlink.
void ZoomVideoSDKDelegate::onSessionNeedPassword(IZoomVideoSDKPasswordHandler *handler) {};

/// \brief Triggered when password is wrong.
/// \param handler is the pointer to password handler object, see \link IZoomVideoSDKPasswordHandler \endlink.
void ZoomVideoSDKDelegate::onSessionPasswordWrong(IZoomVideoSDKPasswordHandler *handler) {};

/// \brief Triggered when mixed audio raw data received.
/// \param data_ is the pointer to audio raw data, see \link AudioRawData \endlink.
void ZoomVideoSDKDelegate::onMixedAudioRawDataReceived(AudioRawData *data_) {};

/// \brief Triggered when one way audio raw data received.
/// \param data_ is the pointer to audio raw data, see \link AudioRawData \endlink.
/// \param pUser is the pointer to user object, see \link IZoomVideoSDKUser \endlink.
void ZoomVideoSDKDelegate::onOneWayAudioRawDataReceived(AudioRawData *data_, IZoomVideoSDKUser *pUser) {};

/// \brief Triggered when share audio data received.
/// \param data_ is the pointer to audio raw data, see \link AudioRawData \endlink.
void ZoomVideoSDKDelegate::onSharedAudioRawDataReceived(AudioRawData *data_) {};

/// \brief Triggered when user get session manager role.
/// \param pUser is the pointer to user object, see \link IZoomVideoSDKUser \endlink.
void ZoomVideoSDKDelegate::onUserManagerChanged(IZoomVideoSDKUser *pUser) {};

/// \brief Triggered when user name changed.
/// \param pUser is the pointer to user object, see \link IZoomVideoSDKUser \endlink.
void ZoomVideoSDKDelegate::onUserNameChanged(IZoomVideoSDKUser *pUser) {};

/// \brief Callback for when the current user is granted camera control access.
/// Once the current user sends the camera control request, this callback will be triggered with the result of the
/// request. \param pUser the pointer to the user who received the request, see \link IZoomVideoSDKUser \endlink.
/// \param isApproved the result of the camera control request
void ZoomVideoSDKDelegate::onCameraControlRequestResult(IZoomVideoSDKUser *pUser, bool isApproved) {};

/// \brief Callback interface for when the current user has received a camera control request.
/// This will be triggered when another user requests control of the current user��s camera.
/// \param pUser is the pointer to the user who sent the request, see \link IZoomVideoSDKUser \endlink.
/// \param requestType the request type, see \link ZoomVideoSDKCameraControlRequestType \endlink.
/// \param pCameraControlRequestHandler the pointer to the helper instance of the camera controller, see \link
/// IZoomVideoSDKCameraControlRequestHandler \endlink.
void ZoomVideoSDKDelegate::onCameraControlRequestReceived(
    IZoomVideoSDKUser *pUser,
    ZoomVideoSDKCameraControlRequestType requestType,
    IZoomVideoSDKCameraControlRequestHandler *pCameraControlRequestHandler) {};

/// \brief Callback for when a message has been received from the command channel.
///        Once the command channel is active, this callback is triggered each time a message has been received.
/// \param pSender The user who sent the command, see \link IZoomVideoSDKUser \endlink.
/// \param strCmd Received command.
void ZoomVideoSDKDelegate::onCommandReceived(IZoomVideoSDKUser *sender, const zchar_t *strCmd)
{
    // strCmd;
    printf("command message received\n");
};

/// \brief Callback for when the command channel is ready to be used.
///        After the SDK attempts to establish a connection for the command channel upon joining a session,
///        this callback will be triggered once the connection attempt has completed.
/// \param isSuccess true: success, command channel is ready to be used.
///        false: Failure, command channel was unable to connect.
void ZoomVideoSDKDelegate::onCommandChannelConnectResult(bool isSuccess)
{
    if (isSuccess)
    {
        printf("command channel connected : success\n");
    }
};

/// \brief Triggered when call Out status changed.
void ZoomVideoSDKDelegate::onInviteByPhoneStatus(PhoneStatus status, PhoneFailedReason reason) {};

/// \brief Callback for when the cloud recording status has changed (e.g. paused, stopped, resumed).
/// \param status cloud recording status defined in \link RecordingStatus \endlink.
void ZoomVideoSDKDelegate::onCloudRecordingStatus(RecordingStatus status, IZoomVideoSDKRecordingConsentHandler *pHandler) {};

/// \brief Triggered when host ask you to unmute.
void ZoomVideoSDKDelegate::onHostAskUnmute() {};

void ZoomVideoSDKDelegate::onMultiCameraStreamStatusChanged(ZoomVideoSDKMultiCameraStreamStatus status, IZoomVideoSDKUser *pUser, IZoomVideoSDKRawDataPipe *pVideoPipe) {}

void ZoomVideoSDKDelegate::onMicSpeakerVolumeChanged(unsigned int micVolume, unsigned int speakerVolume) {}

void ZoomVideoSDKDelegate::onAudioDeviceStatusChanged(ZoomVideoSDKAudioDeviceType type, ZoomVideoSDKAudioDeviceStatus status) {}

void ZoomVideoSDKDelegate::onTestMicStatusChanged(ZoomVideoSDK_TESTMIC_STATUS status) {}

void ZoomVideoSDKDelegate::onSelectedAudioDeviceChanged() {}

void ZoomVideoSDKDelegate::onLiveTranscriptionStatus(ZoomVideoSDKLiveTranscriptionStatus status) {}

/// \brief live transcription message received callback.
/// \param ltMsg: an object pointer to the live transcription message content.
/// \param pUser the pointer to the user who speak the message, see \link IZoomVideoSDKUser \endlink.
/// \param type: the live transcription operation type. For more details, see \link ZoomVideoSDKLiveTranscriptionOperationType \endlink.
void ZoomVideoSDKDelegate::onLiveTranscriptionMsgReceived(const zchar_t *ltMsg, IZoomVideoSDKUser *pUser, ZoomVideoSDKLiveTranscriptionOperationType type) {}

/// \brief The translation message error callback.
/// \param speakingLanguage: an object of the spoken message language.
/// \param transcriptLanguageId: an object of the message language you want to translate.
void ZoomVideoSDKDelegate::onLiveTranscriptionMsgError(ILiveTranscriptionLanguage *spokenLanguage, ILiveTranscriptionLanguage *transcriptLanguage) {}
void ZoomVideoSDKDelegate::onSessionLeave(ZoomVideoSDKSessionLeaveReason eReason) {}
void ZoomVideoSDKDelegate::onUserRecordingConsent(IZoomVideoSDKUser *pUser) {}
void ZoomVideoSDKDelegate::onCalloutJoinSuccess(IZoomVideoSDKUser *pUser, const zchar_t *phoneNumber) {}
void ZoomVideoSDKDelegate::onCameraListChanged() {}
void ZoomVideoSDKDelegate::onOriginalLanguageMsgReceived(ILiveTranscriptionMessageInfo *messageInfo) {}
void ZoomVideoSDKDelegate::onLiveTranscriptionMsgInfoReceived(ILiveTranscriptionMessageInfo *messageInfo) {}
void ZoomVideoSDKDelegate::onChatMsgDeleteNotification(IZoomVideoSDKChatHelper *pChatHelper, const zchar_t *msgID, ZoomVideoSDKChatMessageDeleteType deleteBy) {}
void ZoomVideoSDKDelegate::onChatPrivilegeChanged(IZoomVideoSDKChatHelper *pChatHelper, ZoomVideoSDKChatPrivilegeType privilege) {}
void ZoomVideoSDKDelegate::onSendFileStatus(IZoomVideoSDKSendFile *file, const FileTransferStatus &status) {}
void ZoomVideoSDKDelegate::onReceiveFileStatus(IZoomVideoSDKReceiveFile *file, const FileTransferStatus &status) {}
void ZoomVideoSDKDelegate::onProxyDetectComplete() {}
void ZoomVideoSDKDelegate::onProxySettingNotification(IZoomVideoSDKProxySettingHandler *handler) {}
void ZoomVideoSDKDelegate::onSSLCertVerifiedFailNotification(IZoomVideoSDKSSLCertificateInfo *info) {}
void ZoomVideoSDKDelegate::onUserVideoNetworkStatusChanged(ZoomVideoSDKNetworkStatus status, IZoomVideoSDKUser *pUser) {}
void ZoomVideoSDKDelegate::onCallCRCDeviceStatusChanged(ZoomVideoSDKCRCCallStatus status) {}
void ZoomVideoSDKDelegate::onAnnotationHelperCleanUp(IZoomVideoSDKAnnotationHelper *helper) {}
void ZoomVideoSDKDelegate::onAnnotationPrivilegeChange(IZoomVideoSDKUser *pUser, bool enable) {}
void ZoomVideoSDKDelegate::onAnnotationHelperActived(void *handle) {}
void ZoomVideoSDKDelegate::onVideoAlphaChannelStatusChanged(bool isAlphaModeOn) {}
