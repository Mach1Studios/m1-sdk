//
//  Mach1 SDK
//
//  Multichannel audio format family
//
//  Mixing algorithms UE
//
//  Updated to match: 0.9.5

#pragma once


#include <vector>
#include <cmath>

#define M1_FLT_EPSILON 1.19209290e-07F

//
// Point utility class
//

//////////////

#ifndef DEG_TO_RAD
#define DEG_TO_RAD (PI/180.0)
#endif


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

static float mDegToRad(float degrees) {
    return degrees * DEG_TO_RAD;
}

//Utility function for mapping values
static float mmap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp) {

	if (fabs(inputMin - inputMax) < M1_FLT_EPSILON) {
		return outputMin;
	}
	else {
		float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);

		if (clamp) {
			if (outputMax < outputMin) {
				if (outVal < outputMax)outVal = outputMax;
				else if (outVal > outputMin)outVal = outputMin;
			}
			else {
				if (outVal > outputMax)outVal = outputMax;
				else if (outVal < outputMin)outVal = outputMin;
			}
		}
		return outVal;
	}

}

//Utility function for clamping values 
static float clamp(float a, float min, float max )
{
	return (a < min) ? min : ((a > max) ? max : a);
}

//Utility function for ensuring angle is aligned to -180/180 range
static float alignAngle(float a, float min = -180, float max = 180)
{
    return fmod(a+max, max);
}


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

static std::vector<float> fourChannelAlgorithm(float X, float Y, float Z) {
	float coefficients[4];
	coefficients[0] = 1. - FMath::Min(1., FMath::Min((float)360. - Y, Y) / 90.);
	coefficients[1] = 1. - FMath::Min(1., std::abs((float)90. - Y) / 90.);
	coefficients[2] = 1. - FMath::Min(1., std::abs((float)180. - Y) / 90.);
	coefficients[3] = 1. - FMath::Min(1., std::abs((float)270. - Y) / 90.);


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

static std::vector<float> eightChannelsAlgorithm(float X, float Y, float Z) {
	//ensure the angles are clamped and aligned on input
	X = alignAngle(X, -180, 180);
	X = clamp(X, -90, 90); // -90, 90

	Y = alignAngle(Y, 0, 360);

	Z = alignAngle(Z, -180, 180);
	Z = clamp(Z, -90, 90); // -90, 90
	 
	//setup the yaw math
	float coefficients[8];
	coefficients[0] = 1. - FMath::Min(1., FMath::Min((float)360. - Y, Y) / 90.);
	coefficients[1] = 1. - FMath::Min(1., std::abs((float)90. - Y) / 90.);
	coefficients[2] = 1. - FMath::Min(1., std::abs((float)180. - Y) / 90.);
	coefficients[3] = 1. - FMath::Min(1., std::abs((float)270. - Y) / 90.);

	fourChannelAlgorithm(X, Y, Z);

	float tiltAngle = mmap(Z, -90, 90, 0., 1., true);
	//Equal Power crossfade if needed
	//float tiltHigh = cos(tiltAngle * (0.5 * PI));
	//float tiltLow = cos((1.0 - tiltAngle) * (0.5 * PI));
	float tiltHigh = tiltAngle;
	float tiltLow = 1 - tiltHigh;

	//ISSUE//
	//Able to kill stereo by making both pitch and tilt at max or min values together
	//With an HMD this is difficult for a user to exploit

	//routes the coeffs to all the needed stereo pairs and applies tilt multiplier
	std::vector<float> result;
	result.resize(16);
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

	//Pitch orientation input
	float pitchAngle = mmap(X, 90, -90, 0., 1., true);
	//Equal Power crossfade if needed
	//float pitchHigherHalf = cos(pitchAngle * (0.5*PI));
	//float pitchLowerHalf = cos((1.0 - pitchAngle) * (0.5*PI));
	float pitchHigherHalf = pitchAngle;
	float pitchLowerHalf = 1 - pitchHigherHalf;

	for (int i = 0; i < 8; i++) {
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

static std::vector<float> eightChannelsIsotropicAlgorithm(float Yaw, float Pitch, float Roll) {
    mPoint simulationAngles = mPoint(Yaw, Pitch, Roll);
    
    mPoint faceVector1 = mPoint(  cos(mDegToRad(simulationAngles[1])),
                                sin(mDegToRad(simulationAngles[1]))).normalize();
    
    
    mPoint faceVector2 = faceVector1.getRotated(simulationAngles[0],
                                                mPoint(cos(mDegToRad(simulationAngles[1] - 90)),
                                                       sin(mDegToRad(simulationAngles[1] - 90))).normalize());
    
    
    mPoint faceVector21 = faceVector1.getRotated(simulationAngles[0] + 90,
                                                 mPoint(cos(mDegToRad(simulationAngles[1] - 90)),
                                                        sin(mDegToRad(simulationAngles[1] - 90))).normalize());
    
    mPoint faceVectorLeft = faceVector21.getRotated(-simulationAngles[2] - 90, faceVector2);
    mPoint faceVectorRight = faceVector21.getRotated(-simulationAngles[2] + 90, faceVector2);
    
    
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
        float vL = clamp(mmap(qL[i] * 2, 250, 400, 1., 0., false), 0, 1) / 2;
        float vR = clamp(mmap(qR[i] * 2, 250, 400, 1., 0., false), 0, 1) / 2;
        
        result[i * 2] = vR;
        result[i  * 2 + 1] = vL;
    }
    
    
    return result;
}
