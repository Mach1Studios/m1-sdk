//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#pragma once

#include <math.h>
#include <vector>

namespace M1DSP {
namespace Filters {
class CFilterSimpleLP {
  public:
    void Setup(int cutoff_frequency, double resonance);
    void SetSampleRate(double fs); // TODO: move to global SetSampleRate(double) function
    double Run(double in);

  private:
    int sample_rate;
    int cutoff_frequency;
    double resonance;
    double in1;
    double in2;
    double out1;
    double out2;
};

class CFilterButterworth24db {
  public:
    CFilterButterworth24db(void);
    ~CFilterButterworth24db(void);
    void SetSampleRate(double fs); // TODO: move to global SetSampleRate(double) function
    void Setup(double cutoff, double q);
    double Run(double input);

  private:
    double t0, t1, t2, t3;
    double coef0, coef1, coef2, coef3;
    double history1, history2, history3, history4;
    double gain;
    double min_cutoff, max_cutoff;
};

class CFilterLowPass {
  private:
    double y, a, s;
    bool initialized;

    void setAlpha(double alpha);

  public:
    CFilterLowPass(double alpha, double initval = 0.0);
    double Run(double input);
    double filterWithAlpha(double value, double alpha);
    bool hasLastRawValue(void);
    double lastRawValue(void);
};

class CFilterOneEuro {
  public:
    CFilterOneEuro(void);
    ~CFilterOneEuro(void);
    void Setup(double mincutoff = 1.0, double beta_ = 0.0, double dcutoff = 1.0);
    void SetSampleRate(double fs); // TODO: move to global SetSampleRate(double) function
    double Run(double input);

  private:
    double freq;
    double mincutoff;
    double beta_;
    double dcutoff;
    CFilterLowPass *x;
    CFilterLowPass *dx;

    double alpha(double cutoff);
    void setMinCutoff(double mc);
    void setBeta(double b);
    void setDerivateCutoff(double dc);
};
} // namespace Filters
} // namespace M1DSP
