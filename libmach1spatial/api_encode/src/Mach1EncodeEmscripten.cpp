#include <emscripten/bind.h>

#ifdef M1ENCODE_INLINE_DECODE
#include "Mach1DecodeCAPI.h"
#endif
#include "Mach1EncodeCAPI.h"
#include "../include/Mach1Encode.cpp"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(Mach1Encode) {
    enum_<Mach1EncodeInputModeType>("Mach1EncodeInputModeType")
        .value("Mach1EncodeInputModeMono", Mach1EncodeInputModeMono)
        .value("Mach1EncodeInputModeStereo", Mach1EncodeInputModeStereo)
        .value("Mach1EncodeInputModeLCR", Mach1EncodeInputModeLCR)
        .value("Mach1EncodeInputModeQuad", Mach1EncodeInputModeQuad)
        .value("Mach1EncodeInputModeLCRS", Mach1EncodeInputModeLCRS)
        .value("Mach1EncodeInputModeAFormat", Mach1EncodeInputModeAFormat)
        .value("Mach1EncodeInputMode5dot0", Mach1EncodeInputMode5dot0)
        .value("Mach1EncodeInputMode5dot1Film", Mach1EncodeInputMode5dot1Film)
        .value("Mach1EncodeInputMode5dot1DTS", Mach1EncodeInputMode5dot1DTS)
        .value("Mach1EncodeInputMode5dot1SMTPE", Mach1EncodeInputMode5dot1SMTPE)
        .value("Mach1EncodeInputModeBFOAACN", Mach1EncodeInputModeBFOAACN)
        .value("Mach1EncodeInputModeBFOAFUMA", Mach1EncodeInputModeBFOAFUMA)
        .value("Mach1EncodeInputModeB2OAACN", Mach1EncodeInputModeB2OAACN)
        .value("Mach1EncodeInputModeB2OAFUMA", Mach1EncodeInputModeB2OAFUMA)
        .value("Mach1EncodeInputModeB3OAACN", Mach1EncodeInputModeB3OAACN)
        .value("Mach1EncodeInputModeB3OAFUMA", Mach1EncodeInputModeB3OAFUMA);

    enum_<Mach1EncodeOutputModeType>("Mach1EncodeOutputModeType")
        .value("Mach1EncodeOutputModeM1Spatial_4", Mach1EncodeOutputModeM1Spatial_4)
        .value("Mach1EncodeOutputModeM1Spatial_8", Mach1EncodeOutputModeM1Spatial_8)
        .value("Mach1EncodeOutputModeM1Spatial_12", Mach1EncodeOutputModeM1Spatial_12)
        .value("Mach1EncodeOutputModeM1Spatial_14", Mach1EncodeOutputModeM1Spatial_14);

    enum_<Mach1EncodePannerModeType>("Mach1EncodePannerModeType")
        .value("Mach1EncodePannerModeIsotropicLinear", Mach1EncodePannerModeIsotropicLinear)
        .value("Mach1EncodePannerModeIsotropicEqualPower", Mach1EncodePannerModeIsotropicEqualPower)
        .value("Mach1EncodePannerModePeriphonicLinear", Mach1EncodePannerModePeriphonicLinear);

#ifdef M1ENCODE_INLINE_DECODE
    enum_<Mach1DecodeAlgoType>("Mach1DecodeAlgoType")
        .value("Mach1DecodeAlgoSpatial_4", Mach1DecodeAlgoSpatial_4)
        .value("Mach1DecodeAlgoSpatial_8", Mach1DecodeAlgoSpatial_8)
        .value("Mach1DecodeAlgoSpatial_12", Mach1DecodeAlgoSpatial_12)
        .value("Mach1DecodeAlgoSpatial_14", Mach1DecodeAlgoSpatial_14);
#endif

    register_vector<float>("VectorFloat");
    register_vector<std::string>("VectorString");
    register_vector<Mach1Point3D>("VectorMach1Point3D");
    register_vector<std::vector<float>>("VectorVectorFloat");

    value_object<Mach1Point3D>("Mach1Point3D")
        .field("x", &Mach1Point3D::x)
        .field("y", &Mach1Point3D::y)
        .field("z", &Mach1Point3D::z);

    class_<Mach1Encode>("Mach1EncodeInternal")
        .constructor<>()
        .function("getPoints", &Mach1Encode::getPoints)
        .function("getGains", &Mach1Encode::getGains)
        .function("getPointsNames", &Mach1Encode::getPointsNames)
        .function("getGainsForInputChannelNamed", &Mach1Encode::getGainsForInputChannelNamed)

        .function("generatePointResults", &Mach1Encode::generatePointResults)
        .function("getPointsCount", &Mach1Encode::getPointsCount)

#ifdef M1ENCODE_INLINE_DECODE
        .function("getResultingCoeffsDecoded", &Mach1Encode::getResultingCoeffsDecoded)
#endif
        .function("getInputMode", &Mach1Encode::getInputMode)
        .function("getOutputMode", &Mach1Encode::getOutputMode)
        .function("getInputChannelsCount", &Mach1Encode::setInputMode)
        .function("getOutputChannelsCount", &Mach1Encode::setOutputMode)

        .function("setInputMode", &Mach1Encode::setInputMode)
        .function("setOutputMode", &Mach1Encode::setOutputMode)

        .function("setAzimuth", &Mach1Encode::setAzimuth)
        .function("setAzimuthDegrees", &Mach1Encode::setAzimuthDegrees)
        .function("setAzimuthRadians", &Mach1Encode::setAzimuthRadians)

        .function("setDiverge", &Mach1Encode::setDiverge)

        .function("setElevation", &Mach1Encode::setElevation)
        .function("setElevationDegrees", &Mach1Encode::setElevationDegrees)
        .function("setElevationRadians", &Mach1Encode::setElevationRadians)

        .function("setPannerMode", &Mach1Encode::setPannerMode)
        .function("setFrontSurroundPerspective", &Mach1Encode::setFrontSurroundPerspective)
        .function("setOutputGain", &Mach1Encode::setOutputGain)

        .function("setAutoOrbit", &Mach1Encode::setAutoOrbit)
        .function("setOrbitRotation", &Mach1Encode::setOrbitRotation)
        .function("setOrbitRotationDegrees", &Mach1Encode::setOrbitRotationDegrees)
        .function("setOrbitRotationRadians", &Mach1Encode::setOrbitRotationRadians)
        .function("setStereoSpread", &Mach1Encode::setStereoSpread);
}