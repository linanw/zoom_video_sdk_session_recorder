/*!
* \file zoom_video_sdk_share_helper_interface.h
* \brief share helper
*
*/

#ifndef _ZOOM_VIDEO_SDK_SHARE_HELPER_INTERFACE_H_
#define _ZOOM_VIDEO_SDK_SHARE_HELPER_INTERFACE_H_
#include "zoom_video_sdk_def.h"
#include "zoom_video_sdk_vector_interface.h"

BEGIN_ZOOM_VIDEO_SDK_NAMESPACE
struct ZoomVideoSDKShareOption
{
    bool isWithDeviceAudio;            ///<share option, true: share computer sound when share screen/window, otherwise not.
    bool isOptimizeForSharedVideo;     ///<share option, true: optimize the frame rate when share screen/window, otherwise not.

    ZoomVideoSDKShareOption()
    {
        isWithDeviceAudio = false;
        isOptimizeForSharedVideo = false;
    }

    ZoomVideoSDKShareOption(const ZoomVideoSDKShareOption& option)
    {
        isWithDeviceAudio = option.isWithDeviceAudio;
        isOptimizeForSharedVideo = option.isOptimizeForSharedVideo;
    }

    ZoomVideoSDKShareOption(bool bWithAudio, bool optimize)
    {
        isWithDeviceAudio = bWithAudio;
        isOptimizeForSharedVideo = optimize;
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Share raw data sender interface.
///
class IZoomVideoSDKShareSender
{
public:
	virtual ~IZoomVideoSDKShareSender() {}

	/// \brief Send one frame data.
    /// \param frameBuffer FrameBuffer YUVI420 buffer.
    /// \param width Frame width.
    /// \param height Frame height.
    /// \param frameLength Buffer length.
    /// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors sendShareFrame(char* frameBuffer, int width, int height, int frameLength, FrameDataFormat format = FrameDataFormat_I420_FULL) = 0;
};

/// \brief Share source sink interface.
///
class IZoomVideoSDKShareSource
{
public:
	virtual ~IZoomVideoSDKShareSource() {}

    /// \brief Callback for share source can start send raw data.
    /// \param sender See \link IZoomVideoSDKShareSender \endlink.
	virtual void onShareSendStarted(IZoomVideoSDKShareSender* pSender) = 0; 

    /// \brief Callback for share source stop send raw data.
	virtual void onShareSendStopped() = 0; 
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
class IZoomVideoSDKShareAudioSender
{
public:
	virtual ~IZoomVideoSDKShareAudioSender() {}

	/// \brief Sends audio raw data.
	/// \param data The audio data address.
	/// \param data_length The audio data length. It must be even numbers.
	/// \param sample_rate The audio data sampling rate.
	/// \param channel The channel type.
	/// Supported audio data properties:
	/// When the channel is mono, supported sample rates: 8000/11025/16000/32000/44100/48000/50000/50400/96000/192000
	/// When the channel is stereo, supported sample rates: 8000/16000/32000/44100/48000/50000/50400/96000
	/// Channels: mono and stereo
	/// Resolution: little-endian, 16bit
	virtual ZoomVideoSDKErrors sendShareAudio(char* data, unsigned int data_length, int sample_rate, ZoomVideoSDKAudioChannel channel) = 0;
};

class IZoomVideoSDKShareAudioSource
{
public:
	virtual ~IZoomVideoSDKShareAudioSource() {}

	/// \brief Callback for audio source to start sending raw data.
	/// \param pShareAudioSender See \link IZoomVideoSDKShareAudioSender \endlink.
	virtual void onStartSendAudio(IZoomVideoSDKShareAudioSender* pShareAudioSender) = 0;

	/// \brief Callback for audio source to stop sending raw data.
	virtual void onStopSendAudio() = 0;
};

/// \brief Monitor List Builder.
///
class IMonitorListBuilder
{
public:
    virtual ~IMonitorListBuilder() {}

    /// \brief Add the monitor ID to list. The validity of the monitor ID is not verified here.
    /// \param monitorID The screen name.
    /// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
    ///Otherwise the function fails. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
    virtual ZoomVideoSDKErrors addMonitorID(const zchar_t* monitorID) = 0;

    /// \brief Remove the monitor ID from list.
    /// \param monitorID The screen name.
    /// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
    ///Otherwise the function fails. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
    virtual ZoomVideoSDKErrors removeMonitorID(const zchar_t* monitorID) = 0;

    /// \brief Clear the monitor ID list.
    /// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
    ///Otherwise the function fails. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
    virtual ZoomVideoSDKErrors clear() = 0;

    /// \brief Returns the monitor ID list.
   /// \return If the function succeeds, the return value is the monitor ID list. Otherwise returns NULL.
    virtual IVideoSDKVector<const zchar_t*>* getMonitorIDs() = 0;
};

/// \brief Share helper interface.
///
class IZoomVideoSDKRawDataPipeDelegate;
class IZoomVideoSDKAnnotationHelper;
class IZoomVideoSDKShareHelper
{
public:

	/// \brief Share a selected window, if virtual speaker is enabled, don't support share audio.
    /// \param handle The window handle that will to be share.
    /// \param option See \link ZoomVideoSDKShareOption \endlink.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
    virtual ZoomVideoSDKErrors startShareView(void* handle, ZoomVideoSDKShareOption option = {false, false}) = 0;
	
	/// \brief Share a selected screen, if virtual speaker is enabled, don't support share audio.
    /// \param monitorID The screen name that will to be share.
    /// \param option See \link ZoomVideoSDKShareOption \endlink.
    /// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
    ///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
    virtual ZoomVideoSDKErrors startShareScreen(const zchar_t* monitorID, ZoomVideoSDKShareOption option = { false, false }) = 0;

    /// \brief Returns the monitor ID list builder object, the monitor ID list will be cleared after calling startShareMultiScreen.
    /// \return If the function succeeds, the return value is the monitor ID list builder object. Otherwise returns NULL.
    virtual IMonitorListBuilder* getMonitorListBuilder() = 0;

    /// \brief Share all screens in IMonitorListBuilder, if virtual speaker is enabled, don't support share audio.
    /// \param option See \link ZoomVideoSDKShareOption \endlink.
    /// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
    ///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
    virtual ZoomVideoSDKErrors startShareMultiScreen(ZoomVideoSDKShareOption option = { false, false }) = 0;

    /// \brief Start sharing only the computer audio.
    /// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
    ///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
    virtual ZoomVideoSDKErrors startShareComputerAudio() = 0;

	/// \brief Starts sharing the pure external audio source.
    /// \param pShareAudioSource the pointer of external audio source object. 
    /// \param isPlaying true means audio raw data is played locally at the same time, false not.
    /// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
    ///Otherwise this function fails. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
    /// \remarks The feature is similar to share computer audio, except that the shared content is user-defined.
    virtual ZoomVideoSDKErrors startSharePureAudioSource(IZoomVideoSDKShareAudioSource* pShareAudioSource, bool isPlaying = true) = 0;
    
	/// \brief Stop share.
    /// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
    ///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
    virtual ZoomVideoSDKErrors stopShare() = 0;
    
    /// \brief Pause share.
    /// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
    ///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
    virtual ZoomVideoSDKErrors pauseShare() = 0;

    /// \brief Resume share.
    /// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
    ///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
    virtual ZoomVideoSDKErrors resumeShare() = 0;

	/// \brief Determine whether the current user is sharing.
	/// \return True indicates the current user is sharing. Otherwise returns false.
    virtual bool isSharingOut() = 0;
    
	/// \brief Determine whether the current user is sharing the screen.
    /// \return True indicates the current user is sharing the screen, otherwise false.
    virtual bool isScreenSharingOut() = 0;

	/// \brief Determine if other user is sharing.
    /// \return True indicates another user is sharing, otherwise false.
    virtual bool isOtherSharing() = 0;
    
	/// \brief Lock sharing the view or screen. Only the host can call this method.
    /// \param lock True to lock sharing
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
    virtual ZoomVideoSDKErrors lockShare(bool lock) = 0;
    
	/// \brief Determine whether sharing is locked.
    /// \return True indicates that sharing is locked, otherwise false.
    virtual bool isShareLocked() = 0;

	/// \brief Enable or disable participants can share simultaneously.
	/// \param enable True to enable. False to disable.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise the function fails. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
    /// \warning When you switch multi share from enable to disable, all sharing will be stopped.
    virtual ZoomVideoSDKErrors enableMultiShare(bool enable) = 0;

	/// \brief Determine whether multi share is enabled or not.
    /// \return true enabled, false not enabled.
    virtual bool isMultiShareEnabled() = 0;

	/// \brief Enable or disable the computer sound when sharing. The SDK does not support sharing device audio, for example, when you've enabled virtual speaker.
    /// \param enable True to enable. False to disable.
    /// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
    ///Otherwise the function fails. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors enableShareDeviceAudio(bool enable) = 0;
	
	/// \brief Determine if the SDK has enabled share device sound. This reflects the execution status of `enableShareDeviceAudio` instead of `startShareComputerAudio`.
    /// \return True if enabled, otherwise false.
    virtual bool isShareDeviceAudioEnabled() = 0;

	/// \brief Enable or disable the optimization of frame rate, you can enable it when there is video in shared content.
    /// \param enable True indicates to enable. False disable.
    /// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
    ///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors enableOptimizeForSharedVideo(bool enable) = 0;
	
	/// \brief Determine if optimization for shared video is enabled.
    /// \return True if enabled. Otherwise returns false.
    virtual bool isOptimizeForSharedVideoEnabled() = 0;

    /// \brief Start sharing a camera feed specified by the cameraID as the second camera.
    /// \param cameraID The camera ID.Warning: This must be a different camera than the one sending your primary video.
    /// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
    ///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
    virtual ZoomVideoSDKErrors startShare2ndCamera(const zchar_t* cameraID) = 0;

    /// \brief Subscribe to the raw data stream of the camera that is shared as the second camera.
    /// \param data_handler Data handler object.
    /// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
    ///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
    virtual ZoomVideoSDKErrors subscribeMyShareCamera(IZoomVideoSDKRawDataPipeDelegate* data_handler) = 0;
    
    /// \brief Unsubscribe to the raw data stream of the camera that is shared as the second camera.
    /// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
    ///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
    virtual ZoomVideoSDKErrors unSubscribeMyShareCamera() = 0;

	/// \brief Shares an external source.
	/// \param pSource External share source, \link IZoomVideoSDKShareSource \endlink.
    /// \param pAudioSource external share audio source.
    /// \param isPlaying true means audio raw data is played locally at the same time, false not.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise this function fails. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
    /// \remarks If pAudioSource is non-null, it means share user-defined audio at the same time.
    virtual ZoomVideoSDKErrors startSharingExternalSource(IZoomVideoSDKShareSource* pSource, IZoomVideoSDKShareAudioSource* pAudioSource = NULL, bool isPlaying = true) = 0;

	/// \brief Whether annotation is enabled or not.
	/// \return true support, false not support.
	virtual bool isAnnotationFeatureSupport() = 0;

	/// \brief Disable or enable viewer's annotation by the share owner.
	/// \param bDisable true means annotation is disabled, false means it is enabled.
	/// \warning Only the share owner can call this function.
	virtual ZoomVideoSDKErrors disableViewerAnnotation(bool bDisable) = 0;

	/// \brief Whether annotation on current sharing is disabled or not.
	/// \return true disable, false not disable.
	/// \warning Only the share owner can call this function.
	virtual bool isViewerAnnotationDisabled() = 0;

	/// \brief Creates annotation helper based on shared view.
	/// \param handle the shared view handle. Pass the null will return the helper for self sharing.
	/// \return If the function succeeds, the return value is the IZoomVideoSDKAnnotationHelper helper object. Otherwise returns NULL. For more details, see \link IZoomVideoSDKAnnotationHelper \endlink.
	/// \warning The view passed in this function should be subscribed share view. And if the share view aspect mode is full fill, the annotate not supported.
	/// \When the share owner not support the feature of annotate, the others should not do annotate in that case.
	virtual IZoomVideoSDKAnnotationHelper* createAnnotationHelper(void* handle) = 0;

	/// \brief Destroys annotation helper.
	/// \param helper IZoomVideoSDKAnnotationHelper object.
	virtual ZoomVideoSDKErrors destroyAnnotationHelper(IZoomVideoSDKAnnotationHelper* helper) = 0;

    /// \brief Enable or disable local playback of shared audio raw data.
    /// \param isPlaying true to play shared audio raw data, false not.
    virtual ZoomVideoSDKErrors enablePlaySharingAudioRawdata(bool isPlaying) = 0;
};
END_ZOOM_VIDEO_SDK_NAMESPACE
#endif