#include "M1AudioPlayer.h"

M1AudioPlayer::M1AudioPlayer()
{
    mach1Decode.setPlatformType(Mach1PlatformType::Mach1PlatformAndroid);
}

void M1AudioPlayer::SetAngles(float Yaw, float Pitch, float Roll)
{
    this->Yaw = Yaw;
    this->Pitch = Pitch;
    this->Roll = Roll;
}

bool M1AudioPlayer::Get(short * buf, int samples)
{
    if (ready && running)
    {
        size_t idx = 0;

        float sndL = 0;
        float sndR = 0;
        float volumes[18];

        mach1Decode.setDecodeAlgoType(Mach1DecodeAlgoType::Mach1DecodeAlgoSpatial);
        mach1Decode.beginBuffer();
        for (size_t i = 0; i < samples; i++)
        {
            mach1Decode.decode(Yaw, Pitch, Roll, volumes, samples, i);

            sndL = 0;
            sndR = 0;

            idx = (bufferRead + i) % AUDIO_PLAYER_BUFFERSIZE;

            for (int j = 0; j < 8; j++)
            {
                sndL += volumes[j * 2 + 0] * buffer[j][idx];
                sndR += volumes[j * 2 + 1] * buffer[j][idx];
            }

            if (sndL > 1) sndL = 1;
            if (sndL < -1) sndL = -1;

            if (sndR > 1) sndR = 1;
            if (sndR < -1) sndR = -1;

            buf[i * 2 + 0] = (short) (sndL * (SHRT_MAX-1));
            buf[i * 2 + 1] = (short) (sndR * (SHRT_MAX-1));
        }
        mach1Decode.endBuffer();

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
