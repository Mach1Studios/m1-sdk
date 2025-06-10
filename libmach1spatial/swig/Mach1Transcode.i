%module Mach1TranscodeModule

%import(module="Mach1Point3DModule") "Mach1Point3D.h"
%import(module="M1DSPFiltersModule") "../deps/M1DSP/M1DSPFilters.h"
%import(module="M1DSPUtilitiesModule") "../deps/M1DSP/M1DSPUtilities.h"
%import(module="jsonModule") "../deps/json/json.h"

%{
%include "Mach1TranscodeCore.h"
%include "Mach1TranscodeCAPI.h"
#include "Mach1EncodeCAPI.h"
%}

/* Parse the header file to generate wrappers */
%include "Mach1TranscodeCore.h"
%include "Mach1TranscodeCAPI.h"