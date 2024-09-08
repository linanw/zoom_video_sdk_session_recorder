/*!
* \file zoom_video_sdk_audio_helper_interface.h
* \brief audio helper
*
*/

#ifndef _ZOOM_VIDEO_SDK_AUDIO_HELPER_INTERFACE_H_
#define _ZOOM_VIDEO_SDK_AUDIO_HELPER_INTERFACE_H_
#include "zoom_video_sdk_def.h"
#include "zoom_video_sdk_vector_interface.h"

BEGIN_ZOOM_VIDEO_SDK_NAMESPACE
class IZoomVideoSDKUser;
/// \brief Speaker device interface.
///
class IZoomVideoSDKSpeakerDevice
{
public:
	virtual const zchar_t* getDeviceId() = 0;
	virtual const zchar_t* getDeviceName() = 0;
	virtual bool isSelectedDevice() = 0;
	virtual ~IZoomVideoSDKSpeakerDevice() {};
};

/// \brief Mic device interface.
///
class IZoomVideoSDKMicDevice
{
public:
	virtual const zchar_t* getDeviceId() = 0;
	virtual const zchar_t* getDeviceName() = 0;
	virtual bool isSelectedDevice() = 0;
	virtual ~IZoomVideoSDKMicDevice() {};
};

/// \brief Audio helper interface.
///
class IZoomVideoSDKAudioHelper
{
public:
	/// \brief Start audio with voip.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
    virtual ZoomVideoSDKErrors startAudio() = 0;
    
	/// \brief Stop voip.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
    virtual ZoomVideoSDKErrors stopAudio() = 0;
	
	/// \brief Mute user's voip audio. 0 means current user (myself).
	/// \param pUser The user which you want to mute.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors muteAudio(IZoomVideoSDKUser* pUser) = 0;
	
	/// \brief Unmute user's voip audio.
	/// \param pUser The user which you want to unMute.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
    ///This function will trigger the callback \link onHostAskUnmute() \endlink.
	virtual ZoomVideoSDKErrors unMuteAudio(IZoomVideoSDKUser* pUser) = 0;
    
    /// \brief Mute all user's VOIP audio except my self.
    /// \param allowUnmute True means allow the user to unmute themself, otherwise false.
    /// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
    ///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
    virtual ZoomVideoSDKErrors muteAllAudio(bool allowUnmute) = 0;
    
    /// \brief Allow the others unmute themselves or not.
    /// \param allowUnmute True means allow the user to unmute themself, otherwise false.
    /// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
    ///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
    virtual ZoomVideoSDKErrors allowAudioUnmutedBySelf(bool allowUnmute) = 0;
    
    /// \brief Ask unmute all user's VOIP audio.
    /// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
    ///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
    ///This function will trigger the callback \link onHostAskUnmute() \endlink.
    virtual ZoomVideoSDKErrors unMuteAllAudio() = 0;

#if (defined CMM_ANDROID) || (defined __IOS__)
    virtual int setSpeaker(bool speaker) = 0; //Set audio speaker
    virtual bool getSpeakerStatus() = 0;      //Get audio speaker status
    virtual bool canSwitchSpeaker() = 0;      //Query is audio speaker enable.
#endif
	
	/// \brief Get speaker device list.
	/// \return If the function succeeds, the return value is speaker device list, Otherwise NULL.
	virtual IVideoSDKVector<IZoomVideoSDKSpeakerDevice*>* getSpeakerList() = 0;
	
	/// \brief Get mic device list.
	/// \return If the function succeeds, the return value is mic device list, Otherwise NULL.
	virtual IVideoSDKVector<IZoomVideoSDKMicDevice*>* getMicList() = 0;
	
	/// \brief Select some speaker device as default device.
	/// \param deviceId Device id.
	/// \param deviceName Device name.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors selectSpeaker(const zchar_t* deviceId, const zchar_t* deviceName) = 0;
	
	/// \brief Select some mic device as default device.
	/// \param deviceId Device id.
	/// \param deviceName Device name.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors selectMic(const zchar_t* deviceId, const zchar_t* deviceName) = 0;
	
	/// \brief Subscribe audio raw data.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors subscribe() = 0;
	
	/// \brief UnSubscribe audio raw data.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors unSubscribe() = 0;
};
END_ZOOM_VIDEO_SDK_NAMESPACE
#endif
