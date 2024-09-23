//  Mach1 Spatial SDK
//  Copyright © 2017 Mach1. All rights reserved.

#include "Mach1AudioTimeline.h"
#include "Mach1AudioTimelineCAPI.h"
#include "Mach1KeyPoint.h"
#include <string>
#include <vector>

Mach1AudioObject::Mach1AudioObject() {
    M1obj = Mach1AudioObjectCAPI_create();
}

Mach1AudioObject::~Mach1AudioObject() {
    Mach1AudioObjectCAPI_delete(M1obj);
}

auto Mach1AudioObject::getName() -> std::string {
    return Mach1AudioObjectCAPI_getName(M1obj);
}

auto Mach1AudioObject::getKeyPoints() -> std::vector<Mach1KeyPoint> {
    Mach1KeyPoint *ptr = Mach1AudioObjectCAPI_getKeyPoints(M1obj);
    return {ptr, ptr + Mach1AudioObjectCAPI_getKeyPointsCount(M1obj)};
}

void Mach1AudioObject::setName(const std::string &name) {
    Mach1AudioObjectCAPI_setName(M1obj, name.c_str());
}

void Mach1AudioObject::setKeyPoints(std::vector<Mach1KeyPoint> points) {
    Mach1AudioObjectCAPI_setKeyPoints(M1obj, points.data(), static_cast<int>(points.size()));
}

Mach1AudioTimeline::Mach1AudioTimeline() {
    M1obj = Mach1AudioTimelineCAPI_create();
}

Mach1AudioTimeline::~Mach1AudioTimeline() {
    Mach1AudioTimelineCAPI_delete(M1obj);
}

auto Mach1AudioTimeline::getAudioObjects() -> std::vector<Mach1AudioObject> {
    std::vector<Mach1AudioObject> audio_objects;

    for (int i = 0; i < Mach1AudioTimelineCAPI_getAudioObjectCount(M1obj); i++) {
        void *obj = Mach1AudioTimelineCAPI_getAudioObject(M1obj, i);

        std::string const name = Mach1AudioObjectCAPI_getName(obj);

        Mach1KeyPoint *ptr = Mach1AudioObjectCAPI_getKeyPoints(obj);
        std::vector<Mach1KeyPoint> const points = std::vector<Mach1KeyPoint>(ptr, ptr + Mach1AudioObjectCAPI_getKeyPointsCount(obj));

        Mach1AudioObject audio_object;
        audio_object.setName(name);
        audio_object.setKeyPoints(points);

        audio_objects.push_back(audio_object);
    }

    return audio_objects;
}

void Mach1AudioTimeline::parseADM(char *in_xml) {
    Mach1AudioTimelineCAPI_parseADM(M1obj, in_xml);
}

void Mach1AudioTimeline::parseAtmos(char *in_dot_atmos, char *in_dot_atmos_dot_metadata) {
    Mach1AudioTimelineCAPI_parseAtmos(M1obj, in_dot_atmos, in_dot_atmos_dot_metadata);
}
