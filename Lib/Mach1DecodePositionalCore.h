//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.
//
//  Header file

#pragma once

#include "Mach1DecodeCore.h"

//////////////

class Mach1DecodePositionalCore : public Mach1DecodeCore {

private:
	

public:
 
 
	void setCameraPosition(Mach1Point3DCore* pos) {
	}

	void setCameraRotation(Mach1Point3DCore* euler) {
	}

	void setSpatialAlgoPosition(Mach1Point3DCore* pos) {
	}

	void setSpatialAlgoRotation(Mach1Point3DCore* euler) {
	}

	void setSpatialAlgoScale(Mach1Point3DCore* scale) {
	}


	void setAttenuationCurve(float* curve) {
	}


	std::vector<float> getPostionResults() {
	}

  
     
};
