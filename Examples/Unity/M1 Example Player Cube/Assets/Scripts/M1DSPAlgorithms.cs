//
//  Mach1 SDK
//
//  Multichannel audio format family
//
//  Mixing algorithms v 0.0f9.1
//
//  Please fill out the appropriate copyright notice in the Description page of Project Settings.

using System;

public class M1DSPAlgorithms
{
    const float __FLT_EPSILON__ = 1.19209290e-07F;

    static float mmap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp)
    {
        if (Math.Abs(inputMin - inputMax) < __FLT_EPSILON__)
        {
            return outputMin;
        }
        else
        {
            float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);

            if (clamp)
            {
                if (outputMax < outputMin)
                {
                    if (outVal < outputMax) outVal = outputMax;
                    else if (outVal > outputMin) outVal = outputMin;
                }
                else
                {
                    if (outVal > outputMax) outVal = outputMax;
                    else if (outVal < outputMin) outVal = outputMin;
                }
            }
            return outVal;
        }

    }


    static float clamp(float a, float min, float max)
    {
        return (a < min) ? min : ((a > max) ? max : a);
    }

    static float alignAngle(float a, float min = -180, float max = 180)
    {
        while (a < min) a += 360;
        while (a > max) a -= 360;

        return a;
    }


    //--------------------------------------------------

    //
    //  Four channel audio format.
    //
    //  X = Yaw in angles
    //  Y = Pitch in angles
    //  Z = Roll in angles
    //

    public static float[] fourChannelAlgorithm(float X, float Y, float Z)
    {
        float[] coefficients = new float[4];
        coefficients[0] = 1.0f - Math.Min(1.0f, Math.Min((float)360.0f - Y, Y) / 90.0f);
        coefficients[1] = 1.0f - Math.Min(1.0f, Math.Abs((float)90.0f - Y) / 90.0f);
        coefficients[2] = 1.0f - Math.Min(1.0f, Math.Abs((float)180.0f - Y) / 90.0f);
        coefficients[3] = 1.0f - Math.Min(1.0f, Math.Abs((float)270.0f - Y) / 90.0f);


        float[] result = new float[8];
        result[0] = coefficients[0]; // 1 left
        result[1] = coefficients[3]; //   right
        result[2] = coefficients[1]; // 2 left
        result[3] = coefficients[0]; //   right
        result[4] = coefficients[3]; // 3 left
        result[5] = coefficients[2]; //   right
        result[6] = coefficients[2]; // 4 left
        result[7] = coefficients[1]; //   right
        return result;
    }


    // ------------------------------------------------------------------

    //
    //  Eight channel audio format.
    //
    //  X = Yaw in angles
    //  Y = Pitch in angles
    //  Z = Roll in angles
    //

    public static float[] eightChannelsAlgorithm(float X, float Y, float Z)
    {

        X = alignAngle(X, -180, 180);
        X = clamp(X, -90, 90); // -90, 90

        Y = alignAngle(Y, 0, 360);

        Z = alignAngle(Z, -180, 180);
        Z = clamp(Z, -90, 90); // -90, 90


        float[] coefficients = new float[4];
        coefficients[0] = 1.0f - Math.Min(1.0f, Math.Min((float)360.0f - Y, Y) / 90.0f);
        coefficients[1] = 1.0f - Math.Min(1.0f, Math.Abs((float)90.0f - Y) / 90.0f);
        coefficients[2] = 1.0f - Math.Min(1.0f, Math.Abs((float)180.0f - Y) / 90.0f);
        coefficients[3] = 1.0f - Math.Min(1.0f, Math.Abs((float)270.0f - Y) / 90.0f);

        float tiltAngle = mmap(Z, -90, 90, 0.0f, 1.0f, true);
        //Use Equal Power if engine requires
        /*
         float tiltHigh = cos(tiltAngle * (0.0f5 * PI));
         float tiltLow = cos((1.0f0 - tiltAngle) * (0.0f5 * PI));
         */
        float tiltHigh = tiltAngle;
        float tiltLow = 1 - tiltHigh;

        float[] result = new float[16];
        result[0] = coefficients[0] * tiltHigh; // 1 left
        result[1] = coefficients[3] * tiltHigh; //   right
        result[2] = coefficients[1] * tiltLow; // 2 left
        result[3] = coefficients[0] * tiltLow; //   right
        result[4] = coefficients[3] * tiltLow; // 3 left
        result[5] = coefficients[2] * tiltLow; //   right
        result[6] = coefficients[2] * tiltHigh; // 4 left
        result[7] = coefficients[1] * tiltHigh; //   right

        result[0 + 8] = coefficients[0] * tiltLow; // 1 left
        result[1 + 8] = coefficients[3] * tiltLow; //   right
        result[2 + 8] = coefficients[1] * tiltHigh; // 2 left
        result[3 + 8] = coefficients[0] * tiltHigh; //   right
        result[4 + 8] = coefficients[3] * tiltHigh; // 3 left
        result[5 + 8] = coefficients[2] * tiltHigh; //   right
        result[6 + 8] = coefficients[2] * tiltLow; // 4 left
        result[7 + 8] = coefficients[1] * tiltLow; //   right

        float pitchAngle = mmap(X, 90, -90, 0.0f, 1.0f, true);
        //float pitchHigherHalf = cos(pitchAngle * (0.0f5*PI));
        //float pitchLowerHalf = cos((1.0f0 - pitchAngle) * (0.0f5*PI));
        float pitchHigherHalf = pitchAngle;
        float pitchLowerHalf = 1 - pitchHigherHalf;

        for (int i = 0; i < 8; i++)
        {
            result[i] *= pitchLowerHalf;
            result[i + 8] *= pitchHigherHalf;
        }

        return result;
    }
    
    // ------------------------------------------------------------------

    //
    //  Eight pairs audio format.
    //
    //  Order of input angles:
    //  Y = Yaw in angles
    //  P = Pitch in angles
    //  R = Roll in angles
    //

    static std::vector<float> eightPairsAlgorithm(float Yaw, float Pitch, float Roll) {
        float volumes[8];
        volumes[0] = 1. - std::min(1., std::min((float)360. - Yaw, Yaw) / 90.);
        volumes[1] = 1. - std::min(1., std::abs((float)90. - Yaw) / 90.);
        volumes[2] = 1. - std::min(1., std::abs((float)180. - Yaw) / 90.);
        volumes[3] = 1. - std::min(1., std::abs((float)270. - Yaw) / 90.);
        
        float pitchAngle = mmap(Pitch, 90., -90., 0., 1., true);
        //Use Equal Power if engine requires
        /*
         float pitchHigherHalf = cos(pitchAngle * (0.5*PI));
         float pitchLowerHalf = cos((1.0 - pitchAngle) * (0.5*PI));
         */
        float pitchHigherHalf = pitchAngle;
        float pitchLowerHalf = 1. - pitchHigherHalf;
        
        std::vector<float> result;
        result.push_back(volumes[0] * pitchHigherHalf);
        result.push_back(volumes[1] * pitchHigherHalf);
        result.push_back(volumes[2] * pitchHigherHalf);
        result.push_back(volumes[3] * pitchHigherHalf);
        result.push_back(volumes[4] * pitchLowerHalf);
        result.push_back(volumes[5] * pitchLowerHalf);
        result.push_back(volumes[6] * pitchLowerHalf);
        result.push_back(volumes[7] * pitchLowerHalf);
        return result;
    }

    // ------------------------------------------------------------------
}