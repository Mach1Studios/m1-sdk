//
//  Mach1 SDK
//
//  Multichannel audio format family
//
//  Mixing algorithms v 0.9.94b
//
//  Implementation

/*
DISCLAIMER:
This header file is not an example of use but an decoder that will require periodic
updates and should not be integrated in sections but remain as an update-able factored file.
*/


#include "Mach1Decode.h"

#ifndef DEG_TO_RAD
#define DEG_TO_RAD (PI/180.0)
#endif

#ifndef __FLT_EPSILON__
#define __FLT_EPSILON__ 1.19209290e-07F
#endif


Mach1Decode::mPoint::mPoint() {
	x = 0;
	y = 0;
	z = 0;
}

Mach1Decode::mPoint::mPoint(float X, float Y, float Z) {
	x = X;
	y = Y;
	z = Z;
}

Mach1Decode::mPoint::mPoint(float X, float Y) {
	x = X;
	y = Y;
	z = 0;
}

inline Mach1Decode::mPoint Mach1Decode::mPoint::operator+(const mPoint& pnt) const {
	return Mach1Decode::mPoint(x + pnt.x, y + pnt.y, z + pnt.z);
}


inline Mach1Decode::mPoint Mach1Decode::mPoint::operator*(const float f) const {
	return Mach1Decode::mPoint(x*f, y*f, z*f);
}


inline Mach1Decode::mPoint Mach1Decode::mPoint::operator*(const mPoint& vec) const {
	return Mach1Decode::mPoint(x*vec.x, y*vec.y, z*vec.z);
}


inline Mach1Decode::mPoint Mach1Decode::mPoint::operator-(const mPoint& vec) const {
	return Mach1Decode::mPoint(x - vec.x, y - vec.y, z - vec.z);
}

inline float Mach1Decode::mPoint::length() const {
	return (float)sqrt(x*x + y*y + z*z);
}
 
inline float Mach1Decode::lerp(float x1, float x2, float t)
{
	return x1 + (x2 - x1)*t;
}


float Mach1Decode::mPoint::operator[] (int index) {
	float arr[3] = { x, y, z };
	return arr[index];
}

inline Mach1Decode::mPoint& Mach1Decode::mPoint::rotate(float angle, const mPoint& axis) {
	Mach1Decode::mPoint ax = axis.getNormalized();
	float a = (float)(angle*DEG_TO_RAD);
	float sina = sin(a);
	float cosa = cos(a);
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

inline Mach1Decode::mPoint& Mach1Decode::mPoint::normalize() {
	float length = (float)sqrt(x*x + y*y + z*z);
	if (length > 0) {
		x /= length;
		y /= length;
		z /= length;
	}
	return *this;
}


inline Mach1Decode::mPoint Mach1Decode::mPoint::getNormalized() const {
	float length = (float)sqrt(x*x + y*y + z*z);
	if (length > 0) {
		return Mach1Decode::mPoint(x / length, y / length, z / length);
	}
	else {
		return Mach1Decode::mPoint();
	}
}


inline Mach1Decode::mPoint Mach1Decode::mPoint::getRotated(float angle, const mPoint& axis) const {
	Mach1Decode::mPoint ax = axis.getNormalized();
	float a = (float)(angle*DEG_TO_RAD);
	float sina = sin(a);
	float cosa = cos(a);
	float cosb = 1.0f - cosa;

	return Mach1Decode::mPoint(x*(ax.x*ax.x*cosb + cosa)
		+ y*(ax.x*ax.y*cosb - ax.z*sina)
		+ z*(ax.x*ax.z*cosb + ax.y*sina),
		x*(ax.y*ax.x*cosb + ax.z*sina)
		+ y*(ax.y*ax.y*cosb + cosa)
		+ z*(ax.y*ax.z*cosb - ax.x*sina),
		x*(ax.z*ax.x*cosb - ax.y*sina)
		+ y*(ax.z*ax.y*cosb + ax.x*sina)
		+ z*(ax.z*ax.z*cosb + cosa));
}


//

float Mach1Decode::mDegToRad(float degrees) {
	return (float)(degrees * DEG_TO_RAD);
}


//
// Map utility
//

float Mach1Decode::mmap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp) {

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

float Mach1Decode::clamp(float a, float min, float max)
{
	return (a < min) ? min : ((a > max) ? max : a);
}

float Mach1Decode::alignAngle(float a, float min, float max)
{
	if (a > 5000 || a < -5000) return 0;

	while (a < min) a += 360;
	while (a > max) a -= 360;

	return a;
}

float Mach1Decode::radialDistance(float angle1, float angle2) {
	if ((std::abs(angle2 - angle1)) > (std::abs(std::abs(angle2 - angle1) - 360))) {
		return std::abs(std::abs(angle2 - angle1) - 360);
	}
	else return std::abs(angle2 - angle1);
}

float Mach1Decode::targetDirectionMultiplier(float angleCurrent, float angleTarget) {
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

void Mach1Decode::updateAngles() {
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

	float speedAngle = timeLastUpdate ? filterSpeed * 128 * (getCurrentTime() - timeLastUpdate) : 0;

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
};

// Angular settings functions
void Mach1Decode::fillPlatformAngles(AngularSettingsType type, float* Y, float* P, float* R) {
	switch (type) {
	case m1Default:
		*Y = *Y;
		*P = *P;
		*R = *R;
		break;

	case m1Unity:
		*Y = -*Y;                   // Y in Unity
		*P = -*P - 180;             // X in Unity
		*R = *R;                    // Z in Unity
		break;

	case m1UE:
		*Y = *Y;                    // Y in UE
		*P = (*P < 0 ? 360 + *P : *P);   // Z in UE
		*R = *R;                    // X in UE
		break;

	case m1oFEasyCam:
		*Y = *Y;
		*P = *P;
		*R = *R;
		break;

	case m1Android:
		*Y = -*Y - 90;
		*P = -*P;
		*R = *R;
		break;

	case m1iOS:
		*Y = *Y;
		*P = *P;
		*R = *R;
		break;

	default:
		break;
	}
}

void Mach1Decode::addToLog(std::string str, int maxCount)
{
	if (strLog.size() > maxCount) strLog.erase(strLog.begin() + maxCount, strLog.begin() + strLog.size() - 1);

	strLog.push_back(str);
}

char* Mach1Decode::getLog()
{
	std::vector<std::string> _strLog = strLog;
	strLog.clear();

	static char log[100000] = "";

	log[0] = '\0';
	for (int i = 0; i<_strLog.size(); i++) strcat(log, _strLog[i].c_str());
	return log;
}

Mach1Decode::Mach1Decode() {
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

	angularSetting = m1Default;

	ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

	smoothAngles = true;

	strLog.resize(0);
}

long Mach1Decode::getCurrentTime()
{
	return (long)(duration_cast<milliseconds>(system_clock::now().time_since_epoch()) - ms).count();
}

void Mach1Decode::setAngularSettingsType(AngularSettingsType type) {
	angularSetting = type;
}

//--------------------------------------------------

//  Begin function that has to be called
//  before per-sample coefficient calculation.
//

void Mach1Decode::beginBuffer() {
	previousYaw = currentYaw;
	previousPitch = currentPitch;
	previousRoll = currentRoll;
    
    updateAngles();
}


//  End function that has to be called
//  after per-sample coefficient calculation.
//

void Mach1Decode::endBuffer() {

	//  ;)

}


//
//  Four channel audio format
//
//  Order of input angles:
//  Y = Yaw in degrees
//  P = Pitch in degrees
//  R = Roll in degrees
//

std::vector<float> Mach1Decode::horizonAlgo(float Yaw, float Pitch, float Roll,
	int bufferSize, int sampleIndex) {

	fillPlatformAngles(angularSetting, &Yaw, &Pitch, &Roll);


	if (smoothAngles) {

		targetYaw = Yaw;
		targetPitch = Pitch;
		targetRoll = Roll;


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

// ------------------------------------------------------------------

//
//  Four pairs audio format.
//
//  Order of input angles:
//  Y = Yaw in degrees
//  P = Pitch in degrees
//  R = Roll in degrees
//

std::vector<float> Mach1Decode::horizonPairsAlgo(float Yaw, float Pitch, float Roll,
	int bufferSize, int sampleIndex) {

	fillPlatformAngles(angularSetting, &Yaw, &Pitch, &Roll);

	if (smoothAngles) {

		targetYaw = Yaw;
		targetPitch = Pitch;
		targetRoll = Roll;


		if (bufferSize > 0) {

			// we're in per sample mode
            // returning values from right here!
            
            
            auto volumes1 = horizonPairsAlgoSample(previousYaw, previousPitch, previousRoll);
            auto volumes2 = horizonPairsAlgoSample(currentYaw, currentPitch, currentRoll);
            float phase = (float)sampleIndex / (float)bufferSize;
            std::vector<float> volumes_lerp;
            volumes_lerp.resize(volumes1.size());
            for (int i = 0; i < volumes1.size(); i++) {
                volumes_lerp[i] = volumes1[i] * (1 - phase) + volumes2[i] * phase;
            }
            return volumes_lerp;
            
            
            
		}
		else {

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
	}

    return horizonPairsAlgoSample(Yaw, Pitch, Roll);

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

std::vector<float> Mach1Decode::spatialAlgo(float Yaw, float Pitch, float Roll,
	int bufferSize, int sampleIndex) {

	fillPlatformAngles(angularSetting, &Yaw, &Pitch, &Roll);
	
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
            
            
            auto volumes1 = spatialAlgoSample(previousYaw, previousPitch, previousRoll);
            auto volumes2 = spatialAlgoSample(currentYaw, currentPitch, currentRoll);
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
	}

    return spatialAlgoSample(Yaw, Pitch, Roll);

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

std::vector<float> Mach1Decode::spatialAltAlgo(float Yaw, float Pitch, float Roll,
	int bufferSize, int sampleIndex) {

	fillPlatformAngles(angularSetting, &Yaw, &Pitch, &Roll);


	if (smoothAngles) {

		targetYaw = Yaw;
		targetPitch = Pitch;
		targetRoll = Roll;

        
		if (bufferSize > 0) {

            // we're in per sample mode
            // returning values from right here!
            
            
            auto volumes1 = spatialAltAlgoSample(previousYaw, previousPitch, previousRoll);
            auto volumes2 = spatialAltAlgoSample(currentYaw, currentPitch, currentRoll);
            float phase = (float)sampleIndex / (float)bufferSize;
            std::vector<float> volumes_lerp;
            volumes_lerp.resize(volumes1.size());
            for (int i = 0; i < volumes1.size(); i++) {
                volumes_lerp[i] = volumes1[i] * (1 - phase) + volumes2[i] * phase;
            }
            return volumes_lerp;
            

		}
		else {

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
	}

    return spatialAltAlgoSample(Yaw, Pitch, Roll);

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

std::vector<float> Mach1Decode::spatialPairsAlgo(float Yaw, float Pitch, float Roll,
	int bufferSize, int sampleIndex) {

	fillPlatformAngles(angularSetting, &Yaw, &Pitch, &Roll);


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

	result.push_back(1.0); // static stereo L
	result.push_back(1.0); // static stereo R

	return result;
}

// ------------------------------------------------------------------
