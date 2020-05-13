//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#include "M1DSPFilters.h"

using namespace M1DSP::Filters;

#ifndef PI
#define PI       3.14159265358979323846
#endif 

#define BUDDA_Q_SCALE 6.f

/*
 Filters should utilize the following structure for ease of use
 - Setup(cutoff, q)
 - Run(input)
 
 cutoff: 0hz -> nyquist/2 hz
 q: 0.0 (no q) -> 1.0 (full q)
 
 TODO:
 - Make single global SetSampleRate(sr)
 */

void CFilterSimpleLP::Setup(int cutoff_frequency, float resonance)
{
	// https://www.musicdsp.org/en/latest/Filters/38-lp-and-hp-filter.html
	this->cutoff_frequency = cutoff_frequency;
	this->resonance = 1.0f - resonance;
	in1 = 0.0;
	in2 = 0.0;
	out1 = 0.0;
	out2 = 0.0;
}

void CFilterSimpleLP::SetSampleRate(float fs)
{
    this->sample_rate = fs;
}

float CFilterSimpleLP::Run(float in) {

	float c = 1.0 / tan(PI * cutoff_frequency / sample_rate);

	float a1 = 1.0 / (1.0 + resonance * c + c * c);
	float a2 = 2 * a1;
	float a3 = a1;
	float b1 = 2.0 * (1.0 - c * c) * a1;
	float b2 = (1.0 - resonance * c + c * c) * a1;

	float out = a1 * in + a2 * in1 + a3 * in2 - b1 * out1 - b2 * out2;

	out2 = out1;
	out1 = out;

	in2 = in1;
	in1 = in;

	return out;
}

CFilterButterworth24db::CFilterButterworth24db(void)
{
    this->history1 = 0.f;
    this->history2 = 0.f;
    this->history3 = 0.f;
    this->history4 = 0.f;
    
    this->SetSampleRate(44100.f);
    this->Setup(22050.f, 0.0);
}

CFilterButterworth24db::~CFilterButterworth24db(void)
{
}

void CFilterButterworth24db::SetSampleRate(float fs)
{
    float pi = 4.f * atanf(1.f);
    
    this->t0 = 4.f * fs * fs;
    this->t1 = 8.f * fs * fs;
    this->t2 = 2.f * fs;
    this->t3 = pi / fs;
    
    this->min_cutoff = fs * 0.01f;
    this->max_cutoff = fs * 0.45f;
}

void CFilterButterworth24db::Setup(float cutoff, float q)
{
    if (cutoff < this->min_cutoff)
        cutoff = this->min_cutoff;
    else if(cutoff > this->max_cutoff)
        cutoff = this->max_cutoff;
    
    if(q < 0.f)
        q = 0.f;
    else if(q > 1.f)
        q = 1.f;
    
    float wp = this->t2 * tanf(this->t3 * cutoff);
    float bd, bd_tmp, b1, b2;
    
    q *= BUDDA_Q_SCALE;
    q += 1.f;
    
    b1 = (1.847759f / (q * wp));
    b2 = 1.f / (wp * wp);
    
    bd_tmp = this->t0 * b2 + 1.f;
    
    bd = 1.f / (bd_tmp + this->t2 * b1);
    
    this->gain = bd;
    
    this->coef2 = (2.f - this->t1 * b2);
    
    this->coef0 = this->coef2 * bd;
    this->coef1 = (bd_tmp - this->t2 * b1) * bd;
    
    b1 = (1.847759f / q) / wp;
    
    bd = 1.f / (bd_tmp + this->t2 * b1);
    
    this->gain *= bd;
    this->coef2 *= bd;
    this->coef3 = (bd_tmp - this->t2 * b1) * bd;
}

float CFilterButterworth24db::Run(float input)
{
    float output = input * this->gain;
    float new_hist;
    
    output -= this->history1 * this->coef0;
    new_hist = output - this->history2 * this->coef1;
    
    output = new_hist + this->history1 * 2.f;
    output += this->history2;
    
    this->history2 = this->history1;
    this->history1 = new_hist;
    
    output -= this->history3 * this->coef2;
    new_hist = output - this->history4 * this->coef3;
    
    output = new_hist + this->history3 * 2.f;
    output += this->history4;
    
    this->history4 = this->history3;
    this->history3 = new_hist;
    
    return output;
}
