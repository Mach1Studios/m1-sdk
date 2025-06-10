#include <emscripten/bind.h>

#include "../include/Mach1DecodePositional.cpp"
#include "Mach1DecodePositionalCAPI.h"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(Mach1DecodePositional) {
    enum_<Mach1PlatformType>("Mach1PlatformType")
        .value("Mach1PlatformDefault", Mach1PlatformDefault)
        .value("Mach1PlatformUnity", Mach1PlatformUnity)
        .value("Mach1PlatformUE", Mach1PlatformUE)
        .value("Mach1PlatformOfEasyCam", Mach1PlatformOfEasyCam)
        .value("Mach1PlatformAndroid", Mach1PlatformAndroid)
        .value("Mach1PlatformiOS", Mach1PlatformiOS)
        .value("Mach1PlatformiOSTableTop_ZVertical", Mach1PlatformiOSTableTop_ZVertical)
        .value("Mach1PlatformiOSPortraitHandheld_YVertical", Mach1PlatformiOSPortraitHandheld_YVertical)
        .value("Mach1PlatformiOSPortrait_YawOnly", Mach1PlatformiOSPortrait_YawOnly);

    enum_<Mach1DecodeMode>("Mach1DecodeMode")
        .value("M1Spatial_4", M1DecodeSpatial_4)
        .value("M1Spatial_8", M1DecodeSpatial_8)
        .value("M1Spatial_14", M1DecodeSpatial_14);

    register_vector<float>("VectorFloat");

    value_object<Mach1Point3D>("Mach1Point3D")
        .field("x", &Mach1Point3D::x)
        .field("y", &Mach1Point3D::y)
        .field("z", &Mach1Point3D::z);

    value_object<Mach1Point4D>("Mach1Point4D")
        .field("x", &Mach1Point4D::x)
        .field("y", &Mach1Point4D::y)
        .field("z", &Mach1Point4D::z)
        .field("w", &Mach1Point4D::w);

    class_<Mach1DecodePositional>("Mach1DecodePositionalInternal")
        .constructor<>()
        .function("setPlatformType", &Mach1DecodePositional::setPlatformType)
        .function("setDecodeMode", &Mach1DecodePositional::setDecodeMode)
        .function("setMuteWhenOutsideObject", &Mach1DecodePositional::setMuteWhenOutsideObject)
        .function("setMuteWhenInsideObject", &Mach1DecodePositional::setMuteWhenInsideObject)
        .function("setUseAttenuation", &Mach1DecodePositional::setUseAttenuation)
        .function("setAttenuationCurve", &Mach1DecodePositional::setAttenuationCurve)
        .function("setUsePlaneCalculation", &Mach1DecodePositional::setUsePlaneCalculation)
        .function("setUseYawForRotation", &Mach1DecodePositional::setUseYawForRotation)
        .function("setUsePitchForRotation", &Mach1DecodePositional::setUsePitchForRotation)
        .function("setUseRollForRotation", &Mach1DecodePositional::setUseRollForRotation)
        .function("setListenerPosition", &Mach1DecodePositional::setListenerPosition)
        .function("setListenerRotation", &Mach1DecodePositional::setListenerRotation)
        .function("setListenerRotationQuat", &Mach1DecodePositional::setListenerRotationQuat)
        .function("setDecoderAlgoPosition", &Mach1DecodePositional::setDecoderAlgoPosition)
        .function("setDecoderAlgoRotation", &Mach1DecodePositional::setDecoderAlgoRotation)
        .function("setDecoderAlgoRotationQuat", &Mach1DecodePositional::setDecoderAlgoRotationQuat)
        .function("setDecoderAlgoScale", &Mach1DecodePositional::setDecoderAlgoScale)
        .function("evaluatePositionResults", &Mach1DecodePositional::evaluatePositionResults)
        .function("getCoefficients", &Mach1DecodePositional::getCoefficients)
        .function("getDist", &Mach1DecodePositional::getDist)
        .function("getFormatChannelCount", &Mach1DecodePositional::getFormatChannelCount)
        .function("getFormatCoeffCount", &Mach1DecodePositional::getFormatCoeffCount)
        .function("getCurrentAngle", &Mach1DecodePositional::getCurrentAngle)
        .function("getPositionalRotation", &Mach1DecodePositional::getPositionalRotation)
        .function("setFilterSpeed", &Mach1DecodePositional::setFilterSpeed);
}
