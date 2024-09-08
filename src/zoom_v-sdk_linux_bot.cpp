#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fstream>
#include <iosfwd>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <sstream>

#include "glib.h"
#include "json.hpp"
#include "helpers/zoom_video_sdk_user_helper_interface.h"
#include "helpers/zoom_video_sdk_cmd_channel_interface.h"
#include "zoom_video_sdk_api.h"
#include "zoom_video_sdk_def.h"
#include "zoom_video_sdk_delegate_interface.h"
#include "zoom_video_sdk_interface.h"
#include "raw_data_ffmpeg_encoder.h"

using Json = nlohmann::json;
USING_ZOOM_VIDEO_SDK_NAMESPACE
IZoomVideoSDK *video_sdk_obj;
GMainLoop *loop;
int video_index = -1;
bool is_to_record = false;
FileVideoSource *fileVideoSource = new FileVideoSource();

std::string getSelfDirPath()
{
    char dest[PATH_MAX];
    memset(dest, 0, sizeof(dest)); // readlink does not null terminate!
    if (readlink("/proc/self/exe", dest, PATH_MAX) == -1)
    {
    }

    char *tmp = strrchr(dest, '/');
    if (tmp)
        *tmp = 0;
    return std::string(dest);
}

class ZoomVideoSDKDelegate : public IZoomVideoSDKDelegate
{
public:
    /// \brief Triggered when user enter the session.
    virtual void onSessionJoin()
    {
        printf("Joined session successfully.\n");
        if (video_index >= 0)
        {
            printf("Camera is on.\n");
        }
        else
        {
            video_sdk_obj->getVideoHelper()->startVideo();
        }
        if (is_to_record)
            printf("Ready to recording.\n");
    };

    /// \brief Triggered when session leaveSession
    virtual void onSessionLeave()
    {
        g_main_loop_unref(loop);
        printf("Already left session.\n");
        exit(0);
    };

    /// \brief Triggered when session error.
    /// \param errorCode for more details, see \link ZoomVideoSDKErrors \endlink.
    /// \param detailErrorCode Detailed errorCode.
    virtual void onError(ZoomVideoSDKErrors errorCode, int detailErrorCode)
    {
        printf("join session errorCode : %d  detailErrorCode: %d\n", errorCode, detailErrorCode);
    };

    /// \brief Triggered when myself or other user join session.
    /// \param pUserHelper is the pointer to user helper object, see \link IZoomVideoSDKUserHelper \endlink.
    /// \param userList is the pointer to user object list.
    virtual void onUserJoin(IZoomVideoSDKUserHelper *pUserHelper, IVideoSDKVector<IZoomVideoSDKUser *> *userList)
    {
        if (userList)
        {
            int count = userList->GetCount();
            for (int index = 0; index < count; index++)
            {
                IZoomVideoSDKUser *user = userList->GetItem(index);
                if (user)
                {
                    printf("An user joined/found: %s(%s).\n", user->getUserName(), user->getUserID());
                    if (is_to_record)
                        RawDataFFMPEGEncoder *encoder = new RawDataFFMPEGEncoder(user, fileVideoSource);
                }
            }
        }
    };

    /// \brief Triggered when other users leave session.
    /// \param pUserHelper is the pointer to user helper object, see \link IZoomVideoSDKUserHelper \endlink.
    /// \param userList is the pointer to user object list.
    virtual void onUserLeave(IZoomVideoSDKUserHelper *pUserHelper, IVideoSDKVector<IZoomVideoSDKUser *> *userList)
    {
        printf("An user left.\n");
        if (userList)
        {
            int count = userList->GetCount();
            for (int index = 0; index < count; index++)
            {
                IZoomVideoSDKUser *user = userList->GetItem(index);
                if (user)
                {
                    RawDataFFMPEGEncoder::stop_encoding_for(user);
                }
            }
        }
    };

    /// \brief Triggered when user video status changed.
    /// \param pVideoHelper is the pointer to video helper object, see \link IZoomVideoSDKVideoHelper \endlink.
    /// \param userList is the pointer to user object list.
    virtual void onUserVideoStatusChanged(IZoomVideoSDKVideoHelper *pVideoHelper,
                                          IVideoSDKVector<IZoomVideoSDKUser *> *userList){};

    /// \brief Triggered when user audio status changed.
    /// \param pAudioHelper is the pointer to audio helper object, see \link IZoomVideoSDKAudioHelper \endlink.
    /// \param userList is the pointer to user object list.
    virtual void onUserAudioStatusChanged(IZoomVideoSDKAudioHelper *pAudioHelper,
                                          IVideoSDKVector<IZoomVideoSDKUser *> *userList){};

    /// \brief Triggered when user Share status changed.
    /// \param pShareHelper is the pointer to share helper object, see \link IZoomVideoSDKShareHelper \endlink.
    /// \param pUser is the pointer to user object.
    /// \param status is the share status of the user.
    virtual void onUserShareStatusChanged(IZoomVideoSDKShareHelper *pShareHelper,
                                          IZoomVideoSDKUser *pUser,
                                          ZoomVideoSDKShareStatus status){};

    /// \brief Triggered when user live stream status changed
    /// \param pLiveStreamHelper is the pointer to live stream helper object, see \link IZoomVideoSDKLiveStreamHelper
    /// \endlink. \param status is the current status of live stream.
    virtual void onLiveStreamStatusChanged(IZoomVideoSDKLiveStreamHelper *pLiveStreamHelper,
                                           ZoomVideoSDKLiveStreamStatus status){};

    /// \brief Triggered when chat message received.
    /// \param pChatHelper is the pointer to chat helper object, see \link IZoomVideoSDKChatHelper \endlink.
    /// \param messageItem is the pointer to message object
    virtual void onChatNewMessageNotify(IZoomVideoSDKChatHelper *pChatHelper, IZoomVideoSDKChatMessage *messageItem){};

    /// \brief Triggered when host changed.
    /// \param pUserHelper is the pointer to user helper object, see \link IZoomVideoSDKUserHelper \endlink.
    /// \param pUser is the pointer to user object.
    virtual void onUserHostChanged(IZoomVideoSDKUserHelper *pUserHelper, IZoomVideoSDKUser *pUser){};

    /// \brief Triggered when active audio user changed.
    /// \param pAudioHelper is the pointer to audio helper object, see \link IZoomVideoSDKAudioHelper \endlink.
    /// \param list is the pointer to user object list.
    virtual void onUserActiveAudioChanged(IZoomVideoSDKAudioHelper *pAudioHelper,
                                          IVideoSDKVector<IZoomVideoSDKUser *> *list){};

    /// \brief Triggered when session needs password.
    /// \param handler is the pointer to password handler object, see \link IZoomVideoSDKPasswordHandler \endlink.
    virtual void onSessionNeedPassword(IZoomVideoSDKPasswordHandler *handler){};

    /// \brief Triggered when password is wrong.
    /// \param handler is the pointer to password handler object, see \link IZoomVideoSDKPasswordHandler \endlink.
    virtual void onSessionPasswordWrong(IZoomVideoSDKPasswordHandler *handler){};

    /// \brief Triggered when mixed audio raw data received.
    /// \param data_ is the pointer to audio raw data, see \link AudioRawData \endlink.
    virtual void onMixedAudioRawDataReceived(AudioRawData *data_){};

    /// \brief Triggered when one way audio raw data received.
    /// \param data_ is the pointer to audio raw data, see \link AudioRawData \endlink.
    /// \param pUser is the pointer to user object, see \link IZoomVideoSDKUser \endlink.
    virtual void onOneWayAudioRawDataReceived(AudioRawData *data_, IZoomVideoSDKUser *pUser){};

    /// \brief Triggered when share audio data received.
    /// \param data_ is the pointer to audio raw data, see \link AudioRawData \endlink.
    virtual void onSharedAudioRawDataReceived(AudioRawData *data_){};

    /// \brief Triggered when user get session manager role.
    /// \param pUser is the pointer to user object, see \link IZoomVideoSDKUser \endlink.
    virtual void onUserManagerChanged(IZoomVideoSDKUser *pUser){};

    /// \brief Triggered when user name changed.
    /// \param pUser is the pointer to user object, see \link IZoomVideoSDKUser \endlink.
    virtual void onUserNameChanged(IZoomVideoSDKUser *pUser){};

    /// \brief Callback for when the current user is granted camera control access.
    /// Once the current user sends the camera control request, this callback will be triggered with the result of the
    /// request. \param pUser the pointer to the user who received the request, see \link IZoomVideoSDKUser \endlink.
    /// \param isApproved the result of the camera control request
    virtual void onCameraControlRequestResult(IZoomVideoSDKUser *pUser, bool isApproved){};

    /// \brief Callback interface for when the current user has received a camera control request.
    /// This will be triggered when another user requests control of the current user��s camera.
    /// \param pUser is the pointer to the user who sent the request, see \link IZoomVideoSDKUser \endlink.
    /// \param requestType the request type, see \link ZoomVideoSDKCameraControlRequestType \endlink.
    /// \param pCameraControlRequestHandler the pointer to the helper instance of the camera controller, see \link
    /// IZoomVideoSDKCameraControlRequestHandler \endlink.
    virtual void onCameraControlRequestReceived(
        IZoomVideoSDKUser *pUser,
        ZoomVideoSDKCameraControlRequestType requestType,
        IZoomVideoSDKCameraControlRequestHandler *pCameraControlRequestHandler){};

    /// \brief Callback for when a message has been received from the command channel.
    ///        Once the command channel is active, this callback is triggered each time a message has been received.
    /// \param pSender The user who sent the command, see \link IZoomVideoSDKUser \endlink.
    /// \param strCmd Received command.
    virtual void onCommandReceived(IZoomVideoSDKUser *sender, const zchar_t *strCmd)
    {
        printf("command received\n");
    };

    /// \brief Callback for when the command channel is ready to be used.
    ///        After the SDK attempts to establish a connection for the command channel upon joining a session,
    ///        this callback will be triggered once the connection attempt has completed.
    /// \param isSuccess true: success, command channel is ready to be used.
    ///        false: Failure, command channel was unable to connect.
    virtual void onCommandChannelConnectResult(bool isSuccess)
    {
        printf("command channel connect: %d\n", isSuccess);
        if (isSuccess && video_index >= 0)
        {
            IZoomVideoSDKCmdChannel *cmdChannel = video_sdk_obj->getCmdChannel();
            cmdChannel->sendCommand(NULL, "test");
        }
    };

    /// \brief Triggered when call Out status changed.
    virtual void onInviteByPhoneStatus(PhoneStatus status, PhoneFailedReason reason){};

    /// \brief Callback for when the cloud recording status has changed (e.g. paused, stopped, resumed).
    /// \param status cloud recording status defined in \link RecordingStatus \endlink.
    virtual void onCloudRecordingStatus(RecordingStatus status){};

    /// \brief Triggered when host ask you to unmute.
    virtual void onHostAskUnmute(){};

    virtual void onUserShareStatusChanged(IZoomVideoSDKShareHelper *pShareHelper, IZoomVideoSDKUser *pUser, ZoomVideoSDKShareStatus status, ZoomVideoSDKShareType type) {}

    virtual void onMultiCameraStreamStatusChanged(ZoomVideoSDKMultiCameraStreamStatus status, IZoomVideoSDKUser *pUser, IZoomVideoSDKRawDataPipe *pVideoPipe) {}

    virtual void onMicSpeakerVolumeChanged(unsigned int micVolume, unsigned int speakerVolume) {}

    virtual void onAudioDeviceStatusChanged(ZoomVideoSDKAudioDeviceType type, ZoomVideoSDKAudioDeviceStatus status) {}

    virtual void onTestMicStatusChanged(ZoomVideoSDK_TESTMIC_STATUS status) {}

    virtual void onSelectedAudioDeviceChanged() {}

    virtual void onLiveTranscriptionStatus(ZoomVideoSDKLiveTranscriptionStatus status) {}

    /// \brief live transcription message received callback.
    /// \param ltMsg: an object pointer to the live transcription message content.
    /// \param pUser the pointer to the user who speak the message, see \link IZoomVideoSDKUser \endlink.
    /// \param type: the live transcription operation type. For more details, see \link ZoomVideoSDKLiveTranscriptionOperationType \endlink.
    virtual void onLiveTranscriptionMsgReceived(const zchar_t *ltMsg, IZoomVideoSDKUser *pUser, ZoomVideoSDKLiveTranscriptionOperationType type) {}

    /// \brief The translation message error callback.
    /// \param speakingLanguage: an object of the spoken message language.
    /// \param transcriptLanguageId: an object of the message language you want to translate.
    virtual void onLiveTranscriptionMsgError(ILiveTranscriptionLanguage *spokenLanguage, ILiveTranscriptionLanguage *transcriptLanguage) {}
};

void joinVideoSDKSession(std::string &session_name, std::string &session_psw, std::string &session_token)
{
    video_sdk_obj = CreateZoomVideoSDKObj();
    ZoomVideoSDKInitParams init_params;
    init_params.domain = "https://go.zoom.us";
    init_params.enableLog = true;
    init_params.logFilePrefix = "zoom_v-sdk_linux_bot";
    init_params.videoRawDataMemoryMode = ZoomVideoSDKRawDataMemoryModeHeap;
    init_params.shareRawDataMemoryMode = ZoomVideoSDKRawDataMemoryModeHeap;
    init_params.audioRawDataMemoryMode = ZoomVideoSDKRawDataMemoryModeHeap;
    init_params.enableIndirectRawdata = false;

    ZoomVideoSDKErrors err = video_sdk_obj->initialize(init_params);
    if (err != ZoomVideoSDKErrors_Success)
    {
        printf("Failed to create SDK instance, error code: %d\n", err);
    }
    IZoomVideoSDKDelegate *listener = new ZoomVideoSDKDelegate();
    video_sdk_obj->addListener(dynamic_cast<IZoomVideoSDKDelegate *>(listener));
    ZoomVideoSDKSessionContext session_context;
    session_context.sessionName = session_name.c_str();
    session_context.sessionPassword = session_psw.c_str();
    session_context.userName = "Rocording Bot";
    session_context.token = session_token.c_str();
    session_context.videoOption.localVideoOn = (video_index >= 0); // if no video source arg, turn it off.
    session_context.audioOption.connect = false;
    session_context.audioOption.mute = true;
    session_context.externalVideoSource = fileVideoSource;
    IZoomVideoSDKSession *session = NULL;
    if (video_sdk_obj)
    {
        auto video_helper = video_sdk_obj->getVideoHelper();
        auto cam_list = video_helper->getCameraList();
        int cam_count = cam_list->GetCount();
        for (int i = 0; i < cam_count; i++)
        {
            IZoomVideoSDKCameraDevice *cam = cam_list->GetItem(i);
            const zchar_t *cam_id = cam->getDeviceId();
            const zchar_t *cam_name = cam->getDeviceName();
            printf("cam, id: %s, name: %s\n", cam_id, cam_name);
        }
        if (video_index >= 0 && video_index < cam_count)
        {
            const zchar_t *cam_id = cam_list->GetItem(video_index)->getDeviceId();
            const zchar_t *cam_name = cam_list->GetItem(video_index)->getDeviceName();
            video_helper->selectCamera(cam_id);
            printf("Camera selected: %s\n", cam_name);
        }
        session = video_sdk_obj->joinSession(session_context);
    }
}

gboolean timeout_callback(gpointer data)
{
    return TRUE;
}

void my_handler(int s)
{
    printf("\nCaught signal %d\n", s);
    video_sdk_obj->leaveSession(false);
    printf("Leaving session.\n");
}

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        char *arg = argv[1];
        int i = *arg - '0';
        if (i >= 0 && i <= 9)
        {
            video_index = i;
        }
        else
        {
            // if (strcmp(arg, "--record") == 0 || strcmp(arg, "-r") == 0)
            // {
            // is_to_record = true;
            // }
        }
    }
    else
        is_to_record = true;

    printf("video_index is set: %d\n", video_index);

    std::string self_dir = getSelfDirPath();
    printf("self path: %s\n", self_dir.c_str());
    self_dir.append("/config.json");

    std::ifstream t(self_dir.c_str());
    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    std::string buffer(size, ' ');
    t.seekg(0);
    t.read(&buffer[0], size);

    std::string session_name, session_psw, session_token;

    std::stringstream ss;
    if (char *session_token_char = std::getenv("VSDK_TOKEN"))
    {
        ss.str(session_token_char);
        session_token = ss.str();
        printf("session_token loaded: %s\n", session_token.c_str());
    }
    //     std::cout << "[ENV] session_name: " <<  << '\n';
    // if (session_psw = std::stringstream.str(std::getenv("VSDK_SESSION_PSW")))
    //     std::cout << "[ENV] session_psw: " << session_psw << '\n';
    // if (session_token = std::stringstream.str(std::getenv("VSDK_SESSION_TOKEN")))
    //     std::cout << "[ENV] session_token: " << session_token << '\n';

    do
    {
        Json config_json;
        try
        {
            config_json = Json::parse(buffer);
            printf("Config loaded: %s\n", buffer.c_str());
        }
        catch (Json::parse_error &ex)
        {
            break;
        }

        if (config_json.is_null())
        {
            break;
        }

        Json json_name = config_json["session_name"];
        Json json_psw = config_json["session_psw"];
        Json json_token = config_json["token"];
        if (!json_name.is_null())
        {
            session_name = json_name.get<std::string>();
            // printf("config session_name: %s\n", session_name.c_str());
        }
        if (!json_psw.is_null())
        {
            session_psw = json_psw.get<std::string>();
            // printf("config session_psw: %s\n", session_psw.c_str());
        }
        printf("session_token size: %lu.\n", session_token.size());
        if (!json_token.is_null() && session_token.size() == 0)
        {
            session_token = json_token.get<std::string>();
            // printf("config session_token: %s\n", session_token.c_str());
        }
    } while (false);

    printf("token: %s\n", session_token.c_str());

    if (session_name.size() == 0 || session_token.size() == 0)
    {
        return 0;
    }

    // printf("Begin to join: %s\n", self_dir.c_str());
    printf("Begin to join session...\n");
    joinVideoSDKSession(session_name, session_psw, session_token);

    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = my_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);

    loop = g_main_loop_new(NULL, FALSE);

    // add source to default context
    g_timeout_add(100, timeout_callback, loop);
    g_main_loop_run(loop);
    return 0;
}