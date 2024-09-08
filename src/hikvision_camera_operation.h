#ifndef _PUBLIC_H_
#define _PUBLIC_H_
#define HPR_OK 0
#define HPR_ERROR -1

#if (defined(_WIN32)) // windows
#include <windows.h>
#endif

#include "HCNetSDK.h"
#endif
class HikvisionCameraOperation
{
public:
    static int Demo_Capture();
};