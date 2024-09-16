#include <emscripten/bind.h>

#include "Mach1TranscodeCAPI.h"
#include "../include/Mach1Transcode.tpp"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(Mach1Transcode) {
    register_vector<float>("VectorFloat");
    register_vector<std::string>("VectorString");
    register_vector<Mach1Point3D>("VectorMach1Point3D");
    register_vector<std::vector<float> >("VectorVectorFloat");

    value_object<Mach1Point3D>("Mach1Point3D")
        .field("x", &Mach1Point3D::x)
        .field("y", &Mach1Point3D::y)
        .field("z", &Mach1Point3D::z);

    class_<Mach1Transcode>("Mach1TranscodeInternal")
        .constructor<>()
        .function("getInputNumChannels", &Mach1Transcode::getInputNumChannels)
        .function("getOutputNumChannels", &Mach1Transcode::getOutputNumChannels)
        .function("getFormatFromString", &Mach1Transcode::getFormatFromString)
        .function("getFormatName", &Mach1Transcode::getFormatName)
        .function("getAllFormatNames", &Mach1Transcode::getAllFormatNames)

        .function("processNormalization", &Mach1Transcode::processNormalization)
        .function("processMasterGain", &Mach1Transcode::processMasterGain)

        .function("db2level", &Mach1Transcode::db2level)
        .function("level2db", &Mach1Transcode::level2db)

        .function("setLFESub", &Mach1Transcode::setLFESub)
        .function("setSpatialDownmixer", &Mach1Transcode::setSpatialDownmixer)
        .function("getSpatialDownmixerPossibility", &Mach1Transcode::getSpatialDownmixerPossibility)

        .function("setInputFormat", &Mach1Transcode::setInputFormat)
        .function("setInputFormatCustomPointsJson", &Mach1Transcode::setInputFormatCustomPointsJson)
        .function("setInputFormatCustomPoints", &Mach1Transcode::setInputFormatCustomPoints)

        .function("setOutputFormat", &Mach1Transcode::setOutputFormat)
        .function("setOutputFormatCustomPointsJson", &Mach1Transcode::setOutputFormatCustomPointsJson)
        .function("setOutputFormatCustomPoints", &Mach1Transcode::setOutputFormatCustomPoints)

        .function("processConversionPath", &Mach1Transcode::processConversionPath)
        .function("getMatrixConversion", &Mach1Transcode::getMatrixConversion)
        .function("processConversion", &Mach1Transcode::processConversion)

        .function("getFormatConversionPath", &Mach1Transcode::getFormatConversionPath);
}
