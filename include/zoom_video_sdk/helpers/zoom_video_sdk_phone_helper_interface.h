/*!
* \file zoom_video_sdk_phone_helper_interface.h
* \brief phone helper interface.
*
*/

#ifndef _ZOOM_VIDEO_SDK_PHONE_INTERFACE_H_
#define _ZOOM_VIDEO_SDK_PHONE_INTERFACE_H_
#include "zoom_video_sdk_def.h"
#include "zoom_video_sdk_vector_interface.h"

BEGIN_ZOOM_VIDEO_SDK_NAMESPACE
/// \brief Phone session support country information interface.
class IZoomVideoSDKPhoneSupportCountryInfo
{
public:
	virtual ~IZoomVideoSDKPhoneSupportCountryInfo() {}
	/// \brief Get the country ID of the current information. 
	/// \return The country ID.
	virtual const zchar_t* getCountryID() = 0;
	/// \brief Get the country name of the current information.
	/// \return The country name.
	virtual const zchar_t* getCountryName() = 0;
	/// \brief Get the country code of the current information.
	/// \return The country code.
	virtual const zchar_t* getCountryCode() = 0;
};

/*! \enum ZoomVideoSDKDialInNumType
	\brief Type of telephone call.
	Here are more detailed structural descriptions.
*/
enum ZoomVideoSDKDialInNumType
{
	ZoomVideoSDKDialInNumType_None,    ///<For initialization.
	ZoomVideoSDKDialInNumType_Toll,    ///<Paid.
	ZoomVideoSDKDialInNumType_TollFree,///<Free.
};

/// \brief Dial-in meeting Interface.
class IZoomVideoSDKSessionDialInNumberInfo
{
public:
	virtual ~IZoomVideoSDKSessionDialInNumberInfo() {}
	/// \brief Get the country ID of the current information.
	/// \return The country ID.	
	virtual const zchar_t* getCountryId() = 0;
	/// \brief Get the country code of the current information.
	/// \return The country code.	
	virtual const zchar_t* getCountryCode() = 0;
	/// \brief Get the country name of the current information.
	/// \return The country name.
	virtual const zchar_t* getCountryName() = 0;
	/// \brief Get the telephone number of the current information.
	/// \return The telephone number.
	virtual const zchar_t* getNumber() = 0;
	/// \brief Get the display number of the current information. 
	/// \return The display number.
	virtual const zchar_t* getDisplayNumber() = 0;
	/// \brief Get the call type of the current information.
	/// \return Call type. For more details, see \link ZoomVideoSDKDialInNumType \endlink.
	virtual ZoomVideoSDKDialInNumType  getType() = 0;
};


/*! \enum PhoneStatus
	\brief Status of telephone.
	Here are more detailed structural descriptions.
*/
enum PhoneStatus
{
	PhoneStatus_None,///<No status.
	PhoneStatus_Calling,///<In process of calling out.
	PhoneStatus_Ringing,///<In process of ringing.
	PhoneStatus_Accepted,///<Accept the call.
	PhoneStatus_Success,///<Call successful.
	PhoneStatus_Failed,///<Call failed.
	PhoneStatus_Canceling,///<In process of canceling the response to the previous state.
	PhoneStatus_Canceled, ///<Cancel successfully.
	PhoneStatus_Cancel_Failed,///<Cancel fails.
	PhoneStatus_Timeout,///<Timeout.
};


/*! \enum PhoneFailedReason
	\brief The reason for the failure of the telephone call.
	Here are more detailed structural descriptions.
*/
enum PhoneFailedReason
{
	PhoneFailedReason_None,///<For initialization.
	PhoneFailedReason_Busy,///<The telephone number is busy.
	PhoneFailedReason_Not_Available,///<The telephone number is out of service.
	PhoneFailedReason_User_Hangup,///<The user hangs up.
	PhoneFailedReason_Other_Fail,///<Other reasons.
	PhoneFailedReason_No_Answer,///<The user did not answer the call.
	PhoneFailedReason_Block_No_Host,///<The invitation by phone is blocked by the system due to an absent host.
	PhoneFailedReason_Block_High_Rate,///<The call-out is blocked by the system due to the high cost.
	PhoneFailedReason_Block_Too_Frequent,///<To join the session, the invitee would press one on the phone. An invitee who fails to respond will encounter a timeout. If there are too many invitee timeouts, the call invitation feature for this session will be blocked.
};

/*! \struct InvitePhoneUserInfo
	\brief The info for InvitePhoneUser
*/
struct InvitePhoneUserInfo
{
    const zchar_t* countryCode;///<InvitePhoneUserInfo, The country code of the specified user must be in the support list. 
    const zchar_t* phoneNumber;///<InvitePhoneUserInfo, The phone number of specified user.
    const zchar_t* name;///<InvitePhoneUserInfo, The screen name of the specified user in the session.
    bool bPressOne;///<InvitePhoneUserInfo , true: Need press number one to agree to join the session, otherwise not.
    bool bGreeting;///<InvitePhoneUserInfo, true: When joining the session, play a greeting prompt tone., otherwise not.

    InvitePhoneUserInfo()
    {
        countryCode = nullptr;
        phoneNumber = nullptr;
        name = nullptr;
        bPressOne = true;
        bGreeting = true;
    }
};


/// \brief zoom video sdk phone helper interface.
///
class IZoomVideoSDKPhoneHelper
{
public:
	/// \brief Determine if the session supports join by phone or not.
	/// \return True indicates join by phone is supported, otherwise false.
	virtual bool isSupportPhoneFeature() = 0;

	/// \brief Get the list of the country information where the session supports to join by telephone.
	/// \return List of the country information returns if the session supports to join by telephone. Otherwise NULL.
	virtual IVideoSDKVector<IZoomVideoSDKPhoneSupportCountryInfo* >* getSupportCountryInfo() = 0;

	/// \brief Invite the specified user to join the session by call out.
	/// \param countryCode The country code of the specified user must be in the support list. 
	/// \param phoneNumber The phone number of specified user.
	/// \param name The screen name of the specified user in the session.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	/// \deprecated This interface is marked as deprecated, and is replaced by invitePhoneUser(const InvitePhoneUserInfo& inviteInfo).
	virtual ZoomVideoSDKErrors inviteByPhone(const zchar_t* countryCode, const zchar_t* phoneNumber, const zchar_t* name) = 0;

    /// \\brief Invite the specified user to join the session by call out.
	/// \\param inviteInfo The info for invite by phone. See \link InvitePhoneUserInfo \endlink.
	/// \\return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \\link SDKError \\endlink enum.
	virtual ZoomVideoSDKErrors invitePhoneUser(const InvitePhoneUserInfo& inviteInfo) = 0;

	/// \brief Cancel the invitation that is being invited by phone.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link SDKError \endlink enum.
	virtual ZoomVideoSDKErrors cancelInviteByPhone() = 0;

	/// \brief Get the status of the invitation by phone.
	/// \return If the function succeeds, the method returns the PhoneStatus of the invitation by phone. To get extended error information, see \link PhoneStatus \endlink enum
	virtual PhoneStatus getInviteByPhoneStatus() = 0;

	/// \brief Get the list of dial in numbers supported by session.
	/// \return If the function succeeds, the return value is the list of the dial-in numbers. 
	///Otherwise failed. For more details, see \link ISessionDialInNumberInfo \endlink.
	virtual IVideoSDKVector<IZoomVideoSDKSessionDialInNumberInfo*>* getSessionDialInNumbers() = 0;
};

END_ZOOM_VIDEO_SDK_NAMESPACE
#endif