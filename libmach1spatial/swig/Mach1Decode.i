%module Mach1Decode

%import(module="Mach1Point3DModule") "Mach1Point3D.h"
%import(module="Mach1Point4DModule") "Mach1Point4D.h"

%{
#include "Mach1DecodeCore.h"
#include "Mach1DecodeCAPI.h"
%}

/* Parse the header file to generate wrappers */
%include "Mach1DecodeCore.h"
#include "Mach1DecodeCAPI.h"