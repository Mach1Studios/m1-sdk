%module Mach1DecodePositional

%import(module="Mach1Point3DModule") "Mach1Point3D.h"
%import(module="Mach1Point4DModule") "Mach1Point4D.h"
%import(module="Mach1Decode") "Mach1DecodeCore.h"

%{
#include "Mach1DecodePositionalCore.h"
#include "Mach1DecodePositionalCAPI.h"
%}

/* Parse the header file to generate wrappers */
#include "Mach1DecodePositionalCore.h"
%include "Mach1DecodePositionalCAPI.h"