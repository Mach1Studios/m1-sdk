//  Mach1 SDK
//  Copyright © 2018 Mach1. All rights reserved.
//
//  Implementation

/*
DISCLAIMER:
This header file is not an example of use but an decoder that will require periodic
updates and should not be integrated in sections but remain as an update-able factored file.
*/

#include "Mach1DecodeCore.h"

#ifndef DEG_TO_RAD
#define DEG_TO_RAD (PI/180.0)
#endif

#ifndef __FLT_EPSILON__
#define __FLT_EPSILON__ 1.19209290e-07F
#endif


float Mach1DecodeCore::lerp(float x1, float x2, float t)
{
	return x1 + (x2 - x1)*t;
}

float Mach1DecodeCore::mDegToRad(float degrees) {
	return (float)(degrees * DEG_TO_RAD);
}

//
// Map utility
//

float Mach1DecodeCore::mmap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp) {

	if (fabs(inputMin - inputMax) < __FLT_EPSILON__) {
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

float Mach1DecodeCore::clamp(float a, float min, float max)
{
	return (a < min) ? min : ((a > max) ? max : a);
}

float Mach1DecodeCore::alignAngle(float a, float min, float max)
{
	if (a > 5000 || a < -5000) return 0;

	while (a < min) a += 360;
	while (a > max) a -= 360;

	return a;
}

float Mach1DecodeCore::radialDistance(float angle1, float angle2) {
	if ((std::abs(angle2 - angle1)) > (std::abs(std::abs(angle2 - angle1) - 360))) {
		return std::abs(std::abs(angle2 - angle1) - 360);
	}
	else return std::abs(angle2 - angle1);
}

float Mach1DecodeCore::targetDirectionMultiplier(float angleCurrent, float angleTarget) {
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

	}
	else {
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
void Mach1DecodeCore::updateAngles() {
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

	if (filterSpeed >= 1.0) {
		currentYaw = targetYaw;
		currentPitch = targetPitch;
		currentRoll = targetRoll;
	}
	else {
		float speedAngle = timeLastUpdate ? filterSpeed * 1.0f * (getCurrentTime() - timeLastUpdate) : 0;

		timeLastUpdate = getCurrentTime();

		float distanceYaw = radialDistance(targetYaw, currentYaw);
		if (((distanceYaw) > speedAngle) /* && (distanceYaw < 360) */) {
			currentYaw += speedAngle * targetDirectionMultiplier(currentYaw, targetYaw);
		}
		else {
			currentYaw = targetYaw;
		}

		float distancePitch = radialDistance(targetPitch, currentPitch);
		if (((distancePitch) > speedAngle) /* && (distancePitch < 360) */) {
			currentPitch += speedAngle * targetDirectionMultiplier(currentPitch, targetPitch);
		}
		else {
			currentPitch = targetPitch;
		}

		float distanceRoll = radialDistance(targetRoll, currentRoll);
		if (((distanceRoll) > speedAngle) && (distanceRoll < 360)) {
			currentRoll += speedAngle * targetDirectionMultiplier(currentRoll, targetRoll);
		}
		else {
			currentRoll = targetRoll;
		}
	}
};

// Angular settings functions
void Mach1DecodeCore::convertAnglesToMach1(Mach1PlatformType platformType, float* Y, float* P, float* R) {
	switch (platformType) {
	case Mach1PlatformDefault:
		*Y = *Y;
		*P = *P;
		*R = *R;
		break;

	case Mach1PlatformiOS:
        std::swap(*P, *Y);
        *Y = -*Y;
        *P = -*P;
        *R = *R;
		break;

	case Mach1PlatformUnity:
		std::swap(*P, *Y);
		*Y = *Y;                   // Y in Unity
		*P = *P;             // X in Unity
		*R = *R;                    // Z in Unity
		break;

	case Mach1PlatformUE:
		*Y = -*Y;
		std::swap(*Y, *P);
		std::swap(*P, *R);

		std::swap(*P, *Y);
		*Y = *Y;                   // Y in Unity
		*P = *P;             // X in Unity
		*R = *R;                    // Z in Unity
		break;

	case Mach1PlatformOfEasyCam:
		*Y = *Y;
		*P = -*P;
		*R = -*R;
		break;

	case Mach1PlatformAndroid:
        std::swap(*P, *Y);
		*Y = -*Y;
		*P = -*P;
		*R = *R;
		break;		

	default:
		break;
	}
}

void Mach1DecodeCore::convertAnglesToPlatform(Mach1PlatformType platformType, float * Y, float * P, float * R)
{
	 switch (platformType) {
	 case Mach1PlatformDefault:
		 *Y = *Y;
		 *P = *P;
		 *R = *R;
		 break;

	 case Mach1PlatformiOS:
        *Y = -*Y;
        *P = -*P;
        *R = *R;
        std::swap(*P, *Y);
  	    break;

	 case Mach1PlatformUnity:
		 *Y = *Y;                   // Y in Unity
		 *P = *P;             // X in Unity
		 *R = *R;                    // Z in Unity
		 std::swap(*P, *Y);
		 break;

	 case Mach1PlatformUE:
		 *Y = *Y;                    // Y in UE
		 *P = (*P > 360 ? *P - 360 : *P);   // Z in UE
		 *R = *R;                    // X in UE
		 std::swap(*R, *Y);
		 break;

	 case Mach1PlatformOfEasyCam:
		 *Y = *Y;
		 *P = -*P;
		 *R = -*R;
		 break;

	 case Mach1PlatformAndroid:
		 *Y = -*Y;
		 *P = -*P;
		 *R = *R;
		 std::swap(*P, *Y);
		 break;

	 default:
		 break;
	 }
 }

void Mach1DecodeCore::addToLog(std::string str, int maxCount)
{
	if (strLog.size() > maxCount) strLog.erase(strLog.begin() + maxCount, strLog.begin() + strLog.size() - 1);

	strLog.push_back(str);
}

char* Mach1DecodeCore::getLog()
{
	std::vector<std::string> _strLog = strLog;
	strLog.clear();

	static char log[100000] = "";

	log[0] = '\0';
    /*
	for (int i = 0; i<_strLog.size(); i++) strcat(log, _strLog[i].c_str());
     */
	return log;
}

Mach1DecodeCore::Mach1DecodeCore() {
	currentYaw = 0;
	currentPitch = 0;
	currentRoll = 0;

	targetYaw = 0;
	targetPitch = 0;
	targetRoll = 0;

	previousYaw = 0; 
	previousPitch = 0;
	previousRoll = 0;
		
	filterSpeed = 0.9f;
	timeLastUpdate = 0;
	timeLastCalculation = 0;

	platformType = Mach1PlatformDefault;
    algorithmType = Mach1DecodeAlgoSpatial;

	ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

	smoothAngles = true; // false

	strLog.resize(0);
}

long Mach1DecodeCore::getCurrentTime()
{
	return (long)(duration_cast<milliseconds>(system_clock::now().time_since_epoch()) - ms).count();
}

long Mach1DecodeCore::getLastCalculationTime()
{
	return timeLastCalculation;
}

void Mach1DecodeCore::setPlatformType(Mach1PlatformType type) {
	platformType = type;
}

Mach1PlatformType Mach1DecodeCore::getPlatformType()
{
	return platformType;
}

void Mach1DecodeCore::setFilterSpeed(float newFilterSpeed) {
	filterSpeed = newFilterSpeed;
}

//--------------------------------------------------

void Mach1DecodeCore::setDecodeAlgoType(Mach1DecodeAlgoType newAlgorithmType) {
	algorithmType = newAlgorithmType;
}

Mach1DecodeAlgoType Mach1DecodeCore::getDecodeAlgoType()
{
	return algorithmType;
}

std::vector<float> Mach1DecodeCore::decode(float Yaw, float Pitch, float Roll, int bufferSize, int sampleIndex) {
	long tStart = getCurrentTime();
	std::vector<float> res;

	switch (algorithmType) {
            // m1Spatial = 0, m1AltSpatial, m1Horizon, m1HorizonPairs, m1SpatialPairs
        case Mach1DecodeAlgoSpatial:
			res = spatialAlgo(Yaw, Pitch, Roll, bufferSize, sampleIndex);
            break;

        case Mach1DecodeAlgoAltSpatial:
			res = spatialAltAlgo(Yaw, Pitch, Roll, bufferSize, sampleIndex);
            break;

        case Mach1DecodeAlgoHorizon:
			res = horizonAlgo(Yaw, Pitch, Roll, bufferSize, sampleIndex);
            break;

        case Mach1DecodeAlgoHorizonPairs:
			res = horizonPairsAlgo(Yaw, Pitch, Roll, bufferSize, sampleIndex);
            break;

		case Mach1DecodeAlgoSpatialPairs:
			res = spatialPairsAlgo(Yaw, Pitch, Roll, bufferSize, sampleIndex);
			break;

		case Mach1DecodeAlgoSpatialPlus:
			spatialPlusAlgo(Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
			break;

		case Mach1DecodeAlgoSpatialPlusPlus:
			spatialPlusPlusAlgo(Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
			break;

		case Mach1DecodeAlgoSpatialExt:
			spatialPlusAlgo(Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
			break;

		case Mach1DecodeAlgoSpatialExtPlus:
			spatialPlusPlusAlgo(Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
			break;

		default:
            break;
    }

	timeLastCalculation = getCurrentTime() - tStart;
	return res;
}

// Decode using the current algorithm type in a more efficient way

void Mach1DecodeCore::decode(float Yaw, float Pitch, float Roll, float *result, int bufferSize, int sampleIndex) {
	long tStart = getCurrentTime();

	switch (algorithmType) {
            // m1Spatial = 0, m1AltSpatial, m1Horizon, m1HorizonPairs, m1SpatialPairs
        case Mach1DecodeAlgoSpatial:
            spatialAlgo(Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
            break;
            
        case Mach1DecodeAlgoAltSpatial:
            spatialAltAlgo(Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
            break;
            
        case Mach1DecodeAlgoHorizon:
            horizonAlgo(Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
            break;
            
        case Mach1DecodeAlgoHorizonPairs:
            horizonPairsAlgo(Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
            break;
            
        case Mach1DecodeAlgoSpatialPairs:
            spatialPairsAlgo(Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
            break;

		case Mach1DecodeAlgoSpatialPlus:
			spatialPlusAlgo(Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
			break;

		case Mach1DecodeAlgoSpatialPlusPlus:
			spatialPlusPlusAlgo(Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
			break;

		case Mach1DecodeAlgoSpatialExt:
			spatialPlusAlgo(Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
			break;

		case Mach1DecodeAlgoSpatialExtPlus:
			spatialPlusPlusAlgo(Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
			break;

        default:
            break;
    }

	timeLastCalculation = getCurrentTime() - tStart;
}

// The following functions are deprecated as of now

//--------------------------------------------------

//  Begin function that has to be called
//  before per-sample coefficient calculation.
//

void Mach1DecodeCore::beginBuffer() {
	previousYaw = currentYaw;
	previousPitch = currentPitch;
	previousRoll = currentRoll;
    
    updateAngles();
}


//  End function that has to be called
//  after per-sample coefficient calculation.
//

void Mach1DecodeCore::endBuffer() {

	//  ;)

}

void Mach1DecodeCore::processSample(functionAlgoSampleHP funcAlgoSampleHP, float Yaw, float Pitch, float Roll, float *result, int bufferSize, int sampleIndex) {
	convertAnglesToMach1(platformType, &Yaw, &Pitch, &Roll);

	/*char buff[1024];
	 snprintf(buff, sizeof(buff), "%.5f - current (%.4f %.4f %.4f), target (%.4f %.4f %.4f), previous (%.4f %.4f %.4f) \r\n", timeLastUpdate ? timeLastUpdate / 1000.0 : 0, currentYaw, currentPitch, currentRoll, targetYaw, targetPitch, targetRoll, previousYaw, previousPitch, previousRoll);
	 addToLog(buff);*/

	if (smoothAngles) {

		targetYaw = Yaw;
		targetPitch = Pitch;
		targetRoll = Roll;

		if (bufferSize > 0) {

			// we're in per sample mode
			// returning values from right here!

			float volumes1[18];
			float volumes2[18];
			(this->*funcAlgoSampleHP)(previousYaw, previousPitch, previousRoll, volumes1);
			(this->*funcAlgoSampleHP)(currentYaw, currentPitch, currentRoll, volumes2);
			float phase = (float)sampleIndex / (float)bufferSize;

			float volumes_lerp[18];
			for (int i = 0; i < 18; i++) {
				volumes_lerp[i] = volumes1[i] * (1 - phase) + volumes2[i] * phase;
				result[i] = volumes_lerp[i];
			}
            
            return;
		}
		else {
			// Filtering per-buffer
			 Yaw = currentYaw;
			 Pitch = currentPitch;
			 Roll = currentRoll;

			 previousYaw = currentYaw;
			 previousPitch = currentPitch;
			 previousRoll = currentRoll;
		}

	}
	else {
		// for test purpose only!
		targetYaw = Yaw;
		targetPitch = Pitch;
		targetRoll = Roll;

		currentYaw = Yaw;
		currentPitch = Pitch;
		currentRoll = Roll;

		previousYaw = currentYaw;
		previousPitch = currentPitch;
		previousRoll = currentRoll;
	}
    
	//printf("%f, %f, %f\r\n", Yaw, Pitch, Roll);

    (this->*funcAlgoSampleHP)(Yaw, Pitch, Roll, result);
}

std::vector<float> Mach1DecodeCore::processSample(functionAlgoSample funcAlgoSample, float Yaw, float Pitch, float Roll, int bufferSize, int sampleIndex) {
	convertAnglesToMach1(platformType, &Yaw, &Pitch, &Roll);
	
	/*char buff[1024];
	snprintf(buff, sizeof(buff), "%.5f - current (%.4f %.4f %.4f), target (%.4f %.4f %.4f), previous (%.4f %.4f %.4f) \r\n", timeLastUpdate ? timeLastUpdate / 1000.0 : 0, currentYaw, currentPitch, currentRoll, targetYaw, targetPitch, targetRoll, previousYaw, previousPitch, previousRoll);
	addToLog(buff);*/

	if (smoothAngles) {

		targetYaw = Yaw;
		targetPitch = Pitch;
		targetRoll = Roll;

		if (bufferSize > 0) {

			// we're in per sample mode
            // returning values from right here!
			std::vector<float> volumes1 = (this->*funcAlgoSample)(previousYaw, previousPitch, previousRoll);
			std::vector<float> volumes2 = (this->*funcAlgoSample)(currentYaw, currentPitch, currentRoll);
            float phase = (float)sampleIndex / (float)bufferSize;
            std::vector<float> volumes_lerp;
            volumes_lerp.resize(volumes1.size());
            for (int i = 0; i < volumes1.size(); i++) {
                volumes_lerp[i] = volumes1[i] * (1 - phase) + volumes2[i] * phase;
            }
            return volumes_lerp;
		}
		else {
            // Filtering per-buffer
 			Yaw = currentYaw;
			Pitch = currentPitch;
			Roll = currentRoll;

            previousYaw = currentYaw;
            previousPitch = currentPitch;
            previousRoll = currentRoll;
        }
	}
	else {
		targetYaw = Yaw;
		targetPitch = Pitch;
		targetRoll = Roll;

		currentYaw = Yaw;
		currentPitch = Pitch;
		currentRoll = Roll;
        
        previousYaw = currentYaw;
        previousPitch = currentPitch;
        previousRoll = currentRoll;
	}

    return (this->*funcAlgoSample)(Yaw, Pitch, Roll);
}

//
//  Four channel audio format
//
//  Order of input angles:
//  Y = Yaw in degrees
//  P = Pitch in degrees
//  R = Roll in degrees
//

std::vector<float> Mach1DecodeCore::horizonAlgo(float Yaw, float Pitch, float Roll,
	int bufferSize, int sampleIndex) {
	
	convertAnglesToMach1(platformType, &Yaw, &Pitch, &Roll);


	if (smoothAngles) {

		targetYaw = Yaw;
		targetPitch = Pitch;
		targetRoll = Roll;

        updateAngles();

		Yaw = currentYaw;
		Pitch = currentPitch;
		Roll = currentRoll;

	}
	else {
		targetYaw = Yaw;
		targetPitch = Pitch;
		targetRoll = Roll;

		currentYaw = Yaw;
		currentPitch = Pitch;
		currentRoll = Roll;

        previousYaw = currentYaw;
        previousPitch = currentPitch;
        previousRoll = currentRoll;
	}

	//Orientation input safety clamps/alignment
	Yaw = alignAngle(Yaw, 0, 360);

	float coefficients[4];
	coefficients[0] = 1.f - std::min(1.f, std::min(360.f - Yaw, Yaw) / 90.f);
	coefficients[1] = 1.f - std::min(1.f, std::abs(90.f - Yaw) / 90.f);
	coefficients[2] = 1.f - std::min(1.f, std::abs(180.f - Yaw) / 90.f);
	coefficients[3] = 1.f - std::min(1.f, std::abs(270.f - Yaw) / 90.f);

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

void Mach1DecodeCore::horizonAlgo(float Yaw, float Pitch, float Roll, float *result,
                                            int bufferSize, int sampleIndex) {
    convertAnglesToMach1(platformType, &Yaw, &Pitch, &Roll);
    
    if (smoothAngles) {
        targetYaw = Yaw;
        targetPitch = Pitch;
        targetRoll = Roll;
        
        updateAngles();
        
        Yaw = currentYaw;
        Pitch = currentPitch;
        Roll = currentRoll;
    }
    else {
        targetYaw = Yaw;
        targetPitch = Pitch;
        targetRoll = Roll;
        
        currentYaw = Yaw;
        currentPitch = Pitch;
        currentRoll = Roll;
        
        previousYaw = currentYaw;
        previousPitch = currentPitch;
        previousRoll = currentRoll;
    }

    //Orientation input safety clamps/alignment
    Yaw = alignAngle(Yaw, 0, 360);
    
    float coefficients[4];
    coefficients[0] = 1.f - std::min(1.f, std::min(360.f - Yaw, Yaw) / 90.f);
    coefficients[1] = 1.f - std::min(1.f, std::abs(90.f - Yaw) / 90.f);
    coefficients[2] = 1.f - std::min(1.f, std::abs(180.f - Yaw) / 90.f);
    coefficients[3] = 1.f - std::min(1.f, std::abs(270.f - Yaw) / 90.f);
    
    
    result[0] = coefficients[0]; // 1 left
    result[1] = coefficients[3]; //   right
    result[2] = coefficients[1]; // 2 left
    result[3] = coefficients[0]; //   right
    result[4] = coefficients[3]; // 3 left
    result[5] = coefficients[2]; //   right
    result[6] = coefficients[2]; // 4 left
    result[7] = coefficients[1]; //   right
    
    result[8] = 1.0; // static stereo L
    result[9] = 1.0; // static stereo R
}

// ------------------------------------------------------------------

//
//  Four pairs audio format.
//
//  Order of input angles:
//  Y = Yaw in degrees
//  P = Pitch in degrees
//  R = Roll in degrees
//

std::vector<float> Mach1DecodeCore::horizonPairsAlgo(float Yaw, float Pitch, float Roll,
	int bufferSize, int sampleIndex) {

	return processSample(&Mach1DecodeCore::horizonPairsAlgoSample, Yaw, Pitch, Roll, bufferSize, sampleIndex);
}

void Mach1DecodeCore::horizonPairsAlgo(float Yaw, float Pitch, float Roll, float *result,
                                                 int bufferSize, int sampleIndex) {
    
    return processSample(&Mach1DecodeCore::horizonPairsAlgoSample, Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
}

// ------------------------------------------------------------------

//
//  Eight channel audio format (isotropic version).
//
//  Order of input angles:
//  Y = Yaw in degrees
//  P = Pitch in degrees
//  R = Roll in degrees
//

std::vector<float> Mach1DecodeCore::spatialAlgo(float Yaw, float Pitch, float Roll,
	int bufferSize, int sampleIndex) {

	return processSample(&Mach1DecodeCore::spatialAlgoSample, Yaw, Pitch, Roll, bufferSize, sampleIndex);
}

void Mach1DecodeCore::spatialAlgo(float Yaw, float Pitch, float Roll, float *result, int bufferSize, int sampleIndex) {
    processSample(&Mach1DecodeCore::spatialAlgoSample, Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
}

// ------------------------------------------------------------------

std::vector<float> Mach1DecodeCore::spatialPlusAlgo(float Yaw, float Pitch, float Roll,
	int bufferSize, int sampleIndex) {

	return processSample(&Mach1DecodeCore::spatialPlusAlgoSample, Yaw, Pitch, Roll, bufferSize, sampleIndex);
}

void Mach1DecodeCore::spatialPlusAlgo(float Yaw, float Pitch, float Roll, float *result,
	int bufferSize, int sampleIndex) {

	return processSample(&Mach1DecodeCore::spatialPlusAlgoSample, Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
}

// ------------------------------------------------------------------

std::vector<float> Mach1DecodeCore::spatialPlusPlusAlgo(float Yaw, float Pitch, float Roll,
	int bufferSize, int sampleIndex) {

	return processSample(&Mach1DecodeCore::spatialPlusPlusAlgoSample, Yaw, Pitch, Roll, bufferSize, sampleIndex);
}

void Mach1DecodeCore::spatialPlusPlusAlgo(float Yaw, float Pitch, float Roll, float *result,
	int bufferSize, int sampleIndex) {

	return processSample(&Mach1DecodeCore::spatialPlusPlusAlgoSample, Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
}

// ------------------------------------------------------------------

//
//  Eight channel audio format.
//
//  Order of input angles:
//  Y = Yaw in degrees
//  P = Pitch in degrees
//  R = Roll in degrees
//

std::vector<float> Mach1DecodeCore::spatialAltAlgo(float Yaw, float Pitch, float Roll,
	int bufferSize, int sampleIndex) {

	return processSample(&Mach1DecodeCore::spatialAltAlgoSample, Yaw, Pitch, Roll, bufferSize, sampleIndex);
}

void Mach1DecodeCore::spatialAltAlgo(float Yaw, float Pitch, float Roll, float *result,
                                               int bufferSize, int sampleIndex) {
    
    return processSample(&Mach1DecodeCore::spatialAltAlgoSample, Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
}

// ------------------------------------------------------------------

//
//  Eight pairs audio format.
//
//  Order of input angles:
//  Y = Yaw in degrees
//  P = Pitch in degrees
//  R = Roll in degrees
//

std::vector<float> Mach1DecodeCore::spatialPairsAlgo(float Yaw, float Pitch, float Roll,
	int bufferSize, int sampleIndex) {

	convertAnglesToMach1(platformType, &Yaw, &Pitch, &Roll);


	if (smoothAngles) {

		targetYaw = Yaw;
		targetPitch = Pitch;
		targetRoll = Roll;

		updateAngles();

		if (bufferSize > 0) {

			// we're in per sample mode

			Yaw = alignAngle(lerp(previousYaw + 360, currentYaw + 360, (float)sampleIndex / (float)bufferSize) - 360,
				0, 360);
			Pitch = alignAngle(lerp(previousPitch + 360, currentPitch + 360, (float)sampleIndex / (float)bufferSize) - 360,
				-90, 90);
			Roll = alignAngle(lerp(previousRoll + 360, currentRoll + 360, (float)sampleIndex / (float)bufferSize) - 360,
				-180, 180);

			Yaw = currentYaw;
			Pitch = currentPitch;
			Roll = currentRoll;

		}
		else {

			Yaw = currentYaw;
			Pitch = currentPitch;
			Roll = currentRoll;

		}

	}
	else {
		targetYaw = Yaw;
		targetPitch = Pitch;
		targetRoll = Roll;

		currentYaw = Yaw;
		currentPitch = Pitch;
		currentRoll = Roll;
        
        previousYaw = currentYaw;
        previousPitch = currentPitch;
        previousRoll = currentRoll;
	}


	//Orientation input safety clamps/alignment
	Pitch = alignAngle(Pitch, -180, 180);
	Pitch = clamp(Pitch, -90, 90); // -90, 90

	Yaw = alignAngle(Yaw, 0, 360);

	float volumes[8]; 
	volumes[0] = 1.f - std::min(1.f, std::min(360.f - Yaw, Yaw) / 90.f);
	volumes[1] = 1.f - std::min(1.f, std::abs(90.f - Yaw) / 90.f);
	volumes[2] = 1.f - std::min(1.f, std::abs(180.f - Yaw) / 90.f);
	volumes[3] = 1.f - std::min(1.f, std::abs(270.f - Yaw) / 90.f);

	float pitchAngle = mmap(Pitch, 90., -90., 0., 1., true);
	//Use Equal Power if engine requires
	/*
	float pitchHigherHalf = cos(pitchAngle * (0.5*PI));
	float pitchLowerHalf = cos((1.0 - pitchAngle) * (0.5*PI));
	*/
	float pitchHigherHalf = pitchAngle;
	float pitchLowerHalf = 1.f - pitchHigherHalf;

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

void Mach1DecodeCore::spatialPairsAlgo(float Yaw, float Pitch, float Roll, float *result,
                                                 int bufferSize, int sampleIndex) {
    
    convertAnglesToMach1(platformType, &Yaw, &Pitch, &Roll);
    
    
    if (smoothAngles) {
        
        targetYaw = Yaw;
        targetPitch = Pitch;
        targetRoll = Roll;
        
        updateAngles();
        
        if (bufferSize > 0) {
            
            // we're in per sample mode
            Yaw = alignAngle(lerp(previousYaw + 360, currentYaw + 360, (float)sampleIndex / (float)bufferSize) - 360,
                             0, 360);
            Pitch = alignAngle(lerp(previousPitch + 360, currentPitch + 360, (float)sampleIndex / (float)bufferSize) - 360,
                               -90, 90);
            Roll = alignAngle(lerp(previousRoll + 360, currentRoll + 360, (float)sampleIndex / (float)bufferSize) - 360,
                              -180, 180);
            Yaw = currentYaw;
            Pitch = currentPitch;
            Roll = currentRoll;
        }
        else {
            Yaw = currentYaw;
            Pitch = currentPitch;
            Roll = currentRoll;
        }
    }
    else {
        targetYaw = Yaw;
        targetPitch = Pitch;
        targetRoll = Roll;
        
        currentYaw = Yaw;
        currentPitch = Pitch;
        currentRoll = Roll;
        
        previousYaw = currentYaw;
        previousPitch = currentPitch;
        previousRoll = currentRoll;
    }
    
    //Orientation input safety clamps/alignment
    Pitch = alignAngle(Pitch, -180, 180);
    Pitch = clamp(Pitch, -90, 90); // -90, 90
    
    Yaw = alignAngle(Yaw, 0, 360);
    
    float volumes[8];
    volumes[0] = 1.f - std::min(1.f, std::min(360.f - Yaw, Yaw) / 90.f);
    volumes[1] = 1.f - std::min(1.f, std::abs(90.f - Yaw) / 90.f);
    volumes[2] = 1.f - std::min(1.f, std::abs(180.f - Yaw) / 90.f);
    volumes[3] = 1.f - std::min(1.f, std::abs(270.f - Yaw) / 90.f);
    
    float pitchAngle = mmap(Pitch, 90., -90., 0., 1., true);
    //Use Equal Power if engine requires
    /*
     float pitchHigherHalf = cos(pitchAngle * (0.5*PI));
     float pitchLowerHalf = cos((1.0 - pitchAngle) * (0.5*PI));
     */
    float pitchHigherHalf = pitchAngle;
    float pitchLowerHalf = 1.f - pitchHigherHalf;
    
    result[0] = (volumes[0] * pitchHigherHalf);
    result[1] = (volumes[1] * pitchHigherHalf);
    result[2] = (volumes[2] * pitchHigherHalf);
    result[3] = (volumes[3] * pitchHigherHalf);
    result[4] = (volumes[4] * pitchLowerHalf);
    result[5] = (volumes[5] * pitchLowerHalf);
    result[6] = (volumes[6] * pitchLowerHalf);
    result[7] = (volumes[7] * pitchLowerHalf);
    
}

// ------------------------------------------------------------------

