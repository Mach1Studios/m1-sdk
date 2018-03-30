#include "M1AudioPlayer.h"

M1AudioPlayer::M1AudioPlayer()
{
    mach1Decode = new Mach1Decode();
    mach1Decode->setAngularSettingsType(Mach1Decode::AngularSettingsType::m1Android);
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
        float sndL = 0;
        float sndR = 0;

        mach1Decode->beginBuffer();
        for (size_t i = 0; i < samples; i++)
        {
            float volumes[18];
            mach1Decode->spatialAlgo(Yaw, Pitch, Roll, volumes, samples, i);


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

            buf[i * 2 + 0] = (short) (sndL * (SHRT_MAX-1));
            buf[i * 2 + 1] = (short) (sndR * (SHRT_MAX-1));
        }
        mach1Decode->endBuffer();

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
