#include <emscripten/bind.h>

#ifdef M1ENCODE_INLINE_DECODE
#    include "Mach1DecodeCAPI.h"
#endif

#include "../include/Mach1Encode.h"
#include "Mach1EncodeCAPI.h"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(Mach1Encode) {
    enum_<Mach1EncodeInputMode>("Mach1EncodeInputMode")
        .value("Mono", Mono)
        .value("Stereo", Stereo)
        .value("LCR", LCR)
        .value("Quad", Quad)
        .value("LCRS", LCRS)
        .value("AFormat", AFormat)
        .value("FiveDotZero", FiveDotZero)
        .value("FiveDotOneFilm", FiveDotOneFilm)
        .value("FiveDotOneDTS", FiveDotOneDTS)
        .value("FiveDotOneSMTPE", FiveDotOneSMTPE)
        .value("BFOAACN", BFOAACN)
        .value("BFOAFUMA", BFOAFUMA)
        .value("B2OAACN", B2OAACN)
        .value("B2OAFUMA", B2OAFUMA)
        .value("B3OAACN", B3OAACN)
        .value("B3OAFUMA", B3OAFUMA);

    enum_<Mach1EncodeOutputMode>("Mach1EncodeOutputMode")
        .value("M1Spatial_4", M1Spatial_4)
        .value("M1Spatial_8", M1Spatial_8)
        .value("M1Spatial_14", M1Spatial_14);

    enum_<Mach1EncodePannerMode>("Mach1EncodePannerMode")
        .value("IsotropicLinear", IsotropicLinear)
        .value("IsotropicEqualPower", IsotropicEqualPower)
        .value("PeriphonicLinear", PeriphonicLinear);

#ifdef M1ENCODE_INLINE_DECODE
    enum_<Mach1DecodeMode>("Mach1DecodeMode")
        .value("M1Spatial_4", M1Spatial_4)
        .value("M1Spatial_8", M1Spatial_8)
        .value("M1Spatial_14", M1Spatial_14);
#endif

    register_vector<float>("VectorFloat");
    register_vector<std::string>("VectorString");
    register_vector<Mach1Point3D>("VectorMach1Point3D");
    register_vector<std::vector<float> >("VectorVectorFloat");

    value_object<Mach1Point3D>("Mach1Point3D")
        .field("x", &Mach1Point3D::x)
        .field("y", &Mach1Point3D::y)
        .field("z", &Mach1Point3D::z);

    class_<Mach1Encode<float> >("Mach1EncodeInternal")
        .constructor<>()
        .function("getPoints", &Mach1Encode<float>::getPoints)
        .function("getGains", &Mach1Encode<float>::getGains)
        .function("getPointsNames", &Mach1Encode<float>::getPointsNames)
        .function("getGainsForInputChannelNamed", &Mach1Encode<float>::getGainsForInputChannelNamed)

        .function("generatePointResults", &Mach1Encode<float>::generatePointResults)
        .function("getPointsCount", &Mach1Encode<float>::getPointsCount)

#ifdef M1ENCODE_INLINE_DECODE
        .function("getResultingCoeffsDecoded", &Mach1Encode<float>::getResultingCoeffsDecoded)
#endif
        .function("getInputMode", &Mach1Encode<float>::getInputMode)
        .function("getOutputMode", &Mach1Encode<float>::getOutputMode)
        .function("getInputChannelsCount", &Mach1Encode<float>::setInputMode)
        .function("getOutputChannelsCount", &Mach1Encode<float>::setOutputMode)

        .function("setInputMode", &Mach1Encode<float>::setInputMode)
        .function("setOutputMode", &Mach1Encode<float>::setOutputMode)

        .function("setAzimuth", &Mach1Encode<float>::setAzimuth)
        .function("setAzimuthDegrees", &Mach1Encode<float>::setAzimuthDegrees)
        .function("setAzimuthRadians", &Mach1Encode<float>::setAzimuthRadians)

        .function("setDiverge", &Mach1Encode<float>::setDiverge)

        .function("setElevation", &Mach1Encode<float>::setElevation)
        .function("setElevationDegrees", &Mach1Encode<float>::setElevationDegrees)
        .function("setElevationRadians", &Mach1Encode<float>::setElevationRadians)

        .function("setPannerMode", &Mach1Encode<float>::setPannerMode)
        .function("setFrontSurroundPerspective", &Mach1Encode<float>::setFrontSurroundPerspective)
        .function("setOutputGain", &Mach1Encode<float>::setOutputGain)
        .function("getOutputGain", &Mach1Encode<float>::getOutputGain)
        .function("getGainCompensationActive", &Mach1Encode<float>::getGainCompensationActive)
        .function("setGainCompensationActive", &Mach1Encode<float>::setGainCompensationActive)
        .function("getGainCompensation", &Mach1Encode<float>::getGainCompensation)

        .function("setAutoOrbit", &Mach1Encode<float>::setAutoOrbit)
        .function("setOrbitRotation", &Mach1Encode<float>::setOrbitRotation)
        .function("setOrbitRotationDegrees", &Mach1Encode<float>::setOrbitRotationDegrees)
        .function("setOrbitRotationRadians", &Mach1Encode<float>::setOrbitRotationRadians)
        .function("setStereoSpread", &Mach1Encode<float>::setStereoSpread);
}
