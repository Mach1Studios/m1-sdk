//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#include "M1DSPDynamics.h"

using namespace M1DSP::Dynamics;

inline EnvelopeFollower::EnvelopeFollower()
{
	a = 0;
	r = 0;
    envelope=0;
}

inline void EnvelopeFollower::Setup( double attackMs, double releaseMs, int sampleRate )
{
    a = pow( 0.01, 1.0 / ( attackMs * sampleRate * 0.001 ) );
    r = pow( 0.01, 1.0 / ( releaseMs * sampleRate * 0.001 ) );
}

template<class T, int skip>
void EnvelopeFollower::Process( std::size_t count, const T *src )
{
    while( count-- )
    {
        double v=::fabs( *src );
        src+=skip;
        if( v>envelope )
            envelope = a * ( envelope - v ) + v;
        else
            envelope = r * ( envelope - v ) + v;
    }
}

inline void Limiter::Setup( double attackMs, double releaseMs, int sampleRate )
{
    e.Setup( attackMs, releaseMs, sampleRate );
}

template<class T, int skip>
void Limiter::Process( std::size_t count, T *dest )
{
    while( count-- )
    {
        T v=*dest;
        e.Process<T, skip>( 1, &v );
        if( e.envelope>1 )
            *dest=*dest/e.envelope;
        dest+=skip;
    }
}
