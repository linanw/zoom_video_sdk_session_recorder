/*!
* \file zoom_video_sdk_user_helper_interface.h
* \brief user helper
*
*/

#ifndef _ZOOM_VIDEO_SDK_USER_HELPER_INTERFACE_H_
#define _ZOOM_VIDEO_SDK_USER_HELPER_INTERFACE_H_
#include "zoom_video_sdk_def.h"
#include "zoom_video_sdk_vector_interface.h"

BEGIN_ZOOM_VIDEO_SDK_NAMESPACE
/*! \struct ZoomVideoSDKVideoStatus
	\brief Video status information on the user's video device (the detected compatible video camera device), and status (whether video is turned on or off).
*/
typedef struct
{
	bool isHasVideoDevice;	///<Determine if the user's device has a compatible camera.
	bool isOn;	///<Determine if the camera is turned on.
}ZoomVideoSDKVideoStatus;

typedef enum
{
	ZoomVideoSDKNetwork_None,
	ZoomVideoSDKNetwork_Bad,
	ZoomVideoSDKNetwork_Normal,
	ZoomVideoSDKNetwork_Good,
}ZoomVideoSDKNetworkStatus;

/*! \enum ZoomVideoSDKAudioType
	\brief Audio type: VOIP (Voice over IP), Telephony, or None.
*/
typedef enum {
	ZoomVideoSDKAudioType_VOIP,
	ZoomVideoSDKAudioType_TELEPHONY,
	ZoomVideoSDKAudioType_None,
}ZoomVideoSDKAudioType;

/*! \struct ZoomVideoSDKAudioStatus
	\brief Audio status information.
*/
typedef struct
{
	ZoomVideoSDKAudioType audioType;
	bool isMuted;
	bool isTalking;
}ZoomVideoSDKAudioStatus;

/*! \struct ZoomVideoSDKVideoStatisticInfo
	\brief Video statistic information.
*/
typedef struct
{
	int width;
	int height;
	int fps;
	int bpf;
	ZoomVideoSDKNetworkStatus videoNetworkStatus;
}ZoomVideoSDKVideoStatisticInfo;

/*! \struct ZoomVideoSDKShareStatisticInfo
	\brief Share statistic information.
*/
typedef struct
{
	int width;
	int height;
	int fps;
	int bpf;
}ZoomVideoSDKShareStatisticInfo;

/*! \struct ZoomVideoSDKShareCursorData
	\brief Share cursor information.
*/
typedef struct
{
	unsigned int source_id;
	int x;
	int y;
}ZoomVideoSDKShareCursorData;

typedef enum
{
	ZoomVideoSDKResolution_90P = 0,
	ZoomVideoSDKResolution_180P,
	ZoomVideoSDKResolution_360P,
	ZoomVideoSDKResolution_720P,
	ZoomVideoSDKResolution_1080P,
	ZoomVideoSDKResolution_Auto = 100, ///<Just for video canvas.
}ZoomVideoSDKResolution;

typedef enum 
{
	RawData_On,
	RawData_Off,
}RawDataStatus;

/// \brief Video/share raw data sink interface.
///
class IZoomVideoSDKRawDataPipeDelegate
{
public:
	/// \brief Call when subscribed data received.
	/// \param data_ Data object.
	virtual void onRawDataFrameReceived(YUVRawDataI420* data_) = 0;

	/// \brief Call when subscribed data status changed.
	/// \param status Current data status.
	virtual void onRawDataStatusChanged(RawDataStatus status) = 0;

	/// \brief Call when the cursor data of share received.
	/// \param info Share cursor data object.
	virtual void onShareCursorDataReceived(ZoomVideoSDKShareCursorData info) = 0;

	virtual ~IZoomVideoSDKRawDataPipeDelegate() {};
};

/// \brief Video/share raw data pipe interface.
///
class IZoomVideoSDKRawDataPipe
{
public:
	/// \brief Subscribe video/share.
	/// \param resolution Subscribe size.
	/// \param listener Callback sink object.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors subscribe(ZoomVideoSDKResolution resolution, IZoomVideoSDKRawDataPipeDelegate* listener) = 0;
	
	/// \brief Unsubscribe video/share.
	/// \param listener The callback sink object.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors unSubscribe(IZoomVideoSDKRawDataPipeDelegate* listener) = 0;

	/// \brief Subscribe to the computer audio shared by other user.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	/// \remark Please call this interface after the callback onUserShareStatusChanged is received and the share type is ZoomVideoSDKShareType_PureAudio.
	/// This function has been marked as deprecated, then it will be instead by "IZoomVideoSDKUser.SetUserVolume(1.0, true)". 
	virtual ZoomVideoSDKErrors subscribeToSharedComputerAudio() = 0;

	/// \brief Unsubscribe to the computer audio shared by other user.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	/// This function has been marked as deprecated, then it will be instead by "IZoomVideoSDKUser.SetUserVolume(0, true)".
	virtual ZoomVideoSDKErrors unsubscribeToSharedComputerAudio() = 0;

	/// \brief Get the raw data data type. 
	/// \return Share or Video data type, see \link ZoomVideoSDKRawDataType \endlink enum.
	virtual ZoomVideoSDKRawDataType getRawdataType() = 0;

	/// \brief Get video status.
	/// \return Video status of the user object.
	virtual ZoomVideoSDKVideoStatus getVideoStatus() = 0;

	/// \brief Get video device name.
	/// \return Video device name if the function succeeds, otherwise returns NULL.
	virtual const zchar_t* getVideoDeviceName() = 0;

	/// \brief Get share status.
	/// \return Share status of the user object.
	virtual ZoomVideoSDKShareStatus getShareStatus() = 0;

	/// \brief Get share type.
	/// \return Share type of the user object.
	virtual ZoomVideoSDKShareType getShareType() = 0;

	/// \brief Get video statistic information.
	/// \return video statistic information. For more information, see \link ZoomVideoSDKVideoStatisticInfo \endlink
	virtual ZoomVideoSDKVideoStatisticInfo getVideoStatisticInfo() = 0;
};


/// \brief Camera control interface.
///
class IZoomVideoSDKCameraControlRequestHandler
{
public:
	virtual ~IZoomVideoSDKCameraControlRequestHandler() {}

	/// \brief Approve the remote camera control request.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	virtual ZoomVideoSDKErrors approve() = 0;

	/// \brief Decline the remote camera control request.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	virtual ZoomVideoSDKErrors decline() = 0;
};

class IZoomVideoSDKRemoteCameraControlHelper
{
public:
	virtual ~IZoomVideoSDKRemoteCameraControlHelper() {}

	/// \brief Request to control remote camera.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	virtual ZoomVideoSDKErrors requestControlRemoteCamera() = 0;

	/// \brief Give up control of the remote camera.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	virtual ZoomVideoSDKErrors giveUpControlRemoteCamera() = 0;

	/// \brief Turn the camera to the left.
	/// \param range Rotation range,  10 <= range <= 100.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	virtual ZoomVideoSDKErrors turnLeft(unsigned int range = 50) = 0;

	/// \brief Turn the camera to the right.
	/// \param range Rotation range,  10 <= range <= 100.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	virtual ZoomVideoSDKErrors turnRight(unsigned int range = 50) = 0;

	/// \brief Turn the camera up.
	/// \param range Rotation range,  10 <= range <= 100.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	virtual ZoomVideoSDKErrors turnUp(unsigned int range = 50) = 0;

	/// \brief Turn the camera down.
	/// \param range Rotation range,  10 <= range <= 100.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	virtual ZoomVideoSDKErrors turnDown(unsigned int range = 50) = 0;

	/// \brief Zoom the camera in.
	/// \param range Zoom range,  10 <= range <= 100.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	virtual ZoomVideoSDKErrors zoomIn(unsigned int range = 50) = 0;

	/// \brief Zoom the camera out.
	/// \param range Zoom range,  10 <= range <= 100.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	virtual ZoomVideoSDKErrors zoomOut(unsigned int range = 50) = 0;
};

typedef enum
{
	ZoomVideoSDKVideoAspect_Original, ///<Original aspect ratio.
	ZoomVideoSDKVideoAspect_Full_Filled, ///<Full filled aspect ratio.
	ZoomVideoSDKVideoAspect_LetterBox, ///<Letterbox aspect ratio.
	ZoomVideoSDKVideoAspect_PanAndScan, ///<Pan and scan aspect ratio.
}ZoomVideoSDKVideoAspect;

typedef enum
{
	ZoomVideoSDKSubscribeFailReason_None = 0,
	ZoomVideoSDKSubscribeFailReason_HasSubscribe1080POr720P,
	ZoomVideoSDKSubscribeFailReason_HasSubscribeTwo720P,
	ZoomVideoSDKSubscribeFailReason_HasSubscribeExceededLimit,
	ZoomVideoSDKSubscribeFailReason_HasSubscribeTwoShare,
	ZoomVideoSDKSubscribeFailReason_HasSubscribeVideo1080POr720PAndOneShare
}ZoomVideoSDKSubscribeFailReason;

typedef enum
{	
	ZoomVideoSDKCanvasType_VideoData = 1, ///<Video camera data
	ZoomVideoSDKCanvasType_ShareData, ///<Share data
}ZoomVideoSDKCanvasType;

#if !defined __linux && !defined ANDROID
/// \brief video or share canvas interface.
///
class IZoomVideoSDKCanvas
{
public:
	virtual ~IZoomVideoSDKCanvas() {}

	/// \brief Subscribes to the user's video or share view.
	/// \param handle The window handle of the showing video or share content.
	/// \param aspect Specify a video or share aspect ratio.
	/// \param resolution Specify a video resolution, valid only for video canvas.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise, this function returns an error. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.	
	virtual ZoomVideoSDKErrors subscribeWithView(void* handle, ZoomVideoSDKVideoAspect videoAspect, ZoomVideoSDKResolution resolution = ZoomVideoSDKResolution_Auto) = 0;

	/// \brief Unsubscribes to the user's video or share view.
	/// \param handle The window handle of the showing video or share content.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise, this function returns an error. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors unSubscribeWithView(void* handle) = 0;

	/// \brief Set the render video or share aspect ratio.
	/// \param handle The window handle of the showing video or share content.
	/// \param aspect Specify a new video aspect ratio.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise, this function returns an error. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.	
	virtual ZoomVideoSDKErrors setAspectMode(void* handle, ZoomVideoSDKVideoAspect aspect) = 0;

	/// \brief Sets the resolution for the user's video. Once you specify the value, the resolution will not change even if the size of the window is changed.
	///Specifying a bigger resolution may cause a subscription failure.
	/// \param handle The window handle of the showing video.
	/// \param resolution Specify the resolution for the video in the current render.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors.
	///Otherwise, this function returns an error. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors setResolution(void* handle, ZoomVideoSDKResolution resolution) = 0;

	/// \brief Gets the canvas type. 
	/// \return Share or Video canvas type, see \link ZoomVideoSDKCanvasType \endlink enum.
	virtual ZoomVideoSDKCanvasType canvasType() = 0;
};
#endif //__linux

/// \brief User object interface.
///
class IZoomVideoSDKUser
{
public:
	virtual ~IZoomVideoSDKUser(){}
	
	/// \brief Get the user's custom id.
	/// \return The Custom id of the user object.
	virtual const zchar_t* getCustomIdentity() = 0;

	/// \brief Get the user's name.
	/// \return The name of the user object.
	virtual const zchar_t* getUserName() = 0;

	/// \brief Get the user's id.
	/// \return The user id of the user object.
	virtual const zchar_t* getUserID() = 0;

	/// \brief Get the user's video status.
	/// \return Video status of the user object.
	/// this interface be marked as deprecated, then it will be instead by IZoomVideoSDKRawDataPipe.getVideoStatus()
	virtual ZoomVideoSDKVideoStatus getVideoStatus() = 0;

	/// \brief Get the user's audio status.
	/// \return Audio status of the user object.
	virtual ZoomVideoSDKAudioStatus getAudioStatus() = 0;
	
	/// \brief Get the user's share status.
	/// \return Share status of the user object.
	/// this interface be marked as deprecated, then it will be instead by IZoomVideoSDKRawDataPipe.getShareStatus()
	virtual ZoomVideoSDKShareStatus getShareStatus() = 0;
	
    /// \brief Get the user's reference info.
	/// \return user's reference info.
    virtual const zchar_t* getUserReference() = 0;

	/// \brief Determine whether the user is the host.
	/// \return True indicates that the user is the host, otherwise false.
	virtual bool isHost() = 0;

	/// \brief Determine whether the user is the manager(coHost)
	/// \return True indicates that the user is the manager(coHost), otherwise false.
	virtual bool isManager() = 0;
	
	/// \brief Get the user's video statistic information.
	/// \return The video statistic information. For more information, see \link ZoomVideoSDKVideoStatisticInfo \endlink
	virtual ZoomVideoSDKVideoStatisticInfo getVideoStatisticInfo() = 0;

	/// \brief Get the user's share statistic information.
	/// \return The share statistic information. For more information, see \link ZoomVideoSDKShareStatisticInfo \endlink
	virtual ZoomVideoSDKShareStatisticInfo getShareStatisticInfo() = 0;
	
	/// \brief Get the user's video raw data pipe.
	/// \return The video pipe. For more information, see \link IZoomVideoSDKRawDataPipe \endlink
	virtual IZoomVideoSDKRawDataPipe* GetVideoPipe() = 0;

	/// \brief Get the user's share raw data pipe.
	/// \return The share pipe. For more information, see \link IZoomVideoSDKRawDataPipe \endlink
	virtual IZoomVideoSDKRawDataPipe* GetSharePipe() = 0;

#if !defined __linux && !defined ANDROID
	/// \brief Gets the user's video render canvas object.
	/// \return If the function succeeds, the return value is the video render helper object. Otherwise, this function fails and returns NULL. For more details, see \link IZoomVideoSDKCanvas \endlink.
	virtual IZoomVideoSDKCanvas* GetVideoCanvas() = 0;

	/// \brief Get the user's share render canvas object.
	/// \return If the function succeeds, the return value is the share render helper object. Otherwise, this function fails and returns NULL. For more details, see \link IZoomVideoSDKCanvas \endlink.
	virtual IZoomVideoSDKCanvas* GetShareCanvas() = 0;
#endif

	/// \brief Get the helper class instance to access the remote camera control.
	/// \return If the function succeeds, the return value is the remote camera control helper object.
	///Otherwise returns NULL. For more details, see \link IZoomVideoSDKRemoteCameraControlHelper \endlink.
	virtual IZoomVideoSDKRemoteCameraControlHelper* getRemoteCameraControlHelper() = 0;

	/// \brief Get the user's multi-camera stream list.
	/// \return A list of all streaming cameras pipe. For more information, see see \link IZoomVideoSDKRawDataPipe \endlink.
	virtual IVideoSDKVector<IZoomVideoSDKRawDataPipe*>* getMultiCameraStreamList() = 0;

	/// \brief Set the user's local volume. This does not affect how other participants hear the user.
	/// \param volume The value can be >= 0 and <=10. If volume is 0, you won't be able to hear the related audio.
	/// \param isShareAudio If true, sets the volume of shared audio(such as shared computer audio), otherwise sets the volume of microphone.
	/// \return If success return true, otherwise false.
	virtual bool setUserVolume(float volume, bool isShareAudio = false) = 0;

	/// \brief Get user volume.
	/// \param volume The user volume.
	/// \param isShareAudio If true, gets the volume of shared audio(such as shared computer audio), otherwise gets the volume of the microphone.
	/// \return If successful, return true, otherwise false.
	virtual bool getUserVolume(float& volume, bool isShareAudio = false) = 0;

	/// \brief Determine which audio you can set, shared audio or microphone.
	/// \param isShareAudio If true, checks whether you can set the volume of shared audio, otherwise you can set the volume of the microphone.
	/// \return If can return true, otherwise false.
	virtual bool canSetUserVolume(bool isShareAudio = false) = 0;

	/// \brief Used to determine whether I agree to individual video recording.
	/// \return If agreed return true, otherwise false.
	virtual bool hasIndividualRecordingConsent() = 0;

	/// \brief Send file to current user object.
	/// \param filePath The local path of the file
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	virtual ZoomVideoSDKErrors transferFile(const zchar_t* filePath) = 0;
};

/// \brief User helper interface.
///
class IZoomVideoSDKUserHelper
{
public:
	/// \brief Change a specific user's name.
	/// \param name The new name of the user object.
	/// \param pUser User in the session
	/// \return True indicates that name change is success. Otherwise, this function returns false.
	virtual bool changeName(const zchar_t* name, IZoomVideoSDKUser* pUser) = 0;

	/// \brief Assign a user as the session host.
	/// \param pUser User in the session
	/// \return True indicates that the user is now the host. Otherwise, this function returns false.
	virtual bool makeHost(IZoomVideoSDKUser* pUser) = 0;

	/// \brief Assign a user as the session manager.
	/// \param pUser User in the session.
	/// \return True indicates that the user is now the manager. Otherwise, this function returns false.
	virtual bool makeManager(IZoomVideoSDKUser* pUser) = 0;

	/// \brief Revoke manager rights from a user.
	/// \param pUser User in the session.
	/// \return True indicates that the user no longer has manager rights. Otherwise, this function returns false.
	virtual bool revokeManager(IZoomVideoSDKUser* pUser) = 0;

	/// \brief Remove user from session.
	/// \param pUser User in the session.
	/// \return True indicates that remove user is success. Otherwise, this function returns false.
	virtual bool removeUser(IZoomVideoSDKUser* pUser) = 0;

    /// \brief Reclaim host permission.
    /// \return True indicates that reclaim host is success. Otherwise, this function returns false.
	virtual bool reclaimHost() = 0;
};
END_ZOOM_VIDEO_SDK_NAMESPACE
#endif
