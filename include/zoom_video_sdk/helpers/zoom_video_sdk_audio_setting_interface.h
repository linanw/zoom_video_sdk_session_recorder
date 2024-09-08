/*!
* \file zoom_video_sdk_setting_helper_interface.h
* \brief audio helper
*
*/

#ifndef _ZOOM_VIDEO_SDK_AUDIO_SETTING_INTERFACE_H_
#define _ZOOM_VIDEO_SDK_AUDIO_SETTING_INTERFACE_H_
#include "zoom_video_sdk_def.h"
#include "zoom_video_sdk_vector_interface.h"

BEGIN_ZOOM_VIDEO_SDK_NAMESPACE

typedef enum
{
	ZoomVideoSDKSuppressBackgroundNoiseLevel_Auto = 0,
	ZoomVideoSDKSuppressBackgroundNoiseLevel_Low,
	ZoomVideoSDKSuppressBackgroundNoiseLevel_Medium,
	ZoomVideoSDKSuppressBackgroundNoiseLevel_High
}ZoomVideoSDKSuppressBackgroundNoiseLevel;

typedef enum
{
	ZoomVideoSDKDevice_Microphone = 0,
	ZoomVideoSDKDevice_Speaker
}ZoomVideoSDKAudioDeviceType;

typedef enum {
	ZoomVideoSDK_Device_Error_Unknow = 0,
	ZoomVideoSDK_No_Device,
	ZoomVideoSDK_Device_List_Update,
	ZoomVideoSDK_Audio_No_Input,
	ZoomVideoSDK_Audio_Disconnect_As_Detected_Echo
}ZoomVideoSDKAudioDeviceStatus;

typedef enum {
	ZoomVideoSDKEchoCancellationLevel_Default = 0,
	ZoomVideoSDKEchoCancellationLevel_Aggressive
}ZoomVideoSDKEchoCancellationLevel;

/// \brief Audio setting interface.
///
class IZoomVideoSDKTestAudioDeviceHelper
{
public:
	/// \brief Start the mic test.
	/// \param deviceID Specify to test a mic device. If the param is a wrong mic ID, the SDK will return an error. Otherwise the SDK tests the specified device and sets it as selected.
	///The SDK will test the default device if no parameter is input.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	///\remarks The function is disabled if no event handler is set.
	virtual ZoomVideoSDKErrors startMicTestRecording(const zchar_t* deviceID = NULL) = 0;

	/// \brief Stop the mic test. The SDK will return an error if there was no mic test.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	///\remarks The function is disabled if no event handler is set.
	virtual ZoomVideoSDKErrors stopMicTestRecording() = 0;

	/// \brief Play the mic recorded sound. If there was no mic test,the SDK returns an error.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	///\remarks The function is disabled if no event handler is set.
	virtual ZoomVideoSDKErrors playMicTestRecording() = 0;

	/// \brief Start the speaker test.
	/// \param deviceID Specify to test a speaker device. If the param is a wrong speaker ID, the SDK will return an error. Otherwise the SDK tests the specified device and sets it as selected.
	///The SDK will test the default device if no parameter is input.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	///\remarks The function is disabled if no event handler is set.
	virtual ZoomVideoSDKErrors startSpeakerTest(const zchar_t* deviceID = NULL) = 0;

	/// \brief Stop the speaker test. The SDK will return an error if there was no speaker test.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	///\remarks The function is disabled if no event handler is set.
	virtual ZoomVideoSDKErrors stopSpeakerTest() = 0;

	/// \brief Set the time interval for the audio test. 
	/// \param timerInterval Usually the SDK sends the mic and speaker volumes every 200 ms by default via ITestAudioDeviceHelperEvent::onMicSpeakerVolumeChanged(). With this function, the user can modify the time interval instead of 200.
	///Accepted values between 50 to 1000 milliseconds.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	///\remarks This interface will stop the mic or speaker test (if there is on ongoing). As a best practice, call it before the audio test.
	virtual ZoomVideoSDKErrors setTimerInterval(unsigned int timerInterval) = 0;
};


class IZoomVideoSDKAudioSettingHelper
{
public:	
	/// \brief Get the suppress background noise level.
	/// \param [out] level The suppress background noise level.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors getSuppressBackgroundNoiseLevel(ZoomVideoSDKSuppressBackgroundNoiseLevel& level) = 0;

	/// \brief Set the suppress background noise level.
	/// \param level The new suppress background noise level to be set.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors setSuppressBackgroundNoiseLevel(ZoomVideoSDKSuppressBackgroundNoiseLevel level) = 0;

	/// \brief Call this method to enable or disable the original input of mic.
	/// \param bEnable True to enable the original input of microphone or false to disable it.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors enableMicOriginalInput(bool bEnable) = 0;

	/// \brief Determine whether the original input of the microphone is enabled.
	/// \param [out] bEnable True if the original input of the microphone is enabled, otherwise false.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors isMicOriginalInputEnable(bool& bEnable) = 0;

	/// \brief Set whether to enable the function of high fidelity music mode or not. 
	/// \param bEnable True means to enable the function, FALSE not.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	/// remarks This function is valid only if mic original input is enabled, otherwise invalid.
	virtual ZoomVideoSDKErrors enableHighFidelityMusicMode(bool bEnable) = 0;

	/// \brief Check whether the high fidelity music mode is enabled or not.
	/// \param [out] bEnable True if the high fidelity music mode is enabled, otherwise false.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors isHighFidelityMusicModeEnable(bool& bEnable) = 0;

	/// \brief Set whether to disable the function of echo cancellation or not. 
	/// \param bEnable True means to enable the function, FALSE not.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	/// remarks This function is valid only if mic original input is enabled, otherwise invalid.
	virtual ZoomVideoSDKErrors enableEchoCancellation(bool bEnable) = 0;

	/// \brief Check whether the echo cancellation is enabled or not.
	/// \param [out] bEnable True if the echo cancellation is enabled, otherwise false.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors isEchoCancellationEnable(bool& bEnable) = 0;

	/// \brief Enable or disable the stereo audio.
	/// \param bEnable True indicates to enable the stereo audio.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	/// remarks This function is valid only if mic original input is enabled, otherwise invalid.
	virtual ZoomVideoSDKErrors enableStereoAudio(bool bEnable) = 0;

	/// \brief Get the flag to enable/disable the stereo audio.
	/// \param [out] bEnable True if the stereo audio is enabled, otherwise false.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors isStereoAudioEnable(bool& bEnable) = 0;

	/// \brief Get the echo cancellation level.
	/// \param [out] level The echo cancellation level.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum. 
	virtual ZoomVideoSDKErrors getEchoCancellationLevel(ZoomVideoSDKEchoCancellationLevel& level) = 0;

	/// \brief Set the echo cancellation level.
	/// \param level The new echo cancellation level to be set.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors setEchoCancellationLevel(ZoomVideoSDKEchoCancellationLevel level) = 0;
	
	/// \brief Call this method to enable or disable the auto adjust input of microphone volume.
	/// \param bEnable true to enable auto adjust input or false to disable it.
	virtual ZoomVideoSDKErrors enableAutoAdjustMicVolume(bool bEnable) = 0;

	/// \brief Determine whether the original input of the microphone volume is enabled.
	/// \return true if  auto adjust input  is enabled, otherwise false.
	virtual ZoomVideoSDKErrors isAutoAdjustMicVolumeEnabled(bool& bEnable) = 0;
};
END_ZOOM_VIDEO_SDK_NAMESPACE
#endif