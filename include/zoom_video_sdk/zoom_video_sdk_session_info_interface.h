/*!
* \file zoom_video_sdk_session_info_interface.h
* \brief Zoom video sdk session info interface.
*
*/

#ifndef _ZOOM_VIDEO_SDK_SESSION_INFO_INTERFACE_H_
#define _ZOOM_VIDEO_SDK_SESSION_INFO_INTERFACE_H_
#include "zoom_video_sdk_def.h"
#include "zoom_video_sdk_vector_interface.h"
#include <string.h>
BEGIN_ZOOM_VIDEO_SDK_NAMESPACE
class IZoomVideoSDKUser;

/*! \struct _SessionAudioStatisticInfo
	\brief Session audio statistic information.
*/
typedef struct _SessionAudioStatisticInfo
{
	int   frequency; ///<The frequency in kilohertz (KHz).
	int   latency;   ///<The audio latency in milliseconds (delay in network data transfer).
	int   Jitter;    ///<The audio jitter in milliseconds (change in latency).
	float packetLossAvg; ///<The percentage of average audio packet loss.
	float packetLossMax; ///<The percentage of maximum audio packet loss.

	_SessionAudioStatisticInfo()
	{
		reset();
	}

	void reset() 
	{
		memset(this, 0, sizeof(_SessionAudioStatisticInfo));  //checked safe
	}
}ZoomVideoSDKSessionAudioStatisticInfo;

/*! \struct _SessionASVStatisticInfo
	\brief Session video/share statistic information.
*/
typedef struct _SessionASVStatisticInfo
{
	int	  frame_width;	///<The frame width in pixels.
	int   frame_height;	///<The frame height in pixels.
	int   fps;     ///<The frame rate in FPS (Frames Per Second).
	int   latency; ///<The video latency in milliseconds (delay in network data transfer).
	int   Jitter;  ///<The video jitter in milliseconds (change in latency).
	float packetLossAvg; ///<The percentage of average video packet loss.
	float packetLossMax; ///<The percentage of maximum video packet loss.

	_SessionASVStatisticInfo()
	{
		reset();
	}

	void reset() 
	{
		frame_width = 0;
		frame_height = 0;
		fps = 0;
		latency = 0;
		Jitter = 0;
		packetLossAvg = 0.0;
		packetLossMax = 0.0;
	}
}ZoomVideoSDKSessionASVStatisticInfo;
/// \brief Session information interface.
///
class IZoomVideoSDKSession
{
public:
	/// \brief Get the current session number.
	/// \return If the function succeeds, the return value is the current meeting number. Otherwise returns ZERO(0).
	virtual uint64_t getSessionNumber() = 0;

	/// \brief Get the session name.
	/// \return If the function succeeds, the return value is session name. Otherwise returns NULL.
	virtual const zchar_t* getSessionName() = 0;

	/// \brief Get the session's password.
	/// \return If the function succeeds, the return value is session password. Otherwise returns NULL.
	virtual const zchar_t* getSessionPassword() = 0;

	/// \brief Get the session phone passcode.
	/// \return If the function succeeds, the return value is session phone passcode. Otherwise returns NULL.
	virtual const zchar_t* getSessionPhonePasscode() = 0;

	/// \brief Get the session ID.
	/// \return If the function succeeds, the return value is session ID. Otherwise returns NULL.
	/// \remarks This interface is only valid for the host.
	virtual const zchar_t* getSessionID() = 0;

	/// \brief Get the host's name.
	/// \return If the function succeeds, the return value is session host name. Otherwise returns NULL.
	virtual const zchar_t* getSessionHostName() = 0;

	/// \brief Get the session's host user object.
	/// \return If the function succeeds, the return value is session host user object. Otherwise returns NULL.
	virtual IZoomVideoSDKUser* getSessionHost() = 0;

	/// \brief Get a list of the session's remote users.
	/// \return If the function succeeds, the return value is remote users list. Otherwise returns NULL.
	virtual IVideoSDKVector<IZoomVideoSDKUser*>* getRemoteUsers() = 0;
	
	/// \brief Get the session's user object for myself.
	/// \return If the function succeeds, the return value is myself object. Otherwise returns NULL.
	virtual IZoomVideoSDKUser* getMyself() = 0;
	
	/// \brief Determine if a user object is valid.
	/// \param pUser The user object pointer.
	/// \return True if user object is valid. Otherwise returns false.
	virtual bool IsValidUser(IZoomVideoSDKUser* pUser) = 0;
	
	/// \brief Get session's audio statistic information.
	/// \param [out] send_info Audio send information refer.
	/// \param [out] recv_info Audio receive information refer.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors getSessionAudioStatisticInfo(ZoomVideoSDKSessionAudioStatisticInfo& send_info, ZoomVideoSDKSessionAudioStatisticInfo& recv_info) = 0;

	/// \brief Get session's video statistic information.
	/// \param [out] send_info Video send information refer.
	/// \param [out] recv_info Video receive information refer.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors getSessionVideoStatisticInfo(ZoomVideoSDKSessionASVStatisticInfo& send_info, ZoomVideoSDKSessionASVStatisticInfo& recv_info) = 0;
	
	/// \brief Get session's share statistic information.
	/// \param [out] send_info Share send information refer.
	/// \param [out] recv_info Share receive information refer.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors getSessionShareStatisticInfo(ZoomVideoSDKSessionASVStatisticInfo& send_info, ZoomVideoSDKSessionASVStatisticInfo& recv_info) = 0;

	/// \brief Determine whether file transfer is enabled.
	/// \return True if file transfer is enabled, otherwise false.
	virtual bool isFileTransferEnable() = 0;

	/// \brief Send file to all users in current session.
	/// \param filePath The local path of the file
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	virtual ZoomVideoSDKErrors transferFile(const zchar_t* filePath) = 0;

	/// \brief Get the list of allowed file types in transfer.
	/// \return The value of allowed file types in transfer, comma-separated if there are multiple values.Exe files are by default forbidden from being transferred.
	virtual const zchar_t* getTransferFileTypeWhiteList() = 0;

	///  \brief Get the maximum size for file transfer
	///  \return The maximum number of bytes for file transfer
	virtual uint64_t getMaxTransferFileSize() = 0;
};
END_ZOOM_VIDEO_SDK_NAMESPACE
#endif