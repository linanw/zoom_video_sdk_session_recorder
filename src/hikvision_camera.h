#ifndef _PUBLIC_H_
#define _PUBLIC_H_
#define HPR_OK 0
#define HPR_ERROR -1

#if (defined(_WIN32)) // windows
#include <windows.h>
#endif

#include "HCNetSDK.h"
#endif

enum ISAPI_METHOD
{
    ISAPI_METHOD_GET = 0,
    ISAPI_METHOD_PUT = 1,
};

/*
LIGHT_PWRON
2
Enable light
WIPER_PWRON
3
Enable wiper
FAN_PWRON
4
Enable fan
HEATER_PWRON
5
Enable heater
AUX_PWRON1
6
Enable auxiliary
AUX_PWRON2
7
Enable auxiliary
ZOOM_IN
11
Zoom in
ZOOM_OUT
12
Zoom out
FOCUS_NEAR
13
Focus+
FOCUS_FAR
14
Focus-
IRIS_OPEN
15
Iris+
IRIS_CLOSE
16
Iris-
TILT_UP
21
Tilt up
TILT_DOWN
22
Tilt down
PAN_LEFT
23
Pan left
PAN_RIGHT
24
Pan right
UP_LEFT
25
Tilt up and pan left
UP_RIGHT
26
Tilt up and pan right
DOWN_LEFT
27
Tilt down and pan left
DOWN_RIGHT
28
Tilt down and pan right
PAN_AUTO
29
Auto pan
TILT_DOWN_ZOOM_IN 
58
Tilt down and zoom in
TILT_DOWN_ZOOM_OUT
59
Tilt down and zoom out
PAN_LEFT_ZOOM_IN
60
Pan left and zoom in
PAN_LEFT_ZOOM_OUT
61
Pan left and zoom out
PAN_RIGHT_ZOOM_IN
62
Pan right and zoom in
PAN_RIGHT_ZOOM_OUT
63
Pan right and zoom out
UP_LEFT_ZOOM_IN
64
Tilt up, pan left, and zoom in
UP_LEFT_ZOOM_OUT
65
Tilt up, pan left, and zoom out
UP_RIGHT_ZOOM_IN
66
Tilt up, pan right, and zoom in
UP_RIGHT_ZOOM_OUT
67
Tilt up, pan right, and zoom out
DOWN_LEFT_ZOOM_IN
68
Tilt down, pan left, and zoom in
DOWN_LEFT_ZOOM_OUT
69
Tilt down, pan left, and zoom out
DOWN_RIGHT_ZOOM_IN 
70
Tilt down, pan right, and zoom in
DOWN_RIGHT_ZOOM_OUT
71
Tilt down, pan right, and zoom out
TILT_UP_ZOOM_IN
72
Tilt up and zoom in
TILT_UP_ZOOM_OUT
73
Tilt up and zoom out*/
class HikvisionCamera
{
    long _lUserID;
    NET_DVR_USER_LOGIN_INFO _struLoginInfo = {0};
    NET_DVR_DEVICEINFO_V40 _struDeviceInfoV40 = {0};
public:
    HikvisionCamera();
    ~HikvisionCamera();
    int PtzControlStart(DWORD ptzCommand);
    int PtzControlStop(DWORD ptzCommand);
    int PtzControl(DWORD ptzCommand, int isStop);
    int PtzControlAndStop(DWORD ptzCommand);
    int PtzControlAndStop(DWORD ptzCommand, int millisecondsToStop);
    int Infrared();
    int getCap();
    int InvokeISAPI(ISAPI_METHOD method, const char* apiPath);
    int InvokeISAPI(ISAPI_METHOD method, const char* apiPath, const char* requestXml);
    int tiltUp();
    int panLeft();
    int tiltDown();
    // int panRight();
    // int zoomIn();
    // int zoomOut();
    // int focusNear();
    // int focusFar();
    // int irisOpen();
    // int irisClose();
    // int lightPwrOn();
    // int wiperPwrOn();
    // int fanPwrOn();
    // int heaterPwrOn();
    // int auxPwrOn1();
    // int auxPwrOn2();
    // int panAuto();
    // int tiltDownZoomIn();
    // int tiltDownZoomOut();
    // int panLeftZoomIn();
    // int panLeftZoomOut();
    // int panRightZoomIn();
    // int panRightZoomOut();
    // int upLeftZoomIn();
    // int upLeftZoomOut();
    // int upRightZoomIn();
    // int upRightZoomOut();
    // int downLeftZoomIn();
    // int downLeftZoomOut();
    // int downRightZoomIn();
    // int downRightZoomOut();
    // int tiltUpZoomIn();
    // int tiltUpZoomOut();

    static int Demo_Capture();
};