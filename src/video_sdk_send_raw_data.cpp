#include <fstream>
#include "json.hpp"
#include "zoom_video_sdk_api.h"
#include "zoom_video_sdk_delegate.h"
#include "opencv_video_source.h"

using Json = nlohmann::json;
USING_ZOOM_VIDEO_SDK_NAMESPACE
IZoomVideoSDK *video_sdk_obj;
GMainLoop *loop;

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
    IZoomVideoSDKDelegate *listener = new ZoomVideoSDKDelegate(loop, video_sdk_obj);
    OpenCVVideoSource *openCVVideoSource = new OpenCVVideoSource();
    video_sdk_obj->addListener(dynamic_cast<IZoomVideoSDKDelegate *>(listener));
    ZoomVideoSDKSessionContext session_context;
    session_context.sessionName = session_name.c_str();
    session_context.sessionPassword = session_psw.c_str();
    session_context.userName = "Video Sender";
    session_context.token = session_token.c_str();
    session_context.videoOption.localVideoOn = true;
    session_context.audioOption.connect = true;
    session_context.audioOption.mute = true;
    session_context.sessionIdleTimeoutMins = 0;
    session_context.externalVideoSource = openCVVideoSource;
    IZoomVideoSDKSession *session = NULL;
    if (video_sdk_obj)
        session = video_sdk_obj->joinSession(session_context);
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

    std::string session_name, session_psw, session_token;
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
    } while (false);

    if (session_name.size() == 0 || session_token.size() == 0)
    {
        return 0;
    }

    // Response user input Ctrl+C
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = stdin_signal_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    // Setup main loop
    loop = g_main_loop_new(NULL, FALSE);
    g_timeout_add(100, timeout_callback, loop);

    printf("begin to join: %s\n", self_dir.c_str());
    joinVideoSDKSession(session_name, session_psw, session_token);
    
    // start main loop
    g_main_loop_run(loop);
    return 0;
}
