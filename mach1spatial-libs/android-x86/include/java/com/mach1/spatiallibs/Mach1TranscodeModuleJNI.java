/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 4.0.2
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.mach1.spatiallibs;

class Mach1TranscodeModuleJNI {
 
  static {
    System.loadLibrary("Mach1TranscodeModule_lib");
  }

  public final static native int Mach1TranscodeMAXCHANS_get();
  public final static native int Mach1TranscodeFormatEmpty_get();
  public final static native long Mach1TranscodeCAPI_create();
  public final static native void Mach1TranscodeCAPI_delete(long jarg1);
  public final static native int Mach1TranscodeCAPI_getInputNumChannels(long jarg1);
  public final static native int Mach1TranscodeCAPI_getOutputNumChannels(long jarg1);
  public final static native int Mach1TranscodeCAPI_getFormatFromString(long jarg1, String jarg2);
  public final static native String Mach1TranscodeCAPI_getFormatName(long jarg1, int jarg2);
  public final static native float Mach1TranscodeCAPI_processNormalization(long jarg1, long jarg2, int jarg3);
  public final static native void Mach1TranscodeCAPI_processMasterGain(long jarg1, long jarg2, int jarg3, float jarg4);
  public final static native float Mach1TranscodeCAPI_db2level(long jarg1, float jarg2);
  public final static native float Mach1TranscodeCAPI_level2db(long jarg1, float jarg2);
  public final static native void Mach1TranscodeCAPI_setLFESub(long jarg1, long jarg2, int jarg3, int jarg4);
  public final static native void Mach1TranscodeCAPI_setSpatialDownmixer(long jarg1, float jarg2);
  public final static native boolean Mach1TranscodeCAPI_getSpatialDownmixerPossibility(long jarg1);
  public final static native void Mach1TranscodeCAPI_setInputFormat(long jarg1, int jarg2);
  public final static native void Mach1TranscodeCAPI_setInputFormatADM(long jarg1, String jarg2);
  public final static native void Mach1TranscodeCAPI_setInputFormatTTJson(long jarg1, String jarg2);
  public final static native void Mach1TranscodeCAPI_setInputFormatTTPoints(long jarg1, long jarg2, Mach1Point3D jarg2_, int jarg3);
  public final static native void Mach1TranscodeCAPI_setOutputFormat(long jarg1, int jarg2);
  public final static native void Mach1TranscodeCAPI_setOutputFormatTTJson(long jarg1, String jarg2);
  public final static native void Mach1TranscodeCAPI_setOutputFormatTTPoints(long jarg1, long jarg2, Mach1Point3D jarg2_, int jarg3);
  public final static native boolean Mach1TranscodeCAPI_processConversionPath(long jarg1);
  public final static native void Mach1TranscodeCAPI_getMatrixConversion(long jarg1, long jarg2);
  public final static native void Mach1TranscodeCAPI_processConversion(long jarg1, long jarg2, long jarg3, int jarg4);
  public final static native long Mach1TranscodeCAPI_getFormatConversionPath(long jarg1, long jarg2);
}