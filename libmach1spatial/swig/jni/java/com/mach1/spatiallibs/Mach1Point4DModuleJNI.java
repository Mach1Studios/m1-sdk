/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (https://www.swig.org).
 * Version 4.2.1
 *
 * Do not make changes to this file unless you know what you are doing - modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.mach1.spatiallibs;

class Mach1Point4DModuleJNI {
 
  static {
    System.loadLibrary("Mach1Point4DModule_lib");
  }

  public final static native void Mach1Point4D_x_set(long jarg1, Mach1Point4D jarg1_, float jarg2);
  public final static native float Mach1Point4D_x_get(long jarg1, Mach1Point4D jarg1_);
  public final static native void Mach1Point4D_y_set(long jarg1, Mach1Point4D jarg1_, float jarg2);
  public final static native float Mach1Point4D_y_get(long jarg1, Mach1Point4D jarg1_);
  public final static native void Mach1Point4D_z_set(long jarg1, Mach1Point4D jarg1_, float jarg2);
  public final static native float Mach1Point4D_z_get(long jarg1, Mach1Point4D jarg1_);
  public final static native void Mach1Point4D_w_set(long jarg1, Mach1Point4D jarg1_, float jarg2);
  public final static native float Mach1Point4D_w_get(long jarg1, Mach1Point4D jarg1_);
  public final static native long new_Mach1Point4D();
  public final static native void delete_Mach1Point4D(long jarg1);
  public final static native long new_Mach1Point4DArray(long jarg1);
  public final static native void delete_Mach1Point4DArray(long jarg1);
  public final static native long Mach1Point4DArray_getitem(long jarg1, Mach1Point4DArray jarg1_, long jarg2);
  public final static native void Mach1Point4DArray_setitem(long jarg1, Mach1Point4DArray jarg1_, long jarg2, long jarg3, Mach1Point4D jarg3_);
  public final static native long Mach1Point4DArray_cast(long jarg1, Mach1Point4DArray jarg1_);
  public final static native long Mach1Point4DArray_frompointer(long jarg1, Mach1Point4D jarg1_);
}
