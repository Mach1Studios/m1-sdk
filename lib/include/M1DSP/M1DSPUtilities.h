//  Mach1 Spatial SDK
//  Copyright © 2017-2021 Mach1. All rights reserved.

#pragma once

#include <vector>
#include <math.h>

namespace M1DSP
{
	namespace Utilities
	{
        /*
         Mach1 Spatial Downmix Checker
         
         Description:
         This class contains tools to compare difference of signal between top and bottom of soundfield.
         If soundfield top/bottom difference is less than the input correlation threshold the soundfield
         will be safely downmixed to the lowest Mach1 format.
         
         `threshold` value range = 0.0f [0% signal difference] -> 1.0f [100% signal difference]
         */
        
		class CSpatialDownmixChecker
		{
			std::vector<float> sumSample;
			float threshold;
			int totalSamples;
		public: 
			void Setup(float threshold = 0.1);
			void ProcessBuffer(float** buffer, int bufferSize);
			std::vector<float> getAvgSamplesDiff();
			bool bShouldDownmix();
		};
	}
}
