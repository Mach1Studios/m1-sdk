/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (https://www.swig.org).
 * Version 4.2.1
 *
 * Do not make changes to this file unless you know what you are doing - modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.mach1.spatiallibs;

public class Mach1Point4DArray {
  private transient long swigCPtr;
  protected transient boolean swigCMemOwn;

  protected Mach1Point4DArray(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(Mach1Point4DArray obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected static long swigRelease(Mach1Point4DArray obj) {
    long ptr = 0;
    if (obj != null) {
      if (!obj.swigCMemOwn)
        throw new RuntimeException("Cannot release ownership as memory is not owned");
      ptr = obj.swigCPtr;
      obj.swigCMemOwn = false;
      obj.delete();
    }
    return ptr;
  }

  @SuppressWarnings("deprecation")
  protected void finalize() {
    //delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        Mach1Point4DModuleJNI.delete_Mach1Point4DArray(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public long asVoidPtr() {
    return getCPtr(this);    
  }
  
  public static Mach1Point4DArray frompointer(long cPtr) {
    return (cPtr == 0) ? null : new Mach1Point4DArray(cPtr, false);
  }

  public Mach1Point4DArray(long nelements) {
    this(Mach1Point4DModuleJNI.new_Mach1Point4DArray(nelements), true);
  }

  public Mach1Point4D getitem(long index) {
    return new Mach1Point4D(Mach1Point4DModuleJNI.Mach1Point4DArray_getitem(swigCPtr, this, index), true);
  }

  public void setitem(long index, Mach1Point4D value) {
    Mach1Point4DModuleJNI.Mach1Point4DArray_setitem(swigCPtr, this, index, Mach1Point4D.getCPtr(value), value);
  }

  public Mach1Point4D cast() {
    long cPtr = Mach1Point4DModuleJNI.Mach1Point4DArray_cast(swigCPtr, this);
    return (cPtr == 0) ? null : new Mach1Point4D(cPtr, false);
  }

  public static Mach1Point4DArray frompointer(Mach1Point4D t) {
    long cPtr = Mach1Point4DModuleJNI.Mach1Point4DArray_frompointer(Mach1Point4D.getCPtr(t), t);
    return (cPtr == 0) ? null : new Mach1Point4DArray(cPtr, false);
  }

}
