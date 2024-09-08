/*!
* \file zoom_video_sdk_audio_send_rawdata_interface.h
* \brief send audio raw data
*
*/
#ifndef _ZOOM_VIDEO_SDK_AUDIO_SEND_RAWDATA_INTERFACE_H_
#define _ZOOM_VIDEO_SDK_AUDIO_SEND_RAWDATA_INTERFACE_H_
#include "zoom_video_sdk_def.h"

BEGIN_ZOOM_VIDEO_SDK_NAMESPACE

/// \brief Audio raw data sender interface.
///
class IZoomVideoSDKAudioSender 
{
public:
	virtual ~IZoomVideoSDKAudioSender() {}

	/// \brief Send audio raw data. Audio sample must be 16-bit audio.
	/// \param data the audio data¡¯s address.
	/// \param data_length the audio data¡¯s length. Must be an even number.
	/// \param sample_rate the audio data¡¯s sampling rate.
	/// Supported audio data properties:
	/// When the channel is mono, supported sample rates: 8000/11025/16000/32000/44100/48000/50000/50400/96000/192000/2822400
	/// When the channel is stereo, supported sample rates: 8000/16000/32000/44100/48000/50000/50400/96000/192000
	/// Channels: mono and stereo
	/// resolution: little-endian, 16bit
	virtual ZoomVideoSDKErrors Send(char* data, unsigned int data_length, int sample_rate, ZoomVideoSDKAudioChannel channel = ZoomVideoSDKAudioChannel_Mono) = 0;
};

/// \brief An interface that handles a virtual audio microphone.
///
class IZoomVideoSDKVirtualAudioMic
{
public:
	virtual ~IZoomVideoSDKVirtualAudioMic() {}

	/// \brief Callback for virtual audio microphone initialization.
	/// \param rawdata_sender You can send audio data based on this object, see \link IZoomVideoSDKAudioRawDataSender \endlink.
	virtual void onMicInitialize(IZoomVideoSDKAudioSender* rawdata_sender) = 0;

	/// \brief Callback for microphone to start sending raw data. For example, this could be sent if the user unmuted audio.
	virtual void onMicStartSend() = 0;
	
	/// \brief Callback for microphone to stop sending raw data. For example, this could be sent if the user muted audio.
	virtual void onMicStopSend() = 0;
	
	/// \brief Callback sent when the microphone is uninitialized. For example, if the user left the session.
	virtual void onMicUninitialized() = 0;
};


class IZoomVideoSDKUser;
/// \brief An interface that handles the virtual audio speaker.
///
class IZoomVideoSDKVirtualAudioSpeaker
{
public:
	virtual ~IZoomVideoSDKVirtualAudioSpeaker() {}

	/// \brief Callback event for receiving the mixed audio raw data from the virtual speaker.
	/// \param data_ The mixed audio raw data. A pointer to AudioRawData. For more details, see \link AudioRawData \endlink.
	virtual void onVirtualSpeakerMixedAudioReceived(AudioRawData* data_) = 0;

	/// \brief Callback event for receiving the one way audio raw data of a selected user from the virtual speaker.
	/// \param data_ The one way audio raw data. A pointer to AudioRawData. For more details, see \link AudioRawData \endlink.
	/// \param pUser The corresponding participant of the audio raw data.
	virtual void onVirtualSpeakerOneWayAudioReceived(AudioRawData* data_, IZoomVideoSDKUser* pUser) = 0;

	/// \brief Callback event for receiving the share audio raw data such as buffer, sampleRate, etc.
	/// \param data_ The shared audio raw data in session. A pointer to AudioRawData. For more details, see \link AudioRawData \endlink.
	virtual void onVirtualSpeakerSharedAudioReceived(AudioRawData* data_) = 0;
};
END_ZOOM_VIDEO_SDK_NAMESPACE
#endif