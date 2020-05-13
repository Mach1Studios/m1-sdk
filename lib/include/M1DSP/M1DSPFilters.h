//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#pragma once

#include <vector>
#include <math.h>

namespace M1DSP
{
	namespace Filters
	{
		class CFilterSimpleLP
        {
        private:
			int sample_rate;
			int cutoff_frequency;
			float resonance;
			float in1;
			float in2;
			float out1;
			float out2;

		public:
			void Setup(int cutoff_frequency, float resonance);
            void SetSampleRate(float fs); //TODO: move to global SetSampleRate(float) function
			float Run(float in);
		};
        
        class CFilterButterworth24db
        {
        public:
            CFilterButterworth24db(void);
            ~CFilterButterworth24db(void);
            void SetSampleRate(float fs); //TODO: move to global SetSampleRate(float) function
            void Setup(float cutoff, float q);
            float Run(float input);
            
        private:
            float t0, t1, t2, t3;
            float coef0, coef1, coef2, coef3;
            float history1, history2, history3, history4;
            float gain;
            float min_cutoff, max_cutoff;
        };
	}
}
