#ifndef _ZOOM_VIDEO_SDK_PLATFORM_H_
#define _ZOOM_VIDEO_SDK_PLATFORM_H_

#if defined(WIN32) //windows
#if defined(ZOOM_VIDEO_SDK_IMPLEMENTATION)
#define ZOOM_VIDEO_SDK_EXPORT __declspec(dllexport)
#else
#define ZOOM_VIDEO_SDK_EXPORT __declspec(dllimport)
#endif
#else //other platform
#define ZOOM_VIDEO_SDK_EXPORT __attribute__((visibility("default")))
#endif //defined(WIN32)

#if defined(WIN32)
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
typedef wchar_t zchar_t;
#else
#include <stdint.h>
typedef char zchar_t;
#endif// defined(WIN32)

#endif//#define _ZOOM_VIDEO_SDK_PLATFORM_H_