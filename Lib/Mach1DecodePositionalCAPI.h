//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.
//
//  Header file

#pragma once

#ifdef Mach1DecodeCore_h

#if defined(_WINDOWS) || defined(WIN32)
#define M1_API __declspec(dllexport)
#else
#define M1_API
#endif 

#else

#if defined(_WINDOWS) || defined(WIN32)
#define M1_API __declspec(dllimport)
#else
#define M1_API
#endif 

#endif 
 

extern "C" { 
}
