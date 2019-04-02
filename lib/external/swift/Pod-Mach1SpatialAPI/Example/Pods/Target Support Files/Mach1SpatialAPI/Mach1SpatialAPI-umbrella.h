#ifdef __OBJC__
#import <UIKit/UIKit.h>
#else
#ifndef FOUNDATION_EXPORT
#if defined(__cplusplus)
#define FOUNDATION_EXPORT extern "C"
#else
#define FOUNDATION_EXPORT extern
#endif
#endif
#endif

#import "Mach1Decode-Bridging-Header.h"
#import "Mach1DecodeCAPI.h"
#import "Mach1DecodePositionalCAPI.h"
#import "Mach1EncodeCAPI.h"
#import "Mach1Point3D.h"
#import "Mach1Point4D.h"

FOUNDATION_EXPORT double Mach1SpatialAPIVersionNumber;
FOUNDATION_EXPORT const unsigned char Mach1SpatialAPIVersionString[];

