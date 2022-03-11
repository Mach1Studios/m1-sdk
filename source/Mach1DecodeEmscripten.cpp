#include <emscripten/bind.h>

#include "Mach1DecodeCAPI.h"
#include "include/cpp/Mach1Decode.cpp"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(Mach1Decode) {
    enum_<Mach1PlatformType>("Mach1PlatformType")
        .value("Mach1PlatformDefault", Mach1PlatformDefault)
        .value("Mach1PlatformUnity", Mach1PlatformUnity)
        .value("Mach1PlatformUE", Mach1PlatformUE)
        .value("Mach1PlatformOfEasyCam", Mach1PlatformOfEasyCam)
        .value("Mach1PlatformAndroid", Mach1PlatformAndroid)
        .value("Mach1PlatformiOS", Mach1PlatformiOS)
        ;
    
	enum_<Mach1DecodeAlgoType>("Mach1DecodeAlgoType")
        .value("Mach1DecodeAlgoSpatial", Mach1DecodeAlgoSpatial)
        .value("Mach1DecodeAlgoAltSpatial", Mach1DecodeAlgoAltSpatial)
        .value("Mach1DecodeAlgoHorizon", Mach1DecodeAlgoHorizon)
        .value("Mach1DecodeAlgoHorizonPairs", Mach1DecodeAlgoHorizonPairs)
        .value("Mach1DecodeAlgoSpatialPairs", Mach1DecodeAlgoSpatialPairs)
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

    class_<Mach1Decode>("Mach1DecodeInternal")
        .constructor<>()
        .function("setPlatformType", &Mach1Decode::setPlatformType)
        .function("setDecodeAlgoType", &Mach1Decode::setDecodeAlgoType)
        .function("decode", &Mach1Decode::decode)
        .function("decodeCoeffs", &Mach1Decode::decodeCoeffs)
        .function("setFilterSpeed", &Mach1Decode::setFilterSpeed)
        .function("getFormatChannelCount", &Mach1Decode::getFormatChannelCount)
        .function("setRotation", &Mach1Decode::setRotation)
        .function("setRotationDegrees", &Mach1Decode::setRotationDegrees)
        .function("setRotationRadians", &Mach1Decode::setRotationRadians)
        .function("setRotationQuat", &Mach1Decode::setRotationQuat)
        .function("beginBuffer", &Mach1Decode::beginBuffer)
        .function("endBuffer", &Mach1Decode::endBuffer)
        .function("getCurrentTime", &Mach1Decode::getCurrentTime)
        .function("getCurrentAngle", &Mach1Decode::getCurrentAngle)
        .function("getLog", &Mach1Decode::getLog)
        ;
}
