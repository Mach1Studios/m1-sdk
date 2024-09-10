//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#pragma once

#include <math.h>
#include <vector>

namespace M1DSP {
namespace Dynamics {
class EnvelopeFollower {
  public:
    EnvelopeFollower();

    void Setup(double attackMs, double releaseMs, int sampleRate);

    template <class T, int skip>
    void Process(std::size_t count, const T *src);

    double envelope;

  protected:
    double a;
    double r;
};

//----------

struct Limiter {
    void Setup(double attackMs, double releaseMs, int sampleRate);

    template <class T, int skip>
    void Process(std::size_t nSamples, T *dest);

  private:
    EnvelopeFollower e;
};
} // namespace Dynamics
} // namespace M1DSP
