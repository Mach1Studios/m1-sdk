#include <emscripten/bind.h>

#ifdef M1ENCODE_INLINE_DECODE
#    include "Mach1DecodeCAPI.h"
#endif
#include "../include/Mach1Encode.cpp"
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
        .value("M1Spatial_12", M1Spatial_12)
        .value("M1Spatial_14", M1Spatial_14);

    enum_<Mach1EncodePannerMode>("Mach1EncodePannerMode")
        .value("IsotropicLinear", IsotropicLinear)
        .value("IsotropicEqualPower", IsotropicEqualPower)
        .value("PeriphonicLinear", PeriphonicLinear);

#ifdef M1ENCODE_INLINE_DECODE
    enum_<Mach1DecodeMode>("Mach1DecodeMode")
        .value("M1Spatial_4", M1Spatial_4)
        .value("M1Spatial_8", M1Spatial_8)
        .value("M1Spatial_12", M1Spatial_12)
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
