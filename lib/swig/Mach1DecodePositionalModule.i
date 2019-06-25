%module Mach1DecodePositionalModule

%import(module="Mach1Point3DModule") "Mach1Point3D.h"
%import(module="Mach1Point4DModule") "Mach1Point4D.h"
%import(module="FloatArrayModule") "FloatArray.h"
%import(module="Mach1DecodeModule") "Mach1DecodeCAPI.h"

%{
#include "Mach1DecodePositionalCAPI.h"
%}

%pragma(java) jniclassclassmodifiers="class"
%pragma(java) jniclasscode=%{ 
  static {
    System.loadLibrary("Mach1DecodePositionalModule_lib");
  }
%} 

/* Let's just grab the original header file here */
%include "Mach1DecodePositionalCAPI.h"
