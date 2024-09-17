#include <emscripten/bind.h>

#include "../include/Mach1Transcode.h"
#include "Mach1TranscodeCAPI.h"

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

    class_<Mach1Transcode<float> >("Mach1TranscodeInternal")
        .constructor<>()
        .function("getInputNumChannels", &Mach1Transcode<float>::getInputNumChannels)
        .function("getOutputNumChannels", &Mach1Transcode<float>::getOutputNumChannels)
        .function("getFormatFromString", &Mach1Transcode<float>::getFormatFromString)
        .function("getFormatName", &Mach1Transcode<float>::getFormatName)
        .function("getAllFormatNames", &Mach1Transcode<float>::getAllFormatNames)

        .function("processNormalization", &Mach1Transcode<float>::processNormalization)
        .function("processMasterGain", &Mach1Transcode<float>::processMasterGain)

        .function("db2level", &Mach1Transcode<float>::db2level)
        .function("level2db", &Mach1Transcode<float>::level2db)

        .function("setLFESub", &Mach1Transcode<float>::setLFESub)
        .function("setSpatialDownmixer", &Mach1Transcode<float>::setSpatialDownmixer)
        .function("getSpatialDownmixerPossibility", &Mach1Transcode<float>::getSpatialDownmixerPossibility)

        .function("setInputFormat", &Mach1Transcode<float>::setInputFormat)
        .function("setInputFormatCustomPointsJson", &Mach1Transcode<float>::setInputFormatCustomPointsJson)
        .function("setInputFormatCustomPoints", &Mach1Transcode<float>::setInputFormatCustomPoints)

        .function("setOutputFormat", &Mach1Transcode<float>::setOutputFormat)
        .function("setOutputFormatCustomPointsJson", &Mach1Transcode<float>::setOutputFormatCustomPointsJson)
        .function("setOutputFormatCustomPoints", &Mach1Transcode<float>::setOutputFormatCustomPoints)

        .function("processConversionPath", &Mach1Transcode<float>::processConversionPath)
        .function("getMatrixConversion", &Mach1Transcode<float>::getMatrixConversion)
        .function("processConversion", &Mach1Transcode<float>::processConversion)

        .function("getFormatConversionPath", &Mach1Transcode<float>::getFormatConversionPath);
}
