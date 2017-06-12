
/* -----------------------------------------------------------------------------------------------------------
Software License for The Fraunhofer FDK AAC Codec Library for Android

� Copyright  1995 - 2013 Fraunhofer-Gesellschaft zur F�rderung der angewandten Forschung e.V.
  All rights reserved.

 1.    INTRODUCTION
The Fraunhofer FDK AAC Codec Library for Android ("FDK AAC Codec") is software that implements
the MPEG Advanced Audio Coding ("AAC") encoding and decoding scheme for digital audio.
This FDK AAC Codec software is intended to be used on a wide variety of Android devices.

AAC's HE-AAC and HE-AAC v2 versions are regarded as today's most efficient general perceptual
audio codecs. AAC-ELD is considered the best-performing full-bandwidth communications codec by
independent studies and is widely deployed. AAC has been standardized by ISO and IEC as part
of the MPEG specifications.

Patent licenses for necessary patent claims for the FDK AAC Codec (including those of Fraunhofer)
may be obtained through Via Licensing (www.vialicensing.com) or through the respective patent owners
individually for the purpose of encoding or decoding bit streams in products that are compliant with
the ISO/IEC MPEG audio standards. Please note that most manufacturers of Android devices already license
these patent claims through Via Licensing or directly from the patent owners, and therefore FDK AAC Codec
software may already be covered under those patent licenses when it is used for those licensed purposes only.

Commercially-licensed AAC software libraries, including floating-point versions with enhanced sound quality,
are also available from Fraunhofer. Users are encouraged to check the Fraunhofer website for additional
applications information and documentation.

2.    COPYRIGHT LICENSE

Redistribution and use in source and binary forms, with or without modification, are permitted without
payment of copyright license fees provided that you satisfy the following conditions:

You must retain the complete text of this software license in redistributions of the FDK AAC Codec or
your modifications thereto in source code form.

You must retain the complete text of this software license in the documentation and/or other materials
provided with redistributions of the FDK AAC Codec or your modifications thereto in binary form.
You must make available free of charge copies of the complete source code of the FDK AAC Codec and your
modifications thereto to recipients of copies in binary form.

The name of Fraunhofer may not be used to endorse or promote products derived from this library without
prior written permission.

You may not charge copyright license fees for anyone to use, copy or distribute the FDK AAC Codec
software or your modifications thereto.

Your modified versions of the FDK AAC Codec must carry prominent notices stating that you changed the software
and the date of any change. For modified versions of the FDK AAC Codec, the term
"Fraunhofer FDK AAC Codec Library for Android" must be replaced by the term
"Third-Party Modified Version of the Fraunhofer FDK AAC Codec Library for Android."

3.    NO PATENT LICENSE

NO EXPRESS OR IMPLIED LICENSES TO ANY PATENT CLAIMS, including without limitation the patents of Fraunhofer,
ARE GRANTED BY THIS SOFTWARE LICENSE. Fraunhofer provides no warranty of patent non-infringement with
respect to this software.

You may use this FDK AAC Codec software or modifications thereto only for purposes that are authorized
by appropriate patent licenses.

4.    DISCLAIMER

This FDK AAC Codec software is provided by Fraunhofer on behalf of the copyright holders and contributors
"AS IS" and WITHOUT ANY EXPRESS OR IMPLIED WARRANTIES, including but not limited to the implied warranties
of merchantability and fitness for a particular purpose. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
CONTRIBUTORS BE LIABLE for any direct, indirect, incidental, special, exemplary, or consequential damages,
including but not limited to procurement of substitute goods or services; loss of use, data, or profits,
or business interruption, however caused and on any theory of liability, whether in contract, strict
liability, or tort (including negligence), arising in any way out of the use of this software, even if
advised of the possibility of such damage.

5.    CONTACT INFORMATION

Fraunhofer Institute for Integrated Circuits IIS
Attention: Audio and Multimedia Departments - FDK AAC LL
Am Wolfsmantel 33
91058 Erlangen, Germany

www.iis.fraunhofer.de/amm
amm-info@iis.fraunhofer.de
----------------------------------------------------------------------------------------------------------- */

/*!
  \file   dct.cpp
  \brief  DCT Implementations   
  Library functions to calculate standard DCTs. This will most likely be replaced by hand-optimized
  functions for the specific target processor.

  Three different implementations of the dct type II and the dct type III transforms are provided.

  By default implementations which are based on a single, standard complex FFT-kernel are used (dctII_f() and dctIII_f()).
  These are specifically helpful in cases where optimized FFT libraries are already available. The FFT used in these
  implementation is FFT rad2 from FDK_tools.

  Of course, one might also use DCT-libraries should they be available. The DCT and DST
  type IV implementations are only available in a version based on a complex FFT kernel.
*/

#include "dct.h"


#include "FDK_tools_rom.h"
#include "fft.h"


#if defined(__arm__)
#include "arm/dct_arm.cpp"
#endif


#if !defined(FUNCTION_dct_III)
void dct_III(FIXP_DBL *pDat, /*!< pointer to input/output */
             FIXP_DBL *tmp,  /*!< pointer to temporal working buffer */
             int L,          /*!< lenght of transform */
             int *pDat_e
             )
{
  FDK_ASSERT(L == 64 || L == 32);
  int  i;
  FIXP_DBL xr, accu1, accu2;
  int inc;
  int M = L>>1;
  int ld_M;

  if (L == 64)  ld_M = 5;
  else          ld_M = 4;

  /* This loop performs multiplication for index i (i*inc) */
  inc = (64/2) >> ld_M; /* 64/L */

  FIXP_DBL *pTmp_0 = &tmp[2];
  FIXP_DBL *pTmp_1 = &tmp[(M-1)*2];

  for(i=1; i<M>>1; i++,pTmp_0+=2,pTmp_1-=2) {

    FIXP_DBL accu3,accu4,accu5,accu6;

    cplxMultDiv2(&accu2, &accu1, pDat[L - i], pDat[i], sin_twiddle_L64[i*inc]);
    cplxMultDiv2(&accu4, &accu3, pDat[M+i], pDat[M-i], sin_twiddle_L64[(M-i)*inc]);
    accu3 >>= 1; accu4 >>= 1;

    /* This method is better for ARM926, that uses operand2 shifted right by 1 always */
    cplxMultDiv2(&accu6, &accu5, (accu3 - (accu1>>1)), ((accu2>>1) + accu4), sin_twiddle_L64[(4*i)*inc]);
    xr = (accu1>>1) + accu3;
    pTmp_0[0] = (xr>>1) - accu5;
    pTmp_1[0] = (xr>>1) + accu5;

    xr = (accu2>>1) - accu4;
    pTmp_0[1] =  (xr>>1) - accu6;
    pTmp_1[1] = -((xr>>1) + accu6);

  }

  xr     = fMultDiv2(pDat[M], sin_twiddle_L64[64/2].v.re );/* cos((PI/(2*L))*M); */
  tmp[0] = ((pDat[0]>>1) + xr)>>1;
  tmp[1] = ((pDat[0]>>1) - xr)>>1;

  cplxMultDiv2(&accu2, &accu1, pDat[L - (M/2)], pDat[M/2], sin_twiddle_L64[64/4]);
  tmp[M]   = accu1>>1;
  tmp[M+1] = accu2>>1;

  /* dit_fft expects 1 bit scaled input values */
  fft(M, tmp, pDat_e);

  /* ARM926: 12 cycles per 2-iteration, no overhead code by compiler */
  pTmp_1 = &tmp[L];
  for (i = M>>1; i--;)
  {
    FIXP_DBL tmp1, tmp2, tmp3, tmp4;
    tmp1 = *tmp++;
    tmp2 = *tmp++;
    tmp3 = *--pTmp_1;
    tmp4 = *--pTmp_1;
    *pDat++ = tmp1;
    *pDat++ = tmp3;
    *pDat++ = tmp2;
    *pDat++ = tmp4;
  }

  *pDat_e += 2;
}
#endif

#if !defined(FUNCTION_dct_II)
void dct_II(FIXP_DBL *pDat, /*!< pointer to input/output */
            FIXP_DBL *tmp,  /*!< pointer to temporal working buffer */
            int L,          /*!< lenght of transform */
            int *pDat_e
            )
{
    FDK_ASSERT(L == 64 || L == 32);
    FIXP_DBL accu1,accu2;
    FIXP_DBL *pTmp_0, *pTmp_1;

    int i;
    int inc;
    int M =  L>>1;
    int ld_M;

    FDK_ASSERT(L == 64 || L == 32);
    ld_M = 4 + (L >> 6);  /* L=64: 5,  L=32: 4 */

    inc = (64/2) >> ld_M; /* L=64: 1,  L=32: 2 */

    FIXP_DBL *pdat  = &pDat[0];
    FIXP_DBL accu3, accu4;
    pTmp_0 = &tmp[0];
    pTmp_1 = &tmp[L-1];
    for (i = M>>1; i--; )
    {
      accu1 = *pdat++;
      accu2 = *pdat++;
      accu3 = *pdat++;
      accu4 = *pdat++;
      accu1 >>= 1;
      accu2 >>= 1;
      accu3 >>= 1;
      accu4 >>= 1;
      *pTmp_0++ = accu1;
      *pTmp_0++ = accu3;
      *pTmp_1-- = accu2;
      *pTmp_1-- = accu4;
    }


    fft(M, tmp, pDat_e);

    pTmp_0 = &tmp[2];
    pTmp_1 = &tmp[(M-1)*2];

    for (i=1; i<M>>1; i++,pTmp_0+=2,pTmp_1-=2) {

      FIXP_DBL a1,a2;
      FIXP_DBL accu3, accu4;

      a1 = ((pTmp_0[1]>>1) + (pTmp_1[1]>>1));
      a2 = ((pTmp_1[0]>>1) - (pTmp_0[0]>>1));

      cplxMultDiv2(&accu1, &accu2, a2, a1, sin_twiddle_L64[(4*i)*inc]);
      accu1<<=1; accu2<<=1;

      a1 = ((pTmp_0[0]>>1) + (pTmp_1[0]>>1));
      a2 = ((pTmp_0[1]>>1) - (pTmp_1[1]>>1));

      cplxMultDiv2(&accu3, &accu4, (a1 + accu2), -(accu1 + a2), sin_twiddle_L64[i*inc]);
      pDat[L - i] = accu4;
      pDat[i]     = accu3;

      cplxMultDiv2(&accu3, &accu4, (a1 - accu2), -(accu1 - a2), sin_twiddle_L64[(M-i)*inc]);
      pDat[M + i] = accu4;
      pDat[M - i] = accu3;

    }

    cplxMultDiv2(&accu1, &accu2, tmp[M], tmp[M+1], sin_twiddle_L64[(M/2)*inc]);
    pDat[L - (M/2)] = accu2;
    pDat[M/2]       = accu1;

    pDat[0] = (tmp[0]>>1)+(tmp[1]>>1);
    pDat[M] =  fMult(((tmp[0]>>1)-(tmp[1]>>1)), sin_twiddle_L64[64/2].v.re);/* cos((PI/(2*L))*M); */

    *pDat_e += 2;
}
#endif

static
void getTables(const FIXP_WTP **twiddle, const FIXP_STP **sin_twiddle, int *sin_step, int length)
{
  int ld2_length;

 /* Get ld2 of length - 2 + 1
     -2: because first table entry is window of size 4
     +1: because we already include +1 because of ceil(log2(length)) */
  ld2_length = DFRACT_BITS-1-fNormz((FIXP_DBL)length) - 1;

  /* Extract sort of "eigenvalue" (the 4 left most bits) of length. */
  switch ( (length) >> (ld2_length-1) ) {
    case 0x4: /* radix 2 */
      *sin_twiddle = SineTable512;
      *sin_step = 1<<(9 - ld2_length);
      *twiddle = windowSlopes[0][0][ld2_length-1];
      break;
    case 0x7: /* 10 ms */
      *sin_twiddle = SineTable480;
      *sin_step = 1<<(8 - ld2_length);
      *twiddle = windowSlopes[0][1][ld2_length];
      break;
    default:
      *sin_twiddle = NULL;
      *sin_step = 0;
      *twiddle = NULL;
      break;
  }

  FDK_ASSERT(*twiddle != NULL);

  FDK_ASSERT(*sin_step > 0);

}

#if !defined(FUNCTION_dct_IV)

void dct_IV(FIXP_DBL *pDat,
            int L,
            int *pDat_e)
{
  int sin_step = 0;
  int M = L >> 1;

  const FIXP_WTP *twiddle;
  const FIXP_STP *sin_twiddle;

  FDK_ASSERT(L >= 4);

  getTables(&twiddle, &sin_twiddle, &sin_step, L);

#ifdef FUNCTION_dct_IV_func1
  if (M>=4 && (M&3) == 0) {
     /* ARM926: 44 cycles for 2 iterations = 22 cycles/iteration */
    dct_IV_func1(M>>2, twiddle,  &pDat[0], &pDat[L-1]);
  } else
#endif /* FUNCTION_dct_IV_func1 */
  {
    FIXP_DBL *RESTRICT pDat_0 = &pDat[0];
    FIXP_DBL *RESTRICT pDat_1 = &pDat[L - 2];
    int i;

    /* 29 cycles on ARM926 */
    for (i = 0; i < M-1; i+=2,pDat_0+=2,pDat_1-=2)
    {
      FIXP_DBL accu1,accu2,accu3,accu4;

      accu1 = pDat_1[1]; accu2 = pDat_0[0];
      accu3 = pDat_0[1]; accu4 = pDat_1[0];

      cplxMultDiv2(&accu1, &accu2, accu1, accu2, twiddle[i]);
      cplxMultDiv2(&accu3, &accu4, accu4, accu3, twiddle[i+1]);

      pDat_0[0] = accu2; pDat_0[1] = accu1;
      pDat_1[0] = accu4; pDat_1[1] = -accu3;
    }
    if (M&1)
    {
      FIXP_DBL accu1,accu2;

      accu1 = pDat_1[1]; accu2 = pDat_0[0];

      cplxMultDiv2(&accu1, &accu2, accu1, accu2, twiddle[i]);

      pDat_0[0] = accu2; pDat_0[1] = accu1;
    }
  }

  fft(M, pDat, pDat_e);

#ifdef FUNCTION_dct_IV_func2
  if (M>=4 && (M&3) == 0) {
     /* ARM926: 42 cycles for 2 iterations = 21 cycles/iteration */
    dct_IV_func2(M>>2, sin_twiddle, &pDat[0], &pDat[L], sin_step);
  } else
#endif /* FUNCTION_dct_IV_func2 */
  {
    FIXP_DBL *RESTRICT pDat_0 = &pDat[0];
    FIXP_DBL *RESTRICT pDat_1 = &pDat[L - 2];
    FIXP_DBL accu1,accu2,accu3,accu4;
    int idx, i;

    /* Sin and Cos values are 0.0f and 1.0f */
    accu1 = pDat_1[0];
    accu2 = pDat_1[1];

    pDat_1[1] = -(pDat_0[1]>>1);
    pDat_0[0] = (pDat_0[0]>>1);


    /* 28 cycles for ARM926 */
    for (idx = sin_step,i=1; i<(M+1)>>1; i++, idx+=sin_step)
    {
      FIXP_STP twd = sin_twiddle[idx];
      cplxMultDiv2(&accu3, &accu4, accu1, accu2, twd);
      pDat_0[1] =  accu3;
      pDat_1[0] =  accu4;

      pDat_0+=2;
      pDat_1-=2;

      cplxMultDiv2(&accu3, &accu4, pDat_0[1], pDat_0[0], twd);

      accu1 = pDat_1[0];
      accu2 = pDat_1[1];

      pDat_1[1] = -accu3;
      pDat_0[0] =  accu4;
    }

    if ( (M&1) == 0 )
    {
      /* Last Sin and Cos value pair are the same */
      accu1 = fMultDiv2(accu1, WTC(0x5a82799a));
      accu2 = fMultDiv2(accu2, WTC(0x5a82799a));

      pDat_1[0] = accu1 + accu2;
      pDat_0[1] = accu1 - accu2;
    }
  }

  /* Add twiddeling scale. */
  *pDat_e += 2;
}
#endif /* defined (FUNCTION_dct_IV) */

#if !defined(FUNCTION_dst_IV)
void dst_IV(FIXP_DBL *pDat,
            int L,
            int *pDat_e )
{
  int sin_step = 0;
  int M = L >> 1;

  const FIXP_WTP *twiddle;
  const FIXP_STP *sin_twiddle;

#ifdef DSTIV2_ENABLE
  if (L == 2) {
    const FIXP_STP tab = STCP(0x7641AF3D, 0x30FB9452);
    FIXP_DBL tmp1, tmp2;

    cplxMultDiv2(&tmp2, &tmp1, pDat[0], pDat[1], tab);

    pDat[0] = tmp1;
    pDat[1] = tmp2;

    *pDat_e += 1;

    return;
  }
#else
  FDK_ASSERT(L >= 4);
#endif

  getTables(&twiddle, &sin_twiddle, &sin_step, L);

#ifdef FUNCTION_dst_IV_func1
  if ( (M>=4) && ((M&3) == 0) ) {
    dst_IV_func1(M, twiddle, &pDat[0], &pDat[L]);
  } else
#endif
  {
    FIXP_DBL *RESTRICT pDat_0 = &pDat[0];
    FIXP_DBL *RESTRICT pDat_1 = &pDat[L - 2];

    int i;

    /* 34 cycles on ARM926 */
    for (i = 0; i < M-1; i+=2,pDat_0+=2,pDat_1-=2)
    {
      FIXP_DBL accu1,accu2,accu3,accu4;

      accu1 =  pDat_1[1]; accu2 = -pDat_0[0];
      accu3 =  pDat_0[1]; accu4 = -pDat_1[0];

      cplxMultDiv2(&accu1, &accu2, accu1, accu2, twiddle[i]);
      cplxMultDiv2(&accu3, &accu4, accu4, accu3, twiddle[i+1]);

      pDat_0[0] = accu2; pDat_0[1] = accu1;
      pDat_1[0] = accu4; pDat_1[1] = -accu3;
    }
    if (M&1)
    {
      FIXP_DBL accu1,accu2;

      accu1 =  pDat_1[1]; accu2 = -pDat_0[0];

      cplxMultDiv2(&accu1, &accu2, accu1, accu2, twiddle[i]);

      pDat_0[0] = accu2; pDat_0[1] = accu1;
    }
  }

  fft(M, pDat, pDat_e);

#ifdef FUNCTION_dst_IV_func2
  if ( (M>=4) && ((M&3) == 0) ) {
    dst_IV_func2(M>>2, sin_twiddle + sin_step, &pDat[0], &pDat[L - 1], sin_step);
  } else
#endif /* FUNCTION_dst_IV_func2 */
  {
    FIXP_DBL *RESTRICT pDat_0;
    FIXP_DBL *RESTRICT pDat_1;
    FIXP_DBL accu1,accu2,accu3,accu4;
    int idx, i;

    pDat_0 = &pDat[0];
    pDat_1 = &pDat[L - 2];

    /* Sin and Cos values are 0.0f and 1.0f */
    accu1 = pDat_1[0];
    accu2 = pDat_1[1];

    pDat_1[1] = -(pDat_0[0]>>1);
    pDat_0[0] = (pDat_0[1]>>1);

    for (idx = sin_step,i=1; i<(M+1)>>1; i++, idx+=sin_step)
    {
      FIXP_STP twd = sin_twiddle[idx];

      cplxMultDiv2(&accu3, &accu4, accu1, accu2, twd);
      pDat_1[0] =  -accu3;
      pDat_0[1] =  -accu4;

      pDat_0+=2;
      pDat_1-=2;

      cplxMultDiv2(&accu3, &accu4, pDat_0[1], pDat_0[0], twd);

      accu1 = pDat_1[0];
      accu2 = pDat_1[1];

      pDat_0[0] =  accu3;
      pDat_1[1] = -accu4;
    }

    if ( (M&1) == 0 )
    {
      /* Last Sin and Cos value pair are the same */
      accu1 = fMultDiv2(accu1, WTC(0x5a82799a));
      accu2 = fMultDiv2(accu2, WTC(0x5a82799a));

      pDat_0[1] = - accu1 - accu2;
      pDat_1[0] =   accu2 - accu1;
    }
  }

  /* Add twiddeling scale. */
  *pDat_e += 2;
}
#endif /* !defined(FUNCTION_dst_IV) */


