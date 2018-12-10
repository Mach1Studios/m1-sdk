#pragma once

#include "AudioPlayer.h"
#include "Mach1Decode.h"

class M1AudioPlayer : public AudioPlayer
{
    float Yaw;
    float Pitch;
    float Roll;

    Mach1Decode mach1Decode;

public:

    M1AudioPlayer();

    void SetAngles(float Yaw, float Pitch, float Roll);
    bool Get(short * buf, int samples);

};