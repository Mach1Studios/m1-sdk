#ifndef MACH1AUDIOTIMELINE_H
#define MACH1AUDIOTIMELINE_H

//  Mach1 Spatial SDK
//  Copyright © 2017-2021 Mach1. All rights reserved.

#include "Mach1AudioTimelineCAPI.h"
#include <string>
#include <vector>

class Mach1AudioObject {
  private:
    void *M1obj;

  public:
    Mach1AudioObject();
    ~Mach1AudioObject();

    std::string getName();
    std::vector<Mach1KeyPoint> getKeyPoints();

    void setName(const std::string &name);
    void setKeyPoints(std::vector<Mach1KeyPoint> points);
};

class Mach1AudioTimeline {
  private:
    void *M1obj;

  public:
    Mach1AudioTimeline();
    ~Mach1AudioTimeline();

    std::vector<Mach1AudioObject> getAudioObjects();

    void parseADM(char *inXml);
    void parseAtmos(char *inDotAtmos, char *inDotAtmosDotMetadata);
};

#endif // MACH1AUDIOTIMELINE_H
