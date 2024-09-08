/*!
* \file zoom_video_sdk_video_helper_interface.h
* \brief video helper
*
*/

#ifndef _ZOOM_VIDEO_SDK_VIDEO_HELPER_INTERFACE_H_
#define _ZOOM_VIDEO_SDK_VIDEO_HELPER_INTERFACE_H_
#include "zoom_video_sdk_def.h"
#include "zoom_video_sdk_vector_interface.h"
#include "zoom_video_sdk_user_helper_interface.h"

BEGIN_ZOOM_VIDEO_SDK_NAMESPACE
typedef enum
{
	VideoRotation_0,
	VideoRotation_90,
	VideoRotation_180,
	VideoRotation_270,
}VideoRotation;

/*!  \enum VideoPreferenceMode
	\brief video preference mode types
	Here are more detailed structural descriptions.
*/
typedef enum
{
	ZoomVideoSDKVideoPreferenceMode_Balance, ///<Balance mode
	ZoomVideoSDKVideoPreferenceMode_Sharpness, ///<Sharpness mode
	ZoomVideoSDKVideoPreferenceMode_Smoothness, ///<Smoothness mode	
	ZoomVideoSDKVideoPreferenceMode_Custom	///<Custom mode.
}ZoomVideoSDKVideoPreferenceMode;

/*! \struct ZoomVideoSDKVideoPreferenceSetting
	\brief When setting custom modes, the maximum and minimum frame rates are provided by the developer.
	If the current bandwidth cannot maintain the minimum frame rate, the video system will drop to the next lower resolution.
	The default maximum and minimum frame rates for other modes are 0.
*/
typedef struct tagVideoPreferenceSetting
{
	ZoomVideoSDKVideoPreferenceMode mode;  ///<0: Balance mode; 1: Smoothness mode; 2: Sharpness mode; 3: Custom mode
	uint32_t minimum_frame_rate; ///<0 for the default value,minimum_frame_rate should be less than maximum_frame_rate, range: from 0 to 30 .out of range for frame-rate will use default frame-rate of Zoom	
	uint32_t maximum_frame_rate; ///<0 for the default value,maximum_frame_rate should be less and equal than 30, range: from 0 to 30.out of range for frame-rate will use default frame-rate of Zoom
	tagVideoPreferenceSetting()
	{
		mode = ZoomVideoSDKVideoPreferenceMode_Balance;
		minimum_frame_rate = 0;
		maximum_frame_rate = 0;
	}
} ZoomVideoSDKVideoPreferenceSetting;

enum ZoomVideoSDKMultiCameraStreamStatus
{
	ZoomVideoSDKMultiCameraStreamStatus_JoinStart, ///<Current user only. Sent when the current user attempts to use multiple cameras in the session.
	ZoomVideoSDKMultiCameraStreamStatus_JoinFail, ///<Current user only. Sent when the current user fails to use multiple cameras in the session.
	ZoomVideoSDKMultiCameraStreamStatus_Joined, ///<Broadcast to all users. Sent when a new camera has been successfully added to the session as the multiple cameras.
	ZoomVideoSDKMultiCameraStreamStatus_Left, ///<Broadcast to all users. Sent after a camera that is being used as the multiple cameras leaves the session.
	ZoomVideoSDKMultiCameraStreamStatus_VideoOn, ///<Broadcast to all users. Sent when a multiple camera is started.
	ZoomVideoSDKMultiCameraStreamStatus_VideoOff, ///<Broadcast to all users. Sent when a multiple camera is stopped.
};

#if !defined (__linux) || defined ANDROID
enum VirtualBackgroundDataType
{
	VirtualBackgroundDataType_NONE,
	VirtualBackgroundDataType_IMAGE,
	VirtualBackgroundDataType_BLUR
};
#endif
/// \brief Camera device interface.
///
class IZoomVideoSDKCameraDevice
{
public:
	virtual ~IZoomVideoSDKCameraDevice(){}
	virtual const zchar_t* getDeviceId() = 0;
	virtual const zchar_t* getDeviceName() = 0;
	virtual bool isSelectedDevice() = 0;
#if !defined (__linux) && !defined ANDROID && !defined (__IOS__)
	/// \brief Determine whether the camera is selected as multiple stream video.
	/// \return True if selected. Otherwise returns false.
	virtual bool isSelectedAsMultiCamera() = 0;
	
	/// \brief Determine whether the camera is running as multiple stream video.
	/// \return True if running. Otherwise returns false.
	virtual bool isRunningAsMultiCamera() = 0;
#endif
};


#if !defined (__linux) || defined ANDROID
/// \brief Virtual background item interface.
///
class IVirtualBackgroundItem  
{
public:
	/// \brief Get current item image file path.
	/// \return Current item image file path.
	virtual const zchar_t* getImageFilePath() = 0;

	/// \brief Get current item image name.
	/// \return Current item image name.
	virtual const zchar_t* getImageName() = 0;

	/// \brief Get current item background type.
	/// \return Current item background type.
	virtual VirtualBackgroundDataType getType() = 0;

	/// \brief Determine whether the virtual background can be deleted.
	/// \return True if allow. Otherwise returns false.
	virtual bool canVirtualBackgroundBeDeleted() = 0;

	virtual ~IVirtualBackgroundItem() {};
};
#endif

/// \brief An interface to control video and manage cameras during a video session.
///
class IZoomVideoSDKVideoHelper
{
public:
	virtual ~IZoomVideoSDKVideoHelper(){}

	/// \brief Call this method to start sending local video data from the camera.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
    virtual ZoomVideoSDKErrors startVideo() = 0;

	/// \brief Call this method to stop sending local video data from the camera.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
    virtual ZoomVideoSDKErrors stopVideo() = 0;

	/// \brief Call this method to rotate the video when the device is rotated.
	/// \return Returns true if the rotation was successful, false otherwise.
    virtual bool rotateMyVideo(VideoRotation rotation) = 0;

	/// \brief Switch to the next available camera.
	/// \return Returns true if the switch to the next camera was successful, false otherwise.
    virtual bool switchCamera() = 0;

	/// \brief Switch to the camera passed in as cameraDeviceID.
	/// \return Returns true if the switch was successful, false otherwise.
	virtual bool selectCamera(const zchar_t* cameraDeviceID) = 0;

#if !defined (__linux) && !defined ANDROID && !defined (__IOS__)
	/// \brief Enable multiple stream video if you have multiple cameras 
	///        and other participants can see multiple videos of you.
	/// \param cameraDeviceID The camera ID  for the camera to enable. 
	/// \param customDeviceName The custom device name of the camera. If this parameter is not passed, a default name will be generated.
	/// \return True if success. Otherwise returns false.
	virtual bool enableMultiStreamVideo(const zchar_t* cameraDeviceID, const zchar_t* customDeviceName = NULL) = 0;

	/// \brief Disable multiple stream video 
	/// \param cameraDeviceID The camera id which you want to disable. 
	/// \return True if success. Otherwise returns false.
	virtual bool disableMultiStreamVideo(const zchar_t* cameraDeviceID) = 0;

	/// \brief Turn off multiple stream video 
	/// \param cameraDeviceID The camera id which is running as multiple camera. 
	/// \return True if success. Otherwise returns false.
	virtual bool muteMultiStreamVideo(const zchar_t* cameraDeviceID) = 0;

	/// \brief Turn on multiple stream video 
	/// \param cameraDeviceID The camera id which is running as multiple camera. 
	/// \return True if success. Otherwise returns false.
	virtual bool unmuteMultiStreamVideo(const zchar_t* cameraDeviceID) = 0;

	/// \brief Get the device ID associated with my multi-camera pipe
	/// \param pPipe My multi-camera pipe
	/// \return The video device ID if successful. Otherwise returns NULL.
	virtual const zchar_t* getDeviceIDByMyPipe(IZoomVideoSDKRawDataPipe* pPipe) = 0;
#endif

	/// \brief Returns number of cameras available to share the video.
	/// \return Number of cameras.
    virtual uint32_t getNumberOfCameras() = 0;

	/// \brief Returns a collection of camera devices available to share the video as an object of type \link IZoomVideoSDKCameraDevice \endlink
	/// \return Camera devices list.
    virtual IVideoSDKVector<IZoomVideoSDKCameraDevice*>* getCameraList() = 0;

	/// \brief Check whether the current user has permission to control the camera.	
	/// \param [out] bCan True means able, False not. It validates only when the return value is ZoomVideoSDKErrors_Success.
	/// \param pStrDeviceID The camera device ID to check. The default is the main camera ID.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise the function fails. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors canControlCamera(bool& bCan, const zchar_t* pStrDeviceID = NULL) = 0;

	/// \brief Pan the camera to the left.
	/// \param range Rotation range, 10 <= range <= 100.
	/// \param pStrDeviceID The camera device ID to rotate. The default is the main camera ID.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise it fails. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors turnCameraLeft(unsigned int range = 50, const zchar_t * pStrDeviceID = NULL) = 0;

	/// \brief Pan the camera to the right.
	/// \param range Rotation range, 10 <= range <= 100.
	/// \param pStrDeviceID The camera device ID to rotate.The default is the main camera ID.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors turnCameraRight(unsigned int range = 50, const zchar_t* pStrDeviceID = NULL) = 0;

	/// \brief Tilt the camera up.
	/// \param range Rotation range, 10 <= range <= 100.
	/// \param pStrDeviceID The camera device ID to operate.The default is main camera ID.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise it fails. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors turnCameraUp(unsigned int range = 50, const zchar_t* pStrDeviceID = NULL) = 0;

	/// \brief Tilt the camera down.
	/// \param range Rotation range, 10 <= range <= 100.
	/// \param pStrDeviceID The camera device ID to operate.The default is the main camera ID.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise it fails. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors turnCameraDown(unsigned int range = 50, const zchar_t* pStrDeviceID = NULL) = 0;

	/// \brief Zoom the camera in.
	/// \param range Zoom range, 10 <= range <= 100.
	/// \param pStrDeviceID The camera device ID to operate.The default is the main camera ID.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise it fails. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors zoomCameraIn(unsigned int range = 50, const zchar_t* pStrDeviceID = NULL) = 0;

	/// \brief Zoom the camera out.
	/// \param range Zoom range, 10 <= range <= 100.
	/// \param pStrDeviceID The camera device ID to operate.The default is the main camera ID.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise it fails. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors zoomCameraOut(unsigned int range = 50, const zchar_t* pStrDeviceID = NULL) = 0;

	/// \brief Automatically adjust user's video solution and frame-rate
	/// \param [out] preference_setting Specifies the video quality preference.	
	///when setting custom modes, the maximum and minimum frame rates are provided by the developer.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors setVideoQualityPreference(ZoomVideoSDKVideoPreferenceSetting& preference_setting) = 0;

	/// \brief Subscribe preview video raw data with a callback
	/// \param listener Callback sink object.
	/// \param cameraDeviceID
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors startVideoPreview(IZoomVideoSDKRawDataPipeDelegate* listener, const zchar_t* cameraDeviceID = NULL) = 0;

	/// \brief UnSubscribe preview video raw data with the subscribed callback.
	/// \param listener Callback sink object.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors stopVideoPreview(IZoomVideoSDKRawDataPipeDelegate* listener) = 0;

	/// \brief Starts the preview video.
	/// \param handle The window handle of the preview video.
	/// \param deviceID Specify a camera device to test. If the param is an incorrect camera ID, the SDK returns an error. Otherwise the SDK tests the specified device and sets it to be the selected one.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise, this function returns an error. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.	
	virtual ZoomVideoSDKErrors startVideoCanvasPreview(void* handle, const zchar_t* cameraDeviceID = NULL) = 0;

	/// \brief Stops the preview video.
	/// \param handle The window handle of the preview video.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise, this function returns an error. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.	
	virtual ZoomVideoSDKErrors stopVideoCanvasPreview(void* handle) = 0;

#if !defined (__linux) || defined ANDROID
	/// \brief Add virtual background object.
	/// \param image_path Setting using image path.
	/// \param imageItem Once the function is called successfully, this parameter will store the pointer to the IVirtualBackgroundItem*.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors addVirtualBackgroundItem(const zchar_t* image_path, IVirtualBackgroundItem** imageItem) = 0;
	
	/// \brief Remove virtual background object.
	/// \param imageItem The choosing IVirtualBackgroundItem object.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors removeVirtualBackgroundItem(IVirtualBackgroundItem* imageItem) = 0;

	/// \brief Get virtual background item list.
	/// This function need to call first before create virtual background object to get background item last time.
	/// \return If the function succeeds, the return value is IVirtualBackgroundItem object vector.
	///Otherwise failed. the return value is NULL.
	virtual IVideoSDKVector<IVirtualBackgroundItem* >* getVirtualBackgroundItemList() = 0;

	/// \brief Set virtual background item.
	/// \param imageItem The choosing IVirtualBackgroundItem object.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors setVirtualBackgroundItem(IVirtualBackgroundItem* imageItem) = 0;

	/// \brief Get virtual background item which is been selected.
	/// \return If the function succeeds, the return value is IVirtualBackgroundItem object.
	///Otherwise failed. the return value is NULL.
	virtual IVirtualBackgroundItem* getSelectedVirtualBackgroundItem() = 0;
#endif

	/// \brief Determine whether current aspect ratio is the original aspect ratio of video.
	/// \return True if is original aspect ratio, otherwise false.
	virtual bool isOriginalAspectRatioEnabled() = 0;

	/// \brief This function is used to set the aspect ratio of the video sent out.
	/// \param bEnabled False means the aspect ratio is 16:9, true means that using the original aspect ratio of video
	/// \return True if successful, otherwise false
	/// Remark: If session is using video source and data_mode is not VideoSourceDataMode_None, default always use original aspect ration of video.
	virtual bool enableOriginalAspectRatio(bool bEnabled) = 0;

	/// \\brief Mirrors the current user's video. Valid only for canvas.
	/// \\param bEnable TRUE to enable mirror my video.
	/// \\return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise, this function returns an error. To get extended error information, see \\link ZoomVideoSDKErrors \\endlink enum.
	virtual ZoomVideoSDKErrors mirrorMyVideo(bool bEnabled) = 0;

	/// \\brief Determine whether mirror my video is enabled.
	/// \\return True if mirror my video is enabled, otherwise false.
	virtual bool isMyVideoMirrored() = 0;

	/// \brief Determine if alpha channel mode can be enabled. 
	/// \return true means it can be enabled. Otherwise false.
	virtual bool canEnableAlphaChannelMode() = 0;

	/// \brief Enable or disable video alpha channel mode.
	/// \param enable True indicates to enable alpha channel mode. Otherwise, disable it.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors enableAlphaChannelMode(bool enable) = 0;
	
	/// \brief Determine if alpha channel mode is enabled.
	/// \return True indicates alpha channel mode is enabled. Otherwise false.
	virtual bool isAlphaChannelModeEnabled() = 0;

	/// \brief Determines whether the device hardware capabilities are capable of supporting video alpha mode.
    /// \return True indicates that current device supports it, 
	/// Otherwise, it indicates that the device performance is weak and does not support it.
	virtual bool isDeviceSupportAlphaChannelMode() = 0;
};
END_ZOOM_VIDEO_SDK_NAMESPACE
#endif
