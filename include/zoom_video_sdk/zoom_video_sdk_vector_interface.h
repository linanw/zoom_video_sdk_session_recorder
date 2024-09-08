/*!
* \file zoom_video_sdk_vector_interface.h
* \brief vector interface.
*
*/

#ifndef _ZOOM_VIDEO_SDK_VECTOR_INTERFACE_H_
#define _ZOOM_VIDEO_SDK_VECTOR_INTERFACE_H_
#include "zoom_video_sdk_def.h"

BEGIN_ZOOM_VIDEO_SDK_NAMESPACE
/// \brief SDK defined vector interface.
///
template<class T>
class IVideoSDKVector
{
public:
	virtual ~IVideoSDKVector(){};
	virtual int GetCount() = 0;
	virtual T   GetItem(int index) = 0;
};
END_ZOOM_VIDEO_SDK_NAMESPACE
#endif