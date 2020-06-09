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

void CFilterSimpleLP::Setup(int cutoff_frequency, double resonance)
{
	// https://www.musicdsp.org/en/latest/Filters/38-lp-and-hp-filter.html
	this->cutoff_frequency = cutoff_frequency;
	this->resonance = 1.0f - resonance;
	in1 = 0.0;
	in2 = 0.0;
	out1 = 0.0;
	out2 = 0.0;
}

void CFilterSimpleLP::SetSampleRate(double fs)
{
    this->sample_rate = fs;
}

double CFilterSimpleLP::Run(double in)
{
	double c = 1.0 / tan(PI * cutoff_frequency / sample_rate);

	double a1 = 1.0 / (1.0 + resonance * c + c * c);
	double a2 = 2 * a1;
	double a3 = a1;
	double b1 = 2.0 * (1.0 - c * c) * a1;
	double b2 = (1.0 - resonance * c + c * c) * a1;

	double out = a1 * in + a2 * in1 + a3 * in2 - b1 * out1 - b2 * out2;

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

void CFilterButterworth24db::SetSampleRate(double fs)
{
    double pi = 4.f * atanf(1.f);

    this->t0 = 4.f * fs * fs;
    this->t1 = 8.f * fs * fs;
    this->t2 = 2.f * fs;
    this->t3 = pi / fs;

    this->min_cutoff = fs * 0.01f;
    this->max_cutoff = fs * 0.45f;
}

void CFilterButterworth24db::Setup(double cutoff, double q)
{
    if (cutoff < this->min_cutoff)
        cutoff = this->min_cutoff;
    else if(cutoff > this->max_cutoff)
        cutoff = this->max_cutoff;

    if(q < 0.f)
        q = 0.f;
    else if(q > 1.f)
        q = 1.f;

    double wp = this->t2 * tanf(this->t3 * cutoff);
    double bd, bd_tmp, b1, b2;

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

double CFilterButterworth24db::Run(double input)
{
    double output = input * this->gain;
    double new_hist;

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

void CFilterLowPass::setAlpha(double alpha)
{
    if (alpha<=0.0 || alpha>1.0)
        throw std::range_error("alpha should be in (0.0., 1.0]") ;
    a = alpha ;
}

CFilterLowPass::CFilterLowPass(double alpha, double initval) {
    y = s = initval ;
    setAlpha(alpha) ;
    initialized = false ;
}

double CFilterLowPass::Run(double input) {
    double result;
    if (initialized)
        result = a*input + (1.0-a)*s;
    else {
        result = input;
        initialized = true;
    }
    y = input;
    s = result;
    return result;
}

double CFilterLowPass::filterWithAlpha(double value, double alpha) {
    setAlpha(alpha);
    return Run(value);
}

bool CFilterLowPass::hasLastRawValue(void) {
    return initialized;
}

double CFilterLowPass::lastRawValue(void) {
    return y;
}

void CFilterOneEuro::Setup(double mincutoff, double beta_, double dcutoff)
{
    // http://cristal.univ-lille.fr/~casiez/1euro/
    setMinCutoff(mincutoff);
    setBeta(beta_);
    setDerivateCutoff(dcutoff);
    x = new CFilterLowPass(alpha(mincutoff));
    dx = new CFilterLowPass(alpha(dcutoff));
}

double CFilterOneEuro::alpha(double cutoff) {
    double te = 1.0 / freq ;
    double tau = 1.0 / (2*PI*cutoff) ;
    return 1.0 / (1.0 + tau/te) ;
}

void CFilterOneEuro::setMinCutoff(double mc) {
    if (mc<=0) throw std::range_error("mincutoff should be >0") ;
    mincutoff = mc ;
}

void CFilterOneEuro::setBeta(double b) {
    beta_ = b ;
}

void CFilterOneEuro::setDerivateCutoff(double dc) {
    if (dc<=0) throw std::range_error("dcutoff should be >0") ;
    dcutoff = dc ;
}

double CFilterOneEuro::Run(double input)
{
    // estimate the current variation per second
    double dvalue = x->hasLastRawValue() ? (input - x->lastRawValue())*freq : 0.0 ; // FIXME: 0.0 or value?
    double edvalue = dx->filterWithAlpha(dvalue, alpha(dcutoff)) ;
    // use it to update the cutoff frequency
    double cutoff = mincutoff + beta_*fabs(edvalue) ;
    // filter the given value
    return x->filterWithAlpha(input, alpha(cutoff)) ;
}

void CFilterOneEuro::SetSampleRate(double fs)
{
    if (fs<=0) throw std::range_error("freq should be >0");
    freq = fs;
}

CFilterOneEuro::~CFilterOneEuro(void)
{
    delete x;
    delete dx;
}
