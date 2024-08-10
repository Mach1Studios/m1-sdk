%module Mach1Point3D

%{
#include "Mach1Point3D.h"
%}
%include "Mach1Point3D.h"

%include "typemaps.i"
%include "cpointer.i"
%include "carrays.i"

%typemap(in) Mach1Point3DArray %{
  public long asVoidPtr() {
    return getCPtr(this);    
  }
  
  public static Mach1Point3DArray frompointer(long cPtr) {
    return (cPtr == 0) ? null : new Mach1Point3DArray(cPtr, false);
  }
%}

%typemap(out) Mach1Point3DArray %{
  @SuppressWarnings("deprecation")
  protected void finalize() {
    //delete();
  }
%}

%array_class(Mach1Point3D, Mach1Point3DArray);