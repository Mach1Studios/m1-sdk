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
