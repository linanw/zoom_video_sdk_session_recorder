/*!
* \file zoom_video_sdk_cloud_record_handler_interface.h
* \brief cloud record handler interface.
*
*/

#ifndef _ZOOM_VIDEO_SDK_CLOUD_RECORD_HANDLER_INTERFACE_H_
#define _ZOOM_VIDEO_SDK_CLOUD_RECORD_HANDLER_INTERFACE_H_
#include "zoom_video_sdk_def.h"

BEGIN_ZOOM_VIDEO_SDK_NAMESPACE

enum ConsentType
{
	ConsentType_Invalid,         ///< Invalid type.
	ConsentType_Traditional,     ///< In this case, 'accept' means agree to be recorded to gallery and speaker mode, 'decline' means leave session.
	ConsentType_Individual,      ///< In this case, 'accept' means agree to be recorded to a separate file, 'decline' means stay in session and can't be recorded.
};

/// \brief cloud record handle interface.
///
class IZoomVideoSDKRecordingConsentHandler
{
public:
	virtual ~IZoomVideoSDKRecordingConsentHandler(){}
	virtual bool accept() = 0;
	virtual bool decline() = 0;
	virtual ConsentType getConsentType() = 0;
};
END_ZOOM_VIDEO_SDK_NAMESPACE
#endif