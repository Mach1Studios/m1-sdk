#include <emscripten/bind.h>

#include "Mach1TranscodeCAPI.h"
#include "external/cpp/Mach1Transcode.cpp"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(Mach1Transcode) {
    const int Mach1TranscodeMAXCHANS = 64;

    enum_<Mach1TranscodeFormatType>("Mach1TranscodeFormatType")
        .value("Mach1TranscodeFormatEmpty", Mach1TranscodeFormatEmpty)
        .value("Mach1TranscodeFormatFuMa", Mach1TranscodeFormatFuMa)
        .value("Mach1TranscodeFormatACNSN3D", Mach1TranscodeFormatACNSN3D)
        .value("Mach1TranscodeFormatM1Horizon", Mach1TranscodeFormatM1Horizon)
        .value("Mach1TranscodeFormatM1HorizonS", Mach1TranscodeFormatM1HorizonS)
        .value("Mach1TranscodeFormatM1HorizonPairs", Mach1TranscodeFormatM1HorizonPairs)
        .value("Mach1TranscodeFormatM1Spatial", Mach1TranscodeFormatM1Spatial)
        .value("Mach1TranscodeFormatM1SpatialS", Mach1TranscodeFormatM1SpatialS)
        .value("Mach1TranscodeFormatM1SpatialPairs", Mach1TranscodeFormatM1SpatialPairs)
        .value("Mach1TranscodeFormatM1SpatialPlus", Mach1TranscodeFormatM1SpatialPlus)
        .value("Mach1TranscodeFormatM1SpatialPlusPlus", Mach1TranscodeFormatM1SpatialPlusPlus)
        .value("Mach1TranscodeFormatM1SpatialExtended", Mach1TranscodeFormatM1SpatialExtended)
        .value("Mach1TranscodeFormatM1SpatialExtendedPlus", Mach1TranscodeFormatM1SpatialExtendedPlus)
        .value("Mach1TranscodeFormatMono", Mach1TranscodeFormatMono) 
        .value("Mach1TranscodeFormatStereo", Mach1TranscodeFormatStereo)
        .value("Mach1TranscodeFormatLCR", Mach1TranscodeFormatLCR)
        .value("Mach1TranscodeFormatFiveOh", Mach1TranscodeFormatFiveOh)
        .value("Mach1TranscodeFormatFiveOneFilm", Mach1TranscodeFormatFiveOneFilm)
        .value("Mach1TranscodeFormatFiveOneFilm_Cinema", Mach1TranscodeFormatFiveOneFilm_Cinema)
        .value("Mach1TranscodeFormatFiveOneSmpte", Mach1TranscodeFormatFiveOneSmpte)
        .value("Mach1TranscodeFormatFiveOneDts", Mach1TranscodeFormatFiveOneDts)
        .value("Mach1TranscodeFormatSixOh", Mach1TranscodeFormatSixOh)
        .value("Mach1TranscodeFormatSevenOnePt", Mach1TranscodeFormatSevenOnePt)
        .value("Mach1TranscodeFormatSevenOnePt_Cinema", Mach1TranscodeFormatSevenOnePt_Cinema)
        .value("Mach1TranscodeFormatACNSN3DO2A", Mach1TranscodeFormatACNSN3DO2A)
        .value("Mach1TranscodeFormatFuMaO2A", Mach1TranscodeFormatFuMaO2A)
        .value("Mach1TranscodeFormatTBE", Mach1TranscodeFormatTBE)
        .value("Mach1TranscodeFormatACNSN3DO3A", Mach1TranscodeFormatACNSN3DO3A)
        .value("Mach1TranscodeFormatFuMaO3A", Mach1TranscodeFormatFuMaO3A)
        .value("Mach1TranscodeFormatSevenOneSDDS", Mach1TranscodeFormatSevenOneSDDS)
        .value("Mach1TranscodeFormatSevenZeroSDDS", Mach1TranscodeFormatSevenZeroSDDS)
        .value("Mach1TranscodeFormatSevenOneTwo", Mach1TranscodeFormatSevenOneTwo)
        .value("Mach1TranscodeFormatSevenZeroTwo", Mach1TranscodeFormatSevenZeroTwo)
        .value("Mach1TranscodeFormatNineOne", Mach1TranscodeFormatNineOne)
        .value("Mach1TranscodeFormatNineZero", Mach1TranscodeFormatNineZero)
        .value("Mach1TranscodeFormatStereo_Cinema", Mach1TranscodeFormatStereo_Cinema)
        .value("Mach1TranscodeFormatFiveOneTwo", Mach1TranscodeFormatFiveOneTwo)
        .value("Mach1TranscodeFormatFiveZeroTwo", Mach1TranscodeFormatFiveZeroTwo)
        .value("Mach1TranscodeFormatFiveOneFour", Mach1TranscodeFormatFiveOneFour)
        .value("Mach1TranscodeFormatFiveZeroFour", Mach1TranscodeFormatFiveZeroFour)
        .value("Mach1TranscodeFormatSevenOneFour", Mach1TranscodeFormatSevenOneFour)
        .value("Mach1TranscodeFormatSevenZeroFour", Mach1TranscodeFormatSevenZeroFour)
        .value("Mach1TranscodeFormatSevenZero_Cinema", Mach1TranscodeFormatSevenZero_Cinema)
        .value("Mach1TranscodeFormatM1SpatialFaces", Mach1TranscodeFormatM1SpatialFaces)
        .value("Mach1TranscodeFormatNineOneFour", Mach1TranscodeFormatNineOneFour)
        .value("Mach1TranscodeFormatNineOneSix", Mach1TranscodeFormatNineOneSix)
        .value("Mach1TranscodeFormatMarcoSixteen", Mach1TranscodeFormatMarcoSixteen)
        .value("Mach1TranscodeFormatTTPoints", Mach1TranscodeFormatTTPoints)
        .value("Mach1TranscodeFormatACNSN3DmaxRE1oa", Mach1TranscodeFormatACNSN3DmaxRE1oa)
        .value("Mach1TranscodeFormatACNSN3DmaxRE2oa", Mach1TranscodeFormatACNSN3DmaxRE2oa)
        .value("Mach1TranscodeFormatACNSN3DmaxRE3oa", Mach1TranscodeFormatACNSN3DmaxRE3oa)
        .value("Mach1TranscodeFormatACNSN3DmaxRE4oa", Mach1TranscodeFormatACNSN3DmaxRE4oa)
        .value("Mach1TranscodeFormatACNSN3DmaxRE5oa", Mach1TranscodeFormatACNSN3DmaxRE5oa)
        .value("Mach1TranscodeFormatACNSN3DmaxRE6oa", Mach1TranscodeFormatACNSN3DmaxRE6oa)
        .value("Mach1TranscodeFormatACNSN3DmaxRE7oa", Mach1TranscodeFormatACNSN3DmaxRE7oa)
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

    class_<Mach1Transcode>("Mach1TranscodeInternal")
        .constructor<>()
        .function("getInputNumChannels", &Mach1Transcode::getInputNumChannels)
        .function("getOutputNumChannels", &Mach1Transcode::getOutputNumChannels)
		.function("getFormatFromString", &Mach1Transcode::getFormatFromString)
		.function("getFormatName", &Mach1Transcode::getFormatName)

		.function("processNormalization", &Mach1Transcode::processNormalization)
		.function("processMasterGain", &Mach1Transcode::processMasterGain)

		.function("getResultingCoeffsDecoded", &Mach1Transcode::getResultingCoeffsDecoded)
		.function("getResultingVolumesDecoded", &Mach1Transcode::getResultingVolumesDecoded)

		.function("db2level", &Mach1Transcode::db2level)
		.function("level2db", &Mach1Transcode::level2db)

		.function("setLFESub", &Mach1Transcode::setLFESub)
        .function("setSpatialDownmixer", &Mach1Transcode::setSpatialDownmixer)
        .function("getSpatialDownmixerPossibility", &Mach1Transcode::getSpatialDownmixerPossibility)

		.function("setInputFormat", &Mach1Transcode::setInputFormat)
        .function("setInputFormatADM", &Mach1Transcode::setInputFormatADM)
        .function("setInputFormatTTJson", &Mach1Transcode::setInputFormatTTJson)
        .function("setInputFormatTTPoints", &Mach1Transcode::setInputFormatTTPoints)

        .function("setOutputFormat", &Mach1Transcode::setOutputFormat)
        .function("setOutputFormatTTJson", &Mach1Transcode::setOutputFormatTTJson)
        .function("setOutputFormatTTPoints", &Mach1Transcode::setOutputFormatTTPoints)

        .function("processConversionPath", &Mach1Transcode::processConversionPath)
        .function("getMatrixConversion", &Mach1Transcode::getMatrixConversion)
        .function("processConversion", &Mach1Transcode::processConversion)

        .function("getFormatConversionPath", &Mach1Transcode::getFormatConversionPath)
        ;
}
