%module Mach1TranscodeModule

%import(module="Mach1Point3DModule") "Mach1Point3D.h"
%import(module="M1DSPFiltersModule") "include/M1DSP/M1DSPFilters.h"
%import(module="M1DSPUtilitiesModule") "include/M1DSP/M1DSPUtilities.h"
%import(module="jsonModule") "include/json/json.h"

%{
#include "Mach1EncodeCAPI.h"
%}

%pragma(java) jniclassclassmodifiers="class"
%pragma(java) jniclasscode=%{ 
  static {
    System.loadLibrary("Mach1TranscodeModule_lib");
  }
%} 

/* Let's just grab the original header file here */
%include "Mach1TranscodeCAPI.h"
