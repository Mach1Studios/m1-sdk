//  Mach1 Spatial SDK
//  Copyright © 2017-2021 Mach1. All rights reserved.

#pragma once

#include <vector>
#include <string>
#include "Mach1AudioTimelineCAPI.h"

class Mach1AudioObject {
private:
	void* M1obj;

public:
	Mach1AudioObject();
	~Mach1AudioObject();

	std::string getName();
	std::vector<Mach1KeyPoint> getKeyPoints();

	void setName(std::string name);
	void setKeyPoints(std::vector<Mach1KeyPoint> points);
};

class Mach1AudioTimeline {
private:
	void* M1obj;

public:
	Mach1AudioTimeline();
	~Mach1AudioTimeline();

	std::vector<Mach1AudioObject> getAudioObjects();

	void parseADM(char* inXml);
	void parseAtmos(char* inDotAtmos, char* inDotAtmosDotMetadata);
};
