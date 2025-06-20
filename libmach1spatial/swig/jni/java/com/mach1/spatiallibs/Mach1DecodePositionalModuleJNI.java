/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (https://www.swig.org).
 * Version 4.2.1
 *
 * Do not make changes to this file unless you know what you are doing - modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.mach1.spatiallibs;

class Mach1DecodePositionalModuleJNI {
 
  static {
    System.loadLibrary("Mach1DecodePositionalModule_lib");
  }

  public final static native long Mach1DecodePositionalCAPI_create();
  public final static native void Mach1DecodePositionalCAPI_delete(long jarg1);
  public final static native void Mach1DecodePositionalCAPI_setPlatformType(long jarg1, int jarg2);
  public final static native void Mach1DecodePositionalCAPI_setDecodeMode(long jarg1, int jarg2);
  public final static native void Mach1DecodePositionalCAPI_setMuteWhenOutsideObject(long jarg1, boolean jarg2);
  public final static native void Mach1DecodePositionalCAPI_setMuteWhenInsideObject(long jarg1, boolean jarg2);
  public final static native void Mach1DecodePositionalCAPI_setUseAttenuation(long jarg1, boolean jarg2);
  public final static native void Mach1DecodePositionalCAPI_setAttenuationCurve(long jarg1, float jarg2);
  public final static native void Mach1DecodePositionalCAPI_setUsePlaneCalculation(long jarg1, boolean jarg2);
  public final static native void Mach1DecodePositionalCAPI_setUseYawForRotation(long jarg1, boolean jarg2);
  public final static native void Mach1DecodePositionalCAPI_setUsePitchForRotation(long jarg1, boolean jarg2);
  public final static native void Mach1DecodePositionalCAPI_setUseRollForRotation(long jarg1, boolean jarg2);
  public final static native void Mach1DecodePositionalCAPI_setListenerPosition(long jarg1, long jarg2, Mach1Point3D jarg2_);
  public final static native void Mach1DecodePositionalCAPI_setListenerRotation(long jarg1, long jarg2, Mach1Point3D jarg2_);
  public final static native void Mach1DecodePositionalCAPI_setListenerRotationQuat(long jarg1, long jarg2, Mach1Point4D jarg2_);
  public final static native void Mach1DecodePositionalCAPI_setDecoderAlgoPosition(long jarg1, long jarg2, Mach1Point3D jarg2_);
  public final static native void Mach1DecodePositionalCAPI_setDecoderAlgoRotation(long jarg1, long jarg2, Mach1Point3D jarg2_);
  public final static native void Mach1DecodePositionalCAPI_setDecoderAlgoRotationQuat(long jarg1, long jarg2, Mach1Point4D jarg2_);
  public final static native void Mach1DecodePositionalCAPI_setDecoderAlgoScale(long jarg1, long jarg2, Mach1Point3D jarg2_);
  public final static native void Mach1DecodePositionalCAPI_evaluatePositionResults(long jarg1);
  public final static native void Mach1DecodePositionalCAPI_getCoefficients(long jarg1, long jarg2);
  public final static native float Mach1DecodePositionalCAPI_getDist(long jarg1);
  public final static native int Mach1DecodePositionalCAPI_getFormatChannelCount(long jarg1);
  public final static native int Mach1DecodePositionalCAPI_getFormatCoeffCount(long jarg1);
  public final static native long Mach1DecodePositionalCAPI_getCurrentAngle(long jarg1);
  public final static native long Mach1DecodePositionalCAPI_getCurrentAngleInternal(long jarg1);
  public final static native long Mach1DecodePositionalCAPI_getPositionalRotation(long jarg1);
  public final static native void Mach1DecodePositionalCAPI_setFilterSpeed(long jarg1, float jarg2);
  public final static native long Mach1DecodePositionalCAPI_getClosestPointOnPlane(long jarg1);
  public final static native int Mach1DecodePositionalCAPI_getLastCalculationTime(long jarg1);
}
