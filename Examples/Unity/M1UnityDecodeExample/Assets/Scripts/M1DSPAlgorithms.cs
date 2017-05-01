//
//  Mach1 SDK
//
//  Multichannel audio format family
//
//  Mixing algorithms Unity
//
//  Updated to match: 0.9.8b

using System;

public class M1DSPAlgorithms
{
    const float __FLT_EPSILON__ = 1.19209290e-07F;

    public class mPoint
    {
        public float x;
        public float y;
        public float z;

        public mPoint()
        {
            x = 0F;
            y = 0F;
            z = 0F;
        }

        public mPoint(float X, float Y, float Z)
        {
            x = X;
            y = Y;
            z = Z;
        }

        public mPoint(float X, float Y)
        {
            x = X;
            y = Y;
            z = 0F;
        }

        public static mPoint operator +(mPoint ImpliedObject, mPoint pnt)
        {
            return new mPoint(ImpliedObject.x + pnt.x, ImpliedObject.y + pnt.y, ImpliedObject.z + pnt.z);
        }


        public static mPoint operator *(mPoint ImpliedObject, float f)
        {
            return new mPoint(ImpliedObject.x * f, ImpliedObject.y * f, ImpliedObject.z * f);
        }


        public static mPoint operator *(mPoint ImpliedObject, mPoint vec)
        {
            return new mPoint(ImpliedObject.x * vec.x, ImpliedObject.y * vec.y, ImpliedObject.z * vec.z);
        }


        public static mPoint operator -(mPoint ImpliedObject, mPoint vec)
        {
            return new mPoint(ImpliedObject.x - vec.x, ImpliedObject.y - vec.y, ImpliedObject.z - vec.z);
        }

        public float length()
        {
            return (float)Math.Sqrt(x * x + y * y + z * z);
        }


        public float this[int index]
        {
            get
            {
                float[] arr = { x, y, z };
                return arr[index];
            }
        }

        public mPoint rotate(float angle, mPoint axis)
        {
            mPoint ax = axis.getNormalized();
            float a = (float)(angle * Math.PI / 180.0);
            float sina = (float)Math.Sin(a);
            float cosa = (float)Math.Cos(a);
            float cosb = 1.0f - cosa;

            float nx = x * (ax.x * ax.x * cosb + cosa) + y * (ax.x * ax.y * cosb - ax.z * sina) + z * (ax.x * ax.z * cosb + ax.y * sina);
            float ny = x * (ax.y * ax.x * cosb + ax.z * sina) + y * (ax.y * ax.y * cosb + cosa) + z * (ax.y * ax.z * cosb - ax.x * sina);
            float nz = x * (ax.z * ax.x * cosb - ax.y * sina) + y * (ax.z * ax.y * cosb + ax.x * sina) + z * (ax.z * ax.z * cosb + cosa);
            x = nx;
            y = ny;
            z = nz;
            return this;
        }

        public mPoint normalize()
        {
            float length = (float)Math.Sqrt(x * x + y * y + z * z);
            if (length > 0)
            {
                x /= length;
                y /= length;
                z /= length;
            }
            return this;
        }

        public mPoint getNormalized()
        {
            float length = (float)Math.Sqrt(x * x + y * y + z * z);
            if (length > 0)
            {
                return new mPoint(x / length, y / length, z / length);
            }
            else
            {
                return new mPoint();
            }
        }

        public mPoint getRotated(float angle, mPoint axis)
        {
            mPoint ax = axis.getNormalized();
            float a = (float)(angle * Math.PI / 180.0);
            float sina = (float)Math.Sin(a);
            float cosa = (float)Math.Cos(a);
            float cosb = 1.0f - cosa;

            return new mPoint(x * (ax.x * ax.x * cosb + cosa) + y * (ax.x * ax.y * cosb - ax.z * sina) + z * (ax.x * ax.z * cosb + ax.y * sina), x * (ax.y * ax.x * cosb + ax.z * sina) + y * (ax.y * ax.y * cosb + cosa) + z * (ax.y * ax.z * cosb - ax.x * sina), x * (ax.z * ax.x * cosb - ax.y * sina) + y * (ax.z * ax.y * cosb + ax.x * sina) + z * (ax.z * ax.z * cosb + cosa));
        }

    }
     

    static float mDegToRad(float degrees)
    {
        return (float)(degrees * Math.PI / 180.0);
    }

    //Utility function for mapping values
    static float mmap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp)
    {

        if (Math.Abs(inputMin - inputMax) < 1.19209290e-07F)
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
                    if (outVal < outputMax)
                    {
                        outVal = outputMax;
                    }
                    else if (outVal > outputMin)
                    {
                        outVal = outputMin;
                    }
                }
                else
                {
                    if (outVal > outputMax)
                    {
                        outVal = outputMax;
                    }
                    else if (outVal < outputMin)
                    {
                        outVal = outputMin;
                    }
                }
            }
            return outVal;
        }

    }

    //Utility function for clamping values 
    static float clamp(float a, float min, float max)
    {
        return (a < min) ? min : ((a > max) ? max : a);
    }

    //Utility function for ensuring angle is aligned to -180/180 range
    static float alignAngle(float a, float min = -180, float max = 180)
    {
        return (a + max % max); // fmod
    }


    //static float alignAngle(float a, float min = -180, float max = 180)
    //{
    //    while (a < min) a += 360;
    //    while (a > max) a -= 360;

    //    return a;
    //}



    //--------------------------------------------------

    //
    //  Four channel audio format.
    //
    //	This calculates 4 input channels setup in a quad/square around listener and creates 4 sets of stereo pairs
    //	positioned 0,90,180,270 degrees around the listener and tracks the listeners yaw orientation to crossfade between them.
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
    //	This calculates 8 input channels setup in a cube around listener and creates 8 sets of stereo pairs
    //	positioned in two sets of 0,90,180,270 degrees aabove and below the listener and tracks the listeners 
    //	yaw/pitch/roll orientation to crossfade between them.
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
        result[0] = coefficients[0] * tiltHigh * 2; // 1 left
        result[1] = coefficients[3] * tiltHigh * 2; //   right
        result[2] = coefficients[1] * tiltLow * 2; // 2 left
        result[3] = coefficients[0] * tiltLow * 2; //   right
        result[4] = coefficients[3] * tiltLow * 2; // 3 left
        result[5] = coefficients[2] * tiltLow * 2; //   right
        result[6] = coefficients[2] * tiltHigh * 2; // 4 left
        result[7] = coefficients[1] * tiltHigh * 2; //   right

        result[0 + 8] = coefficients[0] * tiltLow * 2; // 1 left
        result[1 + 8] = coefficients[3] * tiltLow * 2; //   right
        result[2 + 8] = coefficients[1] * tiltHigh * 2; // 2 left
        result[3 + 8] = coefficients[0] * tiltHigh * 2; //   right
        result[4 + 8] = coefficients[3] * tiltHigh * 2; // 3 left
        result[5 + 8] = coefficients[2] * tiltHigh * 2; //   right
        result[6 + 8] = coefficients[2] * tiltLow * 2; // 4 left
        result[7 + 8] = coefficients[1] * tiltLow * 2; //   right

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
    //  Eight channel audio format (isotropic version).
    //
    //  Order of input angles:
    //  Y = Yaw in angles
    //  P = Pitch in angles
    //  R = Roll in angles
    //


    public static float[] eightChannelsIsotropicAlgorithm(float Yaw, float Pitch, float Roll)
    {
        mPoint simulationAngles = new mPoint(-Pitch, Yaw, Roll);

        mPoint faceVector1 = new mPoint((float)Math.Cos(mDegToRad(simulationAngles[1])), (float)Math.Sin(mDegToRad(simulationAngles[1]))).normalize();

        mPoint faceVector2 = faceVector1.getRotated(simulationAngles[0], new mPoint((float)Math.Cos(mDegToRad(simulationAngles[1] - 90)), (float)Math.Sin(mDegToRad(simulationAngles[1] - 90))).normalize());
        mPoint faceVector21 = faceVector1.getRotated(simulationAngles[0] + 90, new mPoint((float)Math.Cos(mDegToRad(simulationAngles[1] - 90)), (float)Math.Sin(mDegToRad(simulationAngles[1] - 90))).normalize());

        mPoint faceVectorLeft = faceVector21.getRotated(-simulationAngles[2] + 90, faceVector2);
        mPoint faceVectorRight = faceVector21.getRotated(-simulationAngles[2] - 90, faceVector2);


        mPoint faceVectorOffsetted = new mPoint((float)Math.Cos(mDegToRad(simulationAngles[1])), (float)Math.Sin(mDegToRad(simulationAngles[1]))).normalize().rotate(simulationAngles[0] + 10, new mPoint((float)Math.Cos(mDegToRad(simulationAngles[1] - 90)), (float)Math.Sin(mDegToRad(simulationAngles[1] - 90))).normalize()) - faceVector2;

        mPoint tiltSphereRotated = faceVectorOffsetted.rotate(-simulationAngles[2], faceVector2);

        // Drawing another 8 dots

        mPoint[] points =
        {
            new mPoint(100, -100, -100),
            new mPoint(100, 100, -100),
            new mPoint(-100, -100, -100),
            new mPoint(-100, 100, -100),
            new mPoint(100, -100, 100),
            new mPoint(100, 100, 100),
            new mPoint(-100, -100, 100),
            new mPoint(-100, 100, 100)
        };

        float[] qL = new float[8];
        for (int i = 0; i < 8; i++)
        {
            qL[i] = (faceVectorLeft * 100 + faceVector2 * 100 - points[i]).length();
        }

        float[] qR = new float[8];
        for (int i = 0; i < 8; i++)
        {
            qR[i] = (faceVectorRight * 100 + faceVector2 * 100 - points[i]).length();
        }

        float[] result = new float[16];

        for (int i = 0; i < 8; i++)
        {
            float vL = clamp(mmap(qL[i], 0, 223, 1.0f, 0.0f, false), 0, 1);
            float vR = clamp(mmap(qR[i], 0, 223, 1.0f, 0.0f, false), 0, 1);

            result[i * 2] = vL;
            result[i * 2 + 1] = vR;
        }

        // Volume Balancer v2.0

        float sumL = 0F;
        float sumR = 0F;
        for (int i = 0; i < 8; i++)
        {
            sumL += result[i * 2];
            sumR += result[i * 2 + 1];
        }

        float[] multipliersL = new float[8];
        float[] multipliersR = new float[8];
        for (int i = 0; i < 8; i++)
        {
            multipliersL[i] = result[i * 2] / sumL;
            multipliersR[i] = result[i * 2 + 1] / sumR;
        }

        float sumDiffL = sumL - 1.0f;
        float sumDiffR = sumR - 1.0f;

        float[] correctedVolumesL = new float[8];
        float[] correctedVolumesR = new float[8];
        for (int i = 0; i < 8; i++)
        {
            correctedVolumesL[i] = result[i * 2] - sumDiffL * multipliersL[i];
            correctedVolumesR[i] = result[i * 2 + 1] - sumDiffR * multipliersR[i];
        }

        for (int i = 0; i < 8; i++)
        {
            result[i * 2] = correctedVolumesL[i];
            result[i * 2 + 1] = correctedVolumesR[i];
        }

        return result;
    }
}