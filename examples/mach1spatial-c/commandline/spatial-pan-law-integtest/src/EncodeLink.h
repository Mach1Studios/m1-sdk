#ifndef MACH1SPATIALSDK_ENCODELINK_H
#define MACH1SPATIALSDK_ENCODELINK_H

#include "RtAudioLink.h"

#include <Mach1Encode.h>

namespace Mach1 {

class EncodeLink : public virtual RtAudioLink {
public:
    void Process(AudioBuffers &buffers, double playback_time) override;

    void SetInputMode(Mach1EncodeInputModeType inputMode);
    void SetOutputMode(Mach1EncodeOutputModeType outputMode);

    void SetAzimuth(float azimuthFromMinus1To1);
    void SetAzimuthDegrees(float azimuthDegrees);
    void SetAzimuthRadians(float azimuthRadians);

    void SetElevation(float elevationFromMinus1to1);
    void SetElevationDegrees(float elevationFromMinus90to90);
    void SetElevationRadians(float elevationFromMinusHalfPItoHalfPI);

    void SetEqualPowerMode(bool is_on);
    void SetIsotropicMode(bool is_on);

    void SetFrontSurroundPerspective(bool frontSurroundPerspective);
    void SetOutputGain(float outputGainMultipler, bool isDecibel);

    void SetOrbitRotation(float orbitRotationFromMinusOnetoOne);
    void SetOrbitRotationDegrees(float orbitRotationDegrees);
    void SetOrbitRotationRadians(float orbitRotationRadians);

    void SetDiverge(float divergeFromMinus1To1);
    void SetStereoSpread(float sSpreadFrom0to1);
    void SetAutoOrbit(bool autoOrbit);

    void GeneratePointResults();
    int GetPointsCount();


private:
    void ResolvePannerMode();

private:
    Mach1Encode m_encode;
    bool m_is_equal_power = false;
    bool m_is_isotropic = false;
};

} // Mach1

#endif //MACH1SPATIALSDK_ENCODELINK_H
