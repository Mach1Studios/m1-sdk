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
        .value("Mach1EncodeInputModeLCRS", Mach1EncodeInputModeLCRS)
        .value("Mach1EncodeInputModeAFormat", Mach1EncodeInputModeAFormat)
        .value("Mach1EncodeInputModeBFormat", Mach1EncodeInputModeBFormat)
        .value("Mach1EncodeInputModeBFOAACN", Mach1EncodeInputModeBFOAACN)
        .value("Mach1EncodeInputModeBFOAFUMA", Mach1EncodeInputModeBFOAFUMA)
        .value("Mach1EncodeInputModeB2OAACN", Mach1EncodeInputModeB2OAACN)
        .value("Mach1EncodeInputModeB2OAFUMA", Mach1EncodeInputModeB2OAFUMA)
        .value("Mach1EncodeInputModeB3OAACN", Mach1EncodeInputModeB3OAACN)
        .value("Mach1EncodeInputModeB3OAFUMA", Mach1EncodeInputModeB3OAFUMA)
        .value("Mach1EncodeInputModeLCR", Mach1EncodeInputModeLCR)
        ;
		
    enum_<Mach1EncodeOutputModeType>("Mach1EncodeOutputModeType")
        .value("Mach1EncodeOutputModeM1Horizon", Mach1EncodeOutputModeM1Horizon)
        .value("Mach1EncodeOutputModeM1Spatial", Mach1EncodeOutputModeM1Spatial)
        .value("Mach1EncodeOutputModeM1SpatialPlus", Mach1EncodeOutputModeM1SpatialPlus)
        .value("Mach1EncodeOutputModeM1SpatialPlusPlus", Mach1EncodeOutputModeM1SpatialPlusPlus)
        .value("Mach1EncodeOutputModeM1SpatialExt", Mach1EncodeOutputModeM1SpatialExt)
        .value("Mach1EncodeOutputModeM1SpatialExtPlus", Mach1EncodeOutputModeM1SpatialExtPlus)
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

		.function("getResultingCoeffsDecoded", &Mach1Encode::getResultingCoeffsDecoded)
		.function("getResultingVolumesDecoded", &Mach1Encode::getResultingVolumesDecoded)

		.function("setInputMode", &Mach1Encode::setInputMode)
		.function("setOutputMode", &Mach1Encode::setOutputMode)

		.function("setRotation", &Mach1Encode::setRotation)
        .function("setAzimuth", &Mach1Encode::setAzimuth)
        .function("setAzimuthDegrees", &Mach1Encode::setAzimuthDegrees)
        .function("setAzimuthRadians", &Mach1Encode::setAzimuthRadians)

		.function("setDiverge", &Mach1Encode::setDiverge)

        .function("setPitch", &Mach1Encode::setPitch)
        .function("setElevation", &Mach1Encode::setElevation)
        .function("setElevationDegrees", &Mach1Encode::setElevationDegrees)
        .function("setElevationRadians", &Mach1Encode::setElevationRadians)
        .function("setIsotropicEncode", &Mach1Encode::setIsotropicEncode)

        .function("setStereoRotate", &Mach1Encode::setStereoRotate)
        .function("setOrbitRotation", &Mach1Encode::setOrbitRotation)
        .function("setOrbitRotationDegrees", &Mach1Encode::setOrbitRotationDegrees)
        .function("setOrbitRotationRadians", &Mach1Encode::setOrbitRotationRadians)

        .function("setStereoSpread", &Mach1Encode::setStereoSpread)
        .function("setAutoOrbit", &Mach1Encode::setAutoOrbit)
        ;
}
