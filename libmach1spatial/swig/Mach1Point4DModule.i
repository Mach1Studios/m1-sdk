%module Mach1Point4DModule

%{
#include "Mach1Point4D.h"
%}

%pragma(java) jniclassclassmodifiers="class"
%pragma(java) jniclasscode=%{ 
  static {
    System.loadLibrary("Mach1Point4DModule_lib");
  }
%} 

/* Let's just grab the original header file here */
%include "Mach1Point4D.h"

%include "typemaps.i"
%include "cpointer.i"
%include "carrays.i"

%typemap(javacode) Mach1Point4DArray %{
  public long asVoidPtr() {
    return getCPtr(this);    
  }
  
  public static Mach1Point4DArray frompointer(long cPtr) {
    return (cPtr == 0) ? null : new Mach1Point4DArray(cPtr, false);
  }
%}

%typemap(javafinalize) Mach1Point4DArray %{
  @SuppressWarnings("deprecation")
  protected void finalize() {
    //delete();
  }
%}

%array_class(Mach1Point4D, Mach1Point4DArray);