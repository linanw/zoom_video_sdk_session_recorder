#include "yuv_writer.h"

void YUVWriter::onPreProcessRawData(YUVProcessDataI420* rawData)
{
    // Write YUV data to file
    FILE* file = fopen("output.yuv", "ab");
    if (file)
    {
        fwrite(rawData->GetYBuffer(), 1, rawData->GetWidth() * rawData->GetHeight(), file);
        fwrite(rawData->GetUBuffer(), 1, rawData->GetWidth()* rawData->GetHeight() / 4, file);
        fwrite(rawData->GetVBuffer(), 1, rawData->GetWidth()* rawData->GetHeight() / 4, file);
        fclose(file);
    }
}