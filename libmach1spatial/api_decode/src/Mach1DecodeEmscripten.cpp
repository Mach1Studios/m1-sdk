#include <emscripten/bind.h>

#include "../include/Mach1Decode.h"
#include "Mach1DecodeCAPI.h"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(Mach1Decode) {
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
        .value("M1Spatial_12", M1DecodeSpatial_12)
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

    class_<Mach1Decode<float> >("Mach1DecodeInternal")
        .constructor<>()
        .function("setPlatformType", &Mach1Decode<float>::setPlatformType)
        .function("setDecodeMode", &Mach1Decode<float>::setDecodeMode)
        .function("decode", &Mach1Decode<float>::decode)
        .function("decodeCoeffs", &Mach1Decode<float>::decodeCoeffs)
        .function("setFilterSpeed", &Mach1Decode<float>::setFilterSpeed)
        .function("getFormatChannelCount", &Mach1Decode<float>::getFormatChannelCount)
        .function("setRotation", &Mach1Decode<float>::setRotation)
        .function("setRotationDegrees", &Mach1Decode<float>::setRotationDegrees)
        .function("setRotationRadians", &Mach1Decode<float>::setRotationRadians)
        .function("setRotationQuat", &Mach1Decode<float>::setRotationQuat)
        .function("getCurrentTime", &Mach1Decode<float>::getCurrentTime)
        .function("getCurrentAngle", &Mach1Decode<float>::getCurrentAngle)
        .function("getLog", &Mach1Decode<float>::getLog);
}
