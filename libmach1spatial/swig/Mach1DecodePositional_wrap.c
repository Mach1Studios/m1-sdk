/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (https://www.swig.org).
 * Version 4.2.1
 *
 * Do not make changes to this file unless you know what you are doing - modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */


#define SWIG_VERSION 0x040201
#define SWIGCSHARP

/* -----------------------------------------------------------------------------
 *  This section contains generic SWIG labels for method/variable
 *  declarations/attributes, and other compiler dependent labels.
 * ----------------------------------------------------------------------------- */

/* template workaround for compilers that cannot correctly implement the C++ standard */
#ifndef SWIGTEMPLATEDISAMBIGUATOR
# if defined(__SUNPRO_CC) && (__SUNPRO_CC <= 0x560)
#  define SWIGTEMPLATEDISAMBIGUATOR template
# elif defined(__HP_aCC)
/* Needed even with `aCC -AA' when `aCC -V' reports HP ANSI C++ B3910B A.03.55 */
/* If we find a maximum version that requires this, the test would be __HP_aCC <= 35500 for A.03.55 */
#  define SWIGTEMPLATEDISAMBIGUATOR template
# else
#  define SWIGTEMPLATEDISAMBIGUATOR
# endif
#endif

/* inline attribute */
#ifndef SWIGINLINE
# if defined(__cplusplus) || (defined(__GNUC__) && !defined(__STRICT_ANSI__))
#   define SWIGINLINE inline
# else
#   define SWIGINLINE
# endif
#endif

/* attribute recognised by some compilers to avoid 'unused' warnings */
#ifndef SWIGUNUSED
# if defined(__GNUC__)
#   if !(defined(__cplusplus)) || (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4))
#     define SWIGUNUSED __attribute__ ((__unused__))
#   else
#     define SWIGUNUSED
#   endif
# elif defined(__ICC)
#   define SWIGUNUSED __attribute__ ((__unused__))
# else
#   define SWIGUNUSED
# endif
#endif

#ifndef SWIG_MSC_UNSUPPRESS_4505
# if defined(_MSC_VER)
#   pragma warning(disable : 4505) /* unreferenced local function has been removed */
# endif
#endif

#ifndef SWIGUNUSEDPARM
# ifdef __cplusplus
#   define SWIGUNUSEDPARM(p)
# else
#   define SWIGUNUSEDPARM(p) p SWIGUNUSED
# endif
#endif

/* internal SWIG method */
#ifndef SWIGINTERN
# define SWIGINTERN static SWIGUNUSED
#endif

/* internal inline SWIG method */
#ifndef SWIGINTERNINLINE
# define SWIGINTERNINLINE SWIGINTERN SWIGINLINE
#endif

/* exporting methods */
#if defined(__GNUC__)
#  if (__GNUC__ >= 4) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4)
#    ifndef GCC_HASCLASSVISIBILITY
#      define GCC_HASCLASSVISIBILITY
#    endif
#  endif
#endif

#ifndef SWIGEXPORT
# if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#   if defined(STATIC_LINKED)
#     define SWIGEXPORT
#   else
#     define SWIGEXPORT __declspec(dllexport)
#   endif
# else
#   if defined(__GNUC__) && defined(GCC_HASCLASSVISIBILITY)
#     define SWIGEXPORT __attribute__ ((visibility("default")))
#   else
#     define SWIGEXPORT
#   endif
# endif
#endif

/* calling conventions for Windows */
#ifndef SWIGSTDCALL
# if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#   define SWIGSTDCALL __stdcall
# else
#   define SWIGSTDCALL
# endif
#endif

/* Deal with Microsoft's attempt at deprecating C standard runtime functions */
#if !defined(SWIG_NO_CRT_SECURE_NO_DEPRECATE) && defined(_MSC_VER) && !defined(_CRT_SECURE_NO_DEPRECATE)
# define _CRT_SECURE_NO_DEPRECATE
#endif

/* Deal with Microsoft's attempt at deprecating methods in the standard C++ library */
#if !defined(SWIG_NO_SCL_SECURE_NO_DEPRECATE) && defined(_MSC_VER) && !defined(_SCL_SECURE_NO_DEPRECATE)
# define _SCL_SECURE_NO_DEPRECATE
#endif

/* Deal with Apple's deprecated 'AssertMacros.h' from Carbon-framework */
#if defined(__APPLE__) && !defined(__ASSERT_MACROS_DEFINE_VERSIONS_WITHOUT_UNDERSCORES)
# define __ASSERT_MACROS_DEFINE_VERSIONS_WITHOUT_UNDERSCORES 0
#endif

/* Intel's compiler complains if a variable which was never initialised is
 * cast to void, which is a common idiom which we use to indicate that we
 * are aware a variable isn't used.  So we just silence that warning.
 * See: https://github.com/swig/swig/issues/192 for more discussion.
 */
#ifdef __INTEL_COMPILER
# pragma warning disable 592
#endif

#if defined(__cplusplus) && __cplusplus >=201103L
# define SWIG_NULLPTR nullptr
#else
# define SWIG_NULLPTR NULL
#endif 

/* -----------------------------------------------------------------------------
 * swigcompat.swg
 *
 * Macros to provide support compatibility with older C and C++ standards.
 * ----------------------------------------------------------------------------- */

/* C99 and C++11 should provide snprintf, but define SWIG_NO_SNPRINTF
 * if you're missing it.
 */
#if ((defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L) || \
     (defined __cplusplus && __cplusplus >= 201103L) || \
     defined SWIG_HAVE_SNPRINTF) && \
    !defined SWIG_NO_SNPRINTF
# define SWIG_snprintf(O,S,F,A) snprintf(O,S,F,A)
# define SWIG_snprintf2(O,S,F,A,B) snprintf(O,S,F,A,B)
#else
/* Fallback versions ignore the buffer size, but most of our uses either have a
 * fixed maximum possible size or dynamically allocate a buffer that's large
 * enough.
 */
# define SWIG_snprintf(O,S,F,A) sprintf(O,F,A)
# define SWIG_snprintf2(O,S,F,A,B) sprintf(O,F,A,B)
#endif


#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/* Support for throwing C# exceptions from C/C++. There are two types: 
 * Exceptions that take a message and ArgumentExceptions that take a message and a parameter name. */
typedef enum {
  SWIG_CSharpApplicationException,
  SWIG_CSharpArithmeticException,
  SWIG_CSharpDivideByZeroException,
  SWIG_CSharpIndexOutOfRangeException,
  SWIG_CSharpInvalidCastException,
  SWIG_CSharpInvalidOperationException,
  SWIG_CSharpIOException,
  SWIG_CSharpNullReferenceException,
  SWIG_CSharpOutOfMemoryException,
  SWIG_CSharpOverflowException,
  SWIG_CSharpSystemException
} SWIG_CSharpExceptionCodes;

typedef enum {
  SWIG_CSharpArgumentException,
  SWIG_CSharpArgumentNullException,
  SWIG_CSharpArgumentOutOfRangeException
} SWIG_CSharpExceptionArgumentCodes;

typedef void (SWIGSTDCALL* SWIG_CSharpExceptionCallback_t)(const char *);
typedef void (SWIGSTDCALL* SWIG_CSharpExceptionArgumentCallback_t)(const char *, const char *);

typedef struct {
  SWIG_CSharpExceptionCodes code;
  SWIG_CSharpExceptionCallback_t callback;
} SWIG_CSharpException_t;

typedef struct {
  SWIG_CSharpExceptionArgumentCodes code;
  SWIG_CSharpExceptionArgumentCallback_t callback;
} SWIG_CSharpExceptionArgument_t;

static SWIG_CSharpException_t SWIG_csharp_exceptions[] = {
  { SWIG_CSharpApplicationException, NULL },
  { SWIG_CSharpArithmeticException, NULL },
  { SWIG_CSharpDivideByZeroException, NULL },
  { SWIG_CSharpIndexOutOfRangeException, NULL },
  { SWIG_CSharpInvalidCastException, NULL },
  { SWIG_CSharpInvalidOperationException, NULL },
  { SWIG_CSharpIOException, NULL },
  { SWIG_CSharpNullReferenceException, NULL },
  { SWIG_CSharpOutOfMemoryException, NULL },
  { SWIG_CSharpOverflowException, NULL },
  { SWIG_CSharpSystemException, NULL }
};

static SWIG_CSharpExceptionArgument_t SWIG_csharp_exceptions_argument[] = {
  { SWIG_CSharpArgumentException, NULL },
  { SWIG_CSharpArgumentNullException, NULL },
  { SWIG_CSharpArgumentOutOfRangeException, NULL }
};

static void SWIGUNUSED SWIG_CSharpSetPendingException(SWIG_CSharpExceptionCodes code, const char *msg) {
  SWIG_CSharpExceptionCallback_t callback = SWIG_csharp_exceptions[SWIG_CSharpApplicationException].callback;
  if ((size_t)code < sizeof(SWIG_csharp_exceptions)/sizeof(SWIG_CSharpException_t)) {
    callback = SWIG_csharp_exceptions[code].callback;
  }
  callback(msg);
}

static void SWIGUNUSED SWIG_CSharpSetPendingExceptionArgument(SWIG_CSharpExceptionArgumentCodes code, const char *msg, const char *param_name) {
  SWIG_CSharpExceptionArgumentCallback_t callback = SWIG_csharp_exceptions_argument[SWIG_CSharpArgumentException].callback;
  if ((size_t)code < sizeof(SWIG_csharp_exceptions_argument)/sizeof(SWIG_CSharpExceptionArgument_t)) {
    callback = SWIG_csharp_exceptions_argument[code].callback;
  }
  callback(msg, param_name);
}


#ifdef __cplusplus
extern "C" 
#endif
SWIGEXPORT void SWIGSTDCALL SWIGRegisterExceptionCallbacks_Mach1DecodePositional(
                                                SWIG_CSharpExceptionCallback_t applicationCallback,
                                                SWIG_CSharpExceptionCallback_t arithmeticCallback,
                                                SWIG_CSharpExceptionCallback_t divideByZeroCallback, 
                                                SWIG_CSharpExceptionCallback_t indexOutOfRangeCallback, 
                                                SWIG_CSharpExceptionCallback_t invalidCastCallback,
                                                SWIG_CSharpExceptionCallback_t invalidOperationCallback,
                                                SWIG_CSharpExceptionCallback_t ioCallback,
                                                SWIG_CSharpExceptionCallback_t nullReferenceCallback,
                                                SWIG_CSharpExceptionCallback_t outOfMemoryCallback, 
                                                SWIG_CSharpExceptionCallback_t overflowCallback, 
                                                SWIG_CSharpExceptionCallback_t systemCallback) {
  SWIG_csharp_exceptions[SWIG_CSharpApplicationException].callback = applicationCallback;
  SWIG_csharp_exceptions[SWIG_CSharpArithmeticException].callback = arithmeticCallback;
  SWIG_csharp_exceptions[SWIG_CSharpDivideByZeroException].callback = divideByZeroCallback;
  SWIG_csharp_exceptions[SWIG_CSharpIndexOutOfRangeException].callback = indexOutOfRangeCallback;
  SWIG_csharp_exceptions[SWIG_CSharpInvalidCastException].callback = invalidCastCallback;
  SWIG_csharp_exceptions[SWIG_CSharpInvalidOperationException].callback = invalidOperationCallback;
  SWIG_csharp_exceptions[SWIG_CSharpIOException].callback = ioCallback;
  SWIG_csharp_exceptions[SWIG_CSharpNullReferenceException].callback = nullReferenceCallback;
  SWIG_csharp_exceptions[SWIG_CSharpOutOfMemoryException].callback = outOfMemoryCallback;
  SWIG_csharp_exceptions[SWIG_CSharpOverflowException].callback = overflowCallback;
  SWIG_csharp_exceptions[SWIG_CSharpSystemException].callback = systemCallback;
}

#ifdef __cplusplus
extern "C" 
#endif
SWIGEXPORT void SWIGSTDCALL SWIGRegisterExceptionArgumentCallbacks_Mach1DecodePositional(
                                                SWIG_CSharpExceptionArgumentCallback_t argumentCallback,
                                                SWIG_CSharpExceptionArgumentCallback_t argumentNullCallback,
                                                SWIG_CSharpExceptionArgumentCallback_t argumentOutOfRangeCallback) {
  SWIG_csharp_exceptions_argument[SWIG_CSharpArgumentException].callback = argumentCallback;
  SWIG_csharp_exceptions_argument[SWIG_CSharpArgumentNullException].callback = argumentNullCallback;
  SWIG_csharp_exceptions_argument[SWIG_CSharpArgumentOutOfRangeException].callback = argumentOutOfRangeCallback;
}


/* Callback for returning strings to C# without leaking memory */
typedef char * (SWIGSTDCALL* SWIG_CSharpStringHelperCallback)(const char *);
static SWIG_CSharpStringHelperCallback SWIG_csharp_string_callback = NULL;


#ifdef __cplusplus
extern "C" 
#endif
SWIGEXPORT void SWIGSTDCALL SWIGRegisterStringCallback_Mach1DecodePositional(SWIG_CSharpStringHelperCallback callback) {
  SWIG_csharp_string_callback = callback;
}


/* Contract support */

#define SWIG_contract_assert(nullreturn, expr, msg) do { if (!(expr)) {SWIG_CSharpSetPendingExceptionArgument(SWIG_CSharpArgumentOutOfRangeException, msg, ""); return nullreturn; } } while (0)


#include "Mach1DecodePositionalCore.h"
#include "Mach1DecodePositionalCAPI.h"


#ifdef __cplusplus
extern "C" {
#endif

SWIGEXPORT void * SWIGSTDCALL CSharp_Mach1DecodePositionalCAPI_create() {
  void * jresult ;
  void *result = 0 ;
  
  result = (void *)Mach1DecodePositionalCAPI_create();
  jresult = (void *)result; 
  return jresult;
}


SWIGEXPORT void SWIGSTDCALL CSharp_Mach1DecodePositionalCAPI_delete(void * jarg1) {
  void *arg1 = (void *) 0 ;
  
  arg1 = (void *)jarg1; 
  Mach1DecodePositionalCAPI_delete(arg1);
}


SWIGEXPORT void SWIGSTDCALL CSharp_Mach1DecodePositionalCAPI_setPlatformType(void * jarg1, int jarg2) {
  void *arg1 = (void *) 0 ;
  enum Mach1PlatformType arg2 ;
  
  arg1 = (void *)jarg1; 
  arg2 = (enum Mach1PlatformType)jarg2; 
  Mach1DecodePositionalCAPI_setPlatformType(arg1,arg2);
}


SWIGEXPORT void SWIGSTDCALL CSharp_Mach1DecodePositionalCAPI_setDecodeAlgoType(void * jarg1, int jarg2) {
  void *arg1 = (void *) 0 ;
  enum Mach1DecodeAlgoType arg2 ;
  
  arg1 = (void *)jarg1; 
  arg2 = (enum Mach1DecodeAlgoType)jarg2; 
  Mach1DecodePositionalCAPI_setDecodeAlgoType(arg1,arg2);
}


SWIGEXPORT void SWIGSTDCALL CSharp_Mach1DecodePositionalCAPI_setMuteWhenOutsideObject(void * jarg1, unsigned int jarg2) {
  void *arg1 = (void *) 0 ;
  bool arg2 ;
  
  arg1 = (void *)jarg1; 
  arg2 = jarg2 ? true : false; 
  Mach1DecodePositionalCAPI_setMuteWhenOutsideObject(arg1,arg2);
}


SWIGEXPORT void SWIGSTDCALL CSharp_Mach1DecodePositionalCAPI_setMuteWhenInsideObject(void * jarg1, unsigned int jarg2) {
  void *arg1 = (void *) 0 ;
  bool arg2 ;
  
  arg1 = (void *)jarg1; 
  arg2 = jarg2 ? true : false; 
  Mach1DecodePositionalCAPI_setMuteWhenInsideObject(arg1,arg2);
}


SWIGEXPORT void SWIGSTDCALL CSharp_Mach1DecodePositionalCAPI_setUseAttenuation(void * jarg1, unsigned int jarg2) {
  void *arg1 = (void *) 0 ;
  bool arg2 ;
  
  arg1 = (void *)jarg1; 
  arg2 = jarg2 ? true : false; 
  Mach1DecodePositionalCAPI_setUseAttenuation(arg1,arg2);
}


SWIGEXPORT void SWIGSTDCALL CSharp_Mach1DecodePositionalCAPI_setAttenuationCurve(void * jarg1, float jarg2) {
  void *arg1 = (void *) 0 ;
  float arg2 ;
  
  arg1 = (void *)jarg1; 
  arg2 = (float)jarg2; 
  Mach1DecodePositionalCAPI_setAttenuationCurve(arg1,arg2);
}


SWIGEXPORT void SWIGSTDCALL CSharp_Mach1DecodePositionalCAPI_setUsePlaneCalculation(void * jarg1, unsigned int jarg2) {
  void *arg1 = (void *) 0 ;
  bool arg2 ;
  
  arg1 = (void *)jarg1; 
  arg2 = jarg2 ? true : false; 
  Mach1DecodePositionalCAPI_setUsePlaneCalculation(arg1,arg2);
}


SWIGEXPORT void SWIGSTDCALL CSharp_Mach1DecodePositionalCAPI_setUseYawForRotation(void * jarg1, unsigned int jarg2) {
  void *arg1 = (void *) 0 ;
  bool arg2 ;
  
  arg1 = (void *)jarg1; 
  arg2 = jarg2 ? true : false; 
  Mach1DecodePositionalCAPI_setUseYawForRotation(arg1,arg2);
}


SWIGEXPORT void SWIGSTDCALL CSharp_Mach1DecodePositionalCAPI_setUsePitchForRotation(void * jarg1, unsigned int jarg2) {
  void *arg1 = (void *) 0 ;
  bool arg2 ;
  
  arg1 = (void *)jarg1; 
  arg2 = jarg2 ? true : false; 
  Mach1DecodePositionalCAPI_setUsePitchForRotation(arg1,arg2);
}


SWIGEXPORT void SWIGSTDCALL CSharp_Mach1DecodePositionalCAPI_setUseRollForRotation(void * jarg1, unsigned int jarg2) {
  void *arg1 = (void *) 0 ;
  bool arg2 ;
  
  arg1 = (void *)jarg1; 
  arg2 = jarg2 ? true : false; 
  Mach1DecodePositionalCAPI_setUseRollForRotation(arg1,arg2);
}


SWIGEXPORT void SWIGSTDCALL CSharp_Mach1DecodePositionalCAPI_setListenerPosition(void * jarg1, void * jarg2) {
  void *arg1 = (void *) 0 ;
  Mach1Point3D arg2 ;
  Mach1Point3D *argp2 ;
  
  arg1 = (void *)jarg1; 
  argp2 = (Mach1Point3D *)jarg2; 
  if (!argp2) {
    SWIG_CSharpSetPendingExceptionArgument(SWIG_CSharpArgumentNullException, "Attempt to dereference null Mach1Point3D", 0);
    return ;
  }
  arg2 = *argp2; 
  Mach1DecodePositionalCAPI_setListenerPosition(arg1,arg2);
}


SWIGEXPORT void SWIGSTDCALL CSharp_Mach1DecodePositionalCAPI_setListenerRotation(void * jarg1, void * jarg2) {
  void *arg1 = (void *) 0 ;
  Mach1Point3D arg2 ;
  Mach1Point3D *argp2 ;
  
  arg1 = (void *)jarg1; 
  argp2 = (Mach1Point3D *)jarg2; 
  if (!argp2) {
    SWIG_CSharpSetPendingExceptionArgument(SWIG_CSharpArgumentNullException, "Attempt to dereference null Mach1Point3D", 0);
    return ;
  }
  arg2 = *argp2; 
  Mach1DecodePositionalCAPI_setListenerRotation(arg1,arg2);
}


SWIGEXPORT void SWIGSTDCALL CSharp_Mach1DecodePositionalCAPI_setListenerRotationQuat(void * jarg1, void * jarg2) {
  void *arg1 = (void *) 0 ;
  Mach1Point4D arg2 ;
  Mach1Point4D *argp2 ;
  
  arg1 = (void *)jarg1; 
  argp2 = (Mach1Point4D *)jarg2; 
  if (!argp2) {
    SWIG_CSharpSetPendingExceptionArgument(SWIG_CSharpArgumentNullException, "Attempt to dereference null Mach1Point4D", 0);
    return ;
  }
  arg2 = *argp2; 
  Mach1DecodePositionalCAPI_setListenerRotationQuat(arg1,arg2);
}


SWIGEXPORT void SWIGSTDCALL CSharp_Mach1DecodePositionalCAPI_setDecoderAlgoPosition(void * jarg1, void * jarg2) {
  void *arg1 = (void *) 0 ;
  Mach1Point3D arg2 ;
  Mach1Point3D *argp2 ;
  
  arg1 = (void *)jarg1; 
  argp2 = (Mach1Point3D *)jarg2; 
  if (!argp2) {
    SWIG_CSharpSetPendingExceptionArgument(SWIG_CSharpArgumentNullException, "Attempt to dereference null Mach1Point3D", 0);
    return ;
  }
  arg2 = *argp2; 
  Mach1DecodePositionalCAPI_setDecoderAlgoPosition(arg1,arg2);
}


SWIGEXPORT void SWIGSTDCALL CSharp_Mach1DecodePositionalCAPI_setDecoderAlgoRotation(void * jarg1, void * jarg2) {
  void *arg1 = (void *) 0 ;
  Mach1Point3D arg2 ;
  Mach1Point3D *argp2 ;
  
  arg1 = (void *)jarg1; 
  argp2 = (Mach1Point3D *)jarg2; 
  if (!argp2) {
    SWIG_CSharpSetPendingExceptionArgument(SWIG_CSharpArgumentNullException, "Attempt to dereference null Mach1Point3D", 0);
    return ;
  }
  arg2 = *argp2; 
  Mach1DecodePositionalCAPI_setDecoderAlgoRotation(arg1,arg2);
}


SWIGEXPORT void SWIGSTDCALL CSharp_Mach1DecodePositionalCAPI_setDecoderAlgoRotationQuat(void * jarg1, void * jarg2) {
  void *arg1 = (void *) 0 ;
  Mach1Point4D arg2 ;
  Mach1Point4D *argp2 ;
  
  arg1 = (void *)jarg1; 
  argp2 = (Mach1Point4D *)jarg2; 
  if (!argp2) {
    SWIG_CSharpSetPendingExceptionArgument(SWIG_CSharpArgumentNullException, "Attempt to dereference null Mach1Point4D", 0);
    return ;
  }
  arg2 = *argp2; 
  Mach1DecodePositionalCAPI_setDecoderAlgoRotationQuat(arg1,arg2);
}


SWIGEXPORT void SWIGSTDCALL CSharp_Mach1DecodePositionalCAPI_setDecoderAlgoScale(void * jarg1, void * jarg2) {
  void *arg1 = (void *) 0 ;
  Mach1Point3D arg2 ;
  Mach1Point3D *argp2 ;
  
  arg1 = (void *)jarg1; 
  argp2 = (Mach1Point3D *)jarg2; 
  if (!argp2) {
    SWIG_CSharpSetPendingExceptionArgument(SWIG_CSharpArgumentNullException, "Attempt to dereference null Mach1Point3D", 0);
    return ;
  }
  arg2 = *argp2; 
  Mach1DecodePositionalCAPI_setDecoderAlgoScale(arg1,arg2);
}


SWIGEXPORT void SWIGSTDCALL CSharp_Mach1DecodePositionalCAPI_evaluatePositionResults(void * jarg1) {
  void *arg1 = (void *) 0 ;
  
  arg1 = (void *)jarg1; 
  Mach1DecodePositionalCAPI_evaluatePositionResults(arg1);
}


SWIGEXPORT void SWIGSTDCALL CSharp_Mach1DecodePositionalCAPI_getCoefficients(void * jarg1, void * jarg2) {
  void *arg1 = (void *) 0 ;
  float *arg2 = (float *) 0 ;
  
  arg1 = (void *)jarg1; 
  arg2 = (float *)jarg2; 
  Mach1DecodePositionalCAPI_getCoefficients(arg1,arg2);
}


SWIGEXPORT float SWIGSTDCALL CSharp_Mach1DecodePositionalCAPI_getDist(void * jarg1) {
  float jresult ;
  void *arg1 = (void *) 0 ;
  float result;
  
  arg1 = (void *)jarg1; 
  result = (float)Mach1DecodePositionalCAPI_getDist(arg1);
  jresult = result; 
  return jresult;
}


SWIGEXPORT int SWIGSTDCALL CSharp_Mach1DecodePositionalCAPI_getFormatChannelCount(void * jarg1) {
  int jresult ;
  void *arg1 = (void *) 0 ;
  int result;
  
  arg1 = (void *)jarg1; 
  result = (int)Mach1DecodePositionalCAPI_getFormatChannelCount(arg1);
  jresult = result; 
  return jresult;
}


SWIGEXPORT int SWIGSTDCALL CSharp_Mach1DecodePositionalCAPI_getFormatCoeffCount(void * jarg1) {
  int jresult ;
  void *arg1 = (void *) 0 ;
  int result;
  
  arg1 = (void *)jarg1; 
  result = (int)Mach1DecodePositionalCAPI_getFormatCoeffCount(arg1);
  jresult = result; 
  return jresult;
}


SWIGEXPORT void * SWIGSTDCALL CSharp_Mach1DecodePositionalCAPI_getCurrentAngle(void * jarg1) {
  void * jresult ;
  void *arg1 = (void *) 0 ;
  Mach1Point3D result;
  
  arg1 = (void *)jarg1; 
  result = Mach1DecodePositionalCAPI_getCurrentAngle(arg1);
  {
    Mach1Point3D * resultptr = (Mach1Point3D *) malloc(sizeof(Mach1Point3D));
    memmove(resultptr, &result, sizeof(Mach1Point3D));
    jresult = resultptr;
  }
  return jresult;
}


SWIGEXPORT void * SWIGSTDCALL CSharp_Mach1DecodePositionalCAPI_getCurrentAngleInternal(void * jarg1) {
  void * jresult ;
  void *arg1 = (void *) 0 ;
  Mach1Point3D result;
  
  arg1 = (void *)jarg1; 
  result = Mach1DecodePositionalCAPI_getCurrentAngleInternal(arg1);
  {
    Mach1Point3D * resultptr = (Mach1Point3D *) malloc(sizeof(Mach1Point3D));
    memmove(resultptr, &result, sizeof(Mach1Point3D));
    jresult = resultptr;
  }
  return jresult;
}


SWIGEXPORT void * SWIGSTDCALL CSharp_Mach1DecodePositionalCAPI_getPositionalRotation(void * jarg1) {
  void * jresult ;
  void *arg1 = (void *) 0 ;
  Mach1Point3D result;
  
  arg1 = (void *)jarg1; 
  result = Mach1DecodePositionalCAPI_getPositionalRotation(arg1);
  {
    Mach1Point3D * resultptr = (Mach1Point3D *) malloc(sizeof(Mach1Point3D));
    memmove(resultptr, &result, sizeof(Mach1Point3D));
    jresult = resultptr;
  }
  return jresult;
}


SWIGEXPORT void SWIGSTDCALL CSharp_Mach1DecodePositionalCAPI_setFilterSpeed(void * jarg1, float jarg2) {
  void *arg1 = (void *) 0 ;
  float arg2 ;
  
  arg1 = (void *)jarg1; 
  arg2 = (float)jarg2; 
  Mach1DecodePositionalCAPI_setFilterSpeed(arg1,arg2);
}


SWIGEXPORT void * SWIGSTDCALL CSharp_Mach1DecodePositionalCAPI_getClosestPointOnPlane(void * jarg1) {
  void * jresult ;
  void *arg1 = (void *) 0 ;
  Mach1Point3D result;
  
  arg1 = (void *)jarg1; 
  result = Mach1DecodePositionalCAPI_getClosestPointOnPlane(arg1);
  {
    Mach1Point3D * resultptr = (Mach1Point3D *) malloc(sizeof(Mach1Point3D));
    memmove(resultptr, &result, sizeof(Mach1Point3D));
    jresult = resultptr;
  }
  return jresult;
}


SWIGEXPORT int SWIGSTDCALL CSharp_Mach1DecodePositionalCAPI_getLastCalculationTime(void * jarg1) {
  int jresult ;
  void *arg1 = (void *) 0 ;
  long result;
  
  arg1 = (void *)jarg1; 
  result = (long)Mach1DecodePositionalCAPI_getLastCalculationTime(arg1);
  jresult = result; 
  return jresult;
}


#ifdef __cplusplus
}
#endif

