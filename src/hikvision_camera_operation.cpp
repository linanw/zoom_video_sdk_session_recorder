#include "hikvision_camera_operation.h"
#include <string.h>
#include <stdio.h>
#include <thread>
#include <chrono>

HikvisionCameraOperation::HikvisionCameraOperation()
{
    NET_DVR_Init();
    // login
    _struLoginInfo.bUseAsynLogin = false;

    _struLoginInfo.wPort = 8000;
    memcpy(_struLoginInfo.sDeviceAddress, "192.168.72.171", NET_DVR_DEV_ADDRESS_MAX_LEN);
    memcpy(_struLoginInfo.sUserName, "zoom", NAME_LEN);
    memcpy(_struLoginInfo.sPassword, "stream-rtsp", NAME_LEN);

    _lUserID = NET_DVR_Login_V40(&_struLoginInfo, &_struDeviceInfoV40);

    if (_lUserID < 0)
    {
        printf("Remote Camera Login error, %d\n", NET_DVR_GetLastError());
    }
}

HikvisionCameraOperation::~HikvisionCameraOperation()
{
    NET_DVR_Logout_V30(_lUserID);
    NET_DVR_Cleanup();
}

int HikvisionCameraOperation::Demo_Capture()
{
    long lUserID;
    // login
    NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
    NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = {0};
    struLoginInfo.bUseAsynLogin = false;

    struLoginInfo.wPort = 8000;
    memcpy(struLoginInfo.sDeviceAddress, "192.168.72.171", NET_DVR_DEV_ADDRESS_MAX_LEN);
    memcpy(struLoginInfo.sUserName, "zoom", NAME_LEN);
    memcpy(struLoginInfo.sPassword, "stream-rtsp", NAME_LEN);

    lUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);

    if (lUserID < 0)
    {
        printf("pyd1---Login error, %d\n", NET_DVR_GetLastError());
        return HPR_ERROR;
    }

    //
    NET_DVR_JPEGPARA strPicPara = {0};
    strPicPara.wPicQuality = 2;
    strPicPara.wPicSize = 0;
    int iRet;
    iRet = NET_DVR_CaptureJPEGPicture(lUserID, struDeviceInfoV40.struDeviceV30.byStartChan, &strPicPara, "./ssss.jpeg");
    if (!iRet)
    {
        printf("pyd1---NET_DVR_CaptureJPEGPicture error, %d\n", NET_DVR_GetLastError());
        return HPR_ERROR;
    }

    // logout
    NET_DVR_Logout_V30(lUserID);
    NET_DVR_Cleanup();

    return HPR_OK;
}

int HikvisionCameraOperation::ptzControl(DWORD ptzCommand)
{
    NET_DVR_PTZControlWithSpeed_Other(_lUserID, _struDeviceInfoV40.struDeviceV30.byStartChan, ptzCommand, 0, 7);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    NET_DVR_PTZControlWithSpeed_Other(_lUserID, _struDeviceInfoV40.struDeviceV30.byStartChan, ptzCommand, 1, 7);
    return HPR_OK;
}

