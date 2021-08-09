//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#include "Mach1AudioTimeline.h"

Mach1AudioObject::Mach1AudioObject()
{
	M1obj = Mach1AudioObjectCAPI_create();
}

Mach1AudioObject::~Mach1AudioObject()
{
	Mach1AudioObjectCAPI_delete(M1obj);
}

std::string Mach1AudioObject::getName()
{
	return Mach1AudioObjectCAPI_getName(M1obj);
}

std::vector<Mach1KeyPoint> Mach1AudioObject::getKeyPoints()
{
	Mach1KeyPoint* ptr = Mach1AudioObjectCAPI_getKeyPoints(M1obj);
	return std::vector<Mach1KeyPoint> (ptr, ptr + Mach1AudioObjectCAPI_getKeyPointsCount(M1obj));
}

void Mach1AudioObject::setName(std::string name)
{
	Mach1AudioObjectCAPI_setName(M1obj, name.c_str());
}

void Mach1AudioObject::setKeyPoints(std::vector<Mach1KeyPoint> points)
{
	Mach1AudioObjectCAPI_setKeyPoints(M1obj, points.data(), points.size());
}

Mach1AudioTimeline::Mach1AudioTimeline()
{
	M1obj = Mach1AudioTimelineCAPI_create();
}

Mach1AudioTimeline::~Mach1AudioTimeline()
{
	Mach1AudioTimelineCAPI_delete(M1obj);
}

std::vector<Mach1AudioObject> Mach1AudioTimeline::getAudioObjects()
{
	std::vector<Mach1AudioObject> audioObjects;

	for (int i = 0; i < Mach1AudioTimelineCAPI_getAudioObjectCount(M1obj); i++)
	{
		void* obj = Mach1AudioTimelineCAPI_getAudioObject(M1obj, i);

		std::string name = Mach1AudioObjectCAPI_getName(obj);

		Mach1KeyPoint* ptr = Mach1AudioObjectCAPI_getKeyPoints(obj);
		std::vector<Mach1KeyPoint> points = std::vector<Mach1KeyPoint>(ptr, ptr + Mach1AudioObjectCAPI_getKeyPointsCount(obj));

		Mach1AudioObject audioObject;
		audioObject.setName(name);
		audioObject.setKeyPoints(points);

		audioObjects.push_back(audioObject);
	}

	return audioObjects;
}

void Mach1AudioTimeline::parseADM(char * inXml)
{

}

void Mach1AudioTimeline::parseAtmos(char * inDotAtmos, char * inDotAtmosDotMetadata)
{
}


