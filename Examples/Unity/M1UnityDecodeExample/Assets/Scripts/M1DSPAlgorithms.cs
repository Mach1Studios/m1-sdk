//
//  Mach1 SDK
//
//  Multichannel audio format family
//
//  Mixing algorithms v 0.9.92a
//

using System;
using System.Collections.Generic;

public class M1DSPAlgorithms
{
    const double PI = 3.14159265358979323846;
    const float __FLT_EPSILON__ = 1.19209290e-07F;

    private float currentYaw;
    private float currentPitch;
    private float currentRoll;
    private float targetYaw;
    private float targetPitch;
    private float targetRoll;


    private class mPoint
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
            float a = (float)(angle * (PI / 180.0));
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
            float a = (float)(angle * (PI / 180.0));
            float sina = (float)Math.Sin(a);
            float cosa = (float)Math.Cos(a);
            float cosb = 1.0f - cosa;

            return new mPoint(x * (ax.x * ax.x * cosb + cosa) + y * (ax.x * ax.y * cosb - ax.z * sina) + z * (ax.x * ax.z * cosb + ax.y * sina), x * (ax.y * ax.x * cosb + ax.z * sina) + y * (ax.y * ax.y * cosb + cosa) + z * (ax.y * ax.z * cosb - ax.x * sina), x * (ax.z * ax.x * cosb - ax.y * sina) + y * (ax.z * ax.y * cosb + ax.x * sina) + z * (ax.z * ax.z * cosb + cosa));
        }
    }

    //
    private static float mDegToRad(float degrees)
    {
        return (float)(degrees * (PI / 180.0));
    }


    //
    // Map utility
    //
    private static float mmap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp = false)
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

    private static float clamp(float a, float min, float max)
    {
        return (a < min) ? min : ((a > max) ? max : a);
    }

    private static float alignAngle(float a, float min = -180, float max = 180)
    {
        while (a < min)
        {
            a += 360;
        }
        while (a > max)
        {
            a -= 360;
        }

        return a;
    }

    private float radialDistance(float angle1, float angle2)
    {
        if ((Math.Abs(angle2 - angle1)) > (Math.Abs(Math.Abs(angle2 - angle1) - 360)))
        {
            return Math.Abs(Math.Abs(angle2 - angle1) - 360);
        }
        else
        {
            return Math.Abs(angle2 - angle1);
        }
    }

    private float targetDirectionMultiplier(float angleCurrent, float angleTarget)
    {
        if (((Math.Abs(angleCurrent - angleTarget)) > (Math.Abs(angleCurrent - angleTarget + 360))) || ((Math.Abs(angleCurrent - angleTarget)) > (Math.Abs(angleCurrent - angleTarget - 360))))
        {
            if (angleCurrent < angleTarget)
            {
                return -1.0f;
            }
            if (angleCurrent > angleTarget)
            {
                return 1.0f;
            }

        }
        else
        {
            if (angleCurrent < angleTarget)
            {
                return 1.0f;
            }
            if (angleCurrent > angleTarget)
            {
                return -1.0f;
            }
        }

        return 0.0f;
    }

    // Envelope follower feature is defined here, in updateAngles()

    private void updateAngles()
    {
        if (targetYaw < 0)
        {
            targetYaw += 360;
        }
        if (targetPitch < 0)
        {
            targetPitch += 360;
        }
        if (targetRoll < 0)
        {
            targetRoll += 360;
        }

        if (targetYaw > 360)
        {
            targetYaw -= 360;
        }
        if (targetPitch > 360)
        {
            targetPitch -= 360;
        }
        if (targetRoll > 360)
        {
            targetRoll -= 360;
        }

        if (currentYaw < 0)
        {
            currentYaw += 360;
        }
        if (currentPitch < 0)
        {
            currentPitch += 360;
        }
        if (currentRoll < 0)
        {
            currentRoll += 360;
        }

        if (currentYaw > 360)
        {
            currentYaw -= 360;
        }
        if (currentPitch > 360)
        {
            currentPitch -= 360;
        }
        if (currentRoll > 360)
        {
            currentRoll -= 360;
        }

        float distanceYaw = radialDistance(targetYaw, currentYaw);
        if (((distanceYaw) > 2.5))
        {
            currentYaw += 2.4f * targetDirectionMultiplier(currentYaw, targetYaw);
        }
        else
        {
            currentYaw = targetYaw;
        }

        float distancePitch = radialDistance(targetPitch, currentPitch);
        if (((distancePitch) > 2.5))
        {
            currentPitch += 2.4f * targetDirectionMultiplier(currentPitch, targetPitch);
        }
        else
        {
            currentPitch = targetPitch;
        }

        float distanceRoll = radialDistance(targetRoll, currentRoll);
        if (((distanceRoll) > 2.5) && (distanceRoll < 360))
        {
            currentRoll += 2.4f * targetDirectionMultiplier(currentRoll, targetRoll);
        }
        else
        {
            currentRoll = targetRoll;
        }
    }


    public M1DSPAlgorithms()
    {
        currentYaw = 0F;
        currentPitch = 0F;
        currentRoll = 0F;

        targetYaw = 0F;
        targetPitch = 0F;
        targetRoll = 0F;
    }



    //--------------------------------------------------

    //
    //  Four channel audio format
    //
    //  Order of input angles:
    //  Y = Yaw in angles
    //  P = Pitch in angles
    //  R = Roll in angles
    //

    public float[] fourChannelAlgorithm(float Yaw, float Pitch, float Roll, bool smoothAngles = false)
    {

        if (smoothAngles)
        {
            targetYaw = Yaw;
            targetPitch = Pitch;
            targetRoll = Roll;

            updateAngles();

            Yaw = currentYaw;
            Pitch = currentPitch;
            Roll = currentRoll;
        }
        else
        {
            targetYaw = Yaw;
            targetPitch = Pitch;
            targetRoll = Roll;

            currentYaw = Yaw;
            currentPitch = Pitch;
            currentRoll = Roll;
        }

        //Orientation input safety clamps/alignment
        Yaw = alignAngle(Yaw, 0, 360);

        float[] coefficients = new float[4];
        coefficients[0] = 1.0f - Math.Min(1.0f, Math.Min((float)360.0 - Yaw, Yaw) / 90.0f);
        coefficients[1] = 1.0f - Math.Min(1.0f, Math.Abs((float)90.0 - Yaw) / 90.0f);
        coefficients[2] = 1.0f - Math.Min(1.0f, Math.Abs((float)180.0 - Yaw) / 90.0f);
        coefficients[3] = 1.0f - Math.Min(1.0f, Math.Abs((float)270.0 - Yaw) / 90.0f);


        float[] result = new float[10];
        result[0] = coefficients[0]; // 1 left
        result[1] = coefficients[3]; //   right
        result[2] = coefficients[1]; // 2 left
        result[3] = coefficients[0]; //   right
        result[4] = coefficients[3]; // 3 left
        result[5] = coefficients[2]; //   right
        result[6] = coefficients[2]; // 4 left
        result[7] = coefficients[1]; //   right
        result[8] = 1.0f; // static stereo L
        result[9] = 1.0f; // static stereo R
        return result;
 
    }

    // ------------------------------------------------------------------

    //
    //  Four pairs audio format.
    //
    //  Order of input angles:
    //  Y = Yaw in angles
    //  P = Pitch in angles
    //  R = Roll in angles
    //

    public float[] fourPairsAlgorithm(float Yaw, float Pitch, float Roll, bool smoothAngles = false)
    {

        if (smoothAngles)
        {
            targetYaw = Yaw;
            targetPitch = Pitch;
            targetRoll = Roll;

            updateAngles();

            Yaw = currentYaw;
            Pitch = currentPitch;
            Roll = currentRoll;
        }
        else
        {
            targetYaw = Yaw;
            targetPitch = Pitch;
            targetRoll = Roll;

            currentYaw = Yaw;
            currentPitch = Pitch;
            currentRoll = Roll;
        }

        //Orientation input safety clamps/alignment
        Yaw = alignAngle(Yaw, 0, 360);

        float[] volumes = new float[4];
        volumes[0] = 1.0f - Math.Min(1.0f, Math.Min((float)360.0 - Yaw, Yaw) / 90.0f);
        volumes[1] = 1.0f - Math.Min(1.0f, Math.Abs((float)90.0 - Yaw) / 90.0f);
        volumes[2] = 1.0f - Math.Min(1.0f, Math.Abs((float)180.0 - Yaw) / 90.0f);
        volumes[3] = 1.0f - Math.Min(1.0f, Math.Abs((float)270.0 - Yaw) / 90.0f);

        float[] result = new float[6];
        result[0] = volumes[0];
        result[1] = volumes[1];
        result[2] = volumes[2];
        result[3] = volumes[3];

        result[0] = 1.0f; // static stereo L
        result[0] = 1.0f; // static stereo R

        return result;
    }



    //
    //  Eight channel audio format (isotropic version).
    //
    //  Order of input angles:
    //  Y = Yaw in angles
    //  P = Pitch in angles
    //  R = Roll in angles
    //

    public float[] eightChannelsIsotropicAlgorithm(float Yaw, float Pitch, float Roll, bool smoothAngles = false)
    {

        if (smoothAngles)
        {
            targetYaw = Yaw;
            targetPitch = Pitch;
            targetRoll = Roll;

            updateAngles();

            Yaw = currentYaw;
            Pitch = currentPitch;
            Roll = currentRoll;
        }
        else
        {
            targetYaw = Yaw;
            targetPitch = Pitch;
            targetRoll = Roll;

            currentYaw = Yaw;
            currentPitch = Pitch;
            currentRoll = Roll;
        }

        mPoint simulationAngles = new mPoint(-Pitch, Yaw, Roll);

        mPoint faceVector1 = new mPoint((float)Math.Cos(mDegToRad(simulationAngles[1])), (float)Math.Sin(mDegToRad(simulationAngles[1]))).normalize();


        mPoint faceVector2 = faceVector1.getRotated(simulationAngles[0], new mPoint((float)Math.Cos(mDegToRad(simulationAngles[1] - 90)), (float)Math.Sin(mDegToRad(simulationAngles[1] - 90))).normalize());


        mPoint faceVector21 = faceVector1.getRotated(simulationAngles[0] + 90, new mPoint((float)Math.Cos(mDegToRad(simulationAngles[1] - 90)), (float)Math.Sin(mDegToRad(simulationAngles[1] - 90))).normalize());

        mPoint faceVectorLeft = faceVector21.getRotated(-simulationAngles[2] + 90, faceVector2);
        mPoint faceVectorRight = faceVector21.getRotated(-simulationAngles[2] - 90, faceVector2);


        mPoint faceVectorOffsetted = new mPoint((float)Math.Cos(mDegToRad(simulationAngles[1])), (float)Math.Sin(mDegToRad(simulationAngles[1]))).normalize().rotate(simulationAngles[0] + 10, new mPoint((float)Math.Cos(mDegToRad(simulationAngles[1] - 90)), (float)Math.Sin(mDegToRad(simulationAngles[1] - 90))).normalize()) - faceVector2;

        mPoint tiltSphereRotated = faceVectorOffsetted.rotate(-simulationAngles[2], faceVector2);

        // Drawing another 8 dots

        mPoint[] points = { new mPoint(100, -100, -100), new mPoint(100, 100, -100), new mPoint(-100, -100, -100), new mPoint(-100, 100, -100), new mPoint(100, -100, 100), new mPoint(100, 100, 100), new mPoint(-100, -100, 100), new mPoint(-100, 100, 100) };

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

        float[] result = new float[18];

        for (int i = 0; i < 8; i++)
        {
            float vL = clamp(mmap(qL[i], 0, 223, 1.0f, 0.0f), 0, 1);
            float vR = clamp(mmap(qR[i], 0, 223, 1.0f, 0.0f), 0, 1);

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


        result[16] = 1.0f; // static stereo L
        result[17] = 1.0f; // static stereo R

        return result;
    }


    //
    //  Eight channel audio format.
    //
    //  Order of input angles:
    //  Y = Yaw in angles
    //  P = Pitch in angles
    //  R = Roll in angles
    //

    public float[] eightChannelsAlgorithm(float Yaw, float Pitch, float Roll, bool smoothAngles = false)
    {

        if (smoothAngles)
        {
            targetYaw = Yaw;
            targetPitch = Pitch;
            targetRoll = Roll;

            updateAngles();

            Yaw = currentYaw;
            Pitch = currentPitch;
            Roll = currentRoll;
        }
        else
        {
            targetYaw = Yaw;
            targetPitch = Pitch;
            targetRoll = Roll;

            currentYaw = Yaw;
            currentPitch = Pitch;
            currentRoll = Roll;
        }

        //Orientation input safety clamps/alignment
        Pitch = alignAngle(Pitch, -180, 180);
        Pitch = clamp(Pitch, -90, 90); // -90, 90

        Yaw = alignAngle(Yaw, 0, 360);

        Roll = alignAngle(Roll, -180, 180);
        Roll = clamp(Roll, -90, 90); // -90, 90

        float[] coefficients = new float[8];
        coefficients[0] = 1.0f - Math.Min(1.0f, Math.Min((float)360.0 - Yaw, Yaw) / 90.0f);
        coefficients[1] = 1.0f - Math.Min(1.0f, Math.Abs((float)90.0 - Yaw) / 90.0f);
        coefficients[2] = 1.0f - Math.Min(1.0f, Math.Abs((float)180.0 - Yaw) / 90.0f);
        coefficients[3] = 1.0f - Math.Min(1.0f, Math.Abs((float)270.0 - Yaw) / 90.0f);

        float tiltAngle = mmap(Roll, -90.0f, 90.0f, 0.0f, 1.0f, true);
        //Use Equal Power if engine requires
        /*
		 float tiltHigh = cos(tiltAngle * (0.5 * PI));
		 float tiltLow = cos((1.0 - tiltAngle) * (0.5 * PI));
		 */
        float tiltHigh = tiltAngle;
        float tiltLow = 1.0f - tiltHigh;

        //ISSUE//
        //Able to kill stereo by making both pitch and tilt at max or min values together without proper clamps

        float[] result = new float[18];
        result[0] = coefficients[0] * tiltHigh * 2.0f; // 1 left
        result[1] = coefficients[3] * tiltHigh * 2.0f; //   right
        result[2] = coefficients[1] * tiltLow * 2.0f; // 2 left
        result[3] = coefficients[0] * tiltLow * 2.0f; //   right
        result[4] = coefficients[3] * tiltLow * 2.0f; // 3 left
        result[5] = coefficients[2] * tiltLow * 2.0f; //   right
        result[6] = coefficients[2] * tiltHigh * 2.0f; // 4 left
        result[7] = coefficients[1] * tiltHigh * 2.0f; //   right

        result[0 + 8] = coefficients[0] * tiltLow * 2.0f; // 1 left
        result[1 + 8] = coefficients[3] * tiltLow * 2.0f; //   right
        result[2 + 8] = coefficients[1] * tiltHigh * 2.0f; // 2 left
        result[3 + 8] = coefficients[0] * tiltHigh * 2.0f; //   right
        result[4 + 8] = coefficients[3] * tiltHigh * 2.0f; // 3 left
        result[5 + 8] = coefficients[2] * tiltHigh * 2.0f; //   right
        result[6 + 8] = coefficients[2] * tiltLow * 2.0f; // 4 left
        result[7 + 8] = coefficients[1] * tiltLow * 2.0f; //   right

        float pitchAngle = mmap(Pitch, 90.0f, -90.0f, 0.0f, 1.0f, true);
        //Use Equal Power if engine requires
        /*
		 float pitchHigherHalf = cos(pitchAngle * (0.5*PI));
		 float pitchLowerHalf = cos((1.0 - pitchAngle) * (0.5*PI));
		 */
        float pitchHigherHalf = pitchAngle;
        float pitchLowerHalf = 1.0f - pitchHigherHalf;

        for (int i = 0; i < 8; i++)
        {
            result[i] *= pitchLowerHalf;
            result[i + 8] *= pitchHigherHalf;
        }

        result[16] = 1.0f; // static stereo L
        result[17] = 1.0f; // static stereo R

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

    public float[] eightPairsAlgorithm(float Yaw, float Pitch, float Roll, bool smoothAngles = false)
    {

        if (smoothAngles)
        {
            targetYaw = Yaw;
            targetPitch = Pitch;
            targetRoll = Roll;

            updateAngles();

            Yaw = currentYaw;
            Pitch = currentPitch;
            Roll = currentRoll;
        }
        else
        {
            targetYaw = Yaw;
            targetPitch = Pitch;
            targetRoll = Roll;

            currentYaw = Yaw;
            currentPitch = Pitch;
            currentRoll = Roll;
        }


        //Orientation input safety clamps/alignment
        Pitch = alignAngle(Pitch, -180, 180);
        Pitch = clamp(Pitch, -90, 90); // -90, 90

        Yaw = alignAngle(Yaw, 0, 360);

        float[] volumes = new float[8];
        volumes[0] = 1.0f - Math.Min(1.0f, Math.Min((float)360.0 - Yaw, Yaw) / 90.0f);
        volumes[1] = 1.0f - Math.Min(1.0f, Math.Abs((float)90.0 - Yaw) / 90.0f);
        volumes[2] = 1.0f - Math.Min(1.0f, Math.Abs((float)180.0 - Yaw) / 90.0f);
        volumes[3] = 1.0f - Math.Min(1.0f, Math.Abs((float)270.0 - Yaw) / 90.0f);

        float pitchAngle = mmap(Pitch, 90.0f, -90.0f, 0.0f, 1.0f, true);
        //Use Equal Power if engine requires
        /*
		 float pitchHigherHalf = cos(pitchAngle * (0.5*PI));
		 float pitchLowerHalf = cos((1.0 - pitchAngle) * (0.5*PI));
		 */
        float pitchHigherHalf = pitchAngle;
        float pitchLowerHalf = 1.0f - pitchHigherHalf;

        float[] result = new float[10];
        result[0] = volumes[0] * pitchHigherHalf;
        result[1] = volumes[1] * pitchHigherHalf;
        result[2] = volumes[2] * pitchHigherHalf;
        result[3] = volumes[3] * pitchHigherHalf;
        result[4] = volumes[4] * pitchLowerHalf;
        result[5] = volumes[5] * pitchLowerHalf;
        result[6] = volumes[6] * pitchLowerHalf;
        result[7] = volumes[7] * pitchLowerHalf;

        result[8] = 1.0f; // static stereo L
        result[9] = 1.0f; // static stereo R

        return result;
    }

    // ------------------------------------------------------------------


}

