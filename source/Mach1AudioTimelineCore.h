//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.
//
// References: 
// - York ambisonics: https://www.york.ac.uk/sadie-project/ambidec.html

#pragma once

#ifndef Mach1AudioTimelineCore_h
#define Mach1AudioTimelineCore_h

#include <math.h>
#include <stddef.h>
#include <vector>
#include <map>
#include "Mach1Point3DCore.h"
#include "Mach1KeyPoint.h"

#include "adm/ADMParser.h"
#include "yaml/Yaml.hpp"

struct Mach1AudioObjectCore {
public:
	std::string name;
	std::vector<Mach1KeyPoint> points;
};

class Mach1AudioTimelineCore {
	std::vector<Mach1AudioObjectCore> audioObjects;

	float clamp(float n, float lower, float upper);
	void ConvertRCtoXYRaw(float r, float d, float& x, float& y);

public:

	std::vector<Mach1AudioObjectCore>* getAudioObjects();

	void parseADM(char* inXml);
	void parseAtmos(char* inDotAtmos, char* inDotAtmosDotMetadata);
};

#endif /* Mach1AudioTimelineCore_h */
