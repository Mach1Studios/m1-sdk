%module Mach1Point4D

%{
#include "Mach1Point4D.h"
%}
%include "Mach1Point4D.h"

%include "typemaps.i"
%include "cpointer.i"
%include "carrays.i"

%typemap(in) Mach1Point4DArray %{
  public long asVoidPtr() {
    return getCPtr(this);    
  }
  
  public static Mach1Point4DArray frompointer(long cPtr) {
    return (cPtr == 0) ? null : new Mach1Point4DArray(cPtr, false);
  }
%}

%typemap(out) Mach1Point4DArray %{
  @SuppressWarnings("deprecation")
  protected void finalize() {
    //delete();
  }
%}

%array_class(Mach1Point4D, Mach1Point4DArray);