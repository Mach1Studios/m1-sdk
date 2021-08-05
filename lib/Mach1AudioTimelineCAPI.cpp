//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#include "Mach1AudioTimelineCore.h"
#include "Mach1AudioTimelineCAPI.h"
#include "Mach1Point3DCore.h"

M1_API void * Mach1AudioObjectCAPI_create()
{
	return new Mach1AudioObjectCore();
}

M1_API void Mach1AudioObjectCAPI_delete(void * M1obj)
{
	if (M1obj != nullptr) {
		delete (Mach1AudioObjectCore*)M1obj;
		M1obj = nullptr;
	}
}

M1_API const char * Mach1AudioObjectCAPI_getName(void * M1obj)
{
	return ((Mach1AudioObjectCore*)M1obj)->name.c_str();
}

M1_API int Mach1AudioObjectCAPI_getKeyPointsCount(void * M1obj)
{
	return ((Mach1AudioObjectCore*)M1obj)->points.size();
}

M1_API Mach1KeyPoint Mach1AudioObjectCAPI_getKeyPoint(void * M1obj, int n)
{
	return ((Mach1AudioObjectCore*)M1obj)->points[n];
}

M1_API void * Mach1AudioTimelineCAPI_create()
{
	return new Mach1AudioTimelineCore();
}

M1_API void Mach1AudioTimelineCAPI_delete(void * M1obj)
{
	if (M1obj != nullptr) {
		delete (Mach1AudioTimelineCore*)M1obj;
		M1obj = nullptr;
	}
}

M1_API int Mach1AudioTimelineCAPI_getAudioObjectCount(void * M1obj)
{
	return ((Mach1AudioTimelineCore*)M1obj)->getAudioObjects()->size();
}

M1_API void * Mach1AudioTimelineCAPI_getAudioObject(void * M1obj, int n)
{
	return (void*)&(((Mach1AudioTimelineCore*)M1obj)->getAudioObjects()->operator[](n));
}