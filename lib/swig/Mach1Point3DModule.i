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
