/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (https://www.swig.org).
 * Version 4.2.1
 *
 * Do not make changes to this file unless you know what you are doing - modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.mach1.spatiallibs;

public class Mach1EncodeModule {
  public static SWIGTYPE_p_void Mach1EncodeCAPI_create() {
    long cPtr = Mach1EncodeModuleJNI.Mach1EncodeCAPI_create();
    return (cPtr == 0) ? null : new SWIGTYPE_p_void(cPtr, false);
  }

  public static void Mach1EncodeCAPI_copy(SWIGTYPE_p_void M1str, SWIGTYPE_p_void M1dst) {
    Mach1EncodeModuleJNI.Mach1EncodeCAPI_copy(SWIGTYPE_p_void.getCPtr(M1str), SWIGTYPE_p_void.getCPtr(M1dst));
  }

  public static void Mach1EncodeCAPI_delete(SWIGTYPE_p_void M1obj) {
    Mach1EncodeModuleJNI.Mach1EncodeCAPI_delete(SWIGTYPE_p_void.getCPtr(M1obj));
  }

  public static SWIGTYPE_p_void Mach1EncodeCAPI_getPoints(SWIGTYPE_p_void M1obj) {
    long cPtr = Mach1EncodeModuleJNI.Mach1EncodeCAPI_getPoints(SWIGTYPE_p_void.getCPtr(M1obj));
    return (cPtr == 0) ? null : new SWIGTYPE_p_void(cPtr, false);
  }

  public static SWIGTYPE_p_void Mach1EncodeCAPI_getGains(SWIGTYPE_p_void M1obj) {
    long cPtr = Mach1EncodeModuleJNI.Mach1EncodeCAPI_getGains(SWIGTYPE_p_void.getCPtr(M1obj));
    return (cPtr == 0) ? null : new SWIGTYPE_p_void(cPtr, false);
  }

  public static SWIGTYPE_p_void Mach1EncodeCAPI_getPointsNames(SWIGTYPE_p_void M1obj) {
    long cPtr = Mach1EncodeModuleJNI.Mach1EncodeCAPI_getPointsNames(SWIGTYPE_p_void.getCPtr(M1obj));
    return (cPtr == 0) ? null : new SWIGTYPE_p_void(cPtr, false);
  }

  public static SWIGTYPE_p_void Mach1EncodeCAPI_getGainsForInputChannelNamed(SWIGTYPE_p_void M1obj, String pointName) {
    long cPtr = Mach1EncodeModuleJNI.Mach1EncodeCAPI_getGainsForInputChannelNamed(SWIGTYPE_p_void.getCPtr(M1obj), pointName);
    return (cPtr == 0) ? null : new SWIGTYPE_p_void(cPtr, false);
  }

  public static void Mach1EncodeCAPI_generatePointResults(SWIGTYPE_p_void M1obj) {
    Mach1EncodeModuleJNI.Mach1EncodeCAPI_generatePointResults(SWIGTYPE_p_void.getCPtr(M1obj));
  }

  public static int Mach1EncodeCAPI_getPointsCount(SWIGTYPE_p_void M1obj) {
    return Mach1EncodeModuleJNI.Mach1EncodeCAPI_getPointsCount(SWIGTYPE_p_void.getCPtr(M1obj));
  }

  public static Mach1EncodeInputMode Mach1EncodeCAPI_getInputMode(SWIGTYPE_p_void M1obj) {
    return Mach1EncodeInputMode.swigToEnum(Mach1EncodeModuleJNI.Mach1EncodeCAPI_getInputMode(SWIGTYPE_p_void.getCPtr(M1obj)));
  }

  public static Mach1EncodeOutputMode Mach1EncodeCAPI_getOutputMode(SWIGTYPE_p_void M1obj) {
    return Mach1EncodeOutputMode.swigToEnum(Mach1EncodeModuleJNI.Mach1EncodeCAPI_getOutputMode(SWIGTYPE_p_void.getCPtr(M1obj)));
  }

  public static Mach1EncodePannerMode Mach1EncodeCAPI_getPannerMode(SWIGTYPE_p_void M1obj) {
    return Mach1EncodePannerMode.swigToEnum(Mach1EncodeModuleJNI.Mach1EncodeCAPI_getPannerMode(SWIGTYPE_p_void.getCPtr(M1obj)));
  }

  public static boolean Mach1EncodeCAPI_getAutoOrbit(SWIGTYPE_p_void M1obj) {
    return Mach1EncodeModuleJNI.Mach1EncodeCAPI_getAutoOrbit(SWIGTYPE_p_void.getCPtr(M1obj));
  }

  public static int Mach1EncodeCAPI_getInputChannelsCount(SWIGTYPE_p_void M1obj) {
    return Mach1EncodeModuleJNI.Mach1EncodeCAPI_getInputChannelsCount(SWIGTYPE_p_void.getCPtr(M1obj));
  }

  public static int Mach1EncodeCAPI_getOutputChannelsCount(SWIGTYPE_p_void M1obj) {
    return Mach1EncodeModuleJNI.Mach1EncodeCAPI_getOutputChannelsCount(SWIGTYPE_p_void.getCPtr(M1obj));
  }

  public static void Mach1EncodeCAPI_setInputMode(SWIGTYPE_p_void M1obj, Mach1EncodeInputMode inputMode) {
    Mach1EncodeModuleJNI.Mach1EncodeCAPI_setInputMode(SWIGTYPE_p_void.getCPtr(M1obj), inputMode.swigValue());
  }

  public static void Mach1EncodeCAPI_setOutputMode(SWIGTYPE_p_void M1obj, Mach1EncodeOutputMode outputMode) {
    Mach1EncodeModuleJNI.Mach1EncodeCAPI_setOutputMode(SWIGTYPE_p_void.getCPtr(M1obj), outputMode.swigValue());
  }

  public static void Mach1EncodeCAPI_setAzimuth(SWIGTYPE_p_void M1obj, float azimuthFromMinus1To1) {
    Mach1EncodeModuleJNI.Mach1EncodeCAPI_setAzimuth(SWIGTYPE_p_void.getCPtr(M1obj), azimuthFromMinus1To1);
  }

  public static void Mach1EncodeCAPI_setAzimuthDegrees(SWIGTYPE_p_void M1obj, float azimuthDegrees) {
    Mach1EncodeModuleJNI.Mach1EncodeCAPI_setAzimuthDegrees(SWIGTYPE_p_void.getCPtr(M1obj), azimuthDegrees);
  }

  public static void Mach1EncodeCAPI_setAzimuthRadians(SWIGTYPE_p_void M1obj, float azimuthRadians) {
    Mach1EncodeModuleJNI.Mach1EncodeCAPI_setAzimuthRadians(SWIGTYPE_p_void.getCPtr(M1obj), azimuthRadians);
  }

  public static void Mach1EncodeCAPI_setDiverge(SWIGTYPE_p_void M1obj, float divergeFromMinus1To1) {
    Mach1EncodeModuleJNI.Mach1EncodeCAPI_setDiverge(SWIGTYPE_p_void.getCPtr(M1obj), divergeFromMinus1To1);
  }

  public static void Mach1EncodeCAPI_setElevation(SWIGTYPE_p_void M1obj, float elevationFromMinus1to1) {
    Mach1EncodeModuleJNI.Mach1EncodeCAPI_setElevation(SWIGTYPE_p_void.getCPtr(M1obj), elevationFromMinus1to1);
  }

  public static void Mach1EncodeCAPI_setElevationDegrees(SWIGTYPE_p_void M1obj, float elevationFromMinus90to90) {
    Mach1EncodeModuleJNI.Mach1EncodeCAPI_setElevationDegrees(SWIGTYPE_p_void.getCPtr(M1obj), elevationFromMinus90to90);
  }

  public static void Mach1EncodeCAPI_setElevationRadians(SWIGTYPE_p_void M1obj, float elevationFromMinusHalfPItoHalfPI) {
    Mach1EncodeModuleJNI.Mach1EncodeCAPI_setElevationRadians(SWIGTYPE_p_void.getCPtr(M1obj), elevationFromMinusHalfPItoHalfPI);
  }

  public static void Mach1EncodeCAPI_setPannerMode(SWIGTYPE_p_void M1obj, Mach1EncodePannerMode pannerMode) {
    Mach1EncodeModuleJNI.Mach1EncodeCAPI_setPannerMode(SWIGTYPE_p_void.getCPtr(M1obj), pannerMode.swigValue());
  }

  public static void Mach1EncodeCAPI_setFrontSurroundPerspective(SWIGTYPE_p_void M1obj, boolean frontSurroundPerspective) {
    Mach1EncodeModuleJNI.Mach1EncodeCAPI_setFrontSurroundPerspective(SWIGTYPE_p_void.getCPtr(M1obj), frontSurroundPerspective);
  }

  public static float Mach1EncodeCAPI_getOutputGain(SWIGTYPE_p_void M1obj, boolean isDecibel) {
    return Mach1EncodeModuleJNI.Mach1EncodeCAPI_getOutputGain(SWIGTYPE_p_void.getCPtr(M1obj), isDecibel);
  }

  public static void Mach1EncodeCAPI_setOutputGain(SWIGTYPE_p_void M1obj, float outputGainMultipler, boolean isDecibel) {
    Mach1EncodeModuleJNI.Mach1EncodeCAPI_setOutputGain(SWIGTYPE_p_void.getCPtr(M1obj), outputGainMultipler, isDecibel);
  }

  public static float Mach1EncodeCAPI_getGainCompensation(SWIGTYPE_p_void M1obj, boolean isDecibel) {
    return Mach1EncodeModuleJNI.Mach1EncodeCAPI_getGainCompensation(SWIGTYPE_p_void.getCPtr(M1obj), isDecibel);
  }

  public static boolean Mach1EncodeCAPI_getGainCompensationActive(SWIGTYPE_p_void M1obj) {
    return Mach1EncodeModuleJNI.Mach1EncodeCAPI_getGainCompensationActive(SWIGTYPE_p_void.getCPtr(M1obj));
  }

  public static void Mach1EncodeCAPI_setGainCompensationActive(SWIGTYPE_p_void M1obj, boolean active) {
    Mach1EncodeModuleJNI.Mach1EncodeCAPI_setGainCompensationActive(SWIGTYPE_p_void.getCPtr(M1obj), active);
  }

  public static void Mach1EncodeCAPI_setAutoOrbit(SWIGTYPE_p_void M1obj, boolean autoOrbit) {
    Mach1EncodeModuleJNI.Mach1EncodeCAPI_setAutoOrbit(SWIGTYPE_p_void.getCPtr(M1obj), autoOrbit);
  }

  public static void Mach1EncodeCAPI_setOrbitRotation(SWIGTYPE_p_void M1obj, float orbitRotationFromMinusOnetoOne) {
    Mach1EncodeModuleJNI.Mach1EncodeCAPI_setOrbitRotation(SWIGTYPE_p_void.getCPtr(M1obj), orbitRotationFromMinusOnetoOne);
  }

  public static void Mach1EncodeCAPI_setOrbitRotationDegrees(SWIGTYPE_p_void M1obj, float orbitRotationDegrees) {
    Mach1EncodeModuleJNI.Mach1EncodeCAPI_setOrbitRotationDegrees(SWIGTYPE_p_void.getCPtr(M1obj), orbitRotationDegrees);
  }

  public static void Mach1EncodeCAPI_setOrbitRotationRadians(SWIGTYPE_p_void M1obj, float orbitRotationRadians) {
    Mach1EncodeModuleJNI.Mach1EncodeCAPI_setOrbitRotationRadians(SWIGTYPE_p_void.getCPtr(M1obj), orbitRotationRadians);
  }

  public static void Mach1EncodeCAPI_setStereoSpread(SWIGTYPE_p_void M1obj, float sSpreadFrom0to1) {
    Mach1EncodeModuleJNI.Mach1EncodeCAPI_setStereoSpread(SWIGTYPE_p_void.getCPtr(M1obj), sSpreadFrom0to1);
  }

  public static int Mach1EncodeCAPI_getLastCalculationTime(SWIGTYPE_p_void M1obj) {
    return Mach1EncodeModuleJNI.Mach1EncodeCAPI_getLastCalculationTime(SWIGTYPE_p_void.getCPtr(M1obj));
  }

}
