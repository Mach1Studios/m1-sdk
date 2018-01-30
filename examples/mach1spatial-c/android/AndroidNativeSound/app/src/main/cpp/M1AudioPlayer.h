#pragma once

#include "AudioPlayer.h"
#include "Mach1DecodeCAPI.h"

class M1AudioPlayer : public AudioPlayer
{
    Mach1Decode::mPoint anglesCur;
    Mach1Decode::mPoint anglesDest;
    /*
    float Yaw;
    float Pitch;
    float Roll;
*/
    Mach1Decode::mPoint lerp (Mach1Decode::mPoint p1, Mach1Decode::mPoint p2, float prc)
    {
        return p1 * prc + p2 * (1-prc);
    }

    Mach1Decode mach1Decode;

public:
    void SetAngles(float Yaw, float Pitch, float Roll)
    {
        anglesDest = { Yaw, Pitch, Roll };
    }

    bool Get(short * buf, int samples)
    {
        if (ready && running)
        {
            std::vector<float> volumes = mach1Decode.spatialAlgo(anglesCur.x, anglesCur.y, anglesCur.z);

            float sndL = 0;
            float sndR = 0;

            for (size_t i = 0; i < samples; i++)
            {
                sndL = 0;
                sndR = 0;

                for (int j = 0; j < 8; j++)
                {
                    sndL += volumes[j * 2 + 0] * buffer[j][(bufferRead + i) % AUDIO_PLAYER_BUFFERSIZE];
                    sndR += volumes[j * 2 + 1] * buffer[j][(bufferRead + i) % AUDIO_PLAYER_BUFFERSIZE];
                }

                if (sndL > 1) sndL = 1;
                if (sndL < -1) sndL = -1;

                if (sndR > 1) sndR = 1;
                if (sndR < -1) sndR = -1;

                buf[i * 2 + 0] = (short) (sndL * SHRT_MAX);
                buf[i * 2 + 1] = (short) (sndR * SHRT_MAX);
/*
                    buf[i*2+0] = (short)( buffer[0][(bufferRead + i) % AUDIO_PLAYER_BUFFERSIZE] * SHRT_MAX );
                    buf[i*2+1] = (short)( buffer[0][(bufferRead + i) % AUDIO_PLAYER_BUFFERSIZE] * SHRT_MAX );
*/
                anglesCur = lerp(anglesCur, anglesDest, 0.5f / samples);
            }
            bufferRead += samples;

            return true;
        }
        else
        {
            for (size_t i = 0; i < samples; i++)
            {
                buf[i * 2 + 0] = buf[i * 2 + 1] = 0;
            }

            return false;
        }
    }

};