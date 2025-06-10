%module Mach1Point3DModule

%{
#include "Mach1Point3D.h"
%}

%pragma(java) jniclassclassmodifiers="class"
%pragma(java) jniclasscode=%{ 
  static {
    System.loadLibrary("Mach1Point3DModule_lib");
  }
%} 

/* Let's just grab the original header file here */
%include "Mach1Point3D.h"

%include "typemaps.i"
%include "cpointer.i"
%include "carrays.i"

%typemap(javacode) Mach1Point3DArray %{
  public long asVoidPtr() {
    return getCPtr(this);    
  }
  
  public static Mach1Point3DArray frompointer(long cPtr) {
    return (cPtr == 0) ? null : new Mach1Point3DArray(cPtr, false);
  }
%}

%typemap(javafinalize) Mach1Point3DArray %{
  @SuppressWarnings("deprecation")
  protected void finalize() {
    //delete();
  }
%}

%array_class(Mach1Point3D, Mach1Point3DArray);