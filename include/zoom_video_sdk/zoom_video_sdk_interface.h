/*!
* \file zoom_video_sdk_interface.h
* \brief zoom video sdk interface.
*
*/

#ifndef _ZOOM_VIDEO_SDK_INTERFACE_H_
#define _ZOOM_VIDEO_SDK_INTERFACE_H_
#include "zoom_video_sdk_def.h"

BEGIN_ZOOM_VIDEO_SDK_NAMESPACE
class IZoomVideoSDKSession;
class IZoomVideoSDKDelegate;
class IZoomVideoSDKAudioHelper;
class IZoomVideoSDKVideoHelper;
class IZoomVideoSDKUserHelper;
class IZoomVideoSDKShareHelper;
class IZoomVideoSDKRecordingHelper;
class IZoomVideoSDKLiveStreamHelper;
class IZoomVideoSDKChatHelper;
class IZoomVideoSDKCmdChannel;
class IZoomVideoSDKPhoneHelper;
class IZoomVideoSDKAudioSettingHelper;
class IZoomVideoSDKTestAudioDeviceHelper;
class IZoomVideoSDKNetworkConnectionHelper;
class IZoomVideoSDKCRCHelper;
class IZoomVideoSDKVideoSettingHelper;
class IZoomVideoSDKShareSettingHelper;
class IZoomVideoSDKLiveTranscriptionHelper;
/// \brief Zoom Video SDK API manager. Main singleton object that controls the video session creation, event callbacks and other main features of video SDK.
///
class IZoomVideoSDK
{
public:
	virtual ~IZoomVideoSDK(){}

	/// \brief Initialize the Zoom Video SDK with the appropriate parameters in the \link ZoomVideoSDKInitParams \endlink structure.
	/// \param [out] params Parameters for init zoom video sdk. For more details, see \link ZoomVideoSDKInitParams \endlink structure.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
    virtual ZoomVideoSDKErrors initialize(ZoomVideoSDKInitParams& params) = 0;
	
	/// \brief Clean up ZOOM Video SDK.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors cleanup() = 0;
	
	/// \brief Add a listener for session events
	/// \param listener A listener class that groups together all the callbacks related to a session.
	virtual void addListener(IZoomVideoSDKDelegate* listener) = 0;
	
	/// \brief Remove a listener for session events
	/// \param listener A listener class that groups together all the callbacks related to a session.
	virtual void removeListener(IZoomVideoSDKDelegate* listener) = 0;
	
	/// \brief Call this method to join a session with the appropriate ZoomVideoSDKSessionContext parameters. 
	/// When seccessful, the SDK will attempt to join a session. Use the callbacks in the listener to confirm whether the SDK actually joined.
	/// \param [out] params The parameter is used to join session. For more details, see \link ZoomVideoSDKSessionContext \endlink structure. 
	/// \return If the function succeeds, the return value is the pointer to IZoomVideoSDKSession object.
	///Otherwise NULL. To get extended error information, see \link IZoomVideoSDKSession \endlink enum.
	virtual IZoomVideoSDKSession* joinSession(ZoomVideoSDKSessionContext& params) = 0;
	
	/// \brief Call this method to leave a session previously joined through joinSession method call.
	/// When successful, the SDK will attempt to leave a session. Use the callbacks in the listener to confirm whether the SDK actually left.
	/// \param end True if the host should end the entire session, or False if the host should just leave the session. 
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors leaveSession(bool end) = 0;
	
	/// \brief Returns the current session information.
	/// \return If the function succeeds, the return value is the pointer to IZoomVideoSDKSession object.
	///Otherwise NULL.
	virtual IZoomVideoSDKSession* getSessionInfo() = 0;
	
	/// \brief Check if there is an active session between participants.
	/// \return True if there is; False if not
	virtual bool isInSession() = 0;
	
	/// \brief Returns the sdk version
	/// \return If the function succeeds, the return value is sdk version. Otherwise returns NULL.
	virtual const zchar_t* getSDKVersion() = 0;

	/// \brief Exports a log file to local disk.
	/// \return If the function succeeds, the return value is the exported log file path. 
	virtual const zchar_t* exportLog() = 0;

	/// \brief Cleans all exported logs.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success. 
	///Otherwise the function fails. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors cleanAllExportedLogs() = 0;
	
	/// \brief Returns an instance to manage audio controls related to the current video SDK session.
	/// \return If the function succeeds, the return value is the audio helper object. Otherwise returns NULL. For more details, see \link IZoomVideoSDKAudioHelper \endlink.
	virtual IZoomVideoSDKAudioHelper* getAudioHelper() = 0;
	
	/// \brief Returns an instance to manage cameras and video during a video SDK session.
	/// \return If the function succeeds, the return value is the video helper object. Otherwise returns NULL. For more details, see \link IZoomVideoSDKVideoHelper \endlink.
	virtual IZoomVideoSDKVideoHelper* getVideoHelper() = 0;
	
	/// \brief Returns an instance to manage cloud recordings during a video SDK session.
	/// \return If the function succeeds, the return value is the recording helper object. Otherwise returns NULL. For more details, see \link IZoomVideoSDKVideoHelper \endlink.
	virtual IZoomVideoSDKRecordingHelper* getRecordingHelper() = 0;

	/// \brief Returns an instance to manage users present in a video SDK session.
	/// \return If the function succeeds, the return value is the user helper object. Otherwise returns NULL. For more details, see \link IZoomVideoSDKUserHelper \endlink.
	virtual IZoomVideoSDKUserHelper*  getUserHelper() = 0;
	
	/// \brief Returns an instance to manage screen sharing during a video SDK session.
	/// \return If the function succeeds, the return value is the share helper object. Otherwise returns NULL. For more details, see \link IZoomVideoSDKShareHelper \endlink.
	virtual IZoomVideoSDKShareHelper* getShareHelper() = 0;
	
	/// \brief Returns an instance to manage live streaming during a video SDK session.
	/// \return If the function succeeds, the return value is the live stream helper object. Otherwise returns NULL. For more details, see \link IZoomVideoSDKLiveStreamHelper \endlink.
	virtual IZoomVideoSDKLiveStreamHelper* getLiveStreamHelper() = 0;


	/// \brief Returns an instance to manage phone invitations during a video SDK session.
	/// \return If the function succeeds, the return value is the phone helper object. Otherwise returns NULL. For more details, see \link IZoomVideoSDKPhoneHelper \endlink.
	virtual IZoomVideoSDKPhoneHelper* getPhoneHelper() = 0;
	
	/// \brief Returns an instance to send and receive chat messages within video SDK session participants.
	/// \return If the function succeeds, the return value is the chat helper object. Otherwise returns NULL. For more details, see \link IZoomVideoSDKChatHelper \endlink.
	virtual IZoomVideoSDKChatHelper* getChatHelper() = 0;
	
	/// \brief Returns an instance to use command channel features during a video SDK session.
	/// \return If the function succeeds, the return value is the command channel object. Otherwise returns NULL. For more details, see \link IZoomVideoSDKCmdChannel \endlink.
	virtual IZoomVideoSDKCmdChannel* getCmdChannel() = 0;

	/// \brief Get audio setting helper object.
	/// \return If the function succeeds, the return value is the audio setting helper object. Otherwise returns NULL. For more details, see \link IZoomVideoSDKSettingHelper \endlink.
	virtual IZoomVideoSDKAudioSettingHelper* getAudioSettingHelper() = 0;

	/// \brief Get audio setting helper object.
	/// \return If the function succeeds, the return value is the audio device test helper object. Otherwise returns NULL. For more details, see \link IZoomVideoSDKTestAudioDeviceHelper \endlink.
	virtual IZoomVideoSDKTestAudioDeviceHelper* GetAudioDeviceTestHelper() = 0;

	/// \brief Get network connection helper object.
	/// \return If the function succeeds, the return value is the network connection helper object. Otherwise returns NULL. For more details, see \link IZoomVideoSDKNetworkConnectionHelper \endlink.
	virtual IZoomVideoSDKNetworkConnectionHelper* getNetworkConnectionHelper() = 0;

	/// \brief Get CRC helper object.
	/// \return If the function succeeds, the return value is the CRC helper object. Otherwise returns NULL. For more details, see \link IZoomVideoSDKCRCHelper
	virtual IZoomVideoSDKCRCHelper* getCRCHelper() = 0;

	/// \brief Get video setting helper object.
	/// \return If the function succeeds, the return value is the video setting helper object. Otherwise returns NULL. For more details, see \link IZoomVideoSDKVideoSettingHelper \endlink.
	virtual IZoomVideoSDKVideoSettingHelper* getVideoSettingHelper() = 0;

	/// \brief Get share setting helper object.
	/// \return If the function succeeds, the return value is the share setting helper object. Otherwise returns NULL. For more details, see \link IZoomVideoSDKShareSettingHelper \endlink.
	virtual IZoomVideoSDKShareSettingHelper* getShareSettingHelper() = 0;

	/// \brief Get live transcription helper object.
	/// \return If the function succeeds, the return value is the live transcription helper object. Otherwise returns NULL. For more details, see \link IZoomVideoSDKLiveTranscriptionHelper \endlink.
	virtual IZoomVideoSDKLiveTranscriptionHelper* getLiveTranscriptionHelper() = 0;
};
END_ZOOM_VIDEO_SDK_NAMESPACE
#endif
