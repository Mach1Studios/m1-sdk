//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#include "Mach1TranscodeCore.h"
#include "Mach1GenerateCoeffs.h"
#include "json/json.h"
#include "yaml/yaml.hpp"
#include <string.h> 
#include <cstring>

#define _USE_MATH_DEFINES
#include <cmath>

#ifndef M_LN10
    #define M_LN10 2.30258509299404568402
#endif

Mach1TranscodeCore::Mach1TranscodeCore()
{
	inFmt = Mach1TranscodeFormats::FormatType::Empty;
	outFmt = Mach1TranscodeFormats::FormatType::Empty;

	bufferSize = 4096;
	for (int i = 0; i < Mach1TranscodeConstants::MAXCHANS; i++)  {
		buffers[i] = new float[bufferSize];
	}
}

Mach1TranscodeCore::~Mach1TranscodeCore()
{
	for (int i = 0; i < Mach1TranscodeConstants::MAXCHANS; i++) {
		delete[] buffers[i];
	}
}

int Mach1TranscodeCore::getInputNumChannels()
{
	return getNumChannels(inFmt, true);
}

int Mach1TranscodeCore::getOutputNumChannels()
{
    return getNumChannels(outFmt, false);
}

Mach1TranscodeFormats::FormatType Mach1TranscodeCore::getFormatFromString(char* str) {
	for (auto it = Mach1TranscodeConstants::FormatNames.begin(); it != Mach1TranscodeConstants::FormatNames.end(); ++it) {
		if (std::strcmp(str, it->second) == 0) {
			return it->first;
		}
	}
	return Mach1TranscodeFormats::Empty;
}

const char* Mach1TranscodeCore::getFormatName(void * M1obj, Mach1TranscodeFormats::FormatType fmt)
{
	return Mach1TranscodeConstants::FormatNames.at(fmt);
}

float Mach1TranscodeCore::processNormalization(float** bufs, int numSamples) {
	float peak = 0;

	int nChannels = getOutputNumChannels();
	for (int i = 0; i < nChannels; i++) {
		for (int sample = 0; sample < numSamples; sample++) {
			float tmp = fabs(bufs[i][sample]);
			if (tmp > peak)
				peak = tmp;
		}
	}

	return peak;
}

void Mach1TranscodeCore::processMasterGain(float** bufs, int numSamples, float masterGain) {
	if (masterGain == 1.0) return;
	if (outFmt == Mach1TranscodeFormats::FormatType::Empty) return;

	int nChannels = getOutputNumChannels();
	for (int i = 0; i < nChannels; i++) {
		for (int sample = 0; sample < numSamples; sample++) {
			bufs[i][sample] *= masterGain;
		}
	}
}

void Mach1TranscodeCore::setLFESub(std::vector<int> subChannelIndices, int sampleRate)
{
	this->subChannelIndices = subChannelIndices;

	lpFilters.resize(subChannelIndices.size());
	for (int k = 0; k < lpFilters.size(); k++) {
		lpFilters[k].SetSampleRate(sampleRate);
		lpFilters[k].Setup(100, 0.0f);
	}
}

void Mach1TranscodeCore::setSpatialDownmixer(float corrThreshold) {
	spatialDownmixChecker.Setup(corrThreshold);
}

bool Mach1TranscodeCore::getSpatialDownmixerPossibility() {
	return spatialDownmixChecker.bShouldDownmix();
}

const std::vector<float>& Mach1TranscodeCore::getAvgSamplesDiff() {
	return spatialDownmixChecker.getAvgSamplesDiff();
}

void Mach1TranscodeCore::setInputFormat(Mach1TranscodeFormats::FormatType inFmt)
{
	this->inFmt = inFmt;
}

void Mach1TranscodeCore::setInputFormatADM(char* inXml, ProcessSettings processSettings)
{
	// TODO:
	ADMParser admParser;
	admParser.ParseString(inXml, audioTracks);

	// output yaml file
	/*
	std::ofstream out("output.atmos");
	out << "sampleRate: 48000" << std::endl;
	out << "events:" << std::endl;
	for (auto & audioTrack : audioTracks) {
		out << "  - ID: " << audioTrack.first << std::endl;
		std::vector<ADMParser::KeyPoint> points = audioTrack.second;
		for (size_t i = 0; i < points.size(); i++) {
			if (i == 0) {
				out << "    ";
			}
			else {
				out << "  - ";
			}

			out << "samplePos: " << (long long int)(points[i].time * 48000) << std::endl;
			out << "    pos: [" << points[i].y << ", " << points[i].z << ", " << points[i].x << "]" << std::endl;
		}
	}
	out.close();
	*/

}

void Mach1TranscodeCore::setInputFormatAtmos(char* inDotAtmos, char* inDotAtmosDotMetadata, ProcessSettings processSettings)
{
	// TODO:
	// parse yaml to audioTracks
	Yaml::Node generalmetadata;
	Yaml::Node objectmetadata;
	int cnt = 0;

	Yaml::Parse(generalmetadata, inDotAtmos, strlen(inDotAtmos));
	Yaml::Parse(objectmetadata, inDotAtmos, strlen(inDotAtmos));

	{
		Yaml::Node & item = generalmetadata["presentations"][0]["bedInstances"][0]["channels"];
		for (auto it = item.Begin(); it != item.End(); it++) {
			string ID = (*it).second["ID"].As<string>();
			std::cout << (*it).first << ": " << (*it).second.As<string>() << std::endl;

			float Rotation = 0;
			float Diverge = 0;
			float Elevation = 0;

			string channel = (*it).second["channel"].As<string>();

			if (channel == "L") {
				Rotation = -45;
				Diverge = 100;
				Elevation = 0;
			}
			else if (channel == "R") {

				Rotation = 45;
				Diverge = 100;
				Elevation = 0;
			}
			else if (channel == "C") {
				Rotation = 0;
				Diverge = 100;
				Elevation = 0;
			}
			else if (channel == "LFE") {
				Rotation = 0;
				Diverge = 0;
				Elevation = 0;
			}
			else if (channel == "Lss") {
				Rotation = -90;
				Diverge = 100;
				Elevation = 0;
			}
			else if (channel == "Rss") {
				Rotation = 90;
				Diverge = 100;
				Elevation = 0;
			}
			else if (channel == "Lrs") {
				Rotation = -135;
				Diverge = 100;
				Elevation = 0;
			}
			else if (channel == "Rrs") {
				Rotation = 135;
				Diverge = 100;
				Elevation = 0;
			}
			else if (channel == "Lts") {
				Rotation = -90;
				Diverge = 35;
				Elevation = 0;
			}
			else if (channel == "Rts") {
				Rotation = 90;
				Diverge = 35;
				Elevation = 0;
			}

			//float x, y;
			//ConvertRCtoXYRaw(Rotation, Diverge, x, y);

			cnt++;
		}
	}

	{
		Yaml::Node & item = objectmetadata["events"];
		for (auto it = item.Begin(); it != item.End(); it++) {
			string ID = (*it).second["ID"].As<string>();
			std::cout << (*it).first << ": " << (*it).second.As<string>() << std::endl;

			if (!(*it).second["pos"].IsNone()) {
				float p = 1.0 * (*it).second["samplePos"].As<int>() / objectmetadata["sampleRate"].As<int>();
				float x = ((*it).second["pos"][0].As<float>());
				float y = ((*it).second["pos"][1].As<float>());
				float z = ((*it).second["pos"][2].As<float>());

				float Rotation = atan2(x, y) * 180 / PI;
				float Diverge = 100 * sqrt(x * x + y * y) / sqrt(2.0);
				
				Mach1Point3DCore a(x, y, z);
				Mach1Point3DCore b(x, y, 0.0);

				float Elevation = 0;
				if (a != b) {
					// vector_angle_between
					Elevation = (acos(Mach1Point3DCore::dot(a, b)) / (a.length() * b.length())) * 180 / PI;
				}

				//float x, y;
				//ConvertRCtoXYRaw(Rotation, Diverge, x, y);	

				cnt++;
			}
		}
	}

	cout << "Imported " << cnt << " points";
}

std::vector<Mach1Point3DCore> parseTTJson(std::string srtJson)
{
	std::vector<Mach1Point3DCore> points;

	auto doc = JSON::parse(srtJson);
	if (doc.size() > 0) {
		auto jsonPoints = JSON::getChildren(doc, "points");
		if (jsonPoints.size() > 0) {
			for (int i = 0; ; i++) {
				auto jsonPoint = JSON::getElement(jsonPoints, 3, i);
				if (jsonPoint.size() > 0) {
					points.push_back(
						Mach1Point3DCore(
							std::stof(JSON::getChildren(jsonPoint, "x")[0]->value),
							std::stof(JSON::getChildren(jsonPoint, "y")[0]->value),
							std::stof(JSON::getChildren(jsonPoint, "z")[0]->value)
						)
					);
				}
			}
		}
	}

	for (int i = 0; i < doc.size(); i++) delete doc[i];

	return points;
}

void Mach1TranscodeCore::setInputFormatTTJson(std::string strJson)
{
	inFmt = Mach1TranscodeFormats::FormatType::TTPoints;
	inTTPoints = parseTTJson(strJson);
}

void Mach1TranscodeCore::setInputFormatTTPoints(std::vector<Mach1Point3DCore> points)
{
    inFmt = Mach1TranscodeFormats::FormatType::TTPoints;
    inTTPoints = points;
}

void Mach1TranscodeCore::setOutputFormat(Mach1TranscodeFormats::FormatType outFmt)
{
	this->outFmt = outFmt;
}

void Mach1TranscodeCore::setOutputFormatTTJson(std::string strJson)
{
	outFmt = Mach1TranscodeFormats::FormatType::TTPoints;
	outTTPoints = parseTTJson(strJson);
}

void Mach1TranscodeCore::setOutputFormatTTPoints(std::vector<Mach1Point3DCore> points)
{
    outFmt = Mach1TranscodeFormats::FormatType::TTPoints;
    outTTPoints = points;
}

bool Mach1TranscodeCore::processConversionPath()
{
    // compute tree of pathes from inFmt to all others
    struct Node {
		int prev;
		Mach1TranscodeFormats::FormatType fmt;
        bool processed;
    };
    
    std::vector<Node> tree;
    Node node;
    node.fmt = inFmt;
    node.prev = -1;
    node.processed = false;
    tree.push_back(node);
    
    for (int i = 0; i < tree.size(); i++) {
        if (tree[i].processed == false) {
			Mach1TranscodeFormats::FormatType fmt = tree[i].fmt;
            for (int j = 0; j < Mach1TranscodeConstants::NUMFMTS; j++) {

                if (Mach1TranscodeConstants::FormatMatrix.find(std::make_pair(fmt, (Mach1TranscodeFormats::FormatType)j)) != Mach1TranscodeConstants::FormatMatrix.end()) {
                    
                    // check if this format already exist on that path
                    int k = i;
                    bool exist = false;
                    while (!exist && k != -1) {
                        if (tree[k].fmt == j) {
                            exist = true;
                            break;
                        }
                        else {
                            k = tree[k].prev;
                        }
                    }
                    
                    if (exist == false) {
                        Node node;
                        node.fmt = (Mach1TranscodeFormats::FormatType)j;
                        node.prev = i;
                        node.processed = false;
                        tree.push_back(node);
                    }
                }
            }
            
            tree[i].processed = true;
        }
    }
    
    // find shortest path first
    std::vector<Mach1TranscodeFormats::FormatType> intermediateFormatsList;
    intermediateFormatsList.reserve(100);
    for (int i = 0; i < tree.size(); i++) {
        if (tree[i].fmt == outFmt) {
            for (int k = i; k != -1; k = tree[k].prev) {
                intermediateFormatsList.insert(intermediateFormatsList.begin(), tree[k].fmt); // push_front
            }
            break;
        }
    }
    
	if (intermediateFormatsList.size() < 2) {
		formatConversionPath.clear();
		return false;
	}
	else {
		formatConversionPath = intermediateFormatsList;
		return true;
	}
}

std::vector<Mach1Point3DCore> getPointsSet(Mach1TranscodeFormats::FormatType fmt) {
	// M1 horizon plane points
	static std::vector<Mach1Point3DCore> m1HorizonDef = { {-1, 1, 0},
												{1, 1, 0},
												{-1, -1, 0},
												{1, -1, 0} };

	// M1 spatial cube points
	static std::vector<Mach1Point3DCore> m1SpatialDef = { {-1, 1, 1},
												{1, 1, 1},
												{-1, -1, 1},
												{1, -1, 1},

												{-1, 1, -1},
												{1, 1, -1},
												{-1, -1, -1},
												{1, -1, -1} };

	// M1 spatial+ cube points
	static std::vector<Mach1Point3DCore> m1SpatialPlusDef = { {-1, 1, 1},
												{1, 1, 1},
												{-1, -1, 1},
												{1, -1, 1},

												{-1, 1, -1},
												{1, 1, -1},
												{-1, -1, -1},
												{1, -1, -1},

												{0, 1 / 0.707f, 0},
												{1 / 0.707f, 0, 0},
												{0, -1 / 0.707f, 0},
												{-1 / 0.707f, 0, 0} };

	// M1 spatial++ cube points
	static std::vector<Mach1Point3DCore> m1SpatialPlusPlusDef = { {-1, 1, 1},
												{1, 1, 1},
												{-1, -1, 1},
												{1, -1, 1},

												{-1, 1, -1},
												{1, 1, -1},
												{-1, -1, -1},
												{1, -1, -1},

												{0, 1 / 0.707f, 0},
												{1 / 0.707f, 0, 0},
												{0, -1 / 0.707f, 0},
												{-1 / 0.707f, 0, 0},

												{ 1 / 0.707f, 0, 0},
												{-1 / 0.707f, 0, 0} };

	// M1 spatial extended cube points
	static std::vector<Mach1Point3DCore> m1SpatialExtendedDef = { {-1, 1, 1},
												{1, 1, 1},
												{-1, -1, 1},
												{1, -1, 1},

												{-1, 1, -1},
												{1, 1, -1},
												{-1, -1, -1},
												{1, -1, -1},

												{0, 1 / 0.707f, 1},
												{1 / 0.707f, 0, 1},
												{0, -1 / 0.707f, 1},
												{-1 / 0.707f, 0, 1},

												{0, 1 / 0.707f, -1},
												{1 / 0.707f, 0, -1},
												{0, -1 / 0.707f, -1},
												{-1 / 0.707f, 0, -1} };

	// M1 spatial extended+ cube points
	static std::vector<Mach1Point3DCore> m1SpatialExtendedPlusDef = { {-1, 1, 1},
												{1, 1, 1},
												{-1, -1, 1},
												{1, -1, 1},

												{-1, 1, -1},
												{1, 1, -1},
												{-1, -1, -1},
												{1, -1, -1},

												{0, 1 / 0.707f, 1},
												{1 / 0.707f, 0, 1},
												{0, -1 / 0.707f, 1},
												{-1 / 0.707f, 0, 1},

												{0, 1 / 0.707f, -1},
												{1 / 0.707f, 0, -1},
												{0, -1 / 0.707f, -1},
												{-1 / 0.707f, 0, -1},

												{1 / 0.707f, 0, 0},
												{-1 / 0.707f, 0, 0} };

	static std::map<Mach1TranscodeFormats::FormatType, std::vector<Mach1Point3DCore>> standards = {
		{Mach1TranscodeFormats::FormatType::M1Horizon, m1HorizonDef},
		{Mach1TranscodeFormats::FormatType::M1Spatial, m1SpatialDef},
		{Mach1TranscodeFormats::FormatType::M1SpatialPlus, m1SpatialPlusDef},
		{Mach1TranscodeFormats::FormatType::M1SpatialPlusPlus, m1SpatialPlusPlusDef},
		{Mach1TranscodeFormats::FormatType::M1SpatialExtended, m1SpatialExtendedDef},
		{Mach1TranscodeFormats::FormatType::M1SpatialExtendedPlus, m1SpatialExtendedPlusDef},
	};

	std::vector<Mach1Point3DCore> vec;
	if (standards.find(fmt) != standards.end()) {
		vec = standards[fmt];
	}
	return vec;
}

void Mach1TranscodeCore::processConversion(Mach1TranscodeFormats::FormatType inFmt, float** inBufs, Mach1TranscodeFormats::FormatType outFmt, float** outBufs, int numSamples)
{
    int inChans = getNumChannels(inFmt, true);
    int outChans = getNumChannels(outFmt, false);
    std::vector<std::vector<float>> currentFormatConversionMatrix;

    if (inFmt == Mach1TranscodeFormats::FormatType::TTPoints && outFmt != Mach1TranscodeFormats::FormatType::TTPoints) {
        currentFormatConversionMatrix = generateCoeffSetForPoints(inTTPoints, getPointsSet(outFmt));
    }
	else if (inFmt != Mach1TranscodeFormats::FormatType::TTPoints && outFmt == Mach1TranscodeFormats::FormatType::TTPoints) {
		currentFormatConversionMatrix = generateCoeffSetForPoints(getPointsSet(inFmt), outTTPoints);
	}
	else if (inFmt == Mach1TranscodeFormats::FormatType::TTPoints && outFmt == Mach1TranscodeFormats::FormatType::TTPoints) {
		currentFormatConversionMatrix = generateCoeffSetForPoints(inTTPoints, outTTPoints);
	}
	else if (inFmt != Mach1TranscodeFormats::FormatType::TTPoints && outFmt != Mach1TranscodeFormats::FormatType::TTPoints) {
        currentFormatConversionMatrix = ((SpatialSoundMatrix*)Mach1TranscodeConstants::FormatMatrix.at(std::make_pair(inFmt, outFmt)))->getData();
    }
    
    // copy pointers to local
    float* inPtrs[Mach1TranscodeConstants::MAXCHANS];
    if (inBufs != NULL) {
        for (int i = 0; i<inChans; i++)
            inPtrs[i] = inBufs[i];
    }
    float* outPtrs[Mach1TranscodeConstants::MAXCHANS];
    if (outBufs != NULL) {
        for (int i = 0; i<outChans; i++)
            outPtrs[i] = outBufs[i];
    }
    
    // matrix mult each sample
    float ins[Mach1TranscodeConstants::MAXCHANS];
    for (int sample = 0; sample < numSamples; sample++) {
        for (int inChannel = 0; inChannel < inChans; inChannel++)
            ins[inChannel] = inPtrs[inChannel][sample];
        for (int channel = inChans; channel < Mach1TranscodeConstants::MAXCHANS; channel++)
            ins[channel] = 0;
    
		/*
		TODO:
		if(perSamplePointsUpdateCallback(inTTPoints*, sample)){  // updates the points, std::function<bool(vector<M1Point>*, int)>  
			currentFormatConversionMatrix = generateCoeffSetForPoints(inTTPoints, outTTPoints);
		}
		*/
		
        for (int outChannel = 0; outChannel < outChans; outChannel++)
        {
            float out = 0;
            for (int inChannel = 0; inChannel < inChans; inChannel++)
                out += currentFormatConversionMatrix[outChannel][inChannel] * ins[inChannel]; // check
            outPtrs[outChannel][sample] = out;
        }
        
        for (int outChannel = 0; outChannel < outChans; outChannel++)
            ins[outChannel] = outPtrs[outChannel][sample];
        for (int outChannel = outChans; outChannel < Mach1TranscodeConstants::MAXCHANS; outChannel++)
            ins[outChannel] = 0;
        
    } 
 
}

int Mach1TranscodeCore::getNumChannels(Mach1TranscodeFormats::FormatType fmt, bool isInput)
{
	if (fmt == Mach1TranscodeFormats::FormatType::TTPoints) {
		return isInput ? inTTPoints.size() : outTTPoints.size();
	}
    return Mach1TranscodeConstants::FormatChannels.at(fmt);
}

float Mach1TranscodeCore::db2level(float db)
{
	return (float)exp(M_LN10 * ((double)db / 20.0));
}

float Mach1TranscodeCore::level2db(float level)
{
	return (float)(20.0f * log((double)level) / M_LN10);
}

void Mach1TranscodeCore::getMatrixConversion(float* matrix)
{
	float mPrev[Mach1TranscodeConstants::MAXCHANS][Mach1TranscodeConstants::MAXCHANS];
	float mCurrent[Mach1TranscodeConstants::MAXCHANS][Mach1TranscodeConstants::MAXCHANS];
	float mRes[Mach1TranscodeConstants::MAXCHANS][Mach1TranscodeConstants::MAXCHANS];
	int mSize = Mach1TranscodeConstants::MAXCHANS * Mach1TranscodeConstants::MAXCHANS * sizeof(float);

	int prevInChans = 0;
	int prevOutChans = 0;

	std::memset(mPrev, 0, mSize);
	std::memset(mCurrent, 0, mSize);
	std::memset(mRes, 0, mSize);

	for (int k = 0; k < formatConversionPath.size() - 1; k++) {
		Mach1TranscodeFormats::FormatType inFmt = formatConversionPath[k];
		Mach1TranscodeFormats::FormatType outFmt = formatConversionPath[k + 1];

		int inChans = getNumChannels(inFmt, true);
		int outChans = getNumChannels(outFmt, false);
		std::vector<std::vector<float>> currentFormatConversionMatrix;

		if (inFmt == Mach1TranscodeFormats::FormatType::TTPoints && outFmt != Mach1TranscodeFormats::FormatType::TTPoints) {
			currentFormatConversionMatrix = generateCoeffSetForPoints(inTTPoints, getPointsSet(outFmt));
		}
		else if (inFmt != Mach1TranscodeFormats::FormatType::TTPoints && outFmt == Mach1TranscodeFormats::FormatType::TTPoints) {
			currentFormatConversionMatrix = generateCoeffSetForPoints(getPointsSet(inFmt), outTTPoints);
		}
		else if (inFmt == Mach1TranscodeFormats::FormatType::TTPoints && outFmt == Mach1TranscodeFormats::FormatType::TTPoints) {
			currentFormatConversionMatrix = generateCoeffSetForPoints(inTTPoints, outTTPoints);
		}
		else if (inFmt != Mach1TranscodeFormats::FormatType::TTPoints && outFmt != Mach1TranscodeFormats::FormatType::TTPoints) {
			currentFormatConversionMatrix = ((SpatialSoundMatrix*)Mach1TranscodeConstants::FormatMatrix.at(std::make_pair(inFmt, outFmt)))->getData();
		}

		std::memset(mCurrent, 0, mSize);
		for (int outChannel = 0; outChannel < outChans; outChannel++)
		{
			for (int inChannel = 0; inChannel < inChans; inChannel++) {
				mCurrent[outChannel][inChannel] = currentFormatConversionMatrix[outChannel][inChannel];
			}
		}

		if (k == 0) {
			std::memcpy(mRes, mCurrent, mSize);
		}
		else {
			std::memset(mRes, 0, mSize);
			// multiply matrix
			for (int i = 0; i < outChans; i++) {
				for (int j = 0; j < prevInChans; j++) {
					for (int k = 0; k < inChans; k++) {
						mRes[i][j] += mCurrent[i][k] * mPrev[k][j];
					}
				}
			}
		}

		prevInChans = inChans;
		prevOutChans = outChans;
		std::memcpy(mPrev, mRes, Mach1TranscodeConstants::MAXCHANS * Mach1TranscodeConstants::MAXCHANS * sizeof(float));
	}

	int inChans = getInputNumChannels();
	int outChans = getOutputNumChannels();
	for (size_t i = 0; i < outChans; i++) {
		for (size_t j = 0; j < inChans; j++) {
			matrix[i *  inChans + j] = mRes[i][j];
		}
	}

	/*
	// debug output
	printf("std::vector<std::vector<float>> res = { \r\n");// , outChans * inChans);
	for (size_t i = 0; i < outChans; i++) {
		printf("{ ");
		for (size_t j = 0; j < inChans; j++) {
			printf(" %f", mRes[i][j]);
			if (j != inChans - 1) printf(",", mRes[i][j]);
		}
		printf("}");
		if (i != outChans - 1) printf(",");
		printf("\r\n");
	}
	printf("};\r\n");
	*/
}

void Mach1TranscodeCore::processConversion(float ** inBufs, float ** outBufs, int numSamples)
{
	// reinit internal buffer
	if (numSamples > bufferSize) {
		bufferSize = numSamples;

		for (int i = 0; i < Mach1TranscodeConstants::MAXCHANS; i++) {
			delete[] buffers[i];
		}

		for (int i = 0; i < Mach1TranscodeConstants::MAXCHANS; i++) {
			buffers[i] = new float[bufferSize];
		}
	}
	
	// copy input
	int nInChannels = getInputNumChannels();
	for (int i = 0; i < nInChannels; i++) {
		std::memcpy(buffers[i], inBufs[i], (int)numSamples * sizeof(float));
	}

	// process input sub channels
	for (int k = 0; k < lpFilters.size(); k++) {
		for (int j = 0; j < numSamples; j++) {
			buffers[subChannelIndices[k]][j] = lpFilters[k].Run(buffers[subChannelIndices[k]][j]);
		}
	}

	for (int k = 0; k < formatConversionPath.size() - 1; k++) {
		processConversion(formatConversionPath[k], k == 0 ? inBufs : buffers, formatConversionPath[k + 1], buffers, (int)numSamples);
	}

	int nOutChannels = getOutputNumChannels();
	for (int i = 0; i < nOutChannels; i++) {
		std::memcpy(outBufs[i], buffers[i], (int)numSamples * sizeof(float));
	}

	// spatial downmix check
	if (outFmt == Mach1TranscodeFormats::FormatType::M1Spatial) {
		spatialDownmixChecker.ProcessBuffer(outBufs, numSamples);
	}
}

std::vector<Mach1TranscodeFormats::FormatType>& Mach1TranscodeCore::getFormatConversionPath() {
	return formatConversionPath;
}
