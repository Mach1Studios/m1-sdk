//
//  Mach1 SDK
//
//  Multichannel audio format family
//
//  Mixing algorithms v 0.9.92a
//

#pragma once

#ifndef M1DSPAlgorithms_h
#define M1DSPAlgorithms_h


#include <vector>
#include <cmath>

#define PI       3.14159265358979323846

//
// Point utility class
//

//////////////

#ifndef DEG_TO_RAD
#define DEG_TO_RAD (PI/180.0)
#endif

class M1DSPAlgorithms {
    
    
    float currentYaw, currentPitch, currentRoll;
    float targetYaw, targetPitch, targetRoll;


    struct mPoint {
        float x, y, z;
        
        mPoint() {
            x = 0;
            y = 0;
            z = 0;
        }
        
        mPoint(float X, float Y, float Z) {
            x = X;
            y = Y;
            z = Z;
        }
        
        mPoint(float X, float Y) {
            x = X;
            y = Y;
            z = 0;
        }
        
        inline mPoint operator+( const mPoint& pnt ) const {
            return mPoint( x+pnt.x, y+pnt.y, z+pnt.z );
        }
        
        
        inline mPoint operator*( const float f ) const {
            return mPoint( x*f, y*f, z*f );
        }
        
        
        inline mPoint operator*( const mPoint& vec ) const {
            return mPoint( x*vec.x, y*vec.y, z*vec.z );
        }
        
        
        inline mPoint operator-( const mPoint& vec ) const {
            return mPoint( x-vec.x, y-vec.y, z-vec.z );
        }
        
        inline float length() const {
            return (float)sqrt( x*x + y*y + z*z );
        }
        
        
        float operator[] (int index) {
            float arr[3] = {x, y, z};
            return arr[index];
        }
        
        inline mPoint& rotate( float angle, const mPoint& axis ) {
            mPoint ax = axis.getNormalized();
            float a = (float)(angle*DEG_TO_RAD);
            float sina = sin( a );
            float cosa = cos( a );
            float cosb = 1.0f - cosa;
            
            float nx = x*(ax.x*ax.x*cosb + cosa)
            + y*(ax.x*ax.y*cosb - ax.z*sina)
            + z*(ax.x*ax.z*cosb + ax.y*sina);
            float ny = x*(ax.y*ax.x*cosb + ax.z*sina)
            + y*(ax.y*ax.y*cosb + cosa)
            + z*(ax.y*ax.z*cosb - ax.x*sina);
            float nz = x*(ax.z*ax.x*cosb - ax.y*sina)
            + y*(ax.z*ax.y*cosb + ax.x*sina)
            + z*(ax.z*ax.z*cosb + cosa);
            x = nx; y = ny; z = nz;
            return *this;
        }
        
        inline mPoint& normalize() {
            float length = (float)sqrt(x*x + y*y + z*z);
            if( length > 0 ) {
                x /= length;
                y /= length;
                z /= length;
            }
            return *this;
        }
        
        
        inline mPoint getNormalized() const {
            float length = (float)sqrt(x*x + y*y + z*z);
            if( length > 0 ) {
                return mPoint( x/length, y/length, z/length );
            } else {
                return mPoint();
            }
        }
        
        
        inline mPoint getRotated( float angle, const mPoint& axis ) const {
            mPoint ax = axis.getNormalized();
            float a = (float)(angle*DEG_TO_RAD);
            float sina = sin( a );
            float cosa = cos( a );
            float cosb = 1.0f - cosa;
            
            return mPoint( x*(ax.x*ax.x*cosb + cosa)
                          + y*(ax.x*ax.y*cosb - ax.z*sina)
                          + z*(ax.x*ax.z*cosb + ax.y*sina),
                          x*(ax.y*ax.x*cosb + ax.z*sina)
                          + y*(ax.y*ax.y*cosb + cosa)
                          + z*(ax.y*ax.z*cosb - ax.x*sina),
                          x*(ax.z*ax.x*cosb - ax.y*sina)
                          + y*(ax.z*ax.y*cosb + ax.x*sina)
                          + z*(ax.z*ax.z*cosb + cosa) );
        }
        
    };

    //

    static float mDegToRad(float degrees) {
        return degrees * DEG_TO_RAD;
    }


    //
    // Map utility
    //

    static float mmap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp = false) {
        
        if (fabs(inputMin - inputMax) < __FLT_EPSILON__){
            return outputMin;
        } else {
            float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
            
            if( clamp ){
                if(outputMax < outputMin){
                    if( outVal < outputMax )outVal = outputMax;
                    else if( outVal > outputMin )outVal = outputMin;
                }else{
                    if( outVal > outputMax )outVal = outputMax;
                    else if( outVal < outputMin )outVal = outputMin;
                }
            }
            return outVal;
        }
        
    }

    static float clamp(float a, float min, float max )
    {
        return (a < min) ? min : ((a > max) ? max : a);
    }

    static float alignAngle(float a, float min = -180, float max = 180)
    {
        while (a < min) a += 360;
        while (a > max) a -= 360;
        
        return a;
    }
    
    float radialDistance(float angle1, float angle2) {
        if ((std::abs(angle2 - angle1)) > (std::abs(std::abs(angle2 - angle1) - 360))) {
            return std::abs(std::abs(angle2 - angle1) - 360);
        } else return std::abs(angle2 - angle1);
    }
    
    float targetDirectionMultiplier(float angleCurrent, float angleTarget) {
        if (((std::abs(angleCurrent - angleTarget)) >
             (std::abs(angleCurrent - angleTarget + 360)))
            ||
            ((std::abs(angleCurrent - angleTarget)) >
             (std::abs(angleCurrent - angleTarget - 360)))
            )
        {
            if (angleCurrent < angleTarget) {
                return -1.;
            }
            if (angleCurrent > angleTarget) {
                return 1.;
            }
            
        } else {
            if (angleCurrent < angleTarget) {
                return 1.;
            }
            if (angleCurrent > angleTarget) {
                return -1.;
            }
        }
        
        return 0.;
    }
    
    // Envelope follower feature is defined here, in updateAngles()
    
    void updateAngles() {
        if (targetYaw < 0) targetYaw += 360;
        if (targetPitch < 0) targetPitch += 360;
        if (targetRoll < 0) targetRoll += 360;
        
        if (targetYaw > 360) targetYaw -= 360;
        if (targetPitch > 360) targetPitch -= 360;
        if (targetRoll > 360) targetRoll -= 360;
        
        if (currentYaw < 0) currentYaw += 360;
        if (currentPitch < 0) currentPitch += 360;
        if (currentRoll < 0) currentRoll += 360;
        
        if (currentYaw > 360) currentYaw -= 360;
        if (currentPitch > 360) currentPitch -= 360;
        if (currentRoll > 360) currentRoll -= 360;
        
        float distanceYaw = radialDistance(targetYaw, currentYaw);
        if (((distanceYaw) > 2.5) /* && (distanceYaw < 360) */) {
            currentYaw += 2.4 * targetDirectionMultiplier(currentYaw, targetYaw);
        } else {
            currentYaw = targetYaw;
        }
        
        float distancePitch = radialDistance(targetPitch, currentPitch);
        if (((distancePitch) > 2.5) /* && (distancePitch < 360) */) {
            currentPitch += 2.4 * targetDirectionMultiplier(currentPitch, targetPitch);
        } else {
            currentPitch = targetPitch;
        }
        
        float distanceRoll = radialDistance(targetRoll, currentRoll);
        if (((distanceRoll) > 2.5) && (distanceRoll < 360)) {
            currentRoll += 2.4 * targetDirectionMultiplier(currentRoll, targetRoll);
        } else {
            currentRoll = targetRoll;
        }
    };

    
public:
    
    M1DSPAlgorithms() {
        currentYaw = 0;
        currentPitch = 0;
        currentRoll = 0;
        
        targetYaw = 0;
        targetPitch = 0;
        targetRoll = 0;
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

    std::vector<float> fourChannelAlgorithm(float Yaw, float Pitch, float Roll, bool smoothAngles = false) {
        
        if (smoothAngles) {
            targetYaw = Yaw;
            targetPitch = Pitch;
            targetRoll = Roll;
            
            updateAngles();
            
            Yaw  = currentYaw;
            Pitch  = currentPitch;
            Roll = currentRoll;
        } else {
            targetYaw = Yaw;
            targetPitch = Pitch;
            targetRoll = Roll;
            
            currentYaw = Yaw;
            currentPitch = Pitch;
            currentRoll = Roll;
        }
        
        //Orientation input safety clamps/alignment
        Yaw = alignAngle(Yaw, 0, 360);
        
        float coefficients[4];
        coefficients[0] = 1. - std::min(1., std::min((float)360. - Yaw, Yaw) / 90.);
        coefficients[1] = 1. - std::min(1., std::abs((float)90. - Yaw) / 90.);
        coefficients[2] = 1. - std::min(1., std::abs((float)180. - Yaw) / 90.);
        coefficients[3] = 1. - std::min(1., std::abs((float)270. - Yaw) / 90.);
        
        
        std::vector<float> result;
        result.resize(8);
        result[0] = coefficients[0]; // 1 left
        result[1] = coefficients[3]; //   right
        result[2] = coefficients[1]; // 2 left
        result[3] = coefficients[0]; //   right
        result[4] = coefficients[3]; // 3 left
        result[5] = coefficients[2]; //   right
        result[6] = coefficients[2]; // 4 left
        result[7] = coefficients[1]; //   right
        
        result.push_back(1.0); // static stereo L
        result.push_back(1.0); // static stereo R
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

    std::vector<float> fourPairsAlgorithm(float Yaw, float Pitch, float Roll, bool smoothAngles = false) {
        
        if (smoothAngles) {
            targetYaw = Yaw;
            targetPitch = Pitch;
            targetRoll = Roll;
            
            updateAngles();
            
            Yaw  = currentYaw;
            Pitch  = currentPitch;
            Roll = currentRoll;
        } else {
            targetYaw = Yaw;
            targetPitch = Pitch;
            targetRoll = Roll;
            
            currentYaw = Yaw;
            currentPitch = Pitch;
            currentRoll = Roll;
        }
        
        //Orientation input safety clamps/alignment
        Yaw = alignAngle(Yaw, 0, 360);
        
        float volumes[4];
        volumes[0] = 1. - std::min(1., std::min((float)360. - Yaw, Yaw) / 90.);
        volumes[1] = 1. - std::min(1., std::abs((float)90. - Yaw) / 90.);
        volumes[2] = 1. - std::min(1., std::abs((float)180. - Yaw) / 90.);
        volumes[3] = 1. - std::min(1., std::abs((float)270. - Yaw) / 90.);
        
        std::vector<float> result;
        result.push_back(volumes[0]);
        result.push_back(volumes[1]);
        result.push_back(volumes[2]);
        result.push_back(volumes[3]);
        
        result.push_back(1.0); // static stereo L
        result.push_back(1.0); // static stereo R
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

    std::vector<float> eightChannelsIsotropicAlgorithm(float Yaw, float Pitch, float Roll, bool smoothAngles = false) {
        
        if (smoothAngles) {
            targetYaw = Yaw;
            targetPitch = Pitch;
            targetRoll = Roll;
            
            updateAngles();
            
            Yaw  = currentYaw;
            Pitch  = currentPitch;
            Roll = currentRoll;
        } else {
            targetYaw = Yaw;
            targetPitch = Pitch;
            targetRoll = Roll;
            
            currentYaw = Yaw;
            currentPitch = Pitch;
            currentRoll = Roll;
        }
        
        mPoint simulationAngles = mPoint(-Pitch, Yaw, Roll);
        
        mPoint faceVector1 = mPoint(  cos(mDegToRad(simulationAngles[1])),
                                    sin(mDegToRad(simulationAngles[1]))).normalize();
        
        
        mPoint faceVector2 = faceVector1.getRotated(simulationAngles[0],
                                                    mPoint(cos(mDegToRad(simulationAngles[1] - 90)),
                                                           sin(mDegToRad(simulationAngles[1] - 90))).normalize());
        
        
        mPoint faceVector21 = faceVector1.getRotated(simulationAngles[0] + 90,
                                                     mPoint(cos(mDegToRad(simulationAngles[1] - 90)),
                                                            sin(mDegToRad(simulationAngles[1] - 90))).normalize());
        
        mPoint faceVectorLeft = faceVector21.getRotated(-simulationAngles[2] + 90, faceVector2);
        mPoint faceVectorRight = faceVector21.getRotated(-simulationAngles[2] - 90, faceVector2);
        
        
        mPoint faceVectorOffsetted = mPoint(cos(mDegToRad(simulationAngles[1])),
                                            sin(mDegToRad(simulationAngles[1]))).normalize().rotate(
                                                                                                    simulationAngles[0] + 10,
                                                                                                    mPoint(cos(mDegToRad(simulationAngles[1] - 90)),
                                                                                                           sin(mDegToRad(simulationAngles[1] - 90))).normalize()) - faceVector2;
        
        mPoint tiltSphereRotated = faceVectorOffsetted.rotate(-simulationAngles[2], faceVector2);
        
        // Drawing another 8 dots
        
        mPoint points[8] =
        {   mPoint(100, -100, -100),
            mPoint(100, 100, -100),
            mPoint(-100, -100, -100),
            mPoint(-100, 100, -100),
            
            mPoint(100, -100, 100),
            mPoint(100, 100, 100),
            mPoint(-100, -100, 100),
            mPoint(-100, 100, 100)
            
        };
        
        float qL[8];
        for (int i = 0; i < 8; i++) {
            qL[i] = (faceVectorLeft * 100 + faceVector2 * 100 - points[i]).length();
        }
        
        float qR[8];
        for (int i = 0; i < 8; i++) {
            qR[i] = (faceVectorRight * 100 + faceVector2 * 100 - points[i]).length();
        }
        
        std::vector<float> result;
        result.resize(16);
        
        for (int i = 0; i < 8; i++) {
            float vL = clamp(mmap(qL[i], 0, 223, 1., 0.), 0, 1);
            float vR = clamp(mmap(qR[i], 0, 223, 1., 0.), 0, 1);
            
            result[i * 2] = vL;
            result[i  * 2 + 1] = vR;
            
        }
        
        // Volume Balancer v2.0
        
        float sumL = 0, sumR = 0;
        for (int i = 0; i < 8; i++) {
            sumL += result[i * 2];
            sumR += result[i * 2 + 1];
        }
        
        float multipliersL[8], multipliersR[8];
        for (int i = 0; i < 8; i++) {
            multipliersL[i] = result[i * 2] / sumL;
            multipliersR[i] = result[i * 2 + 1] / sumR;
        }
        
        float sumDiffL = sumL - 1.;
        float sumDiffR = sumR - 1.;
        
        float correctedVolumesL[8], correctedVolumesR[8];
        for (int i = 0; i < 8; i++) {
            correctedVolumesL[i] = result[i * 2] - sumDiffL * multipliersL[i];
            correctedVolumesR[i] = result[i * 2 + 1] - sumDiffR * multipliersR[i];
        }
        
        for (int i = 0; i < 8; i++) {
            result[i * 2] = correctedVolumesL[i];
            result[i * 2 + 1] = correctedVolumesR[i];
        }
        
        
        result.push_back(1.0); // static stereo L
        result.push_back(1.0); // static stereo R
        
        return result;
    }

    // ------------------------------------------------------------------

    //
    //  Eight channel audio format.
    //
    //  Order of input angles:
    //  Y = Yaw in angles
    //  P = Pitch in angles
    //  R = Roll in angles
    //

    std::vector<float> eightChannelsAlgorithm(float Yaw, float Pitch, float Roll, bool smoothAngles = false) {
        
        if (smoothAngles) {
            targetYaw = Yaw;
            targetPitch = Pitch;
            targetRoll = Roll;
            
            updateAngles();
            
            Yaw  = currentYaw;
            Pitch  = currentPitch;
            Roll = currentRoll;
        } else {
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
        
        float coefficients[8];
        coefficients[0] = 1. - std::min(1., std::min((float)360. - Yaw, Yaw) / 90.);
        coefficients[1] = 1. - std::min(1., std::abs((float)90. - Yaw) / 90.);
        coefficients[2] = 1. - std::min(1., std::abs((float)180. - Yaw) / 90.);
        coefficients[3] = 1. - std::min(1., std::abs((float)270. - Yaw) / 90.);
        
        float tiltAngle = mmap(Roll, -90., 90., 0., 1., true);
        //Use Equal Power if engine requires
        /*
         float tiltHigh = cos(tiltAngle * (0.5 * PI));
         float tiltLow = cos((1.0 - tiltAngle) * (0.5 * PI));
         */
        float tiltHigh = tiltAngle;
        float tiltLow = 1. - tiltHigh;
        
        //ISSUE//
        //Able to kill stereo by making both pitch and tilt at max or min values together without proper clamps
        
        std::vector<float> result;
        result.resize(16);
        result[0] = coefficients[0] * tiltHigh * 2.0; // 1 left
        result[1] = coefficients[3] * tiltHigh * 2.0; //   right
        result[2] = coefficients[1] * tiltLow * 2.0; // 2 left
        result[3] = coefficients[0] * tiltLow * 2.0; //   right
        result[4] = coefficients[3] * tiltLow * 2.0; // 3 left
        result[5] = coefficients[2] * tiltLow * 2.0; //   right
        result[6] = coefficients[2] * tiltHigh * 2.0; // 4 left
        result[7] = coefficients[1] * tiltHigh * 2.0; //   right
        
        result[0 + 8] = coefficients[0] * tiltLow * 2.0; // 1 left
        result[1 + 8] = coefficients[3] * tiltLow * 2.0; //   right
        result[2 + 8] = coefficients[1] * tiltHigh * 2.0; // 2 left
        result[3 + 8] = coefficients[0] * tiltHigh * 2.0; //   right
        result[4 + 8] = coefficients[3] * tiltHigh * 2.0; // 3 left
        result[5 + 8] = coefficients[2] * tiltHigh * 2.0; //   right
        result[6 + 8] = coefficients[2] * tiltLow * 2.0; // 4 left
        result[7 + 8] = coefficients[1] * tiltLow * 2.0; //   right
        
        float pitchAngle = mmap(Pitch, 90., -90., 0., 1., true);
        //Use Equal Power if engine requires
        /*
         float pitchHigherHalf = cos(pitchAngle * (0.5*PI));
         float pitchLowerHalf = cos((1.0 - pitchAngle) * (0.5*PI));
         */
        float pitchHigherHalf = pitchAngle;
        float pitchLowerHalf = 1. - pitchHigherHalf;
        
        for (int i = 0; i < 8; i++) {
            result[i] *= pitchLowerHalf;
            result[i + 8] *= pitchHigherHalf;
        }
        
        result.push_back(1.0); // static stereo L
        result.push_back(1.0); // static stereo R
        
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

    std::vector<float> eightPairsAlgorithm(float Yaw, float Pitch, float Roll, bool smoothAngles = false) {
        
        if (smoothAngles) {
            targetYaw = Yaw;
            targetPitch = Pitch;
            targetRoll = Roll;
            
            updateAngles();
            
            Yaw  = currentYaw;
            Pitch  = currentPitch;
            Roll = currentRoll;
        } else {
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
        
        result.push_back(1.0); // static stereo L
        result.push_back(1.0); // static stereo R
        
        return result;
    }

    // ------------------------------------------------------------------
        
};

#endif /* M1DSPAlgorithms_h */