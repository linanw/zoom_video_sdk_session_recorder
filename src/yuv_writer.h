#include "helpers/zoom_video_sdk_video_source_helper_interface.h"

using namespace ZOOMVIDEOSDK;

class YUVWriter : public IZoomVideoSDKVideoSourcePreProcessor 
{
public:
	/// \brief Callback on device capture video frame.
	/// \param rawData See \link YUVProcessDataI420 \endlink.
	virtual void onPreProcessRawData(YUVProcessDataI420* rawData);
};