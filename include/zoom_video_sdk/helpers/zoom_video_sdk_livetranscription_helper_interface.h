/*!
* \file zoom_video_sdk_livetranscription_interface.h
* \brief live transcription
*
*/

#ifndef _ZOOM_VIDEO_SDK_LIVE_TRANSCRIPTION_INTERFACE_H_
#define _ZOOM_VIDEO_SDK_LIVE_TRANSCRIPTION_INTERFACE_H_
#include "zoom_video_sdk_def.h"
#include "zoom_video_sdk_vector_interface.h"

BEGIN_ZOOM_VIDEO_SDK_NAMESPACE
/**
 * @brief Enumerations of the type for live transcription status.
 */
typedef enum
{
	ZoomVideoSDKLiveTranscription_Status_Stop = 0,///<not start
	ZoomVideoSDKLiveTranscription_Status_Start = 1,  ///<start	
}ZoomVideoSDKLiveTranscriptionStatus;

/**
 * @brief Enumerations of the type for live transcription operation type.
 */
typedef enum
{
	ZoomVideoSDKLiveTranscription_OperationType_None = 0,
	ZoomVideoSDKLiveTranscription_OperationType_Add,
	ZoomVideoSDKLiveTranscription_OperationType_Update,
	ZoomVideoSDKLiveTranscription_OperationType_Delete,
	ZoomVideoSDKLiveTranscription_OperationType_Complete,	
	ZoomVideoSDKLiveTranscription_OperationType_NotSupported,
}ZoomVideoSDKLiveTranscriptionOperationType;

/// \brief live transcription language interface.
///
class ILiveTranscriptionLanguage
{
public:
	/// \brief Get id of the language.
	/// \return The id of the language.
	virtual int getLTTLanguageID() = 0;

	/// \brief Get name of the language.
	/// \return The name of the language.
	virtual const zchar_t* getLTTLanguageName() = 0;

	virtual ~ILiveTranscriptionLanguage() {};
};

/// \brief live transcription message interface.
///
class ILiveTranscriptionMessageInfo
{
public:
	/// \brief Get the message ID of the current message.
	/// \return If the function succeeds, the return value is the message ID of the current message.
	///Otherwise it fails, and the return value is the string of length zero(0)
	virtual const zchar_t* getMessageID() = 0;

	/// \brief Get the speaker's ID.
	/// \return The user object's speaker ID.
	virtual const zchar_t* getSpeakerID() = 0;

	/// \brief Get the speaker's name.
	/// \return The user object's speaker name.
	virtual const zchar_t* getSpeakerName() = 0;

	/// \brief Get the content of the current message.
	/// \return The current message's content.
	virtual const zchar_t* getMessageContent() = 0;

	/// \brief Get the timestamp of the current message.
	/// \return The current message's timestamp.
	virtual time_t getTimeStamp() = 0;

	/// \brief Get the type of the current message.
	/// \return The current message's type.
	virtual ZoomVideoSDKLiveTranscriptionOperationType getMessageType() = 0;

	virtual ~ILiveTranscriptionMessageInfo() {};
};

/// \brief live transcription helper interface.
class IZoomVideoSDKLiveTranscriptionHelper
{
public:
	/// \brief Query if the user can start live transcription.
	/// \return True if the user can start live transcription, otherwise false.
	virtual bool canStartLiveTranscription() = 0;

	/// \brief Get the current live transcription status.
	/// \param [out] status The current live transcription status. For more details, see \link ZoomVideoSDKLiveTranscriptionStatus \endlink.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors getLiveTranscriptionStatus(ZoomVideoSDKLiveTranscriptionStatus& status) = 0;

	/// \brief Start live transcription.
	/// \If the session allows multi-language transcription,all users can start live transcription.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	/// Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors startLiveTranscription() = 0;

	/// \brief Stop live transcription.
	/// \If the session allows multi-language transcription,all users can stop live transcription.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	/// Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors stopLiveTranscription() = 0;

	/// \brief Get the list of all available spoken languages in session.
	/// \return If the function succeeds, the return value is the list of the available spoken languages in a session.
	///Otherwise failed, the return value is NULL.	
	virtual IVideoSDKVector<ILiveTranscriptionLanguage*>* getAvailableSpokenLanguages() = 0;

	/// \brief Set the spoken language of the current user.
	/// \param languageID The spoken language ID.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors setSpokenLanguage(int languageID) = 0;

	/// \brief Get the spoken language of the current user.
	/// \return The spoken language of the current user.
	virtual ILiveTranscriptionLanguage* getSpokenLanguage() = 0;

	/// \brief Get the list of all available translation languages in a session.
	/// \return If the function succeeds, the return value is the list of all available translation languages in a session.
	///Otherwise failed, the return value is NULL.
	virtual IVideoSDKVector<ILiveTranscriptionLanguage*>* getAvailableTranslationLanguages() = 0;

	/// \brief Set the translation language of the current user.	
	/// \param languageID The translation language ID.
	/// If the language id is set to -1, live translation will be disabled.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors setTranslationLanguage(int languageID) = 0;

	/// \brief Get the translation language of the current user.
	/// \return The translation language of the current user.
	virtual ILiveTranscriptionLanguage* getTranslationLanguage() = 0;

	/// \brief Enable or disable to receive original and translated content.If you enable this feature,you must start live transcription.
	/// \param bEnable True to enable the temporal de-noise of video or false to disable it.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors.
	/// Otherwise it fails. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors enableReceiveSpokenLanguageContent(bool bEnable) = 0;

	/// \brief Determine whether the feature to receive original and translated is available.
	/// \return True indicates that the feature to receive original and translated is available. Otherwise False.
	virtual bool isReceiveSpokenLanguageContentEnabled() = 0;

	/// \brief Determine whether the view history translation message is available.
	/// \return True indicates that the view history transcription message is available. Otherwise False.
	virtual bool isAllowViewHistoryTranslationMessageEnabled() = 0;

	/// \brief Get the list of all history translation messages in a session.
	/// \return If the function succeeds, the return value is a list of all history translation messages in a session.
	///Otherwise it fails, and the return value is NULL.
	virtual IVideoSDKVector<ILiveTranscriptionMessageInfo*>* getHistoryTranslationMessageList() = 0;
};
END_ZOOM_VIDEO_SDK_NAMESPACE
#endif