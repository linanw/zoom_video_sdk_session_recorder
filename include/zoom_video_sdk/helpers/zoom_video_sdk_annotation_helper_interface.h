/*!
* \file zoom_video_sdk_annotation_helper_interface.h
* \brief annotation helper
*
*/

#ifndef _ZOOM_VIDEO_SDK_ANNOTATION_HELPER_INTERFACE_H_
#define _ZOOM_VIDEO_SDK_ANNOTATION_HELPER_INTERFACE_H_
#include "zoom_video_sdk_def.h"

BEGIN_ZOOM_VIDEO_SDK_NAMESPACE

enum ZoomVideoSDKAnnotationToolType
{
	ZoomVideoSDKAnnotationToolType_None,
	ZoomVideoSDKAnnotationToolType_Pen,
	ZoomVideoSDKAnnotationToolType_HighLighter,
	ZoomVideoSDKAnnotationToolType_AutoLine,
	ZoomVideoSDKAnnotationToolType_AutoRectangle,
	ZoomVideoSDKAnnotationToolType_AutoEllipse,
	ZoomVideoSDKAnnotationToolType_AutoArrow,
	ZoomVideoSDKAnnotationToolType_AutoRectangleFill,
	ZoomVideoSDKAnnotationToolType_AutoEllipseFill,
	ZoomVideoSDKAnnotationToolType_SpotLight,
	ZoomVideoSDKAnnotationToolType_Arrow,
	ZoomVideoSDKAnnotationToolType_ERASER,
	ZoomVideoSDKAnnotationToolType_Textbox,
	ZoomVideoSDKAnnotationToolType_Picker,
	ZoomVideoSDKAnnotationToolType_AutoRectangleSemiFill,
	ZoomVideoSDKAnnotationToolType_AutoEllipseSemiFill,
	ZoomVideoSDKAnnotationToolType_AutoDoubleArrow,
	ZoomVideoSDKAnnotationToolType_AutoDiamond,
	ZoomVideoSDKAnnotationToolType_AutoStampArrow,
	ZoomVideoSDKAnnotationToolType_AutoStampCheck,
	ZoomVideoSDKAnnotationToolType_AutoStampX,
	ZoomVideoSDKAnnotationToolType_AutoStampStar,
	ZoomVideoSDKAnnotationToolType_AutoStampHeart,
	ZoomVideoSDKAnnotationToolType_AutoStampQm,
};

enum ZoomVideoSDKAnnotationClearType
{
	ZoomVideoSDKAnnotationClearType_All,///<Clear all annotations.
	ZoomVideoSDKAnnotationClearType_Others,///<Clear only the others' annotations.
	ZoomVideoSDKAnnotationClearType_My,///<Clear only your own annotations.
};

/// \brief Annotation helper interface.
///
class IZoomVideoSDKAnnotationHelper
{
public:
	virtual ~IZoomVideoSDKAnnotationHelper() {};

	/// \brief Determine whether annotation was disabled or not by the share owner.
	/// \return true disable, false not disable.
	virtual bool isSenderDisableAnnotation() = 0;

	/// \brief Starts annotation.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors startAnnotation() = 0;

	/// \brief Stops annotation.	
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors stopAnnotation() = 0;

	/// \brief Sets the annotation tool type.
	/// \param toolType The specified tool type.
	///For the definitions of the tools, see \link ZoomVideoSDKAnnotationToolType \endlink enum.
	/// \return If the function succeeds, the return value is SDKErr_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	/// \remarks The tool type ZoomVideoSDKAnnotationToolType_Picker and ZoomVideoSDKAnnotationToolType_SpotLight are not support for viewer.
	virtual ZoomVideoSDKErrors setToolType(ZoomVideoSDKAnnotationToolType toolType) = 0;
	
	/// \brief Gets the annotation tool type.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors getToolType(ZoomVideoSDKAnnotationToolType& toolType) = 0;

	/// \brief Sets the annotation tool color.
	/// \param color The specified color.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors setToolColor(unsigned long color) = 0;

	/// \brief Gets the annotation tool color.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors getToolColor(unsigned long& color) = 0;

	/// \brief Sets the annotation tool width.
	/// \param lineWidth The specified tool width.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors setToolWidth(long lineWidth) = 0;

	/// \brief Gets the annotation tool width.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors getToolWidth(long& lineWidth) = 0;

	/// \brief ClearS the annotation content.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///	\Host and manager can clear all and clear self; Share owner can clear all, clear othersand clear self; Attendee can only clear self.
	virtual ZoomVideoSDKErrors clear(ZoomVideoSDKAnnotationClearType clearType) = 0;

	/// \brief Undo one annotation content step.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors undo() = 0;

	/// \brief Redo one annotation content step.
	/// \return If the function succeeds, the return value is ZoomVideoSDKErrors_Success.
	///Otherwise failed. To get extended error information, see \link ZoomVideoSDKErrors \endlink enum.
	virtual ZoomVideoSDKErrors redo() = 0;
	

};
END_ZOOM_VIDEO_SDK_NAMESPACE
#endif