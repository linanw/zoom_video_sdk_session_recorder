/*!
* \file zoom_video_sdk_network_connection_helper_interface.h
* \brief network connection helper
*
*/

#ifndef _ZOOM_VIDEO_SDK_NETWORK_CONNECTION_HELPER_INTERFACE_H_
#define _ZOOM_VIDEO_SDK_NETWORK_CONNECTION_HELPER_INTERFACE_H_
#include "zoom_video_sdk_def.h"

BEGIN_ZOOM_VIDEO_SDK_NAMESPACE

/*! \struct tagProxySettings
	\brief The proxy that the user want to use according to the net environment.
	Here are more detailed structural descriptions.
*/
typedef struct tagProxySettings
{
	const zchar_t* proxy;///<The proxy ip address and port that user want to use, should be organized like ip:port, such as '255.255.255.255:8080'. Use ';' to separate multi proxies.
	bool auto_detect;///<True indicates automatic detection.
	tagProxySettings()
	{
		proxy = NULL;
		auto_detect = false;
	}
}ProxySettings;

/// \brief Proxy setting information callback interface.
///
class IZoomVideoSDKProxySettingHandler
{
public:
	virtual ~IZoomVideoSDKProxySettingHandler() {};
	/// \brief Get the address of the proxy host.
	/// \return The address of the proxy host.
	virtual const zchar_t* getProxyHost() = 0;
	/// \brief Get the proxy port.
	/// \return The value of the proxy port.
	virtual unsigned int getProxyPort() = 0;
	/// \brief Get the description of the proxy.
	/// \return The description of the proxy.
	virtual const zchar_t* getProxyDescription() = 0;

	/// \brief Input the username and password when using the proxy.
	/// \param userName The username when using the proxy.
	/// \param psw The password when using the proxy.
	virtual void inputUsernamePassword(const zchar_t* userName, const zchar_t* psw) = 0;

	/// \brief Cancel the process to input the username and password of the proxy.
	virtual void cancel() = 0;

};

/// \brief the info of SSL certificate interface.
///
class IZoomVideoSDKSSLCertificateInfo
{
public:
	virtual ~IZoomVideoSDKSSLCertificateInfo() {};

	/// \brief Get the value of whom the SSL certificate is issued to.
	/// \return The value of whom the SSL certificate is issued to.
	virtual const zchar_t* getCertIssuedTo() = 0;

	/// \brief Get the value that who issues the SSL certificate.
	/// \return The value that who issues the SSL certificate.
	virtual const zchar_t* getCertIssuedBy() = 0;

	/// \brief Get the serial number of the SSL certificate.
	/// \return The serial number of the SSL certificate.
	virtual const zchar_t* getCertSerialNum() = 0;

	/// \brief get the SSL certificate's fingerprint.
	/// \return The SSL certificate's fingerprint.
	virtual const zchar_t* getCertFingerprint() = 0;
};

/// \brief NetworkConnection helper interface.
///
class IZoomVideoSDKNetworkConnectionHelper
{
public:
	/// \brief Set the proxy that the user want to use according to your net environment.
	/// \param [out] proxy_setting The proxy ip address and port that user want to use, should be organized like ip:port, such as '255.255.255.255:8080'. Using ';' to separate multi proxies.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors configureProxy(ProxySettings& proxy_setting) = 0;
};
END_ZOOM_VIDEO_SDK_NAMESPACE
#endif