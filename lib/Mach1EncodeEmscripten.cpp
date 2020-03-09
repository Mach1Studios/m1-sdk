#include <emscripten/bind.h>

#include "Mach1EncodeCAPI.h"
#include "Mach1DecodeCAPI.h"
#include "external/cpp/Mach1Encode.cpp"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(Mach1Encode) {
    enum_<Mach1EncodeInputModeType>("Mach1EncodeInputModeType")
        .value("Mach1EncodeInputModeMono", Mach1EncodeInputModeMono)
        .value("Mach1EncodeInputModeStereo", Mach1EncodeInputModeStereo)
        .value("Mach1EncodeInputModeQuad", Mach1EncodeInputModeQuad)
        .value("Mach1EncodeInputModeAFormat", Mach1EncodeInputModeAFormat)
        .value("Mach1EncodeInputModeBFormat", Mach1EncodeInputModeBFormat)
        ;
		
    enum_<Mach1EncodeOutputModeType>("Mach1EncodeOutputModeType")
        .value("Mach1EncodeOutputMode4Ch", Mach1EncodeOutputMode4Ch)
        .value("Mach1EncodeOutputMode8Ch", Mach1EncodeOutputMode8Ch)
        ;

	enum_<Mach1DecodeAlgoType>("Mach1DecodeAlgoType")
        .value("Mach1DecodeAlgoSpatial", Mach1DecodeAlgoSpatial)
        .value("Mach1DecodeAlgoAltSpatial", Mach1DecodeAlgoAltSpatial)
        .value("Mach1DecodeAlgoHorizon", Mach1DecodeAlgoHorizon)
        .value("Mach1DecodeAlgoHorizonPairs", Mach1DecodeAlgoHorizonPairs)
        .value("Mach1DecodeAlgoSpatialPairs", Mach1DecodeAlgoSpatialPairs)
        ;

	register_vector<float>("VectorFloat");
	register_vector<std::string>("VectorString");
	register_vector<Mach1Point3D>("VectorMach1Point3D");
	register_vector<std::vector<float>>("VectorVectorFloat");

    value_object<Mach1Point3D>("Mach1Point3D")
		.field("x", &Mach1Point3D::x)
		.field("y", &Mach1Point3D::y)
		.field("z", &Mach1Point3D::z)
        ;

    class_<Mach1Encode>("Mach1EncodeInternal")
        .constructor<>()
        .function("getPoints", &Mach1Encode::getPoints)
        .function("getGains", &Mach1Encode::getGains)
		.function("getPointsNames", &Mach1Encode::getPointsNames)
		.function("getGainsForInputChannelNamed", &Mach1Encode::getGainsForInputChannelNamed)

		.function("generatePointResults", &Mach1Encode::generatePointResults)
		.function("getPointsCount", &Mach1Encode::getPointsCount)

		.function("getResultingVolumesDecoded", &Mach1Encode::getResultingVolumesDecoded)

		.function("setInputMode", &Mach1Encode::setInputMode)
		.function("setOutputMode", &Mach1Encode::setOutputMode)
		.function("setRotation", &Mach1Encode::setRotation)
        .function("setAzimuthDegrees", &Mach1Encode::setAzimuthDegrees)
        .function("setAzimuthRadians", &Mach1Encode::setAzimuthRadians)
        .function("setAzimuth0to1", &Mach1Encode::setAzimuth0to1)

		.function("setDiverge", &Mach1Encode::setDiverge)
        .function("setPitch", &Mach1Encode::setPitch)
        .function("setElevationDegrees", &Mach1Encode::setElevationDegrees)
        .function("setElevationRadians", &Mach1Encode::setElevationRadians)
        .function("setElevation0to1", &Mach1Encode::setElevation0to1)
        .function("setIsotropicEncode", &Mach1Encode::setIsotropicEncode)

        .function("setStereoRotate", &Mach1Encode::setStereoRotate)
        .function("setOrbitRotationDegrees", &Mach1Encode::setOrbitRotationDegrees)
        .function("setOrbitRotationRadians", &Mach1Encode::setOrbitRotationRadians)
        .function("setOrbitRotation0to1", &Mach1Encode::setOrbitRotation0to1)

        .function("setStereoSpread", &Mach1Encode::setStereoSpread)
        .function("setAutoOrbit", &Mach1Encode::setAutoOrbit)
        ;
}
