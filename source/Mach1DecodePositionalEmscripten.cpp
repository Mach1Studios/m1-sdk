#include <emscripten/bind.h>

#include "Mach1DecodePositionalCAPI.h"
#include "include/cpp/Mach1DecodePositional.cpp"

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
        .value("Mach1PlatformiOSPortrait_YawOnly", Mach1PlatformiOSPortrait_YawOnly)
        ;

    enum_<Mach1DecodeAlgoType>("Mach1DecodeAlgoType")
        .value("Mach1DecodeAlgoSpatial_8", Mach1DecodeAlgoSpatial_8)
        .value("Mach1DecodeAlgoSpatialAlt_8", Mach1DecodeAlgoSpatialAlt_8)
        .value("Mach1DecodeAlgoHorizon_4", Mach1DecodeAlgoHorizon_4)
        .value("Mach1DecodeAlgoHorizonPairs", Mach1DecodeAlgoHorizonPairs)
        .value("Mach1DecodeAlgoSpatialPairs", Mach1DecodeAlgoSpatialPairs)
        .value("Mach1DecodeAlgoSpatial_12", Mach1DecodeAlgoSpatial_12)
        .value("Mach1DecodeAlgoSpatial_14", Mach1DecodeAlgoSpatial_14)
        .value("Mach1DecodeAlgoSpatial_18", Mach1DecodeAlgoSpatial_18)
        .value("Mach1DecodeAlgoSpatial_22", Mach1DecodeAlgoSpatial_22)
        .value("Mach1DecodeAlgoSpatial_32", Mach1DecodeAlgoSpatial_32)
        .value("Mach1DecodeAlgoSpatial_36", Mach1DecodeAlgoSpatial_36)
        .value("Mach1DecodeAlgoSpatial_48", Mach1DecodeAlgoSpatial_48)
        .value("Mach1DecodeAlgoSpatial_60", Mach1DecodeAlgoSpatial_60)
        ;

	register_vector<float>("VectorFloat");

    value_object<Mach1Point3D>("Mach1Point3D")
		.field("x", &Mach1Point3D::x)
		.field("y", &Mach1Point3D::y)
		.field("z", &Mach1Point3D::z)
        ;

    value_object<Mach1Point4D>("Mach1Point4D")
        .field("x", &Mach1Point4D::x)
        .field("y", &Mach1Point4D::y)
        .field("z", &Mach1Point4D::z)
        .field("w", &Mach1Point4D::w)
        ;

    class_<Mach1DecodePositional>("Mach1DecodePositionalInternal")
        .constructor<>()
        .function("setPlatformType", &Mach1DecodePositional::setPlatformType)
        .function("setDecodeAlgoType", &Mach1DecodePositional::setDecodeAlgoType)
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
        .function("getCurrentAngle", &Mach1DecodePositional::getCurrentAngle)
        .function("getCoefficientsRotation", &Mach1DecodePositional::getCoefficientsRotation)
        .function("setFilterSpeed", &Mach1DecodePositional::setFilterSpeed)
        .function("setUseBlendMode", &Mach1DecodePositional::setUseBlendMode)
        .function("setIgnoreTopBottom", &Mach1DecodePositional::setIgnoreTopBottom)
        .function("setAttenuationCurveBlendMode", &Mach1DecodePositional::setAttenuationCurveBlendMode)
        .function("getCoefficientsInterior", &Mach1DecodePositional::getCoefficientsInterior)
        ;
}
