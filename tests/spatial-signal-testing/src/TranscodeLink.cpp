#include "TranscodeLink.h"

#include "Settings.h"
#include <utility>

using namespace Mach1;

void TranscodeLink::Process(AudioBuffers &buffers, double playback_time) {
#if REBUFFERED_TRANSCODE
    m_transcode.processConversionRebuffer(buffers.GetOutputBuffers(), buffers.GetOutputBuffers(), buffers.GetBufferSize());
#else
    m_transcode.processConversion(buffers.GetOutputBuffers(), buffers.GetOutputBuffers(), buffers.GetBufferSize());
#endif
}

void TranscodeLink::SetLFESub(std::vector<int> subChannelIndices, int sampleRate) {
    m_transcode.setLFESub(std::move(subChannelIndices), sampleRate);
}

void TranscodeLink::SetSpatialDownmixer(float corrThreshold) {
    m_transcode.setSpatialDownmixer(corrThreshold);
}

void TranscodeLink::SetInputFormat(int inFmt) {
    m_transcode.setInputFormat(inFmt);
}

void TranscodeLink::SetInputFormatCustomPointsJson(const std::string &inJson) {
    m_transcode.setInputFormatCustomPointsJson(inJson);
}

void TranscodeLink::SetInputFormatCustomPoints(std::vector<Mach1Point3D> points) {
    m_transcode.setInputFormatCustomPoints(std::move(points));
}

void TranscodeLink::SetOutputFormat(int outFmt) {
    m_transcode.setOutputFormat(outFmt);
}

void TranscodeLink::SetOutputFormatCustomPointsJson(const std::string &outJson) {
    m_transcode.setOutputFormatCustomPointsJson(outJson);
}

void TranscodeLink::SetOutputFormatCustomPoints(std::vector<Mach1Point3D> points) {
    m_transcode.setOutputFormatCustomPoints(std::move(points));
}

void TranscodeLink::SetCustomPointsSamplerCallback(Mach1Point3D *(*callback)(long long int, int &)) {

}

bool TranscodeLink::ProcessConversionPath() {
    return m_transcode.processConversionPath();
}
