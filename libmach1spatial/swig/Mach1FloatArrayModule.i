%module Mach1FloatArrayModule

%include "typemaps.i"
%include "cpointer.i"
%include "carrays.i"

%typemap(javacode) Mach1FloatArray %{
  public long asVoidPtr() {
    return getCPtr(this);    
  }

  public static Mach1FloatArray frompointer(long cPtr) {
    return (cPtr == 0) ? null : new Mach1FloatArray(cPtr, false);
  }
%}

%typemap(javafinalize) Mach1FloatArray %{
  @SuppressWarnings("deprecation")
  protected void finalize() {
    //delete();
  }
%}

%pragma(java) jniclassclassmodifiers="class"
%pragma(java) jniclasscode=%{ 
  static {
    System.loadLibrary("Mach1FloatArrayModule_lib");
  }
%}

%array_class(float, Mach1FloatArray);
