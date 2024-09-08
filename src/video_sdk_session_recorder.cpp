#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fstream>
#include <iosfwd>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <curl/curl.h>

#include "glib.h"
#include "json.hpp"
#include "helpers/zoom_video_sdk_user_helper_interface.h"
#include "zoom_video_sdk_interface.h"
#include "zoom_video_sdk_api.h"
#include "zoom_video_sdk_def.h"
#include "zoom_video_sdk_delegate.h"
#include "hikvision_camera_operation.h"

using Json = nlohmann::json;
USING_ZOOM_VIDEO_SDK_NAMESPACE
IZoomVideoSDK *video_sdk_obj;
GMainLoop *loop;
std::string FINALJWTToken;
std::string claw_control_url;

std::string getSelfDirPath()
{
    char dest[PATH_MAX];
    memset(dest, 0, sizeof(dest)); // readlink does not null terminate!
    if (readlink("/proc/self/exe", dest, PATH_MAX) == -1)
    {
        return "";
    }
    char *tmp = strrchr(dest, '/');
    if (tmp)
        *tmp = 0;
    printf("getpath\n");
    return std::string(dest);
}

void joinVideoSDKSession(std::string &session_name, std::string &session_psw, std::string &session_token)
{
    video_sdk_obj = CreateZoomVideoSDKObj();
    ZoomVideoSDKInitParams init_params;
    init_params.domain = "https://go.zoom.us";
    init_params.enableLog = true;
    init_params.logFilePrefix = "zoom_videosdk_demo";
    init_params.videoRawDataMemoryMode = ZoomVideoSDKRawDataMemoryModeHeap;
    init_params.shareRawDataMemoryMode = ZoomVideoSDKRawDataMemoryModeHeap;
    init_params.audioRawDataMemoryMode = ZoomVideoSDKRawDataMemoryModeHeap;
    init_params.enableIndirectRawdata = false;

    ZoomVideoSDKErrors err = video_sdk_obj->initialize(init_params);
    if (err != ZoomVideoSDKErrors_Success)
    {
        return;
    }
    IZoomVideoSDKDelegate *listener = new ZoomVideoSDKDelegate(loop, video_sdk_obj, claw_control_url);
    video_sdk_obj->addListener(dynamic_cast<IZoomVideoSDKDelegate *>(listener));
    ZoomVideoSDKSessionContext session_context;
    session_context.sessionName = session_name.c_str();
    session_context.sessionPassword = session_psw.c_str();
    session_context.userName = "Linux Bot";
    session_context.token = session_token.c_str();
    session_context.videoOption.localVideoOn = true;
    session_context.audioOption.connect = true;
    session_context.audioOption.mute = true;
    session_context.sessionIdleTimeoutMins = 0;
    IZoomVideoSDKSession *session = NULL;
    if (video_sdk_obj)
        session = video_sdk_obj->joinSession(session_context);
}

void startLiveStreaming()
{
    // Get the IZoomVideoSDKLiveStreamHelper to perform livestream actions.
    IZoomVideoSDKLiveStreamHelper *pLiveStreamHelper = video_sdk_obj->getLiveStreamHelper();

    // Check if live stream can start.
    if (pLiveStreamHelper->canStartLiveStream() == ZoomVideoSDKErrors_Success)
    {

        const zchar_t *strStreamUrl = "rtmp://a.rtmp.youtube.com/live2";
        const zchar_t *strKey = "6kft-yswg-uf68-0sj1-49gm";
        const zchar_t *strBroadcastUrl = "https://www.youtube.com/watch?v=oCJdsKSrTHo";
        // Call startLiveStream to begin live stream.
        int err = pLiveStreamHelper->startLiveStream(strStreamUrl, strKey, strBroadcastUrl);

        if (err == ZoomVideoSDKErrors_Success)
        {
            // Live stream successfully began.
            printf(" Live stream successfully began.\n");
        }
        else
        {
            // Live stream could not start.

            printf(" Live stream could not start.\n");
        }
    }
}

void startCommandChannel()
{

    //     IZoomVideoSDKCmdChannel* commandChannel = video_sdk_obj->getCmdChannel();
    //   commandChannel->sendCommand(NULL,"init");
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{

    // TODO dreamtcs improve if we have time
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    std::string response = (char *)contents;
    std::string response2 = response.substr(14, response.length());
    FINALJWTToken = response2.substr(0, response2.length() - 2);
    return size * nmemb;
}

int getJWTToken(std::string remote_url, std::string session_name)
{
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    char *json = NULL;
    struct curl_slist *headers = NULL;

    curl = curl_easy_init();
    if (curl)
    {
        // curl_easy_setopt(curl, CURLOPT_URL, "https://asdc.cc/video/");
        curl_easy_setopt(curl, CURLOPT_URL, remote_url.c_str());

        // buffer size
        curl_easy_setopt(curl, CURLOPT_BUFFERSIZE, 120000L);
        // region for callback
        //  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        //  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        //  res = curl_easy_perform(curl);
        //  curl_easy_cleanup(curl);
        // std::cout << readBuffer << std::endl;
        // region for call back

        // headers
        headers = curl_slist_append(headers, "Expect:");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // std::string json = "{\"sessionName\":\"herochun6871\",\"role\":1,\"user_identity\":\"user_identity6871\",\"session_key\":\"session_key6871\"}";
        std::string json = "{\"sessionName\":\"" + session_name + "\",\"role\":1,\"user_identity\":\"user_identity6871\",\"session_key\":\"session_key6871\"}";
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());
        // callback
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        // perform
        res = curl_easy_perform(curl);
        std::cout << readBuffer << std::endl;
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* always cleanup */
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    return 0;
}

gboolean timeout_callback(gpointer data)
{
    return TRUE;
}

void stdin_signal_handler(int s)
{
    printf("\nCaught signal %d\n", s);
    video_sdk_obj->leaveSession(false);
    printf("Leaving session.\n");
}

int main(int argc, char *argv[])
{
    std::string self_dir = getSelfDirPath();
    printf("self path: %s\n", self_dir.c_str());
    self_dir.append("/config.json");

    std::ifstream t(self_dir.c_str());
    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    std::string buffer(size, ' ');
    t.seekg(0);
    t.read(&buffer[0], size);

    std::string session_name, session_psw, session_token, local_url, remote_url;
    do
    {
        Json config_json;
        try
        {
            config_json = Json::parse(buffer);
            printf("config all_content: %s\n", buffer.c_str());
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
        Json json_local_url = config_json["local_url_for_serial_control"];
        Json json_remote_url = config_json["remote_url_for_jwt_token"];
        if (!json_name.is_null())
        {
            session_name = json_name.get<std::string>();
            printf("config session_name: %s\n", session_name.c_str());
        }
        if (!json_psw.is_null())
        {
            session_psw = json_psw.get<std::string>();
            printf("config session_psw: %s\n", session_psw.c_str());
        }
        if (!json_token.is_null())
        {
            session_token = json_token.get<std::string>();
            printf("config session_token: %s\n", session_token.c_str());
        }

        if (!json_local_url.is_null())
        {
            local_url = json_local_url.get<std::string>();
            claw_control_url = local_url;
            printf("config local_url: %s\n", local_url.c_str());
        }

        if (!json_remote_url.is_null())
        {
            remote_url = json_remote_url.get<std::string>();
            printf("config remote_url: %s\n", remote_url.c_str());
        }
    } while (false);

    if (session_name.size() == 0 || session_token.size() == 0)
    {
        return 0;
    }

    HikvisionCameraOperation::Demo_Capture();

    // getJWTToken(remote_url,session_name);
    printf("begin to join: %s\n", self_dir.c_str());
    joinVideoSDKSession(session_name, session_psw, session_token);

    // joinVideoSDKSession(session_name, session_psw, FINALJWTToken);
    // startCommandChannel();
    // startLiveStreaming();
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = stdin_signal_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);

    loop = g_main_loop_new(NULL, FALSE);

    // add source to default context
    g_timeout_add(100, timeout_callback, loop);
    g_main_loop_run(loop);
    return 0;
}
