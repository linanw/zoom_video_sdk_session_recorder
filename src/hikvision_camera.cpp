#include "hikvision_camera.h"
#include <string.h>
#include <stdio.h>
#include <thread>
#include <chrono>
#include <iostream>

HikvisionCamera::HikvisionCamera()
{
    NET_DVR_Init();
    // login
    _struLoginInfo.bUseAsynLogin = false;

    _struLoginInfo.wPort = 8000;
    memcpy(_struLoginInfo.sDeviceAddress, "192.168.72.145", NET_DVR_DEV_ADDRESS_MAX_LEN);
    memcpy(_struLoginInfo.sUserName, "admin", NAME_LEN);
    memcpy(_struLoginInfo.sPassword, "Pass@word1", NAME_LEN);

    _lUserID = NET_DVR_Login_V40(&_struLoginInfo, &_struDeviceInfoV40);

    if (_lUserID < 0)
    {
        printf("Remote Camera Login error, %d\n", NET_DVR_GetLastError());
    }
}

HikvisionCamera::~HikvisionCamera()
{
    NET_DVR_Logout_V30(_lUserID);
    NET_DVR_Cleanup();
}

int HikvisionCamera::Demo_Capture()
{
    long lUserID;
    // login
    NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
    NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = {0};
    struLoginInfo.bUseAsynLogin = false;

    struLoginInfo.wPort = 8000;
    memcpy(struLoginInfo.sDeviceAddress, "192.168.72.145", NET_DVR_DEV_ADDRESS_MAX_LEN);
    memcpy(struLoginInfo.sUserName, "admin", NAME_LEN);
    memcpy(struLoginInfo.sPassword, "Pass@word1", NAME_LEN);

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

int HikvisionCamera::PtzControlAndStop(DWORD ptzCommand)
{
    PtzControlAndStop(ptzCommand, 200);
}

int HikvisionCamera::PtzControlAndStop(DWORD ptzCommand, int millisecondsToStop)
{
    NET_DVR_PTZControlWithSpeed_Other(_lUserID, _struDeviceInfoV40.struDeviceV30.byStartChan, ptzCommand, 0, 7);
    std::this_thread::sleep_for(std::chrono::milliseconds(millisecondsToStop));
    NET_DVR_PTZControlWithSpeed_Other(_lUserID, _struDeviceInfoV40.struDeviceV30.byStartChan, ptzCommand, 1, 7);
    return HPR_OK;
}

int HikvisionCamera::PtzControlStart(DWORD ptzCommand){
    PtzControl(ptzCommand, 0);
}

int HikvisionCamera::PtzControlStop(DWORD ptzCommand){
    PtzControl(ptzCommand, 1);
}

int HikvisionCamera::PtzControl(DWORD ptzCommand, int isStop)
{
    if(NET_DVR_PTZControlWithSpeed_Other(_lUserID, _struDeviceInfoV40.struDeviceV30.byStartChan, ptzCommand, isStop, 7))
    {
        std::cout << "PTZ Command: " << ptzCommand << " executed successfully!" << std::endl;
        return HPR_OK;
    }
    else
    {
        std::cout << "Failed to execute PTZ Command: " << ptzCommand << ", Error: " << NET_DVR_GetLastError() << std::endl;
        return HPR_ERROR;
    }
    
}

int HikvisionCamera::Infrared()
{
    // Get DVR Configuration (e.g., Device Name)
    DWORD dwReturned = 0;
    NET_DVR_INFRARE_CFG deviceCfg = { 0 };
    
    if (NET_DVR_GetDVRConfig(_lUserID, NET_DVR_GET_INFRARECFG, 1, &deviceCfg, sizeof(deviceCfg), &dwReturned))
    {
        std::cout<< "Infrared status: " << deviceCfg.byIrControlMode << std::endl;
    }
    else
    {
        std::cout << "Failed to get infrared configuration, Error: " << NET_DVR_GetLastError() << std::endl;
    }

    deviceCfg.byIrControlMode = 1;

    // Set the new focus configuration
    if (NET_DVR_SetDVRConfig(_lUserID, NET_DVR_GET_INFRARECFG, 1, &deviceCfg, sizeof(deviceCfg)))
    {
        std::cout << "Infrared status set to: " << deviceCfg.byIrControlMode << std::endl;
        return HPR_OK;
    }
    else
    {
        std::cout << "Failed to set infrared configuration, Error: " << NET_DVR_GetLastError() << std::endl;
        return HPR_ERROR;
    }
}

int HikvisionCamera::InvokeISAPI(ISAPI_METHOD method, const char* apiPath)
{
    InvokeISAPI(method, apiPath, "");
}

int HikvisionCamera::InvokeISAPI(ISAPI_METHOD method, const char* apiPath, const char* requestXml)
{
    // Prepare the input structure
    NET_DVR_XML_CONFIG_INPUT xmlInput = {0};
    xmlInput.dwSize = sizeof(NET_DVR_XML_CONFIG_INPUT);

    // Set the request URL (e.g., to retrieve device information)
    std::string requestUrl = std::string(method == ISAPI_METHOD::ISAPI_METHOD_GET ? "GET " : "PUT ") + apiPath;
    xmlInput.lpRequestUrl = (void*)requestUrl.c_str();
    xmlInput.dwRequestUrlLen = requestUrl.length();

    // No XML data is required for a GET request, so no input buffer is used
    const char* inputBuffer = requestXml;
    xmlInput.lpInBuffer = (void*)inputBuffer;
    xmlInput.dwInBufferSize = strlen(inputBuffer);

    xmlInput.dwRecvTimeOut = 5000; // 5 seconds timeout
    xmlInput.byForceEncrpt = 0; // Disable encryption
    xmlInput.byNumOfMultiPart = 0; // No multipart message
    xmlInput.byRes[30] = {0}; // Reserved

    // Prepare the output buffer for the response
    const int bufferSize = 1024 * 10; // 10KB buffer
    char* outputBuffer = new char[bufferSize];
    memset(outputBuffer, 0, bufferSize);

    NET_DVR_XML_CONFIG_OUTPUT xmlOutput = {0};
    xmlOutput.dwSize = sizeof(NET_DVR_XML_CONFIG_OUTPUT);
    xmlOutput.lpOutBuffer = outputBuffer;
    xmlOutput.dwOutBufferSize = bufferSize;
    xmlOutput.dwReturnedXMLSize = 0;
    xmlOutput.lpStatusBuffer = NULL;
    xmlOutput.dwStatusSize = 0;
    xmlOutput.byRes[32] = {0};

    // Get the device information
    if (_lUserID != -1 && NET_DVR_STDXMLConfig(_lUserID, &xmlInput, &xmlOutput))
    {
        std::cout << "Response:" << std::endl << outputBuffer << std::endl;
    }
    else
    {
        std::cout << "Error: " << NET_DVR_GetLastError() << std::endl;
    }

    // Clean up
    delete[] outputBuffer;
}