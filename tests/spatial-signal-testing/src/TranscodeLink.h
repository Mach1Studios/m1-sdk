#ifndef MACH1SPATIALSDK_TRANSCODELINK_H
#define MACH1SPATIALSDK_TRANSCODELINK_H

#include "RtAudioLink.h"

#include <Mach1Transcode.h>

namespace Mach1 {

class TranscodeLink : public RtAudioLink {
public:
    void Process(AudioBuffers &buffers, double playback_time) override;

    bool ProcessConversionPath();

    void SetLFESub(std::vector<int> subChannelIndices, int sampleRate);
    void SetSpatialDownmixer(float corrThreshold = 0.1);
    void SetInputFormat(int inFmt);
    void SetInputFormatCustomPointsJson(const std::string& inJson);
    void SetInputFormatCustomPoints(std::vector<Mach1Point3D> points);
    void SetOutputFormat(int outFmt);
    void SetOutputFormatCustomPointsJson(const std::string& outJson);
    void SetOutputFormatCustomPoints(std::vector<Mach1Point3D> points);
    void SetCustomPointsSamplerCallback(Mach1Point3D *(*callback)(long long, int &));

private:
    Mach1Transcode<float> m_transcode{};

};

} // Mach1

#endif //MACH1SPATIALSDK_TRANSCODELINK_H
