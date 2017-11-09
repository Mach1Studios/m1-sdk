
/* -----------------------------------------------------------------------------------------------------------
Software License for The Fraunhofer FDK AAC Codec Library for Android

� Copyright  1995 - 2015 Fraunhofer-Gesellschaft zur F�rderung der angewandten Forschung e.V.
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

/******************************************************************************

   Initial authors:      M. Lohwasser, M. Gayer
   Contents/description:

******************************************************************************/

#include "aacEnc_rom.h"

/*
   Huffman Tables
*/
const ULONG FDKaacEnc_huff_ltab1_2[3][3][3][3]=
{
  {
    { {0x000b0009,0x00090007,0x000b0009}, {0x000a0008,0x00070006,0x000a0008}, {0x000b0009,0x00090008,0x000b0009} },
    { {0x000a0008,0x00070006,0x000a0007}, {0x00070006,0x00050005,0x00070006}, {0x00090007,0x00070006,0x000a0008} },
    { {0x000b0009,0x00090007,0x000b0008}, {0x00090008,0x00070006,0x00090008}, {0x000b0009,0x00090007,0x000b0009} }
  },
  {
    { {0x00090008,0x00070006,0x00090007}, {0x00070006,0x00050005,0x00070006}, {0x00090007,0x00070006,0x00090008} },
    { {0x00070006,0x00050005,0x00070006}, {0x00050005,0x00010003,0x00050005}, {0x00070006,0x00050005,0x00070006} },
    { {0x00090008,0x00070006,0x00090007}, {0x00070006,0x00050005,0x00070006}, {0x00090008,0x00070006,0x00090008} }
  },
  {
    { {0x000b0009,0x00090007,0x000b0009}, {0x00090008,0x00070006,0x00090008}, {0x000b0008,0x00090007,0x000b0009} },
    { {0x000a0008,0x00070006,0x00090007}, {0x00070006,0x00050004,0x00070006}, {0x00090008,0x00070006,0x000a0007} },
    { {0x000b0009,0x00090007,0x000b0009}, {0x000a0007,0x00070006,0x00090008}, {0x000b0009,0x00090007,0x000b0009} }
  }
};


const ULONG FDKaacEnc_huff_ltab3_4[3][3][3][3]=
{
  {
    { {0x00010004,0x00040005,0x00080008}, {0x00040005,0x00050004,0x00080008}, {0x00090009,0x00090008,0x000a000b} },
    { {0x00040005,0x00060005,0x00090008}, {0x00060005,0x00060004,0x00090008}, {0x00090008,0x00090007,0x000a000a} },
    { {0x00090009,0x000a0008,0x000d000b}, {0x00090008,0x00090008,0x000b000a}, {0x000b000b,0x000a000a,0x000c000b} }
  },
  {
    { {0x00040004,0x00060005,0x000a0008}, {0x00060004,0x00070004,0x000a0008}, {0x000a0008,0x000a0008,0x000c000a} },
    { {0x00050004,0x00070004,0x000b0008}, {0x00060004,0x00070004,0x000a0007}, {0x00090008,0x00090007,0x000b0009} },
    { {0x00090008,0x000a0008,0x000d000a}, {0x00080007,0x00090007,0x000c0009}, {0x000a000a,0x000b0009,0x000c000a} }
  },
  {
    { {0x00080008,0x000a0008,0x000f000b}, {0x00090008,0x000b0007,0x000f000a}, {0x000d000b,0x000e000a,0x0010000c} },
    { {0x00080008,0x000a0007,0x000e000a}, {0x00090007,0x000a0007,0x000e0009}, {0x000c000a,0x000c0009,0x000f000b} },
    { {0x000b000b,0x000c000a,0x0010000c}, {0x000a000a,0x000b0009,0x000f000b}, {0x000c000b,0x000c000a,0x000f000b} }
  }
};

const ULONG FDKaacEnc_huff_ltab5_6[9][9]=
{
  {0x000d000b, 0x000c000a, 0x000b0009, 0x000b0009, 0x000a0009, 0x000b0009, 0x000b0009, 0x000c000a, 0x000d000b},
  {0x000c000a, 0x000b0009, 0x000a0008, 0x00090007, 0x00080007, 0x00090007, 0x000a0008, 0x000b0009, 0x000c000a},
  {0x000c0009, 0x000a0008, 0x00090006, 0x00080006, 0x00070006, 0x00080006, 0x00090006, 0x000a0008, 0x000b0009},
  {0x000b0009, 0x00090007, 0x00080006, 0x00050004, 0x00040004, 0x00050004, 0x00080006, 0x00090007, 0x000b0009},
  {0x000a0009, 0x00080007, 0x00070006, 0x00040004, 0x00010004, 0x00040004, 0x00070006, 0x00080007, 0x000b0009},
  {0x000b0009, 0x00090007, 0x00080006, 0x00050004, 0x00040004, 0x00050004, 0x00080006, 0x00090007, 0x000b0009},
  {0x000b0009, 0x000a0008, 0x00090006, 0x00080006, 0x00070006, 0x00080006, 0x00090006, 0x000a0008, 0x000b0009},
  {0x000c000a, 0x000b0009, 0x000a0008, 0x00090007, 0x00080007, 0x00090007, 0x000a0007, 0x000b0008, 0x000c000a},
  {0x000d000b, 0x000c000a, 0x000c0009, 0x000b0009, 0x000a0009, 0x000a0009, 0x000b0009, 0x000c000a, 0x000d000b}
};

const ULONG FDKaacEnc_huff_ltab7_8[8][8]=
{
  {0x00010005, 0x00030004, 0x00060005, 0x00070006, 0x00080007, 0x00090008, 0x000a0009, 0x000b000a},
  {0x00030004, 0x00040003, 0x00060004, 0x00070005, 0x00080006, 0x00080007, 0x00090007, 0x00090008},
  {0x00060005, 0x00060004, 0x00070004, 0x00080005, 0x00080006, 0x00090007, 0x00090007, 0x000a0008},
  {0x00070006, 0x00070005, 0x00080005, 0x00080006, 0x00090006, 0x00090007, 0x000a0008, 0x000a0008},
  {0x00080007, 0x00080006, 0x00090006, 0x00090006, 0x000a0007, 0x000a0007, 0x000a0008, 0x000b0009},
  {0x00090008, 0x00080007, 0x00090006, 0x00090007, 0x000a0007, 0x000a0008, 0x000b0008, 0x000b000a},
  {0x000a0009, 0x00090007, 0x00090007, 0x000a0008, 0x000a0008, 0x000b0008, 0x000c0009, 0x000c0009},
  {0x000b000a, 0x000a0008, 0x000a0008, 0x000a0008, 0x000b0009, 0x000b0009, 0x000c0009, 0x000c000a}
};

const ULONG FDKaacEnc_huff_ltab9_10[13][13]=
{
  {0x00010006, 0x00030005, 0x00060006, 0x00080006, 0x00090007, 0x000a0008, 0x000a0009, 0x000b000a, 0x000b000a, 0x000c000a, 0x000c000b, 0x000d000b, 0x000d000c},
  {0x00030005, 0x00040004, 0x00060004, 0x00070005, 0x00080006, 0x00080007, 0x00090007, 0x000a0008, 0x000a0008, 0x000a0009, 0x000b000a, 0x000c000a, 0x000c000b},
  {0x00060006, 0x00060004, 0x00070005, 0x00080005, 0x00080006, 0x00090006, 0x000a0007, 0x000a0008, 0x000a0008, 0x000b0009, 0x000c0009, 0x000c000a, 0x000c000a},
  {0x00080006, 0x00070005, 0x00080005, 0x00090005, 0x00090006, 0x000a0007, 0x000a0007, 0x000b0008, 0x000b0008, 0x000b0009, 0x000c0009, 0x000c000a, 0x000d000a},
  {0x00090007, 0x00080006, 0x00090006, 0x00090006, 0x000a0006, 0x000a0007, 0x000b0007, 0x000b0008, 0x000b0008, 0x000c0009, 0x000c0009, 0x000c000a, 0x000d000a},
  {0x000a0008, 0x00090007, 0x00090006, 0x000a0007, 0x000b0007, 0x000b0007, 0x000b0008, 0x000c0008, 0x000b0008, 0x000c0009, 0x000c000a, 0x000d000a, 0x000d000b},
  {0x000b0009, 0x00090007, 0x000a0007, 0x000b0007, 0x000b0007, 0x000b0008, 0x000c0008, 0x000c0009, 0x000c0009, 0x000c0009, 0x000d000a, 0x000d000a, 0x000d000b},
  {0x000b0009, 0x000a0008, 0x000a0008, 0x000b0008, 0x000b0008, 0x000c0008, 0x000c0009, 0x000d0009, 0x000d0009, 0x000d000a, 0x000d000a, 0x000d000b, 0x000d000b},
  {0x000b0009, 0x000a0008, 0x000a0008, 0x000b0008, 0x000b0008, 0x000b0008, 0x000c0009, 0x000c0009, 0x000d000a, 0x000d000a, 0x000e000a, 0x000d000b, 0x000e000b},
  {0x000b000a, 0x000a0009, 0x000b0009, 0x000b0009, 0x000c0009, 0x000c0009, 0x000c0009, 0x000c000a, 0x000d000a, 0x000d000a, 0x000e000b, 0x000e000b, 0x000e000c},
  {0x000c000a, 0x000b0009, 0x000b0009, 0x000c0009, 0x000c0009, 0x000c000a, 0x000d000a, 0x000d000a, 0x000d000a, 0x000e000b, 0x000e000b, 0x000e000b, 0x000f000c},
  {0x000c000b, 0x000b000a, 0x000c0009, 0x000c000a, 0x000c000a, 0x000d000a, 0x000d000a, 0x000d000a, 0x000d000b, 0x000e000b, 0x000e000b, 0x000f000b, 0x000f000c},
  {0x000d000b, 0x000c000a, 0x000c000a, 0x000c000a, 0x000d000a, 0x000d000a, 0x000d000a, 0x000d000b, 0x000e000b, 0x000e000c, 0x000e000c, 0x000e000c, 0x000f000c}
};

const UCHAR FDKaacEnc_huff_ltab11[17][17]=
{
  {0x04, 0x05, 0x06, 0x07, 0x08, 0x08, 0x09, 0x0a, 0x0a, 0x0a, 0x0b, 0x0b, 0x0c, 0x0b, 0x0c, 0x0c, 0x0a},
  {0x05, 0x04, 0x05, 0x06, 0x07, 0x07, 0x08, 0x08, 0x09, 0x09, 0x09, 0x0a, 0x0a, 0x0a, 0x0a, 0x0b, 0x08},
  {0x06, 0x05, 0x05, 0x06, 0x07, 0x07, 0x08, 0x08, 0x08, 0x09, 0x09, 0x09, 0x0a, 0x0a, 0x0a, 0x0a, 0x08},
  {0x07, 0x06, 0x06, 0x06, 0x07, 0x07, 0x08, 0x08, 0x08, 0x09, 0x09, 0x09, 0x0a, 0x0a, 0x0a, 0x0a, 0x08},
  {0x08, 0x07, 0x07, 0x07, 0x07, 0x08, 0x08, 0x08, 0x08, 0x09, 0x09, 0x09, 0x0a, 0x0a, 0x0a, 0x0a, 0x08},
  {0x08, 0x07, 0x07, 0x07, 0x07, 0x08, 0x08, 0x08, 0x09, 0x09, 0x09, 0x09, 0x0a, 0x0a, 0x0a, 0x0a, 0x08},
  {0x09, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x09, 0x09, 0x09, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x08},
  {0x09, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x09, 0x09, 0x09, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x08},
  {0x0a, 0x09, 0x08, 0x08, 0x09, 0x09, 0x09, 0x09, 0x09, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0b, 0x08},
  {0x0a, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0b, 0x0b, 0x08},
  {0x0b, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0b, 0x0a, 0x0b, 0x0b, 0x08},
  {0x0b, 0x0a, 0x09, 0x09, 0x0a, 0x09, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x08},
  {0x0b, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x09},
  {0x0b, 0x0a, 0x09, 0x09, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x09},
  {0x0b, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x09},
  {0x0c, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0c, 0x0c, 0x09},
  {0x09, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x09, 0x05}
};

const UCHAR FDKaacEnc_huff_ltabscf[121]=
{
  0x12, 0x12, 0x12, 0x12, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13,
  0x13, 0x13, 0x13, 0x12, 0x13, 0x12, 0x11, 0x11, 0x10, 0x11, 0x10, 0x10, 0x10, 0x10, 0x0f, 0x0f,
  0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0d, 0x0d, 0x0c, 0x0c, 0x0c, 0x0b, 0x0c, 0x0b, 0x0a, 0x0a,
  0x0a, 0x09, 0x09, 0x08, 0x08, 0x08, 0x07, 0x06, 0x06, 0x05, 0x04, 0x03, 0x01, 0x04, 0x04, 0x05,
  0x06, 0x06, 0x07, 0x07, 0x08, 0x08, 0x09, 0x09, 0x0a, 0x0a, 0x0a, 0x0b, 0x0b, 0x0b, 0x0b, 0x0c,
  0x0c, 0x0d, 0x0d, 0x0d, 0x0e, 0x0e, 0x10, 0x0f, 0x10, 0x0f, 0x12, 0x13, 0x13, 0x13, 0x13, 0x13,
  0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13,
  0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13
};


const USHORT FDKaacEnc_huff_ctab1[3][3][3][3]=
{
  {
    { {0x07f8,0x01f1,0x07fd}, {0x03f5,0x0068,0x03f0}, {0x07f7,0x01ec,0x07f5} },
    { {0x03f1,0x0072,0x03f4}, {0x0074,0x0011,0x0076}, {0x01eb,0x006c,0x03f6} },
    { {0x07fc,0x01e1,0x07f1}, {0x01f0,0x0061,0x01f6}, {0x07f2,0x01ea,0x07fb} }
  },
  {
    { {0x01f2,0x0069,0x01ed}, {0x0077,0x0017,0x006f}, {0x01e6,0x0064,0x01e5} },
    { {0x0067,0x0015,0x0062}, {0x0012,0x0000,0x0014}, {0x0065,0x0016,0x006d} },
    { {0x01e9,0x0063,0x01e4}, {0x006b,0x0013,0x0071}, {0x01e3,0x0070,0x01f3} }
  },
  {
    { {0x07fe,0x01e7,0x07f3}, {0x01ef,0x0060,0x01ee}, {0x07f0,0x01e2,0x07fa} },
    { {0x03f3,0x006a,0x01e8}, {0x0075,0x0010,0x0073}, {0x01f4,0x006e,0x03f7} },
    { {0x07f6,0x01e0,0x07f9}, {0x03f2,0x0066,0x01f5}, {0x07ff,0x01f7,0x07f4} }
  }
};

const USHORT FDKaacEnc_huff_ctab2[3][3][3][3]=
{
  {
    { {0x01f3,0x006f,0x01fd}, {0x00eb,0x0023,0x00ea}, {0x01f7,0x00e8,0x01fa} },
    { {0x00f2,0x002d,0x0070}, {0x0020,0x0006,0x002b}, {0x006e,0x0028,0x00e9} },
    { {0x01f9,0x0066,0x00f8}, {0x00e7,0x001b,0x00f1}, {0x01f4,0x006b,0x01f5} }
  },
  {
    { {0x00ec,0x002a,0x006c}, {0x002c,0x000a,0x0027}, {0x0067,0x001a,0x00f5} },
    { {0x0024,0x0008,0x001f}, {0x0009,0x0000,0x0007}, {0x001d,0x000b,0x0030} },
    { {0x00ef,0x001c,0x0064}, {0x001e,0x000c,0x0029}, {0x00f3,0x002f,0x00f0} }
  },
  {
    { {0x01fc,0x0071,0x01f2}, {0x00f4,0x0021,0x00e6}, {0x00f7,0x0068,0x01f8} },
    { {0x00ee,0x0022,0x0065}, {0x0031,0x0002,0x0026}, {0x00ed,0x0025,0x006a} },
    { {0x01fb,0x0072,0x01fe}, {0x0069,0x002e,0x00f6}, {0x01ff,0x006d,0x01f6} }
  }
};

const USHORT FDKaacEnc_huff_ctab3[3][3][3][3]=
{
  {
    { {0x0000,0x0009,0x00ef}, {0x000b,0x0019,0x00f0}, {0x01eb,0x01e6,0x03f2} },
    { {0x000a,0x0035,0x01ef}, {0x0034,0x0037,0x01e9}, {0x01ed,0x01e7,0x03f3} },
    { {0x01ee,0x03ed,0x1ffa}, {0x01ec,0x01f2,0x07f9}, {0x07f8,0x03f8,0x0ff8} }
  },
  {
    { {0x0008,0x0038,0x03f6}, {0x0036,0x0075,0x03f1}, {0x03eb,0x03ec,0x0ff4} },
    { {0x0018,0x0076,0x07f4}, {0x0039,0x0074,0x03ef}, {0x01f3,0x01f4,0x07f6} },
    { {0x01e8,0x03ea,0x1ffc}, {0x00f2,0x01f1,0x0ffb}, {0x03f5,0x07f3,0x0ffc} }
  },
  {
    { {0x00ee,0x03f7,0x7ffe}, {0x01f0,0x07f5,0x7ffd}, {0x1ffb,0x3ffa,0xffff} },
    { {0x00f1,0x03f0,0x3ffc}, {0x01ea,0x03ee,0x3ffb}, {0x0ff6,0x0ffa,0x7ffc} },
    { {0x07f2,0x0ff5,0xfffe}, {0x03f4,0x07f7,0x7ffb}, {0x0ff7,0x0ff9,0x7ffa} }
  }
};

const USHORT FDKaacEnc_huff_ctab4[3][3][3][3]=
{
  {
    { {0x0007,0x0016,0x00f6}, {0x0018,0x0008,0x00ef}, {0x01ef,0x00f3,0x07f8} },
    { {0x0019,0x0017,0x00ed}, {0x0015,0x0001,0x00e2}, {0x00f0,0x0070,0x03f0} },
    { {0x01ee,0x00f1,0x07fa}, {0x00ee,0x00e4,0x03f2}, {0x07f6,0x03ef,0x07fd} }
  },
  {
    { {0x0005,0x0014,0x00f2}, {0x0009,0x0004,0x00e5}, {0x00f4,0x00e8,0x03f4} },
    { {0x0006,0x0002,0x00e7}, {0x0003,0x0000,0x006b}, {0x00e3,0x0069,0x01f3} },
    { {0x00eb,0x00e6,0x03f6}, {0x006e,0x006a,0x01f4}, {0x03ec,0x01f0,0x03f9} }
  },
  {
    { {0x00f5,0x00ec,0x07fb}, {0x00ea,0x006f,0x03f7}, {0x07f9,0x03f3,0x0fff} },
    { {0x00e9,0x006d,0x03f8}, {0x006c,0x0068,0x01f5}, {0x03ee,0x01f2,0x07f4} },
    { {0x07f7,0x03f1,0x0ffe}, {0x03ed,0x01f1,0x07f5}, {0x07fe,0x03f5,0x07fc} }
  }
};
const USHORT FDKaacEnc_huff_ctab5[9][9]=
{
  {0x1fff, 0x0ff7, 0x07f4, 0x07e8, 0x03f1, 0x07ee, 0x07f9, 0x0ff8, 0x1ffd},
  {0x0ffd, 0x07f1, 0x03e8, 0x01e8, 0x00f0, 0x01ec, 0x03ee, 0x07f2, 0x0ffa},
  {0x0ff4, 0x03ef, 0x01f2, 0x00e8, 0x0070, 0x00ec, 0x01f0, 0x03ea, 0x07f3},
  {0x07eb, 0x01eb, 0x00ea, 0x001a, 0x0008, 0x0019, 0x00ee, 0x01ef, 0x07ed},
  {0x03f0, 0x00f2, 0x0073, 0x000b, 0x0000, 0x000a, 0x0071, 0x00f3, 0x07e9},
  {0x07ef, 0x01ee, 0x00ef, 0x0018, 0x0009, 0x001b, 0x00eb, 0x01e9, 0x07ec},
  {0x07f6, 0x03eb, 0x01f3, 0x00ed, 0x0072, 0x00e9, 0x01f1, 0x03ed, 0x07f7},
  {0x0ff6, 0x07f0, 0x03e9, 0x01ed, 0x00f1, 0x01ea, 0x03ec, 0x07f8, 0x0ff9},
  {0x1ffc, 0x0ffc, 0x0ff5, 0x07ea, 0x03f3, 0x03f2, 0x07f5, 0x0ffb, 0x1ffe}
};

const USHORT FDKaacEnc_huff_ctab6[9][9]=
{
  {0x07fe, 0x03fd, 0x01f1, 0x01eb, 0x01f4, 0x01ea, 0x01f0, 0x03fc, 0x07fd},
  {0x03f6, 0x01e5, 0x00ea, 0x006c, 0x0071, 0x0068, 0x00f0, 0x01e6, 0x03f7},
  {0x01f3, 0x00ef, 0x0032, 0x0027, 0x0028, 0x0026, 0x0031, 0x00eb, 0x01f7},
  {0x01e8, 0x006f, 0x002e, 0x0008, 0x0004, 0x0006, 0x0029, 0x006b, 0x01ee},
  {0x01ef, 0x0072, 0x002d, 0x0002, 0x0000, 0x0003, 0x002f, 0x0073, 0x01fa},
  {0x01e7, 0x006e, 0x002b, 0x0007, 0x0001, 0x0005, 0x002c, 0x006d, 0x01ec},
  {0x01f9, 0x00ee, 0x0030, 0x0024, 0x002a, 0x0025, 0x0033, 0x00ec, 0x01f2},
  {0x03f8, 0x01e4, 0x00ed, 0x006a, 0x0070, 0x0069, 0x0074, 0x00f1, 0x03fa},
  {0x07ff, 0x03f9, 0x01f6, 0x01ed, 0x01f8, 0x01e9, 0x01f5, 0x03fb, 0x07fc}
};

const USHORT FDKaacEnc_huff_ctab7[8][8]=
{
  {0x0000, 0x0005, 0x0037, 0x0074, 0x00f2, 0x01eb, 0x03ed, 0x07f7},
  {0x0004, 0x000c, 0x0035, 0x0071, 0x00ec, 0x00ee, 0x01ee, 0x01f5},
  {0x0036, 0x0034, 0x0072, 0x00ea, 0x00f1, 0x01e9, 0x01f3, 0x03f5},
  {0x0073, 0x0070, 0x00eb, 0x00f0, 0x01f1, 0x01f0, 0x03ec, 0x03fa},
  {0x00f3, 0x00ed, 0x01e8, 0x01ef, 0x03ef, 0x03f1, 0x03f9, 0x07fb},
  {0x01ed, 0x00ef, 0x01ea, 0x01f2, 0x03f3, 0x03f8, 0x07f9, 0x07fc},
  {0x03ee, 0x01ec, 0x01f4, 0x03f4, 0x03f7, 0x07f8, 0x0ffd, 0x0ffe},
  {0x07f6, 0x03f0, 0x03f2, 0x03f6, 0x07fa, 0x07fd, 0x0ffc, 0x0fff}
};

const USHORT FDKaacEnc_huff_ctab8[8][8]=
{
  {0x000e, 0x0005, 0x0010, 0x0030, 0x006f, 0x00f1, 0x01fa, 0x03fe},
  {0x0003, 0x0000, 0x0004, 0x0012, 0x002c, 0x006a, 0x0075, 0x00f8},
  {0x000f, 0x0002, 0x0006, 0x0014, 0x002e, 0x0069, 0x0072, 0x00f5},
  {0x002f, 0x0011, 0x0013, 0x002a, 0x0032, 0x006c, 0x00ec, 0x00fa},
  {0x0071, 0x002b, 0x002d, 0x0031, 0x006d, 0x0070, 0x00f2, 0x01f9},
  {0x00ef, 0x0068, 0x0033, 0x006b, 0x006e, 0x00ee, 0x00f9, 0x03fc},
  {0x01f8, 0x0074, 0x0073, 0x00ed, 0x00f0, 0x00f6, 0x01f6, 0x01fd},
  {0x03fd, 0x00f3, 0x00f4, 0x00f7, 0x01f7, 0x01fb, 0x01fc, 0x03ff}
};

const USHORT FDKaacEnc_huff_ctab9[13][13]=
{
  {0x0000, 0x0005, 0x0037, 0x00e7, 0x01de, 0x03ce, 0x03d9, 0x07c8, 0x07cd, 0x0fc8, 0x0fdd, 0x1fe4, 0x1fec},
  {0x0004, 0x000c, 0x0035, 0x0072, 0x00ea, 0x00ed, 0x01e2, 0x03d1, 0x03d3, 0x03e0, 0x07d8, 0x0fcf, 0x0fd5},
  {0x0036, 0x0034, 0x0071, 0x00e8, 0x00ec, 0x01e1, 0x03cf, 0x03dd, 0x03db, 0x07d0, 0x0fc7, 0x0fd4, 0x0fe4},
  {0x00e6, 0x0070, 0x00e9, 0x01dd, 0x01e3, 0x03d2, 0x03dc, 0x07cc, 0x07ca, 0x07de, 0x0fd8, 0x0fea, 0x1fdb},
  {0x01df, 0x00eb, 0x01dc, 0x01e6, 0x03d5, 0x03de, 0x07cb, 0x07dd, 0x07dc, 0x0fcd, 0x0fe2, 0x0fe7, 0x1fe1},
  {0x03d0, 0x01e0, 0x01e4, 0x03d6, 0x07c5, 0x07d1, 0x07db, 0x0fd2, 0x07e0, 0x0fd9, 0x0feb, 0x1fe3, 0x1fe9},
  {0x07c4, 0x01e5, 0x03d7, 0x07c6, 0x07cf, 0x07da, 0x0fcb, 0x0fda, 0x0fe3, 0x0fe9, 0x1fe6, 0x1ff3, 0x1ff7},
  {0x07d3, 0x03d8, 0x03e1, 0x07d4, 0x07d9, 0x0fd3, 0x0fde, 0x1fdd, 0x1fd9, 0x1fe2, 0x1fea, 0x1ff1, 0x1ff6},
  {0x07d2, 0x03d4, 0x03da, 0x07c7, 0x07d7, 0x07e2, 0x0fce, 0x0fdb, 0x1fd8, 0x1fee, 0x3ff0, 0x1ff4, 0x3ff2},
  {0x07e1, 0x03df, 0x07c9, 0x07d6, 0x0fca, 0x0fd0, 0x0fe5, 0x0fe6, 0x1feb, 0x1fef, 0x3ff3, 0x3ff4, 0x3ff5},
  {0x0fe0, 0x07ce, 0x07d5, 0x0fc6, 0x0fd1, 0x0fe1, 0x1fe0, 0x1fe8, 0x1ff0, 0x3ff1, 0x3ff8, 0x3ff6, 0x7ffc},
  {0x0fe8, 0x07df, 0x0fc9, 0x0fd7, 0x0fdc, 0x1fdc, 0x1fdf, 0x1fed, 0x1ff5, 0x3ff9, 0x3ffb, 0x7ffd, 0x7ffe},
  {0x1fe7, 0x0fcc, 0x0fd6, 0x0fdf, 0x1fde, 0x1fda, 0x1fe5, 0x1ff2, 0x3ffa, 0x3ff7, 0x3ffc, 0x3ffd, 0x7fff}
};

const USHORT FDKaacEnc_huff_ctab10[13][13]=
{
  {0x0022, 0x0008, 0x001d, 0x0026, 0x005f, 0x00d3, 0x01cf, 0x03d0, 0x03d7, 0x03ed, 0x07f0, 0x07f6, 0x0ffd},
  {0x0007, 0x0000, 0x0001, 0x0009, 0x0020, 0x0054, 0x0060, 0x00d5, 0x00dc, 0x01d4, 0x03cd, 0x03de, 0x07e7},
  {0x001c, 0x0002, 0x0006, 0x000c, 0x001e, 0x0028, 0x005b, 0x00cd, 0x00d9, 0x01ce, 0x01dc, 0x03d9, 0x03f1},
  {0x0025, 0x000b, 0x000a, 0x000d, 0x0024, 0x0057, 0x0061, 0x00cc, 0x00dd, 0x01cc, 0x01de, 0x03d3, 0x03e7},
  {0x005d, 0x0021, 0x001f, 0x0023, 0x0027, 0x0059, 0x0064, 0x00d8, 0x00df, 0x01d2, 0x01e2, 0x03dd, 0x03ee},
  {0x00d1, 0x0055, 0x0029, 0x0056, 0x0058, 0x0062, 0x00ce, 0x00e0, 0x00e2, 0x01da, 0x03d4, 0x03e3, 0x07eb},
  {0x01c9, 0x005e, 0x005a, 0x005c, 0x0063, 0x00ca, 0x00da, 0x01c7, 0x01ca, 0x01e0, 0x03db, 0x03e8, 0x07ec},
  {0x01e3, 0x00d2, 0x00cb, 0x00d0, 0x00d7, 0x00db, 0x01c6, 0x01d5, 0x01d8, 0x03ca, 0x03da, 0x07ea, 0x07f1},
  {0x01e1, 0x00d4, 0x00cf, 0x00d6, 0x00de, 0x00e1, 0x01d0, 0x01d6, 0x03d1, 0x03d5, 0x03f2, 0x07ee, 0x07fb},
  {0x03e9, 0x01cd, 0x01c8, 0x01cb, 0x01d1, 0x01d7, 0x01df, 0x03cf, 0x03e0, 0x03ef, 0x07e6, 0x07f8, 0x0ffa},
  {0x03eb, 0x01dd, 0x01d3, 0x01d9, 0x01db, 0x03d2, 0x03cc, 0x03dc, 0x03ea, 0x07ed, 0x07f3, 0x07f9, 0x0ff9},
  {0x07f2, 0x03ce, 0x01e4, 0x03cb, 0x03d8, 0x03d6, 0x03e2, 0x03e5, 0x07e8, 0x07f4, 0x07f5, 0x07f7, 0x0ffb},
  {0x07fa, 0x03ec, 0x03df, 0x03e1, 0x03e4, 0x03e6, 0x03f0, 0x07e9, 0x07ef, 0x0ff8, 0x0ffe, 0x0ffc, 0x0fff}
};

const USHORT FDKaacEnc_huff_ctab11[21][17]=
{
  {0x0000, 0x0006, 0x0019, 0x003d, 0x009c, 0x00c6, 0x01a7, 0x0390, 0x03c2, 0x03df, 0x07e6, 0x07f3, 0x0ffb, 0x07ec, 0x0ffa, 0x0ffe, 0x038e},
  {0x0005, 0x0001, 0x0008, 0x0014, 0x0037, 0x0042, 0x0092, 0x00af, 0x0191, 0x01a5, 0x01b5, 0x039e, 0x03c0, 0x03a2, 0x03cd, 0x07d6, 0x00ae},
  {0x0017, 0x0007, 0x0009, 0x0018, 0x0039, 0x0040, 0x008e, 0x00a3, 0x00b8, 0x0199, 0x01ac, 0x01c1, 0x03b1, 0x0396, 0x03be, 0x03ca, 0x009d},
  {0x003c, 0x0015, 0x0016, 0x001a, 0x003b, 0x0044, 0x0091, 0x00a5, 0x00be, 0x0196, 0x01ae, 0x01b9, 0x03a1, 0x0391, 0x03a5, 0x03d5, 0x0094},
  {0x009a, 0x0036, 0x0038, 0x003a, 0x0041, 0x008c, 0x009b, 0x00b0, 0x00c3, 0x019e, 0x01ab, 0x01bc, 0x039f, 0x038f, 0x03a9, 0x03cf, 0x0093},
  {0x00bf, 0x003e, 0x003f, 0x0043, 0x0045, 0x009e, 0x00a7, 0x00b9, 0x0194, 0x01a2, 0x01ba, 0x01c3, 0x03a6, 0x03a7, 0x03bb, 0x03d4, 0x009f},
  {0x01a0, 0x008f, 0x008d, 0x0090, 0x0098, 0x00a6, 0x00b6, 0x00c4, 0x019f, 0x01af, 0x01bf, 0x0399, 0x03bf, 0x03b4, 0x03c9, 0x03e7, 0x00a8},
  {0x01b6, 0x00ab, 0x00a4, 0x00aa, 0x00b2, 0x00c2, 0x00c5, 0x0198, 0x01a4, 0x01b8, 0x038c, 0x03a4, 0x03c4, 0x03c6, 0x03dd, 0x03e8, 0x00ad},
  {0x03af, 0x0192, 0x00bd, 0x00bc, 0x018e, 0x0197, 0x019a, 0x01a3, 0x01b1, 0x038d, 0x0398, 0x03b7, 0x03d3, 0x03d1, 0x03db, 0x07dd, 0x00b4},
  {0x03de, 0x01a9, 0x019b, 0x019c, 0x01a1, 0x01aa, 0x01ad, 0x01b3, 0x038b, 0x03b2, 0x03b8, 0x03ce, 0x03e1, 0x03e0, 0x07d2, 0x07e5, 0x00b7},
  {0x07e3, 0x01bb, 0x01a8, 0x01a6, 0x01b0, 0x01b2, 0x01b7, 0x039b, 0x039a, 0x03ba, 0x03b5, 0x03d6, 0x07d7, 0x03e4, 0x07d8, 0x07ea, 0x00ba},
  {0x07e8, 0x03a0, 0x01bd, 0x01b4, 0x038a, 0x01c4, 0x0392, 0x03aa, 0x03b0, 0x03bc, 0x03d7, 0x07d4, 0x07dc, 0x07db, 0x07d5, 0x07f0, 0x00c1},
  {0x07fb, 0x03c8, 0x03a3, 0x0395, 0x039d, 0x03ac, 0x03ae, 0x03c5, 0x03d8, 0x03e2, 0x03e6, 0x07e4, 0x07e7, 0x07e0, 0x07e9, 0x07f7, 0x0190},
  {0x07f2, 0x0393, 0x01be, 0x01c0, 0x0394, 0x0397, 0x03ad, 0x03c3, 0x03c1, 0x03d2, 0x07da, 0x07d9, 0x07df, 0x07eb, 0x07f4, 0x07fa, 0x0195},
  {0x07f8, 0x03bd, 0x039c, 0x03ab, 0x03a8, 0x03b3, 0x03b9, 0x03d0, 0x03e3, 0x03e5, 0x07e2, 0x07de, 0x07ed, 0x07f1, 0x07f9, 0x07fc, 0x0193},
  {0x0ffd, 0x03dc, 0x03b6, 0x03c7, 0x03cc, 0x03cb, 0x03d9, 0x03da, 0x07d3, 0x07e1, 0x07ee, 0x07ef, 0x07f5, 0x07f6, 0x0ffc, 0x0fff, 0x019d},
  {0x01c2, 0x00b5, 0x00a1, 0x0096, 0x0097, 0x0095, 0x0099, 0x00a0, 0x00a2, 0x00ac, 0x00a9, 0x00b1, 0x00b3, 0x00bb, 0x00c0, 0x018f, 0x0004},
  {0x0018, 0x002e, 0x0000, 0x005a, 0x00a5, 0x00f8, 0x00b7, 0x0094, 0x00f9, 0x004d, 0x0021, 0x002b, 0x004f, 0x007b, 0x00bc, 0x0046, 0x0015},
  {0x0042, 0x0037, 0x0078, 0x000d, 0x0068, 0x005f, 0x000d, 0x005e, 0x005a, 0x00be, 0x0063, 0x007e, 0x001f, 0x0092, 0x001a, 0x00ab, 0x0032},
  {0x00e6, 0x0037, 0x0000, 0x0058, 0x000b, 0x005a, 0x00e1, 0x005d, 0x0029, 0x0017, 0x007e, 0x0069, 0x00aa, 0x0054, 0x0029, 0x0032, 0x0041},
  {0x0046, 0x00ea, 0x0034, 0x00ea, 0x0011, 0x001b, 0x00a9, 0x0094, 0x00e2, 0x0031, 0x00d0, 0x00e5, 0x0007, 0x0070, 0x0069, 0x003e, 0x0021}
};

const ULONG FDKaacEnc_huff_ctabscf[121]=
{
  0x0003ffe8, 0x0003ffe6, 0x0003ffe7, 0x0003ffe5, 0x0007fff5, 0x0007fff1, 0x0007ffed, 0x0007fff6,
  0x0007ffee, 0x0007ffef, 0x0007fff0, 0x0007fffc, 0x0007fffd, 0x0007ffff, 0x0007fffe, 0x0007fff7,
  0x0007fff8, 0x0007fffb, 0x0007fff9, 0x0003ffe4, 0x0007fffa, 0x0003ffe3, 0x0001ffef, 0x0001fff0,
  0x0000fff5, 0x0001ffee, 0x0000fff2, 0x0000fff3, 0x0000fff4, 0x0000fff1, 0x00007ff6, 0x00007ff7,
  0x00003ff9, 0x00003ff5, 0x00003ff7, 0x00003ff3, 0x00003ff6, 0x00003ff2, 0x00001ff7, 0x00001ff5,
  0x00000ff9, 0x00000ff7, 0x00000ff6, 0x000007f9, 0x00000ff4, 0x000007f8, 0x000003f9, 0x000003f7,
  0x000003f5, 0x000001f8, 0x000001f7, 0x000000fa, 0x000000f8, 0x000000f6, 0x00000079, 0x0000003a,
  0x00000038, 0x0000001a, 0x0000000b, 0x00000004, 0x00000000, 0x0000000a, 0x0000000c, 0x0000001b,
  0x00000039, 0x0000003b, 0x00000078, 0x0000007a, 0x000000f7, 0x000000f9, 0x000001f6, 0x000001f9,
  0x000003f4, 0x000003f6, 0x000003f8, 0x000007f5, 0x000007f4, 0x000007f6, 0x000007f7, 0x00000ff5,
  0x00000ff8, 0x00001ff4, 0x00001ff6, 0x00001ff8, 0x00003ff8, 0x00003ff4, 0x0000fff0, 0x00007ff4,
  0x0000fff6, 0x00007ff5, 0x0003ffe2, 0x0007ffd9, 0x0007ffda, 0x0007ffdb, 0x0007ffdc, 0x0007ffdd,
  0x0007ffde, 0x0007ffd8, 0x0007ffd2, 0x0007ffd3, 0x0007ffd4, 0x0007ffd5, 0x0007ffd6, 0x0007fff2,
  0x0007ffdf, 0x0007ffe7, 0x0007ffe8, 0x0007ffe9, 0x0007ffea, 0x0007ffeb, 0x0007ffe6, 0x0007ffe0,
  0x0007ffe1, 0x0007ffe2, 0x0007ffe3, 0x0007ffe4, 0x0007ffe5, 0x0007ffd7, 0x0007ffec, 0x0007fff4,
  0x0007fff3
};

/*
  table of (0.50000...1.00000) ^0.75
*/
const FIXP_QTD FDKaacEnc_mTab_3_4[MANT_SIZE] =
{
  QTC(0x4c1bf829), QTC(0x4c3880de), QTC(0x4c550603), QTC(0x4c71879c), QTC(0x4c8e05aa), QTC(0x4caa8030), QTC(0x4cc6f72f), QTC(0x4ce36aab),
  QTC(0x4cffdaa4), QTC(0x4d1c471d), QTC(0x4d38b019), QTC(0x4d55159a), QTC(0x4d7177a1), QTC(0x4d8dd631), QTC(0x4daa314b), QTC(0x4dc688f3),
  QTC(0x4de2dd2a), QTC(0x4dff2df2), QTC(0x4e1b7b4d), QTC(0x4e37c53d), QTC(0x4e540bc5), QTC(0x4e704ee6), QTC(0x4e8c8ea3), QTC(0x4ea8cafd),
  QTC(0x4ec503f7), QTC(0x4ee13992), QTC(0x4efd6bd0), QTC(0x4f199ab4), QTC(0x4f35c640), QTC(0x4f51ee75), QTC(0x4f6e1356), QTC(0x4f8a34e4),
  QTC(0x4fa65321), QTC(0x4fc26e10), QTC(0x4fde85b2), QTC(0x4ffa9a0a), QTC(0x5016ab18), QTC(0x5032b8e0), QTC(0x504ec362), QTC(0x506acaa1),
  QTC(0x5086cea0), QTC(0x50a2cf5e), QTC(0x50becce0), QTC(0x50dac725), QTC(0x50f6be31), QTC(0x5112b205), QTC(0x512ea2a3), QTC(0x514a900d),
  QTC(0x51667a45), QTC(0x5182614c), QTC(0x519e4524), QTC(0x51ba25cf), QTC(0x51d60350), QTC(0x51f1dda7), QTC(0x520db4d6), QTC(0x522988e0),
  QTC(0x524559c6), QTC(0x52612789), QTC(0x527cf22d), QTC(0x5298b9b1), QTC(0x52b47e19), QTC(0x52d03f65), QTC(0x52ebfd98), QTC(0x5307b8b4),
  QTC(0x532370b9), QTC(0x533f25aa), QTC(0x535ad789), QTC(0x53768656), QTC(0x53923215), QTC(0x53addac6), QTC(0x53c9806b), QTC(0x53e52306),
  QTC(0x5400c298), QTC(0x541c5f24), QTC(0x5437f8ab), QTC(0x54538f2e), QTC(0x546f22af), QTC(0x548ab330), QTC(0x54a640b3), QTC(0x54c1cb38),
  QTC(0x54dd52c2), QTC(0x54f8d753), QTC(0x551458eb), QTC(0x552fd78d), QTC(0x554b5339), QTC(0x5566cbf3), QTC(0x558241bb), QTC(0x559db492),
  QTC(0x55b9247b), QTC(0x55d49177), QTC(0x55effb87), QTC(0x560b62ad), QTC(0x5626c6eb), QTC(0x56422842), QTC(0x565d86b4), QTC(0x5678e242),
  QTC(0x56943aee), QTC(0x56af90b9), QTC(0x56cae3a4), QTC(0x56e633b2), QTC(0x570180e4), QTC(0x571ccb3b), QTC(0x573812b8), QTC(0x5753575e),
  QTC(0x576e992e), QTC(0x5789d829), QTC(0x57a51450), QTC(0x57c04da6), QTC(0x57db842b), QTC(0x57f6b7e1), QTC(0x5811e8c9), QTC(0x582d16e6),
  QTC(0x58484238), QTC(0x58636ac0), QTC(0x587e9081), QTC(0x5899b37c), QTC(0x58b4d3b1), QTC(0x58cff123), QTC(0x58eb0bd3), QTC(0x590623c2),
  QTC(0x592138f2), QTC(0x593c4b63), QTC(0x59575b19), QTC(0x59726812), QTC(0x598d7253), QTC(0x59a879da), QTC(0x59c37eab), QTC(0x59de80c6),
  QTC(0x59f9802d), QTC(0x5a147ce0), QTC(0x5a2f76e2), QTC(0x5a4a6e34), QTC(0x5a6562d6), QTC(0x5a8054cb), QTC(0x5a9b4414), QTC(0x5ab630b2),
  QTC(0x5ad11aa6), QTC(0x5aec01f1), QTC(0x5b06e696), QTC(0x5b21c895), QTC(0x5b3ca7ef), QTC(0x5b5784a6), QTC(0x5b725ebc), QTC(0x5b8d3631),
  QTC(0x5ba80b06), QTC(0x5bc2dd3e), QTC(0x5bddacd9), QTC(0x5bf879d8), QTC(0x5c13443d), QTC(0x5c2e0c09), QTC(0x5c48d13e), QTC(0x5c6393dc),
  QTC(0x5c7e53e5), QTC(0x5c99115a), QTC(0x5cb3cc3c), QTC(0x5cce848d), QTC(0x5ce93a4e), QTC(0x5d03ed80), QTC(0x5d1e9e24), QTC(0x5d394c3b),
  QTC(0x5d53f7c7), QTC(0x5d6ea0c9), QTC(0x5d894742), QTC(0x5da3eb33), QTC(0x5dbe8c9e), QTC(0x5dd92b84), QTC(0x5df3c7e5), QTC(0x5e0e61c3),
  QTC(0x5e28f920), QTC(0x5e438dfc), QTC(0x5e5e2059), QTC(0x5e78b037), QTC(0x5e933d99), QTC(0x5eadc87e), QTC(0x5ec850e9), QTC(0x5ee2d6da),
  QTC(0x5efd5a53), QTC(0x5f17db54), QTC(0x5f3259e0), QTC(0x5f4cd5f6), QTC(0x5f674f99), QTC(0x5f81c6c8), QTC(0x5f9c3b87), QTC(0x5fb6add4),
  QTC(0x5fd11db3), QTC(0x5feb8b23), QTC(0x6005f626), QTC(0x60205ebd), QTC(0x603ac4e9), QTC(0x605528ac), QTC(0x606f8a05), QTC(0x6089e8f7),
  QTC(0x60a44583), QTC(0x60be9fa9), QTC(0x60d8f76b), QTC(0x60f34cca), QTC(0x610d9fc7), QTC(0x6127f062), QTC(0x61423e9e), QTC(0x615c8a7a),
  QTC(0x6176d3f9), QTC(0x61911b1b), QTC(0x61ab5fe1), QTC(0x61c5a24d), QTC(0x61dfe25f), QTC(0x61fa2018), QTC(0x62145b7a), QTC(0x622e9485),
  QTC(0x6248cb3b), QTC(0x6262ff9d), QTC(0x627d31ab), QTC(0x62976167), QTC(0x62b18ed1), QTC(0x62cbb9eb), QTC(0x62e5e2b6), QTC(0x63000933),
  QTC(0x631a2d62), QTC(0x63344f45), QTC(0x634e6edd), QTC(0x63688c2b), QTC(0x6382a730), QTC(0x639cbfec), QTC(0x63b6d661), QTC(0x63d0ea90),
  QTC(0x63eafc7a), QTC(0x64050c1f), QTC(0x641f1982), QTC(0x643924a2), QTC(0x64532d80), QTC(0x646d341f), QTC(0x6487387e), QTC(0x64a13a9e),
  QTC(0x64bb3a81), QTC(0x64d53828), QTC(0x64ef3393), QTC(0x65092cc4), QTC(0x652323bb), QTC(0x653d1879), QTC(0x65570b00), QTC(0x6570fb50),
  QTC(0x658ae96b), QTC(0x65a4d550), QTC(0x65bebf01), QTC(0x65d8a680), QTC(0x65f28bcc), QTC(0x660c6ee8), QTC(0x66264fd3), QTC(0x66402e8f),
  QTC(0x665a0b1c), QTC(0x6673e57d), QTC(0x668dbdb0), QTC(0x66a793b8), QTC(0x66c16795), QTC(0x66db3949), QTC(0x66f508d4), QTC(0x670ed636),
  QTC(0x6728a172), QTC(0x67426a87), QTC(0x675c3177), QTC(0x6775f643), QTC(0x678fb8eb), QTC(0x67a97971), QTC(0x67c337d5), QTC(0x67dcf418),
  QTC(0x67f6ae3b), QTC(0x6810663f), QTC(0x682a1c25), QTC(0x6843cfed), QTC(0x685d8199), QTC(0x68773129), QTC(0x6890de9f), QTC(0x68aa89fa),
  QTC(0x68c4333d), QTC(0x68ddda67), QTC(0x68f77f7a), QTC(0x69112277), QTC(0x692ac35e), QTC(0x69446230), QTC(0x695dfeee), QTC(0x6977999a),
  QTC(0x69913232), QTC(0x69aac8ba), QTC(0x69c45d31), QTC(0x69ddef98), QTC(0x69f77ff0), QTC(0x6a110e3a), QTC(0x6a2a9a77), QTC(0x6a4424a8),
  QTC(0x6a5daccc), QTC(0x6a7732e6), QTC(0x6a90b6f6), QTC(0x6aaa38fd), QTC(0x6ac3b8fb), QTC(0x6add36f2), QTC(0x6af6b2e2), QTC(0x6b102ccd),
  QTC(0x6b29a4b2), QTC(0x6b431a92), QTC(0x6b5c8e6f), QTC(0x6b76004a), QTC(0x6b8f7022), QTC(0x6ba8ddf9), QTC(0x6bc249d0), QTC(0x6bdbb3a7),
  QTC(0x6bf51b80), QTC(0x6c0e815a), QTC(0x6c27e537), QTC(0x6c414718), QTC(0x6c5aa6fd), QTC(0x6c7404e7), QTC(0x6c8d60d7), QTC(0x6ca6bace),
  QTC(0x6cc012cc), QTC(0x6cd968d2), QTC(0x6cf2bce1), QTC(0x6d0c0ef9), QTC(0x6d255f1d), QTC(0x6d3ead4b), QTC(0x6d57f985), QTC(0x6d7143cc),
  QTC(0x6d8a8c21), QTC(0x6da3d283), QTC(0x6dbd16f5), QTC(0x6dd65976), QTC(0x6def9a08), QTC(0x6e08d8ab), QTC(0x6e221560), QTC(0x6e3b5027),
  QTC(0x6e548902), QTC(0x6e6dbff1), QTC(0x6e86f4f5), QTC(0x6ea0280e), QTC(0x6eb9593e), QTC(0x6ed28885), QTC(0x6eebb5e3), QTC(0x6f04e15a),
  QTC(0x6f1e0aea), QTC(0x6f373294), QTC(0x6f505859), QTC(0x6f697c39), QTC(0x6f829e35), QTC(0x6f9bbe4e), QTC(0x6fb4dc85), QTC(0x6fcdf8d9),
  QTC(0x6fe7134d), QTC(0x70002be0), QTC(0x70194293), QTC(0x70325767), QTC(0x704b6a5d), QTC(0x70647b76), QTC(0x707d8ab1), QTC(0x70969811),
  QTC(0x70afa394), QTC(0x70c8ad3d), QTC(0x70e1b50c), QTC(0x70fabb01), QTC(0x7113bf1d), QTC(0x712cc161), QTC(0x7145c1ce), QTC(0x715ec064),
  QTC(0x7177bd24), QTC(0x7190b80f), QTC(0x71a9b124), QTC(0x71c2a866), QTC(0x71db9dd4), QTC(0x71f49170), QTC(0x720d8339), QTC(0x72267331),
  QTC(0x723f6159), QTC(0x72584db0), QTC(0x72713838), QTC(0x728a20f1), QTC(0x72a307db), QTC(0x72bbecf9), QTC(0x72d4d049), QTC(0x72edb1ce),
  QTC(0x73069187), QTC(0x731f6f75), QTC(0x73384b98), QTC(0x735125f3), QTC(0x7369fe84), QTC(0x7382d54d), QTC(0x739baa4e), QTC(0x73b47d89),
  QTC(0x73cd4efd), QTC(0x73e61eab), QTC(0x73feec94), QTC(0x7417b8b8), QTC(0x74308319), QTC(0x74494bb6), QTC(0x74621291), QTC(0x747ad7aa),
  QTC(0x74939b02), QTC(0x74ac5c98), QTC(0x74c51c6f), QTC(0x74ddda86), QTC(0x74f696de), QTC(0x750f5178), QTC(0x75280a54), QTC(0x7540c174),
  QTC(0x755976d7), QTC(0x75722a7e), QTC(0x758adc69), QTC(0x75a38c9b), QTC(0x75bc3b12), QTC(0x75d4e7cf), QTC(0x75ed92d4), QTC(0x76063c21),
  QTC(0x761ee3b6), QTC(0x76378994), QTC(0x76502dbc), QTC(0x7668d02e), QTC(0x768170eb), QTC(0x769a0ff3), QTC(0x76b2ad47), QTC(0x76cb48e7),
  QTC(0x76e3e2d5), QTC(0x76fc7b10), QTC(0x7715119a), QTC(0x772da673), QTC(0x7746399b), QTC(0x775ecb13), QTC(0x77775adc), QTC(0x778fe8f6),
  QTC(0x77a87561), QTC(0x77c1001f), QTC(0x77d98930), QTC(0x77f21095), QTC(0x780a964d), QTC(0x78231a5b), QTC(0x783b9cbd), QTC(0x78541d75),
  QTC(0x786c9c84), QTC(0x788519e9), QTC(0x789d95a6), QTC(0x78b60fbb), QTC(0x78ce8828), QTC(0x78e6feef), QTC(0x78ff740f), QTC(0x7917e78a),
  QTC(0x7930595f), QTC(0x7948c990), QTC(0x7961381d), QTC(0x7979a506), QTC(0x7992104c), QTC(0x79aa79f0), QTC(0x79c2e1f1), QTC(0x79db4852),
  QTC(0x79f3ad11), QTC(0x7a0c1031), QTC(0x7a2471b0), QTC(0x7a3cd191), QTC(0x7a552fd3), QTC(0x7a6d8c76), QTC(0x7a85e77d), QTC(0x7a9e40e6),
  QTC(0x7ab698b2), QTC(0x7aceeee3), QTC(0x7ae74378), QTC(0x7aff9673), QTC(0x7b17e7d2), QTC(0x7b303799), QTC(0x7b4885c5), QTC(0x7b60d259),
  QTC(0x7b791d55), QTC(0x7b9166b9), QTC(0x7ba9ae86), QTC(0x7bc1f4bc), QTC(0x7bda395c), QTC(0x7bf27c66), QTC(0x7c0abddb), QTC(0x7c22fdbb),
  QTC(0x7c3b3c07), QTC(0x7c5378c0), QTC(0x7c6bb3e5), QTC(0x7c83ed78), QTC(0x7c9c2579), QTC(0x7cb45be9), QTC(0x7ccc90c7), QTC(0x7ce4c414),
  QTC(0x7cfcf5d2), QTC(0x7d152600), QTC(0x7d2d549f), QTC(0x7d4581b0), QTC(0x7d5dad32), QTC(0x7d75d727), QTC(0x7d8dff8f), QTC(0x7da6266a),
  QTC(0x7dbe4bba), QTC(0x7dd66f7d), QTC(0x7dee91b6), QTC(0x7e06b264), QTC(0x7e1ed188), QTC(0x7e36ef22), QTC(0x7e4f0b34), QTC(0x7e6725bd),
  QTC(0x7e7f3ebd), QTC(0x7e975636), QTC(0x7eaf6c28), QTC(0x7ec78093), QTC(0x7edf9378), QTC(0x7ef7a4d7), QTC(0x7f0fb4b1), QTC(0x7f27c307),
  QTC(0x7f3fcfd8), QTC(0x7f57db25), QTC(0x7f6fe4ef), QTC(0x7f87ed36), QTC(0x7f9ff3fb), QTC(0x7fb7f93e), QTC(0x7fcffcff), QTC(0x7fe7ff40)
};

/*
  table of pow(2.0,0.25*q)/2.0, q[0..4)
*/
const FIXP_QTD FDKaacEnc_quantTableQ[4] = { QTC(0x40000000), QTC(0x4c1bf7ff), QTC(0x5a82797f), QTC(0x6ba27e7f) };

/*
  table of pow(2.0,0.75*e)/8.0, e[0..4)
*/
const FIXP_QTD FDKaacEnc_quantTableE[4] = { QTC(0x10000000), QTC(0x1ae89f99), QTC(0x2d413ccd), QTC(0x4c1bf828) };


/*
  table to count used number of bits
*/
const SHORT FDKaacEnc_sideInfoTabLong[MAX_SFB_LONG + 1] =
{
  0x0009, 0x0009, 0x0009, 0x0009, 0x0009, 0x0009, 0x0009, 0x0009,
  0x0009, 0x0009, 0x0009, 0x0009, 0x0009, 0x0009, 0x0009, 0x0009,
  0x0009, 0x0009, 0x0009, 0x0009, 0x0009, 0x0009, 0x0009, 0x0009,
  0x0009, 0x0009, 0x0009, 0x0009, 0x0009, 0x0009, 0x0009, 0x000e,
  0x000e, 0x000e, 0x000e, 0x000e, 0x000e, 0x000e, 0x000e, 0x000e,
  0x000e, 0x000e, 0x000e, 0x000e, 0x000e, 0x000e, 0x000e, 0x000e,
  0x000e, 0x000e, 0x000e, 0x000e
};


const SHORT FDKaacEnc_sideInfoTabShort[MAX_SFB_SHORT + 1] =
{
  0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x000a,
  0x000a, 0x000a, 0x000a, 0x000a, 0x000a, 0x000a, 0x000d, 0x000d
};






/*
  Psy Configuration constants
*/

const SFB_PARAM_LONG p_FDKaacEnc_8000_long_1024 = {
    40,
    { 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 16, 16, 16, 16, 16, 16, 16,
      20, 20, 20, 20, 24, 24, 24, 28, 28, 32, 36, 36, 40, 44, 48, 52, 56, 60, 64, 80 }
};
const SFB_PARAM_SHORT p_FDKaacEnc_8000_short_128 = {
    15,
    {  4,  4,  4,  4,  4,  4,  4,  8,  8,  8,  8, 12, 16, 20, 20 }
};

const SFB_PARAM_LONG p_FDKaacEnc_11025_long_1024 = {
    43,
    {  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8, 12, 12, 12, 12, 12, 12, 12, 12, 12,
      16, 16, 16, 16, 20, 20, 20, 24, 24, 28, 28, 32, 36, 40, 40, 44, 48, 52, 56, 60,
      64, 64, 64 }
};
const SFB_PARAM_SHORT p_FDKaacEnc_11025_short_128 = {
    15,
    {  4,  4,  4,  4,  4,  4,  4,  4,  8,  8, 12, 12, 16, 20, 20 }
};

const SFB_PARAM_LONG p_FDKaacEnc_12000_long_1024 = {
    43,
    {  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8, 12, 12, 12, 12, 12, 12, 12, 12, 12,
      16, 16, 16, 16, 20, 20, 20, 24, 24, 28, 28, 32, 36, 40, 40, 44, 48, 52, 56, 60,
      64, 64, 64 }
};
const SFB_PARAM_SHORT p_FDKaacEnc_12000_short_128 = {
    15,
    {  4,  4,  4,  4,  4,  4,  4,  4,  8,  8, 12, 12, 16, 20, 20 }
};

const SFB_PARAM_LONG p_FDKaacEnc_16000_long_1024 = {
    43,
    {  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8, 12, 12, 12, 12, 12, 12, 12, 12, 12,
      16, 16, 16, 16, 20, 20, 20, 24, 24, 28, 28, 32, 36, 40, 40, 44, 48, 52, 56, 60,
      64, 64, 64 }
};
const SFB_PARAM_SHORT p_FDKaacEnc_16000_short_128 = {
    15,
    {  4,  4,  4,  4,  4,  4,  4,  4,  8,  8, 12, 12, 16, 20, 20 }
};
const SFB_PARAM_LONG p_FDKaacEnc_22050_long_1024 = {
    47,
    {  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  8,  8,  8,  8,  8,  8,  8,  8,  8,
       8, 12, 12, 12, 12, 16, 16, 16, 20, 20, 24, 24, 28, 28, 32, 36, 36, 40, 44, 48,
      52, 52, 64, 64, 64, 64, 64 }
};
const SFB_PARAM_SHORT p_FDKaacEnc_22050_short_128 = {
    15,
    {  4,  4,  4,  4,  4,  4,  4,  8,  8,  8, 12, 12, 16, 16, 20 }
};
const SFB_PARAM_LONG p_FDKaacEnc_24000_long_1024 = {
    47,
    {  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  8,  8,  8,  8,  8,  8,  8,  8,  8,
       8, 12, 12, 12, 12, 16, 16, 16, 20, 20, 24, 24, 28, 28, 32, 36, 36, 40, 44, 48,
      52, 52, 64, 64, 64, 64, 64 }
};
const SFB_PARAM_SHORT p_FDKaacEnc_24000_short_128 = {
    15,
    {  4,  4,  4,  4,  4,  4,  4,  8,  8,  8, 12, 12, 16, 16, 20 }
};
const SFB_PARAM_LONG p_FDKaacEnc_32000_long_1024 = {
    51,
    {  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  8,  8,  8,  8,  8,  8,  8, 12, 12, 12,
      12, 16, 16, 20, 20, 24, 24, 28, 28, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32 }
};
const SFB_PARAM_SHORT p_FDKaacEnc_32000_short_128 = {
    14,
    {  4,  4,  4,  4,  4,  8,  8,  8, 12, 12, 12, 16, 16, 16 }
};
const SFB_PARAM_LONG p_FDKaacEnc_44100_long_1024 = {
    49,
    {  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  8,  8,  8,  8,  8,  8,  8, 12, 12, 12,
      12, 16, 16, 20, 20, 24, 24, 28, 28, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 96 }
};
const SFB_PARAM_SHORT p_FDKaacEnc_44100_short_128 = {
    14,
    {  4,  4,  4,  4,  4,  8,  8,  8, 12, 12, 12, 16, 16, 16 }
};
const SFB_PARAM_LONG p_FDKaacEnc_48000_long_1024 = {
    49,
    {  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  8,  8,  8,  8,  8,  8,  8, 12, 12, 12,
      12, 16, 16, 20, 20, 24, 24, 28, 28, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
      32, 32, 32, 32, 32, 32, 32, 32, 96 }
};
const SFB_PARAM_SHORT p_FDKaacEnc_48000_short_128 = {
    14,
    {  4,  4,  4,  4,  4,  8,  8,  8, 12, 12, 12, 16, 16, 16 }
};
const SFB_PARAM_LONG p_FDKaacEnc_64000_long_1024 = {
    47,
    {  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  8,  8,  8,  8, 12, 12,
      12, 16, 16, 16, 20, 24, 24, 28, 36, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
      40, 40, 40, 40, 40, 40, 40 }
};
const SFB_PARAM_SHORT p_FDKaacEnc_64000_short_128 = {
    12,
    {  4,  4,  4,  4,  4,  4,  8,  8,  8, 16, 28, 36 }
};
const SFB_PARAM_LONG p_FDKaacEnc_88200_long_1024 = {
    41,
    {  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  8,  8,  8,  8,  8, 12,
      12, 12, 12, 12, 16, 16, 24, 28, 36, 44, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64 }
};
const SFB_PARAM_SHORT p_FDKaacEnc_88200_short_128 = {
    12,
    {  4,  4,  4,  4,  4,  4,  8,  8,  8, 16, 28, 36 }
};
const SFB_PARAM_LONG p_FDKaacEnc_96000_long_1024 = {
    41,
    {  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  8,  8,  8,  8,  8, 12,
      12, 12, 12, 12, 16, 16, 24, 28, 36, 44, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64 }
};
const SFB_PARAM_SHORT p_FDKaacEnc_96000_short_128 = {
    12,
    {  4,  4,  4,  4,  4,  4,  8,  8,  8, 16, 28, 36 }
};


/*
  TNS filter coefficients
*/

/*
  3 bit resolution
*/
const FIXP_DBL FDKaacEnc_tnsEncCoeff3[8]=
{
  (FIXP_DBL)0x81f1d201, (FIXP_DBL)0x91261481, (FIXP_DBL)0xadb92301, (FIXP_DBL)0xd438af00, (FIXP_DBL)0x00000000, (FIXP_DBL)0x37898080, (FIXP_DBL)0x64130dff, (FIXP_DBL)0x7cca6fff
};
const FIXP_DBL FDKaacEnc_tnsCoeff3Borders[8]={
  (FIXP_DBL)0x80000001 /*-4*/,  (FIXP_DBL)0x87b826df /*-3*/,  (FIXP_DBL)0x9df24154 /*-2*/,  (FIXP_DBL)0xbfffffe5 /*-1*/,
  (FIXP_DBL)0xe9c5e578 /* 0*/,  (FIXP_DBL)0x1c7b90f0 /* 1*/,  (FIXP_DBL)0x4fce83a9 /* 2*/,  (FIXP_DBL)0x7352f2c3 /* 3*/
};

/*
  4 bit resolution
*/
const FIXP_DBL FDKaacEnc_tnsEncCoeff4[16]=
{
  (FIXP_DBL)0x808bc881, (FIXP_DBL)0x84e2e581, (FIXP_DBL)0x8d6b4a01, (FIXP_DBL)0x99da9201, (FIXP_DBL)0xa9c45701, (FIXP_DBL)0xbc9dde81, (FIXP_DBL)0xd1c2d500, (FIXP_DBL)0xe87ae540,
  (FIXP_DBL)0x00000000, (FIXP_DBL)0x1a9cd9c0, (FIXP_DBL)0x340ff240, (FIXP_DBL)0x4b3c8bff, (FIXP_DBL)0x5f1f5e7f, (FIXP_DBL)0x6ed9eb7f, (FIXP_DBL)0x79bc387f, (FIXP_DBL)0x7f4c7e7f
};
const FIXP_DBL FDKaacEnc_tnsCoeff4Borders[16]=
{
  (FIXP_DBL)0x80000001 /*-8*/,  (FIXP_DBL)0x822deff0 /*-7*/,  (FIXP_DBL)0x88a4bfe6 /*-6*/,  (FIXP_DBL)0x932c159d /*-5*/,
  (FIXP_DBL)0xa16827c2 /*-4*/,  (FIXP_DBL)0xb2dcde27 /*-3*/,  (FIXP_DBL)0xc6f20b91 /*-2*/,  (FIXP_DBL)0xdcf89c64 /*-1*/,
  (FIXP_DBL)0xf4308ce1 /* 0*/,  (FIXP_DBL)0x0d613054 /* 1*/,  (FIXP_DBL)0x278dde80 /* 2*/,  (FIXP_DBL)0x4000001b /* 3*/,
  (FIXP_DBL)0x55a6127b /* 4*/,  (FIXP_DBL)0x678dde8f /* 5*/,  (FIXP_DBL)0x74ef0ed7 /* 6*/,  (FIXP_DBL)0x7d33f0da /* 7*/
};
const FIXP_DBL FDKaacEnc_mTab_4_3Elc[512]={
  FL2FXCONST_DBL(0.3968502629920499),FL2FXCONST_DBL(0.3978840634868335),FL2FXCONST_DBL(0.3989185359354711),FL2FXCONST_DBL(0.3999536794661432),
  FL2FXCONST_DBL(0.4009894932098531),FL2FXCONST_DBL(0.4020259763004115),FL2FXCONST_DBL(0.4030631278744227),FL2FXCONST_DBL(0.4041009470712695),
  FL2FXCONST_DBL(0.4051394330330996),FL2FXCONST_DBL(0.4061785849048110),FL2FXCONST_DBL(0.4072184018340380),FL2FXCONST_DBL(0.4082588829711372),
  FL2FXCONST_DBL(0.4093000274691739),FL2FXCONST_DBL(0.4103418344839078),FL2FXCONST_DBL(0.4113843031737798),FL2FXCONST_DBL(0.4124274326998980),
  FL2FXCONST_DBL(0.4134712222260245),FL2FXCONST_DBL(0.4145156709185620),FL2FXCONST_DBL(0.4155607779465400),FL2FXCONST_DBL(0.4166065424816022),
  FL2FXCONST_DBL(0.4176529636979932),FL2FXCONST_DBL(0.4187000407725452),FL2FXCONST_DBL(0.4197477728846652),FL2FXCONST_DBL(0.4207961592163222),
  FL2FXCONST_DBL(0.4218451989520345),FL2FXCONST_DBL(0.4228948912788567),FL2FXCONST_DBL(0.4239452353863673),FL2FXCONST_DBL(0.4249962304666564),
  FL2FXCONST_DBL(0.4260478757143130),FL2FXCONST_DBL(0.4271001703264124),FL2FXCONST_DBL(0.4281531135025046),FL2FXCONST_DBL(0.4292067044446017),
  FL2FXCONST_DBL(0.4302609423571658),FL2FXCONST_DBL(0.4313158264470970),FL2FXCONST_DBL(0.4323713559237216),FL2FXCONST_DBL(0.4334275299987803),
  FL2FXCONST_DBL(0.4344843478864161),FL2FXCONST_DBL(0.4355418088031630),FL2FXCONST_DBL(0.4365999119679339),FL2FXCONST_DBL(0.4376586566020096),
  FL2FXCONST_DBL(0.4387180419290272),FL2FXCONST_DBL(0.4397780671749683),FL2FXCONST_DBL(0.4408387315681480),FL2FXCONST_DBL(0.4419000343392039),
  FL2FXCONST_DBL(0.4429619747210847),FL2FXCONST_DBL(0.4440245519490388),FL2FXCONST_DBL(0.4450877652606038),FL2FXCONST_DBL(0.4461516138955953),
  FL2FXCONST_DBL(0.4472160970960963),FL2FXCONST_DBL(0.4482812141064458),FL2FXCONST_DBL(0.4493469641732286),FL2FXCONST_DBL(0.4504133465452648),
  FL2FXCONST_DBL(0.4514803604735984),FL2FXCONST_DBL(0.4525480052114875),FL2FXCONST_DBL(0.4536162800143939),FL2FXCONST_DBL(0.4546851841399719),
  FL2FXCONST_DBL(0.4557547168480591),FL2FXCONST_DBL(0.4568248774006652),FL2FXCONST_DBL(0.4578956650619623),FL2FXCONST_DBL(0.4589670790982746),
  FL2FXCONST_DBL(0.4600391187780688),FL2FXCONST_DBL(0.4611117833719430),FL2FXCONST_DBL(0.4621850721526184),FL2FXCONST_DBL(0.4632589843949278),
  FL2FXCONST_DBL(0.4643335193758069),FL2FXCONST_DBL(0.4654086763742842),FL2FXCONST_DBL(0.4664844546714713),FL2FXCONST_DBL(0.4675608535505532),
  FL2FXCONST_DBL(0.4686378722967790),FL2FXCONST_DBL(0.4697155101974522),FL2FXCONST_DBL(0.4707937665419216),FL2FXCONST_DBL(0.4718726406215713),
  FL2FXCONST_DBL(0.4729521317298118),FL2FXCONST_DBL(0.4740322391620711),FL2FXCONST_DBL(0.4751129622157845),FL2FXCONST_DBL(0.4761943001903867),
  FL2FXCONST_DBL(0.4772762523873015),FL2FXCONST_DBL(0.4783588181099338),FL2FXCONST_DBL(0.4794419966636599),FL2FXCONST_DBL(0.4805257873558190),
  FL2FXCONST_DBL(0.4816101894957042),FL2FXCONST_DBL(0.4826952023945537),FL2FXCONST_DBL(0.4837808253655421),FL2FXCONST_DBL(0.4848670577237714),
  FL2FXCONST_DBL(0.4859538987862632),FL2FXCONST_DBL(0.4870413478719488),FL2FXCONST_DBL(0.4881294043016621),FL2FXCONST_DBL(0.4892180673981298),
  FL2FXCONST_DBL(0.4903073364859640),FL2FXCONST_DBL(0.4913972108916533),FL2FXCONST_DBL(0.4924876899435545),FL2FXCONST_DBL(0.4935787729718844),
  FL2FXCONST_DBL(0.4946704593087116),FL2FXCONST_DBL(0.4957627482879484),FL2FXCONST_DBL(0.4968556392453423),FL2FXCONST_DBL(0.4979491315184684),
  FL2FXCONST_DBL(0.4990432244467211),FL2FXCONST_DBL(0.5001379173713062),FL2FXCONST_DBL(0.5012332096352328),FL2FXCONST_DBL(0.5023291005833056),
  FL2FXCONST_DBL(0.5034255895621171),FL2FXCONST_DBL(0.5045226759200399),FL2FXCONST_DBL(0.5056203590072181),FL2FXCONST_DBL(0.5067186381755611),
  FL2FXCONST_DBL(0.5078175127787346),FL2FXCONST_DBL(0.5089169821721536),FL2FXCONST_DBL(0.5100170457129749),FL2FXCONST_DBL(0.5111177027600893),
  FL2FXCONST_DBL(0.5122189526741143),FL2FXCONST_DBL(0.5133207948173868),FL2FXCONST_DBL(0.5144232285539552),FL2FXCONST_DBL(0.5155262532495726),
  FL2FXCONST_DBL(0.5166298682716894),FL2FXCONST_DBL(0.5177340729894460),FL2FXCONST_DBL(0.5188388667736652),FL2FXCONST_DBL(0.5199442489968457),
  FL2FXCONST_DBL(0.5210502190331544),FL2FXCONST_DBL(0.5221567762584198),FL2FXCONST_DBL(0.5232639200501247),FL2FXCONST_DBL(0.5243716497873989),
  FL2FXCONST_DBL(0.5254799648510130),FL2FXCONST_DBL(0.5265888646233705),FL2FXCONST_DBL(0.5276983484885021),FL2FXCONST_DBL(0.5288084158320574),
  FL2FXCONST_DBL(0.5299190660412995),FL2FXCONST_DBL(0.5310302985050975),FL2FXCONST_DBL(0.5321421126139198),FL2FXCONST_DBL(0.5332545077598274),
  FL2FXCONST_DBL(0.5343674833364678),FL2FXCONST_DBL(0.5354810387390675),FL2FXCONST_DBL(0.5365951733644262),FL2FXCONST_DBL(0.5377098866109097),
  FL2FXCONST_DBL(0.5388251778784438),FL2FXCONST_DBL(0.5399410465685075),FL2FXCONST_DBL(0.5410574920841272),FL2FXCONST_DBL(0.5421745138298695),
  FL2FXCONST_DBL(0.5432921112118353),FL2FXCONST_DBL(0.5444102836376534),FL2FXCONST_DBL(0.5455290305164744),FL2FXCONST_DBL(0.5466483512589642),
  FL2FXCONST_DBL(0.5477682452772976),FL2FXCONST_DBL(0.5488887119851529),FL2FXCONST_DBL(0.5500097507977050),FL2FXCONST_DBL(0.5511313611316194),
  FL2FXCONST_DBL(0.5522535424050467),FL2FXCONST_DBL(0.5533762940376158),FL2FXCONST_DBL(0.5544996154504284),FL2FXCONST_DBL(0.5556235060660528),
  FL2FXCONST_DBL(0.5567479653085183),FL2FXCONST_DBL(0.5578729926033087),FL2FXCONST_DBL(0.5589985873773569),FL2FXCONST_DBL(0.5601247490590389),
  FL2FXCONST_DBL(0.5612514770781683),FL2FXCONST_DBL(0.5623787708659898),FL2FXCONST_DBL(0.5635066298551742),FL2FXCONST_DBL(0.5646350534798125),
  FL2FXCONST_DBL(0.5657640411754097),FL2FXCONST_DBL(0.5668935923788799),FL2FXCONST_DBL(0.5680237065285404),FL2FXCONST_DBL(0.5691543830641059),
  FL2FXCONST_DBL(0.5702856214266832),FL2FXCONST_DBL(0.5714174210587655),FL2FXCONST_DBL(0.5725497814042271),FL2FXCONST_DBL(0.5736827019083177),
  FL2FXCONST_DBL(0.5748161820176573),FL2FXCONST_DBL(0.5759502211802304),FL2FXCONST_DBL(0.5770848188453810),FL2FXCONST_DBL(0.5782199744638067),
  FL2FXCONST_DBL(0.5793556874875542),FL2FXCONST_DBL(0.5804919573700131),FL2FXCONST_DBL(0.5816287835659116),FL2FXCONST_DBL(0.5827661655313104),
  FL2FXCONST_DBL(0.5839041027235979),FL2FXCONST_DBL(0.5850425946014850),FL2FXCONST_DBL(0.5861816406250000),FL2FXCONST_DBL(0.5873212402554834),
  FL2FXCONST_DBL(0.5884613929555826),FL2FXCONST_DBL(0.5896020981892474),FL2FXCONST_DBL(0.5907433554217242),FL2FXCONST_DBL(0.5918851641195517),
  FL2FXCONST_DBL(0.5930275237505556),FL2FXCONST_DBL(0.5941704337838434),FL2FXCONST_DBL(0.5953138936897999),FL2FXCONST_DBL(0.5964579029400819),
  FL2FXCONST_DBL(0.5976024610076139),FL2FXCONST_DBL(0.5987475673665825),FL2FXCONST_DBL(0.5998932214924321),FL2FXCONST_DBL(0.6010394228618597),
  FL2FXCONST_DBL(0.6021861709528106),FL2FXCONST_DBL(0.6033334652444733),FL2FXCONST_DBL(0.6044813052172748),FL2FXCONST_DBL(0.6056296903528761),
  FL2FXCONST_DBL(0.6067786201341671),FL2FXCONST_DBL(0.6079280940452625),FL2FXCONST_DBL(0.6090781115714966),FL2FXCONST_DBL(0.6102286721994192),
  FL2FXCONST_DBL(0.6113797754167908),FL2FXCONST_DBL(0.6125314207125777),FL2FXCONST_DBL(0.6136836075769482),FL2FXCONST_DBL(0.6148363355012674),
  FL2FXCONST_DBL(0.6159896039780929),FL2FXCONST_DBL(0.6171434125011708),FL2FXCONST_DBL(0.6182977605654305),FL2FXCONST_DBL(0.6194526476669808),
  FL2FXCONST_DBL(0.6206080733031054),FL2FXCONST_DBL(0.6217640369722584),FL2FXCONST_DBL(0.6229205381740598),FL2FXCONST_DBL(0.6240775764092919),
  FL2FXCONST_DBL(0.6252351511798939),FL2FXCONST_DBL(0.6263932619889586),FL2FXCONST_DBL(0.6275519083407275),FL2FXCONST_DBL(0.6287110897405869),
  FL2FXCONST_DBL(0.6298708056950635),FL2FXCONST_DBL(0.6310310557118203),FL2FXCONST_DBL(0.6321918392996523),FL2FXCONST_DBL(0.6333531559684823),
  FL2FXCONST_DBL(0.6345150052293571),FL2FXCONST_DBL(0.6356773865944432),FL2FXCONST_DBL(0.6368402995770224),FL2FXCONST_DBL(0.6380037436914881),
  FL2FXCONST_DBL(0.6391677184533411),FL2FXCONST_DBL(0.6403322233791856),FL2FXCONST_DBL(0.6414972579867254),FL2FXCONST_DBL(0.6426628217947594),
  FL2FXCONST_DBL(0.6438289143231779),FL2FXCONST_DBL(0.6449955350929588),FL2FXCONST_DBL(0.6461626836261636),FL2FXCONST_DBL(0.6473303594459330),
  FL2FXCONST_DBL(0.6484985620764839),FL2FXCONST_DBL(0.6496672910431047),FL2FXCONST_DBL(0.6508365458721518),FL2FXCONST_DBL(0.6520063260910459),
  FL2FXCONST_DBL(0.6531766312282679),FL2FXCONST_DBL(0.6543474608133552),FL2FXCONST_DBL(0.6555188143768979),FL2FXCONST_DBL(0.6566906914505349),
  FL2FXCONST_DBL(0.6578630915669509),FL2FXCONST_DBL(0.6590360142598715),FL2FXCONST_DBL(0.6602094590640603),FL2FXCONST_DBL(0.6613834255153149),
  FL2FXCONST_DBL(0.6625579131504635),FL2FXCONST_DBL(0.6637329215073610),FL2FXCONST_DBL(0.6649084501248851),FL2FXCONST_DBL(0.6660844985429335),
  FL2FXCONST_DBL(0.6672610663024197),FL2FXCONST_DBL(0.6684381529452691),FL2FXCONST_DBL(0.6696157580144163),FL2FXCONST_DBL(0.6707938810538011),
  FL2FXCONST_DBL(0.6719725216083646),FL2FXCONST_DBL(0.6731516792240465),FL2FXCONST_DBL(0.6743313534477807),FL2FXCONST_DBL(0.6755115438274927),
  FL2FXCONST_DBL(0.6766922499120955),FL2FXCONST_DBL(0.6778734712514865),FL2FXCONST_DBL(0.6790552073965435),FL2FXCONST_DBL(0.6802374578991223),
  FL2FXCONST_DBL(0.6814202223120524),FL2FXCONST_DBL(0.6826035001891340),FL2FXCONST_DBL(0.6837872910851345),FL2FXCONST_DBL(0.6849715945557853),
  FL2FXCONST_DBL(0.6861564101577784),FL2FXCONST_DBL(0.6873417374487629),FL2FXCONST_DBL(0.6885275759873420),FL2FXCONST_DBL(0.6897139253330697),
  FL2FXCONST_DBL(0.6909007850464473),FL2FXCONST_DBL(0.6920881546889198),FL2FXCONST_DBL(0.6932760338228737),FL2FXCONST_DBL(0.6944644220116332),
  FL2FXCONST_DBL(0.6956533188194565),FL2FXCONST_DBL(0.6968427238115332),FL2FXCONST_DBL(0.6980326365539813),FL2FXCONST_DBL(0.6992230566138435),
  FL2FXCONST_DBL(0.7004139835590845),FL2FXCONST_DBL(0.7016054169585869),FL2FXCONST_DBL(0.7027973563821499),FL2FXCONST_DBL(0.7039898014004843),
  FL2FXCONST_DBL(0.7051827515852106),FL2FXCONST_DBL(0.7063762065088554),FL2FXCONST_DBL(0.7075701657448483),FL2FXCONST_DBL(0.7087646288675196),
  FL2FXCONST_DBL(0.7099595954520960),FL2FXCONST_DBL(0.7111550650746988),FL2FXCONST_DBL(0.7123510373123402),FL2FXCONST_DBL(0.7135475117429202),
  FL2FXCONST_DBL(0.7147444879452244),FL2FXCONST_DBL(0.7159419654989200),FL2FXCONST_DBL(0.7171399439845538),FL2FXCONST_DBL(0.7183384229835486),
  FL2FXCONST_DBL(0.7195374020782005),FL2FXCONST_DBL(0.7207368808516762),FL2FXCONST_DBL(0.7219368588880097),FL2FXCONST_DBL(0.7231373357720997),
  FL2FXCONST_DBL(0.7243383110897066),FL2FXCONST_DBL(0.7255397844274496),FL2FXCONST_DBL(0.7267417553728043),FL2FXCONST_DBL(0.7279442235140992),
  FL2FXCONST_DBL(0.7291471884405130),FL2FXCONST_DBL(0.7303506497420724),FL2FXCONST_DBL(0.7315546070096487),FL2FXCONST_DBL(0.7327590598349553),
  FL2FXCONST_DBL(0.7339640078105445),FL2FXCONST_DBL(0.7351694505298055),FL2FXCONST_DBL(0.7363753875869610),FL2FXCONST_DBL(0.7375818185770647),
  FL2FXCONST_DBL(0.7387887430959987),FL2FXCONST_DBL(0.7399961607404706),FL2FXCONST_DBL(0.7412040711080108),FL2FXCONST_DBL(0.7424124737969701),
  FL2FXCONST_DBL(0.7436213684065166),FL2FXCONST_DBL(0.7448307545366334),FL2FXCONST_DBL(0.7460406317881158),FL2FXCONST_DBL(0.7472509997625686),
  FL2FXCONST_DBL(0.7484618580624036),FL2FXCONST_DBL(0.7496732062908372),FL2FXCONST_DBL(0.7508850440518872),FL2FXCONST_DBL(0.7520973709503704),
  FL2FXCONST_DBL(0.7533101865919009),FL2FXCONST_DBL(0.7545234905828862),FL2FXCONST_DBL(0.7557372825305252),FL2FXCONST_DBL(0.7569515620428062),
  FL2FXCONST_DBL(0.7581663287285035),FL2FXCONST_DBL(0.7593815821971756),FL2FXCONST_DBL(0.7605973220591619),FL2FXCONST_DBL(0.7618135479255810),
  FL2FXCONST_DBL(0.7630302594083277),FL2FXCONST_DBL(0.7642474561200708),FL2FXCONST_DBL(0.7654651376742505),FL2FXCONST_DBL(0.7666833036850760),
  FL2FXCONST_DBL(0.7679019537675227),FL2FXCONST_DBL(0.7691210875373307),FL2FXCONST_DBL(0.7703407046110011),FL2FXCONST_DBL(0.7715608046057948),
  FL2FXCONST_DBL(0.7727813871397293),FL2FXCONST_DBL(0.7740024518315765),FL2FXCONST_DBL(0.7752239983008605),FL2FXCONST_DBL(0.7764460261678551),
  FL2FXCONST_DBL(0.7776685350535814),FL2FXCONST_DBL(0.7788915245798054),FL2FXCONST_DBL(0.7801149943690360),FL2FXCONST_DBL(0.7813389440445223),
  FL2FXCONST_DBL(0.7825633732302513),FL2FXCONST_DBL(0.7837882815509458),FL2FXCONST_DBL(0.7850136686320621),FL2FXCONST_DBL(0.7862395340997874),
  FL2FXCONST_DBL(0.7874658775810378),FL2FXCONST_DBL(0.7886926987034559),FL2FXCONST_DBL(0.7899199970954088),FL2FXCONST_DBL(0.7911477723859853),
  FL2FXCONST_DBL(0.7923760242049944),FL2FXCONST_DBL(0.7936047521829623),FL2FXCONST_DBL(0.7948339559511308),FL2FXCONST_DBL(0.7960636351414546),
  FL2FXCONST_DBL(0.7972937893865995),FL2FXCONST_DBL(0.7985244183199399),FL2FXCONST_DBL(0.7997555215755570),FL2FXCONST_DBL(0.8009870987882359),
  FL2FXCONST_DBL(0.8022191495934644),FL2FXCONST_DBL(0.8034516736274301),FL2FXCONST_DBL(0.8046846705270185),FL2FXCONST_DBL(0.8059181399298110),
  FL2FXCONST_DBL(0.8071520814740822),FL2FXCONST_DBL(0.8083864947987989),FL2FXCONST_DBL(0.8096213795436166),FL2FXCONST_DBL(0.8108567353488784),
  FL2FXCONST_DBL(0.8120925618556127),FL2FXCONST_DBL(0.8133288587055308),FL2FXCONST_DBL(0.8145656255410253),FL2FXCONST_DBL(0.8158028620051674),
  FL2FXCONST_DBL(0.8170405677417053),FL2FXCONST_DBL(0.8182787423950622),FL2FXCONST_DBL(0.8195173856103341),FL2FXCONST_DBL(0.8207564970332875),
  FL2FXCONST_DBL(0.8219960763103580),FL2FXCONST_DBL(0.8232361230886477),FL2FXCONST_DBL(0.8244766370159234),FL2FXCONST_DBL(0.8257176177406150),
  FL2FXCONST_DBL(0.8269590649118125),FL2FXCONST_DBL(0.8282009781792650),FL2FXCONST_DBL(0.8294433571933784),FL2FXCONST_DBL(0.8306862016052132),
  FL2FXCONST_DBL(0.8319295110664831),FL2FXCONST_DBL(0.8331732852295520),FL2FXCONST_DBL(0.8344175237474336),FL2FXCONST_DBL(0.8356622262737878),
  FL2FXCONST_DBL(0.8369073924629202),FL2FXCONST_DBL(0.8381530219697793),FL2FXCONST_DBL(0.8393991144499545),FL2FXCONST_DBL(0.8406456695596752),
  FL2FXCONST_DBL(0.8418926869558079),FL2FXCONST_DBL(0.8431401662958544),FL2FXCONST_DBL(0.8443881072379507),FL2FXCONST_DBL(0.8456365094408642),
  FL2FXCONST_DBL(0.8468853725639923),FL2FXCONST_DBL(0.8481346962673606),FL2FXCONST_DBL(0.8493844802116208),FL2FXCONST_DBL(0.8506347240580492),
  FL2FXCONST_DBL(0.8518854274685442),FL2FXCONST_DBL(0.8531365901056253),FL2FXCONST_DBL(0.8543882116324307),FL2FXCONST_DBL(0.8556402917127157),
  FL2FXCONST_DBL(0.8568928300108512),FL2FXCONST_DBL(0.8581458261918209),FL2FXCONST_DBL(0.8593992799212207),FL2FXCONST_DBL(0.8606531908652563),
  FL2FXCONST_DBL(0.8619075586907414),FL2FXCONST_DBL(0.8631623830650962),FL2FXCONST_DBL(0.8644176636563452),FL2FXCONST_DBL(0.8656734001331161),
  FL2FXCONST_DBL(0.8669295921646375),FL2FXCONST_DBL(0.8681862394207371),FL2FXCONST_DBL(0.8694433415718407),FL2FXCONST_DBL(0.8707008982889695),
  FL2FXCONST_DBL(0.8719589092437391),FL2FXCONST_DBL(0.8732173741083574),FL2FXCONST_DBL(0.8744762925556232),FL2FXCONST_DBL(0.8757356642589241),
  FL2FXCONST_DBL(0.8769954888922352),FL2FXCONST_DBL(0.8782557661301171),FL2FXCONST_DBL(0.8795164956477146),FL2FXCONST_DBL(0.8807776771207545),
  FL2FXCONST_DBL(0.8820393102255443),FL2FXCONST_DBL(0.8833013946389704),FL2FXCONST_DBL(0.8845639300384969),FL2FXCONST_DBL(0.8858269161021629),
  FL2FXCONST_DBL(0.8870903525085819),FL2FXCONST_DBL(0.8883542389369399),FL2FXCONST_DBL(0.8896185750669933),FL2FXCONST_DBL(0.8908833605790678),
  FL2FXCONST_DBL(0.8921485951540565),FL2FXCONST_DBL(0.8934142784734187),FL2FXCONST_DBL(0.8946804102191776),FL2FXCONST_DBL(0.8959469900739191),
  FL2FXCONST_DBL(0.8972140177207906),FL2FXCONST_DBL(0.8984814928434985),FL2FXCONST_DBL(0.8997494151263077),FL2FXCONST_DBL(0.9010177842540390),
  FL2FXCONST_DBL(0.9022865999120682),FL2FXCONST_DBL(0.9035558617863242),FL2FXCONST_DBL(0.9048255695632878),FL2FXCONST_DBL(0.9060957229299895),
  FL2FXCONST_DBL(0.9073663215740092),FL2FXCONST_DBL(0.9086373651834729),FL2FXCONST_DBL(0.9099088534470528),FL2FXCONST_DBL(0.9111807860539647),
  FL2FXCONST_DBL(0.9124531626939672),FL2FXCONST_DBL(0.9137259830573594),FL2FXCONST_DBL(0.9149992468349805),FL2FXCONST_DBL(0.9162729537182071),
  FL2FXCONST_DBL(0.9175471033989524),FL2FXCONST_DBL(0.9188216955696648),FL2FXCONST_DBL(0.9200967299233258),FL2FXCONST_DBL(0.9213722061534494),
  FL2FXCONST_DBL(0.9226481239540795),FL2FXCONST_DBL(0.9239244830197896),FL2FXCONST_DBL(0.9252012830456805),FL2FXCONST_DBL(0.9264785237273793),
  FL2FXCONST_DBL(0.9277562047610376),FL2FXCONST_DBL(0.9290343258433305),FL2FXCONST_DBL(0.9303128866714547),FL2FXCONST_DBL(0.9315918869431275),
  FL2FXCONST_DBL(0.9328713263565848),FL2FXCONST_DBL(0.9341512046105802),FL2FXCONST_DBL(0.9354315214043836),FL2FXCONST_DBL(0.9367122764377792),
  FL2FXCONST_DBL(0.9379934694110648),FL2FXCONST_DBL(0.9392751000250497),FL2FXCONST_DBL(0.9405571679810542),FL2FXCONST_DBL(0.9418396729809072),
  FL2FXCONST_DBL(0.9431226147269456),FL2FXCONST_DBL(0.9444059929220124),FL2FXCONST_DBL(0.9456898072694558),FL2FXCONST_DBL(0.9469740574731275),
  FL2FXCONST_DBL(0.9482587432373810),FL2FXCONST_DBL(0.9495438642670713),FL2FXCONST_DBL(0.9508294202675522),FL2FXCONST_DBL(0.9521154109446763),
  FL2FXCONST_DBL(0.9534018360047926),FL2FXCONST_DBL(0.9546886951547455),FL2FXCONST_DBL(0.9559759881018738),FL2FXCONST_DBL(0.9572637145540087),
  FL2FXCONST_DBL(0.9585518742194732),FL2FXCONST_DBL(0.9598404668070802),FL2FXCONST_DBL(0.9611294920261317),FL2FXCONST_DBL(0.9624189495864168),
  FL2FXCONST_DBL(0.9637088391982110),FL2FXCONST_DBL(0.9649991605722750),FL2FXCONST_DBL(0.9662899134198524),FL2FXCONST_DBL(0.9675810974526697),
  FL2FXCONST_DBL(0.9688727123829343),FL2FXCONST_DBL(0.9701647579233330),FL2FXCONST_DBL(0.9714572337870316),FL2FXCONST_DBL(0.9727501396876727),
  FL2FXCONST_DBL(0.9740434753393749),FL2FXCONST_DBL(0.9753372404567313),FL2FXCONST_DBL(0.9766314347548087),FL2FXCONST_DBL(0.9779260579491460),
  FL2FXCONST_DBL(0.9792211097557527),FL2FXCONST_DBL(0.9805165898911081),FL2FXCONST_DBL(0.9818124980721600),FL2FXCONST_DBL(0.9831088340163232),
  FL2FXCONST_DBL(0.9844055974414786),FL2FXCONST_DBL(0.9857027880659716),FL2FXCONST_DBL(0.9870004056086111),FL2FXCONST_DBL(0.9882984497886684),
  FL2FXCONST_DBL(0.9895969203258759),FL2FXCONST_DBL(0.9908958169404255),FL2FXCONST_DBL(0.9921951393529680),FL2FXCONST_DBL(0.9934948872846116),
  FL2FXCONST_DBL(0.9947950604569206),FL2FXCONST_DBL(0.9960956585919144),FL2FXCONST_DBL(0.9973966814120665),FL2FXCONST_DBL(0.9986981286403025)
};

const FIXP_DBL FDKaacEnc_specExpMantTableCombElc[4][14] =
{
  {FL2FXCONST_DBL(0.5000000000000000), FL2FXCONST_DBL(0.6299605249474366), FL2FXCONST_DBL(0.7937005259840998), FL2FXCONST_DBL(0.5000000000000000),
   FL2FXCONST_DBL(0.6299605249474366), FL2FXCONST_DBL(0.7937005259840998), FL2FXCONST_DBL(0.5000000000000000), FL2FXCONST_DBL(0.6299605249474366),
   FL2FXCONST_DBL(0.7937005259840998), FL2FXCONST_DBL(0.5000000000000000), FL2FXCONST_DBL(0.6299605249474366), FL2FXCONST_DBL(0.7937005259840998),
   FL2FXCONST_DBL(0.5000000000000000), FL2FXCONST_DBL(0.6299605249474366)},

  {FL2FXCONST_DBL(0.5946035575013605), FL2FXCONST_DBL(0.7491535384383408), FL2FXCONST_DBL(0.9438743126816935), FL2FXCONST_DBL(0.5946035575013605),
   FL2FXCONST_DBL(0.7491535384383408), FL2FXCONST_DBL(0.9438743126816935), FL2FXCONST_DBL(0.5946035575013605), FL2FXCONST_DBL(0.7491535384383408),
   FL2FXCONST_DBL(0.9438743126816935), FL2FXCONST_DBL(0.5946035575013605), FL2FXCONST_DBL(0.7491535384383408), FL2FXCONST_DBL(0.9438743126816935),
   FL2FXCONST_DBL(0.5946035575013605), FL2FXCONST_DBL(0.7491535384383408)},

  {FL2FXCONST_DBL(0.7071067811865476), FL2FXCONST_DBL(0.8908987181403393), FL2FXCONST_DBL(0.5612310241546865), FL2FXCONST_DBL(0.7071067811865476),
   FL2FXCONST_DBL(0.8908987181403393), FL2FXCONST_DBL(0.5612310241546865), FL2FXCONST_DBL(0.7071067811865476), FL2FXCONST_DBL(0.8908987181403393),
   FL2FXCONST_DBL(0.5612310241546865), FL2FXCONST_DBL(0.7071067811865476), FL2FXCONST_DBL(0.8908987181403393), FL2FXCONST_DBL(0.5612310241546865),
   FL2FXCONST_DBL(0.7071067811865476), FL2FXCONST_DBL(0.8908987181403393)},

  {FL2FXCONST_DBL(0.8408964152537145), FL2FXCONST_DBL(0.5297315471796477), FL2FXCONST_DBL(0.6674199270850172), FL2FXCONST_DBL(0.8408964152537145),
   FL2FXCONST_DBL(0.5297315471796477), FL2FXCONST_DBL(0.6674199270850172), FL2FXCONST_DBL(0.8408964152537145), FL2FXCONST_DBL(0.5297315471796477),
   FL2FXCONST_DBL(0.6674199270850172), FL2FXCONST_DBL(0.8408964152537145), FL2FXCONST_DBL(0.5297315471796477), FL2FXCONST_DBL(0.6674199270850172),
   FL2FXCONST_DBL(0.8408964152537145), FL2FXCONST_DBL(0.5297315471796477)}
};

const UCHAR FDKaacEnc_specExpTableComb[4][14] =
{
  {1, 2, 3, 5, 6, 7, 9, 10, 11, 13, 14, 15, 17, 18},
  {1, 2, 3, 5, 6, 7, 9, 10, 11, 13, 14, 15, 17, 18},
  {1, 2, 4, 5, 6, 8, 9, 10, 12, 13, 14, 16, 17, 18},
  {1, 3, 4, 5, 7, 8, 9, 11, 12, 13, 15, 16, 17, 19}
};


#define WTS0 1
#define WTS1 0
#define WTS2 -2

const FIXP_WTB ELDAnalysis512[1536] = {
  /* part 0 */
  WTC0(0xfac5a770), WTC0(0xfaafbab8), WTC0(0xfa996a40), WTC0(0xfa82bbd0), WTC0(0xfa6bb538), WTC0(0xfa545c38), WTC0(0xfa3cb698), WTC0(0xfa24ca28), 
  WTC0(0xfa0c9ca8), WTC0(0xf9f433e8), WTC0(0xf9db9580), WTC0(0xf9c2c298), WTC0(0xf9a9b800), WTC0(0xf9907250), WTC0(0xf976ee38), WTC0(0xf95d2b88), 
  WTC0(0xf9432d10), WTC0(0xf928f5c0), WTC0(0xf90e8868), WTC0(0xf8f3e400), WTC0(0xf8d903a0), WTC0(0xf8bde238), WTC0(0xf8a27af0), WTC0(0xf886cde8), 
  WTC0(0xf86ae020), WTC0(0xf84eb6c0), WTC0(0xf83256f8), WTC0(0xf815c4b8), WTC0(0xf7f902c0), WTC0(0xf7dc13b0), WTC0(0xf7befa60), WTC0(0xf7a1ba40), 
  WTC0(0xf78457c0), WTC0(0xf766d780), WTC0(0xf7493d90), WTC0(0xf72b8990), WTC0(0xf70db5f0), WTC0(0xf6efbd30), WTC0(0xf6d19a20), WTC0(0xf6b352e0), 
  WTC0(0xf694f8c0), WTC0(0xf6769da0), WTC0(0xf6585310), WTC0(0xf63a28d0), WTC0(0xf61c2c60), WTC0(0xf5fe6b10), WTC0(0xf5e0f250), WTC0(0xf5c3ceb0), 
  WTC0(0xf5a70be0), WTC0(0xf58ab5a0), WTC0(0xf56ed7b0), WTC0(0xf5537e40), WTC0(0xf538b610), WTC0(0xf51e8bf0), WTC0(0xf5050c90), WTC0(0xf4ec4330), 
  WTC0(0xf4d439b0), WTC0(0xf4bcf9b0), WTC0(0xf4a68ce0), WTC0(0xf490fa80), WTC0(0xf47c4760), WTC0(0xf4687830), WTC0(0xf4558f00), WTC0(0xf4434fc0), 
  WTC0(0xf4314070), WTC0(0xf41ee450), WTC0(0xf40bc130), WTC0(0xf3f799c0), WTC0(0xf3e26d30), WTC0(0xf3cc3d70), WTC0(0xf3b50c80), WTC0(0xf39cdd60), 
  WTC0(0xf383b440), WTC0(0xf3699550), WTC0(0xf34e84c0), WTC0(0xf33286b0), WTC0(0xf3159f10), WTC0(0xf2f7d1b0), WTC0(0xf2d92290), WTC0(0xf2b994d0), 
  WTC0(0xf2992ad0), WTC0(0xf277e6d0), WTC0(0xf255cb60), WTC0(0xf232dd00), WTC0(0xf20f2240), WTC0(0xf1eaa1d0), WTC0(0xf1c56240), WTC0(0xf19f63d0), 
  WTC0(0xf178a0f0), WTC0(0xf15113a0), WTC0(0xf128b5c0), WTC0(0xf0ff7fd0), WTC0(0xf0d56860), WTC0(0xf0aa6610), WTC0(0xf07e6fd0), WTC0(0xf0518190), 
  WTC0(0xf0239cd0), WTC0(0xeff4c320), WTC0(0xefc4f720), WTC0(0xef945080), WTC0(0xef62fce0), WTC0(0xef312a40), WTC0(0xeeff05c0), WTC0(0xeecca2c0), 
  WTC0(0xee99faa0), WTC0(0xee6705a0), WTC0(0xee33bb60), WTC0(0xee000060), WTC0(0xedcba660), WTC0(0xed967e80), WTC0(0xed605b80), WTC0(0xed293b40), 
  WTC0(0xecf146a0), WTC0(0xecb8a8a0), WTC0(0xec7f8bc0), WTC0(0xec461260), WTC0(0xec0c5720), WTC0(0xebd27440), WTC0(0xeb988220), WTC0(0xeb5e7040), 
  WTC0(0xeb2404c0), WTC0(0xeae90440), WTC0(0xeaad33c0), WTC0(0xea7066c0), WTC0(0xea327f60), WTC0(0xe9f36000), WTC0(0xe9b2ed60), WTC0(0xe9713920), 
  WTC0(0xe92e81e0), WTC0(0xe8eb08c0), WTC0(0xe8a70e60), WTC0(0xe862d8e0), WTC0(0xe81eb340), WTC0(0xe7dae8a0), WTC0(0xe797c1a0), WTC0(0xe7554ca0), 
  WTC0(0xe7135dc0), WTC0(0xe6d1c6a0), WTC0(0xe6905720), WTC0(0xe64eb9c0), WTC0(0xe60c7300), WTC0(0xe5c90600), WTC0(0xe583f920), WTC0(0xe53d1ce0), 
  WTC0(0xe4f48c80), WTC0(0xe4aa6640), WTC0(0xe45ecaa0), WTC0(0xe4120be0), WTC0(0xe3c4ae60), WTC0(0xe3773860), WTC0(0xe32a2ea0), WTC0(0xe2ddeea0), 
  WTC0(0xe292af00), WTC0(0xe248a4a0), WTC0(0xe2000140), WTC0(0xe1b8b640), WTC0(0xe1727440), WTC0(0xe12ce900), WTC0(0xe0e7c280), WTC0(0xe0a2b420), 
  WTC0(0xe05d76c0), WTC0(0xe017c360), WTC0(0xdfd15440), WTC0(0xdf8a0540), WTC0(0xdf41d300), WTC0(0xdef8bb40), WTC0(0xdeaebd40), WTC0(0xde63e7c0), 
  WTC0(0xde185940), WTC0(0xddcc3180), WTC0(0xdd7f9000), WTC0(0xdd329e80), WTC0(0xdce58e80), WTC0(0xdc989300), WTC0(0xdc4bde40), WTC0(0xdbff96c0), 
  WTC0(0xdbb3d780), WTC0(0xdb68bb80), WTC0(0xdb1e5c80), WTC0(0xdad4c380), WTC0(0xda8be840), WTC0(0xda43c1c0), WTC0(0xd9fc4740), WTC0(0xd9b56640), 
  WTC0(0xd96f0440), WTC0(0xd9290600), WTC0(0xd8e35080), WTC0(0xd89dcd40), WTC0(0xd8586b40), WTC0(0xd8131940), WTC0(0xd7cdc640), WTC0(0xd7886180), 
  WTC0(0xd742dc80), WTC0(0xd6fd2780), WTC0(0xd6b73400), WTC0(0xd670fd80), WTC0(0xd62a8a40), WTC0(0xd5e3e080), WTC0(0xd59d0840), WTC0(0xd5562b80), 
  WTC0(0xd50f9540), WTC0(0xd4c992c0), WTC0(0xd4846f80), WTC0(0xd4405a80), WTC0(0xd3fd6580), WTC0(0xd3bba140), WTC0(0xd37b1c80), WTC0(0xd33bb780), 
  WTC0(0xd2fd2400), WTC0(0xd2bf1240), WTC0(0xd2813300), WTC0(0xd2435ac0), WTC0(0xd2057fc0), WTC0(0xd1c79a00), WTC0(0xd189a240), WTC0(0xd14b9dc0), 
  WTC0(0xd10d9e00), WTC0(0xd0cfb580), WTC0(0xd091f6c0), WTC0(0xd0548100), WTC0(0xd0177f40), WTC0(0xcfdb1cc0), WTC0(0xcf9f84c0), WTC0(0xcf64d780), 
  WTC0(0xcf2b2b00), WTC0(0xcef29440), WTC0(0xcebb2640), WTC0(0xce84c000), WTC0(0xce4f0bc0), WTC0(0xce19b200), WTC0(0xcde45d40), WTC0(0xcdaeedc0), 
  WTC0(0xcd7979c0), WTC0(0xcd4419c0), WTC0(0xcd0ee6c0), WTC0(0xccda0540), WTC0(0xcca5a500), WTC0(0xcc71f640), WTC0(0xcc3f2800), WTC0(0xcc0d4300), 
  WTC0(0xcbdc2a00), WTC0(0xcbabbe80), WTC0(0xcb7be200), WTC0(0xcb4c8200), WTC0(0xcb1d9800), WTC0(0xcaef1d40), WTC0(0xcac10bc0), WTC0(0xca936440), 
  WTC0(0xca662d00), WTC0(0xca396d40), WTC0(0xca0d2b80), WTC0(0xc9e16f80), WTC0(0xc9b63f80), WTC0(0xc98ba2c0), WTC0(0xc961a000), WTC0(0xc9383ec0), 
  WTC0(0xc90a0440), WTC0(0xc8e0d280), WTC0(0xc8b73b80), WTC0(0xc88d4900), WTC0(0xc86304c0), WTC0(0xc83878c0), WTC0(0xc80dae80), WTC0(0xc7e2afc0), 
  WTC0(0xc7b78640), WTC0(0xc78c3c40), WTC0(0xc760da80), WTC0(0xc7356640), WTC0(0xc709de40), WTC0(0xc6de41c0), WTC0(0xc6b28fc0), WTC0(0xc686bd40), 
  WTC0(0xc65ab600), WTC0(0xc62e6580), WTC0(0xc601b880), WTC0(0xc5d4bac0), WTC0(0xc5a79640), WTC0(0xc57a76c0), WTC0(0xc54d8780), WTC0(0xc520e840), 
  WTC0(0xc4f4acc0), WTC0(0xc4c8e880), WTC0(0xc49dad80), WTC0(0xc472e640), WTC0(0xc44856c0), WTC0(0xc41dc140), WTC0(0xc3f2e940), WTC0(0xc3c7bc00), 
  WTC0(0xc39c4f00), WTC0(0xc370b9c0), WTC0(0xc34513c0), WTC0(0xc3197940), WTC0(0xc2ee0a00), WTC0(0xc2c2e640), WTC0(0xc2982d80), WTC0(0xc26df5c0), 
  WTC0(0xc2444b00), WTC0(0xc21b3940), WTC0(0xc1f2cbc0), WTC0(0xc1cb05c0), WTC0(0xc1a3e340), WTC0(0xc17d5f00), WTC0(0xc15773c0), WTC0(0xc1320940), 
  WTC0(0xc10cf480), WTC0(0xc0e80a00), WTC0(0xc0c31f00), WTC0(0xc09e2640), WTC0(0xc0792ec0), WTC0(0xc0544940), WTC0(0xc02f86c0), WTC0(0xc00b04c0), 
  WTC0(0xbfe6ed01), WTC0(0xbfc36a01), WTC0(0xbfa0a581), WTC0(0xbf7eb581), WTC0(0xbf5d9a81), WTC0(0xbf3d5501), WTC0(0xbf1de601), WTC0(0xbeff4801), 
  WTC0(0xbee17201), WTC0(0xbec45881), WTC0(0xbea7f301), WTC0(0xbe8c3781), WTC0(0xbe712001), WTC0(0xbe56a381), WTC0(0xbe3cbc01), WTC0(0xbe236001), 
  WTC0(0xbe0a8581), WTC0(0xbdf22181), WTC0(0xbdda2a01), WTC0(0xbdc29a81), WTC0(0xbdab7181), WTC0(0xbd94b001), WTC0(0xbd7e5581), WTC0(0xbd686681), 
  WTC0(0xbd52eb01), WTC0(0xbd3deb81), WTC0(0xbd297181), WTC0(0xbd158801), WTC0(0xbd023f01), WTC0(0xbcefa601), WTC0(0xbcddcc81), WTC0(0xbcccbd01), 
  WTC0(0xbcbc7e01), WTC0(0xbcad1501), WTC0(0xbc9e8801), WTC0(0xbc90d481), WTC0(0xbc83f201), WTC0(0xbc77d601), WTC0(0xbc6c7781), WTC0(0xbc61c401), 
  WTC0(0xbc57a301), WTC0(0xbc4dfb81), WTC0(0xbc44b481), WTC0(0xbc3bbc01), WTC0(0xbc330781), WTC0(0xbc2a8c81), WTC0(0xbc224181), WTC0(0xbc1a2401), 
  WTC0(0xbc123b81), WTC0(0xbc0a8f01), WTC0(0xbc032601), WTC0(0xbbfc0f81), WTC0(0xbbf56181), WTC0(0xbbef3301), WTC0(0xbbe99981), WTC0(0xbbe49d01), 
  WTC0(0xbbe03801), WTC0(0xbbdc6481), WTC0(0xbbd91b81), WTC0(0xbbd64d01), WTC0(0xbbd3e101), WTC0(0xbbd1bd81), WTC0(0xbbcfca81), WTC0(0xbbce0601), 
  WTC0(0xbbcc8201), WTC0(0xbbcb5301), WTC0(0xbbca8d01), WTC0(0xbbca5081), WTC0(0xbbcaca01), WTC0(0xbbcc2681), WTC0(0xbbce9181), WTC0(0xbbd21281), 
  WTC0(0xbbd68c81), WTC0(0xbbdbe201), WTC0(0xbbe1f401), WTC0(0xbbe89901), WTC0(0xbbef9b81), WTC0(0xbbf6c601), WTC0(0xbbfde481), WTC0(0xbc04e381), 
  WTC0(0xbc0bcf81), WTC0(0xbc12b801), WTC0(0xbc19ab01), WTC0(0xbc20ae01), WTC0(0xbc27bd81), WTC0(0xbc2ed681), WTC0(0xbc35f501), WTC0(0xbc3d1801), 
  WTC0(0xbc444081), WTC0(0xbc4b6e81), WTC0(0xbc52a381), WTC0(0xbc59df81), WTC0(0xbc612301), WTC0(0xbc686e01), WTC0(0xbc6fc101), WTC0(0xbc771c01), 
  WTC0(0xbc7e7e01), WTC0(0xbc85e801), WTC0(0xbc8d5901), WTC0(0xbc94d201), WTC0(0xbc9c5281), WTC0(0xbca3db01), WTC0(0xbcab6c01), WTC0(0xbcb30601), 
  WTC0(0xbcbaa801), WTC0(0xbcc25181), WTC0(0xbcca0301), WTC0(0xbcd1bb81), WTC0(0xbcd97c81), WTC0(0xbce14601), WTC0(0xbce91801), WTC0(0xbcf0f381), 
  WTC0(0xbcf8d781), WTC0(0xbd00c381), WTC0(0xbd08b781), WTC0(0xbd10b381), WTC0(0xbd18b781), WTC0(0xbd20c401), WTC0(0xbd28d981), WTC0(0xbd30f881), 
  WTC0(0xbd391f81), WTC0(0xbd414f01), WTC0(0xbd498601), WTC0(0xbd51c481), WTC0(0xbd5a0b01), WTC0(0xbd625981), WTC0(0xbd6ab101), WTC0(0xbd731081), 
  WTC0(0xbd7b7781), WTC0(0xbd83e681), WTC0(0xbd8c5c01), WTC0(0xbd94d801), WTC0(0xbd9d5b81), WTC0(0xbda5e601), WTC0(0xbdae7881), WTC0(0xbdb71201), 
  WTC0(0xbdbfb281), WTC0(0xbdc85981), WTC0(0xbdd10681), WTC0(0xbdd9b981), WTC0(0xbde27201), WTC0(0xbdeb3101), WTC0(0xbdf3f701), WTC0(0xbdfcc301), 
  WTC0(0xbe059481), WTC0(0xbe0e6c01), WTC0(0xbe174781), WTC0(0xbe202801), WTC0(0xbe290d01), WTC0(0xbe31f701), WTC0(0xbe3ae601), WTC0(0xbe43da81), 
  WTC0(0xbe4cd381), WTC0(0xbe55d001), WTC0(0xbe5ed081), WTC0(0xbe67d381), WTC0(0xbe70da01), WTC0(0xbe79e481), WTC0(0xbe82f301), WTC0(0xbe8c0501), 
  WTC0(0xbe951a81), WTC0(0xbe9e3281), WTC0(0xbea74c81), WTC0(0xbeb06881), WTC0(0xbeb98681), WTC0(0xbec2a781), WTC0(0xbecbca81), WTC0(0xbed4f081), 
  WTC0(0xbede1901), WTC0(0xbee74281), WTC0(0xbef06d01), WTC0(0xbef99901), WTC0(0xbf02c581), WTC0(0xbf0bf381), WTC0(0xbf152381), WTC0(0xbf1e5501), 
  WTC0(0xbf278801), WTC0(0xbf30bb01), WTC0(0xbf39ee81), WTC0(0xbf432281), WTC0(0xbf4c5681), WTC0(0xbf558b01), WTC0(0xbf5ec101), WTC0(0xbf67f801), 
  WTC0(0xbf712f01), WTC0(0xbf7a6681), WTC0(0xbf839d81), WTC0(0xbf8cd481), WTC0(0xbf960b01), WTC0(0xbf9f4181), WTC0(0xbfa87901), WTC0(0xbfb1b101), 
  WTC0(0xbfbae981), WTC0(0xbfc42201), WTC0(0xbfcd5a01), WTC0(0xbfd69101), WTC0(0xbfdfc781), WTC0(0xbfe8fc01), WTC0(0xbff22f81), WTC0(0xbffb6081), 
  /* part 1 */
  WTC1(0x80093e01), WTC1(0x801b9b01), WTC1(0x802df701), WTC1(0x80405101), WTC1(0x8052a881), WTC1(0x8064fc81), WTC1(0x80774c81), WTC1(0x80899881), 
  WTC1(0x809bdf01), WTC1(0x80ae1f81), WTC1(0x80c05a01), WTC1(0x80d28d81), WTC1(0x80e4bb81), WTC1(0x80f6e481), WTC1(0x81090981), WTC1(0x811b2981), 
  WTC1(0x812d4481), WTC1(0x813f5981), WTC1(0x81516701), WTC1(0x81636d81), WTC1(0x81756d81), WTC1(0x81876781), WTC1(0x81995c01), WTC1(0x81ab4b01), 
  WTC1(0x81bd3401), WTC1(0x81cf1581), WTC1(0x81e0ee81), WTC1(0x81f2bf81), WTC1(0x82048881), WTC1(0x82164a81), WTC1(0x82280581), WTC1(0x8239b981), 
  WTC1(0x824b6601), WTC1(0x825d0901), WTC1(0x826ea201), WTC1(0x82803101), WTC1(0x8291b601), WTC1(0x82a33281), WTC1(0x82b4a601), WTC1(0x82c61101), 
  WTC1(0x82d77201), WTC1(0x82e8c801), WTC1(0x82fa1181), WTC1(0x830b4f81), WTC1(0x831c8101), WTC1(0x832da781), WTC1(0x833ec381), WTC1(0x834fd481), 
  WTC1(0x8360d901), WTC1(0x8371d081), WTC1(0x8382ba01), WTC1(0x83939501), WTC1(0x83a46181), WTC1(0x83b52101), WTC1(0x83c5d381), WTC1(0x83d67881), 
  WTC1(0x83e70f01), WTC1(0x83f79681), WTC1(0x84080d81), WTC1(0x84187401), WTC1(0x8428ca01), WTC1(0x84391081), WTC1(0x84494881), WTC1(0x84597081), 
  WTC1(0x84698881), WTC1(0x84798f81), WTC1(0x84898481), WTC1(0x84996701), WTC1(0x84a93801), WTC1(0x84b8f801), WTC1(0x84c8a701), WTC1(0x84d84601), 
  WTC1(0x84e7d381), WTC1(0x84f74e01), WTC1(0x8506b581), WTC1(0x85160981), WTC1(0x85254a81), WTC1(0x85347901), WTC1(0x85439601), WTC1(0x8552a181), 
  WTC1(0x85619a01), WTC1(0x85707f81), WTC1(0x857f5101), WTC1(0x858e0e01), WTC1(0x859cb781), WTC1(0x85ab4f01), WTC1(0x85b9d481), WTC1(0x85c84801), 
  WTC1(0x85d6a981), WTC1(0x85e4f801), WTC1(0x85f33281), WTC1(0x86015981), WTC1(0x860f6e01), WTC1(0x861d7081), WTC1(0x862b6201), WTC1(0x86394301), 
  WTC1(0x86471281), WTC1(0x8654d001), WTC1(0x86627b01), WTC1(0x86701381), WTC1(0x867d9a81), WTC1(0x868b1001), WTC1(0x86987581), WTC1(0x86a5ca81), 
  WTC1(0x86b30f01), WTC1(0x86c04381), WTC1(0x86cd6681), WTC1(0x86da7901), WTC1(0x86e77b81), WTC1(0x86f46d81), WTC1(0x87014f81), WTC1(0x870e2301), 
  WTC1(0x871ae981), WTC1(0x8727a381), WTC1(0x87345381), WTC1(0x8740f681), WTC1(0x874d8681), WTC1(0x8759fd01), WTC1(0x87665481), WTC1(0x87729701), 
  WTC1(0x877ede01), WTC1(0x878b4301), WTC1(0x8797dd81), WTC1(0x87a48b01), WTC1(0x87b0ef01), WTC1(0x87bcab81), WTC1(0x87c76201), WTC1(0x87d0ca81), 
  WTC1(0x87fdd781), WTC1(0x881dd301), WTC1(0x88423301), WTC1(0x886a8a81), WTC1(0x88962981), WTC1(0x88c45e81), WTC1(0x88f47901), WTC1(0x8925f101), 
  WTC1(0x89586901), WTC1(0x898b8301), WTC1(0x89bee581), WTC1(0x89f26101), WTC1(0x8a25f301), WTC1(0x8a599a81), WTC1(0x8a8d5801), WTC1(0x8ac13381), 
  WTC1(0x8af53e81), WTC1(0x8b298b81), WTC1(0x8b5e2c81), WTC1(0x8b933001), WTC1(0x8bc8a401), WTC1(0x8bfe9401), WTC1(0x8c350d01), WTC1(0x8c6c1b01), 
  WTC1(0x8ca3cb01), WTC1(0x8cdc2901), WTC1(0x8d154081), WTC1(0x8d4f1b01), WTC1(0x8d89be81), WTC1(0x8dc53001), WTC1(0x8e017581), WTC1(0x8e3e9481), 
  WTC1(0x8e7c9301), WTC1(0x8ebb7581), WTC1(0x8efb4181), WTC1(0x8f3bfb01), WTC1(0x8f7da401), WTC1(0x8fc03f01), WTC1(0x9003ce81), WTC1(0x90485401), 
  WTC1(0x908dd101), WTC1(0x90d44781), WTC1(0x911bb981), WTC1(0x91642781), WTC1(0x91ad9281), WTC1(0x91f7f981), WTC1(0x92435d01), WTC1(0x928fbe01), 
  WTC1(0x92dd1b01), WTC1(0x932b7501), WTC1(0x937acb01), WTC1(0x93cb1c81), WTC1(0x941c6901), WTC1(0x946eaf81), WTC1(0x94c1ee01), WTC1(0x95162381), 
  WTC1(0x956b4f81), WTC1(0x95c17081), WTC1(0x96188501), WTC1(0x96708b81), WTC1(0x96c98381), WTC1(0x97236b01), WTC1(0x977e4181), WTC1(0x97da0481), 
  WTC1(0x9836b201), WTC1(0x98944901), WTC1(0x98f2c601), WTC1(0x99522801), WTC1(0x99b26c81), WTC1(0x9a139101), WTC1(0x9a759301), WTC1(0x9ad87081), 
  WTC1(0x9b3c2801), WTC1(0x9ba0b701), WTC1(0x9c061b81), WTC1(0x9c6c5481), WTC1(0x9cd35f81), WTC1(0x9d3b3b81), WTC1(0x9da3e601), WTC1(0x9e0d5e01), 
  WTC1(0x9e779f81), WTC1(0x9ee2a901), WTC1(0x9f4e7801), WTC1(0x9fbb0981), WTC1(0xa0285d81), WTC1(0xa0967201), WTC1(0xa1054701), WTC1(0xa174da81), 
  WTC1(0xa1e52a81), WTC1(0xa2563501), WTC1(0xa2c7f801), WTC1(0xa33a7201), WTC1(0xa3ada281), WTC1(0xa4218801), WTC1(0xa4962181), WTC1(0xa50b6e81), 
  WTC1(0xa5816e81), WTC1(0xa5f81f81), WTC1(0xa66f8201), WTC1(0xa6e79401), WTC1(0xa7605601), WTC1(0xa7d9c681), WTC1(0xa853e501), WTC1(0xa8ceb201), 
  WTC1(0xa94a2c01), WTC1(0xa9c65401), WTC1(0xaa432981), WTC1(0xaac0ad01), WTC1(0xab3edf01), WTC1(0xabbdc001), WTC1(0xac3d5001), WTC1(0xacbd9081), 
  WTC1(0xad3e8101), WTC1(0xadc02281), WTC1(0xae427481), WTC1(0xaec57801), WTC1(0xaf492f01), WTC1(0xafcd9a81), WTC1(0xb052bc01), WTC1(0xb0d89401), 
  WTC1(0xb15f2381), WTC1(0xb1e66a01), WTC1(0xb26e6881), WTC1(0xb2f71f01), WTC1(0xb3808d81), WTC1(0xb40ab501), WTC1(0xb4959501), WTC1(0xb5212e81), 
  WTC1(0x4a6cf67f), WTC1(0x49dffeff), WTC1(0x495265ff), WTC1(0x48c4277f), WTC1(0x4835407f), WTC1(0x47a5aeff), WTC1(0x471570ff), WTC1(0x468484ff), 
  WTC1(0x45f2eaff), WTC1(0x4560a2ff), WTC1(0x44cdad7f), WTC1(0x443a0c7f), WTC1(0x43a5c07f), WTC1(0x4310caff), WTC1(0x427b2bff), WTC1(0x41e4e3ff), 
  WTC1(0x414df2ff), WTC1(0x40b6557f), WTC1(0x401e06ff), WTC1(0x3f8503c0), WTC1(0x3eeb4e00), WTC1(0x3e50ebc0), WTC1(0x3db5e680), WTC1(0x3d1a4680), 
  WTC1(0x3c7e10c0), WTC1(0x3be14cc0), WTC1(0x3b4402c0), WTC1(0x3aa63800), WTC1(0x3a07e840), WTC1(0x39690880), WTC1(0x38c98700), WTC1(0x38295b40), 
  WTC1(0x37888a80), WTC1(0x36e71d40), WTC1(0x36451d80), WTC1(0x35a29400), WTC1(0x34ff8800), WTC1(0x345c04c0), WTC1(0x33b81940), WTC1(0x3313d200), 
  WTC1(0x326f3800), WTC1(0x31ca5600), WTC1(0x31253840), WTC1(0x307fe8c0), WTC1(0x2fda6e40), WTC1(0x2f34ce40), WTC1(0x2e8f0e40), WTC1(0x2de92ec0), 
  WTC1(0x2d432780), WTC1(0x2c9cea40), WTC1(0x2bf66300), WTC1(0x2b4f88c0), WTC1(0x2aa864c0), WTC1(0x2a010240), WTC1(0x29596e40), WTC1(0x28b1ba80), 
  WTC1(0x2809ff40), WTC1(0x27625b80), WTC1(0x26baf580), WTC1(0x2613e7c0), WTC1(0x256d3dc0), WTC1(0x24c70300), WTC1(0x24214380), WTC1(0x237c0800), 
  WTC1(0x22d75400), WTC1(0x22332a80), WTC1(0x218f8cc0), WTC1(0x20ec7e40), WTC1(0x204a04c0), WTC1(0x1fa82540), WTC1(0x1f06e300), WTC1(0x1e664000), 
  WTC1(0x1dc63bc0), WTC1(0x1d26d3c0), WTC1(0x1c8803a0), WTC1(0x1be9cc40), WTC1(0x1b4c34c0), WTC1(0x1aaf4480), WTC1(0x1a130260), WTC1(0x197774a0), 
  WTC1(0x18dca260), WTC1(0x184294e0), WTC1(0x17a95840), WTC1(0x1710fd80), WTC1(0x16799ce0), WTC1(0x15e35340), WTC1(0x154e41a0), WTC1(0x14ba8360), 
  WTC1(0x14282be0), WTC1(0x13975100), WTC1(0x13080aa0), WTC1(0x127a6240), WTC1(0x11ee50a0), WTC1(0x1163cc80), WTC1(0x10dacb20), WTC1(0x105333a0), 
  WTC1(0x0fccdb30), WTC1(0x0f478f40), WTC1(0x0ec31700), WTC1(0x0e3f4e80), WTC1(0x0dbc27f0), WTC1(0x0d399000), WTC1(0x0cb76d00), WTC1(0x0c359d50), 
  WTC1(0x0bb3fd50), WTC1(0x0b326bd0), WTC1(0x0ab0ca80), WTC1(0x0a2f0dc0), WTC1(0x09ad40c0), WTC1(0x092b7a90), WTC1(0x08a9db80), WTC1(0x08285c80), 
  WTC1(0x07a6c7b8), WTC1(0x0724e4e0), WTC1(0x06a27b80), WTC1(0x061f52f8), WTC1(0x059b2ad0), WTC1(0x0515b568), WTC1(0x048ea058), WTC1(0x04066408), 
  WTC1(0x037e52d8), WTC1(0x02f7d3c8), WTC1(0x0274614c), WTC1(0x01f63008), WTC1(0x0180403a), WTC1(0x0115c442), WTC1(0x00ba09e2), WTC1(0x006f077c), 
  WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), 
  WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), 
  WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), 
  WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), 
  WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), 
  WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), 
  WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), 
  WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), 
  WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), 
  WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), 
  WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), 
  WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), 
  WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), 
  WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), 
  WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), 
  WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), 
  /* part 2 */
  WTC2(0xfff36be1), WTC2(0xffdafbc1), WTC2(0xffc28035), WTC2(0xffa9fe8a), WTC2(0xff917c08), WTC2(0xff78fdfc), WTC2(0xff6089af), WTC2(0xff48246c), 
  WTC2(0xff2fd37f), WTC2(0xff179c31), WTC2(0xfeff83b6), WTC2(0xfee78d18), WTC2(0xfecfb93e), WTC2(0xfeb808f2), WTC2(0xfea07d06), WTC2(0xfe8916b4), 
  WTC2(0xfe71d7a0), WTC2(0xfe5ac174), WTC2(0xfe43d5d6), WTC2(0xfe2d167e), WTC2(0xfe16852e), WTC2(0xfe0023a6), WTC2(0xfde9f3f8), WTC2(0xfdd3ff7c), 
  WTC2(0xfdbe56c0), WTC2(0xfda90aa8), WTC2(0xfd942b78), WTC2(0xfd7fbb20), WTC2(0xfd6bad50), WTC2(0xfd57f510), WTC2(0xfd44857c), WTC2(0xfd3153fc), 
  WTC2(0xfd1e5840), WTC2(0xfd0b8a0c), WTC2(0xfcf8e180), WTC2(0xfce65eec), WTC2(0xfcd40ad0), WTC2(0xfcc1ee0c), WTC2(0xfcb011e8), WTC2(0xfc9e896c), 
  WTC2(0xfc8d716c), WTC2(0xfc7ce720), WTC2(0xfc6d072c), WTC2(0xfc5de09c), WTC2(0xfc4f74e8), WTC2(0xfc41c4e8), WTC2(0xfc34d0dc), WTC2(0xfc288a68), 
  WTC2(0xfc1cd49c), WTC2(0xfc1191e0), WTC2(0xfc06a4d0), WTC2(0xfbfbf3e8), WTC2(0xfbf16990), WTC2(0xfbe6f068), WTC2(0xfbdc7428), WTC2(0xfbd1fc68), 
  WTC2(0xfbc7ac50), WTC2(0xfbbda868), WTC2(0xfbb41500), WTC2(0xfbab1438), WTC2(0xfba2c5f8), WTC2(0xfb9b4a00), WTC2(0xfb94bfa8), WTC2(0xfb8f3b48), 
  WTC2(0xfb8ac638), WTC2(0xfb876970), WTC2(0xfb852d20), WTC2(0xfb840ae0), WTC2(0xfb83ed60), WTC2(0xfb84bec0), WTC2(0xfb866918), WTC2(0xfb88d4a8), 
  WTC2(0xfb8be810), WTC2(0xfb8f89d0), WTC2(0xfb93a080), WTC2(0xfb981418), WTC2(0xfb9ccdf0), WTC2(0xfba1b770), WTC2(0xfba6bae0), WTC2(0xfbabd5c0), 
  WTC2(0xfbb118d8), WTC2(0xfbb695c0), WTC2(0xfbbc5e90), WTC2(0xfbc29030), WTC2(0xfbc95268), WTC2(0xfbd0cd78), WTC2(0xfbd929c8), WTC2(0xfbe294d0), 
  WTC2(0xfbed4108), WTC2(0xfbf96118), WTC2(0xfc0726c8), WTC2(0xfc16b064), WTC2(0xfc280890), WTC2(0xfc3b3920), WTC2(0xfc504a98), WTC2(0xfc67271c), 
  WTC2(0xfc7f9a74), WTC2(0xfc996f18), WTC2(0xfcb46eb8), WTC2(0xfcd050b0), WTC2(0xfcecba24), WTC2(0xfd094f64), WTC2(0xfd25b720), WTC2(0xfd41ce40), 
  WTC2(0xfd5da7f8), WTC2(0xfd7959d8), WTC2(0xfd94fb74), WTC2(0xfdb0d3fc), WTC2(0xfdcd5a34), WTC2(0xfdeb06e4), WTC2(0xfe0a5184), WTC2(0xfe2b92c4), 
  WTC2(0xfe4f0486), WTC2(0xfe74df54), WTC2(0xfe9d5886), WTC2(0xfec85b92), WTC2(0xfef58a16), WTC2(0xff248275), WTC2(0xff54e401), WTC2(0xff866330), 
  WTC2(0xffb8c99b), WTC2(0xffebe1c9), WTC2(0x001f786a), WTC2(0x00538bf9), WTC2(0x00884cbc), WTC2(0x00bded23), WTC2(0x00f49f54), WTC2(0x012c8ee4), 
  WTC2(0x0165e0d2), WTC2(0x01a0b9d6), WTC2(0x01dd3d80), WTC2(0x021b74d4), WTC2(0x025b4e48), WTC2(0x029cb730), WTC2(0x02df9d0c), WTC2(0x0323f1a4), 
  WTC2(0x0369ab00), WTC2(0x03b0bf5c), WTC2(0x03f925a0), WTC2(0x0442e3d8), WTC2(0x048e0f40), WTC2(0x04dabdb0), WTC2(0x05290430), WTC2(0x0578e428), 
  WTC2(0x05ca4b60), WTC2(0x061d26c0), WTC2(0x067163d8), WTC2(0x06c6ff10), WTC2(0x071e03b0), WTC2(0x07767da0), WTC2(0x07d07918), WTC2(0x082c08e0), 
  WTC2(0x08894660), WTC2(0x08e84b70), WTC2(0x094930b0), WTC2(0x09abf8d0), WTC2(0x0a109020), WTC2(0x0a76e210), WTC2(0x0adeda50), WTC2(0x0b486b80), 
  WTC2(0x0bb38f00), WTC2(0x0c203e80), WTC2(0x0c8e73e0), WTC2(0x0cfe2c30), WTC2(0x0d6f6820), WTC2(0x0de22850), WTC2(0x0e566d90), WTC2(0x0ecc3dd0), 
  WTC2(0x0f43a3a0), WTC2(0x0fbca9f0), WTC2(0x10375b80), WTC2(0x10b3be20), WTC2(0x1131d280), WTC2(0x11b19960), WTC2(0x123313a0), WTC2(0x12b64380), 
  WTC2(0x133b2d00), WTC2(0x13c1d440), WTC2(0x144a3d60), WTC2(0x14d46900), WTC2(0x15605480), WTC2(0x15edfd20), WTC2(0x167d6040), WTC2(0x170e7e80), 
  WTC2(0x17a15b80), WTC2(0x1835fb00), WTC2(0x18cc60a0), WTC2(0x19648dc0), WTC2(0x19fe80e0), WTC2(0x1a9a38a0), WTC2(0x1b37b3e0), WTC2(0x1bd6f400), 
  WTC2(0x1c77fd20), WTC2(0x1d1ad400), WTC2(0x1dbf7c80), WTC2(0x1e65f820), WTC2(0x1f0e4540), WTC2(0x1fb861e0), WTC2(0x20644cc0), WTC2(0x21120640), 
  WTC2(0x21c19240), WTC2(0x2272f480), WTC2(0x23263000), WTC2(0x23db4580), WTC2(0x24923340), WTC2(0x254af700), WTC2(0x26058e80), WTC2(0x26c1fa00), 
  WTC2(0x27803d00), WTC2(0x28405a40), WTC2(0x29025500), WTC2(0x29c62d40), WTC2(0x2a8be0c0), WTC2(0x2b536cc0), WTC2(0x2c1ccf80), WTC2(0x2ce80840), 
  WTC2(0x2db519c0), WTC2(0x2e840600), WTC2(0x2f54cf80), WTC2(0x302775c0), WTC2(0x30fbf640), WTC2(0x31d24e00), WTC2(0x32aa7a00), WTC2(0x338479c0), 
  WTC2(0x34604e40), WTC2(0x353df900), WTC2(0x361d7ac0), WTC2(0x36fed200), WTC2(0x37e1fb40), WTC2(0x38c6f240), WTC2(0x39adb2c0), WTC2(0x3a963a00), 
  WTC2(0x3b808740), WTC2(0x3c6c9880), WTC2(0x3d5a6cc0), WTC2(0x3e4a0040), WTC2(0x3f3b4bc0), WTC2(0x402e48ff), WTC2(0x4122f17f), WTC2(0x42193f7f), 
  WTC2(0x43112eff), WTC2(0x440abbff), WTC2(0x4505e2ff), WTC2(0x46029e7f), WTC2(0x4700e9ff), WTC2(0x4800bfff), WTC2(0x49021bff), WTC2(0x4a050eff), 
  WTC2(0x4b09bc7f), WTC2(0x4c104aff), WTC2(0x4d18df7f), WTC2(0x4e23a07f), WTC2(0x4f30b2ff), WTC2(0x50403c7f), WTC2(0x515262ff), WTC2(0x52674b7f), 
  WTC2(0x001678b2), WTC2(0x00061a3b), WTC2(0xfffb4622), WTC2(0xfff5ea94), WTC2(0xfff5f5b9), WTC2(0xfffb55bd), WTC2(0x0005f8cb), WTC2(0x0015cd0c), 
  WTC2(0x002ac0ac), WTC2(0x0044c1d5), WTC2(0x0063beb2), WTC2(0x0087a56d), WTC2(0x00b06431), WTC2(0x00dde929), WTC2(0x01102280), WTC2(0x0146fe5e), 
  WTC2(0x01826af2), WTC2(0x01c25662), WTC2(0x0206aedc), WTC2(0x024f6288), WTC2(0x029c5f94), WTC2(0x02ed9424), WTC2(0x0342ee6c), WTC2(0x039c5c90), 
  WTC2(0x03f9ccbc), WTC2(0x045b2d18), WTC2(0x04c06bd8), WTC2(0x05297718), WTC2(0x05963d10), WTC2(0x0606abe8), WTC2(0x067ab1c0), WTC2(0x06f23cd0), 
  WTC2(0x076d3b40), WTC2(0x07eb9b38), WTC2(0x086d4ae0), WTC2(0x08f23860), WTC2(0x097a51f0), WTC2(0x0a0585b0), WTC2(0x0a93c1d0), WTC2(0x0b24f470), 
  WTC2(0x0bb90bc0), WTC2(0x0c4ff5f0), WTC2(0x0ce9a130), WTC2(0x0d85fb90), WTC2(0x0e24f360), WTC2(0x0ec676b0), WTC2(0x0f6a73b0), WTC2(0x1010d880), 
  WTC2(0x10b99360), WTC2(0x11649280), WTC2(0x1211c400), WTC2(0x12c115e0), WTC2(0x137276a0), WTC2(0x1425d420), WTC2(0x14db1ca0), WTC2(0x15923e60), 
  WTC2(0x164b2780), WTC2(0x1705c620), WTC2(0x17c20860), WTC2(0x187fdca0), WTC2(0x193f30e0), WTC2(0x19fff340), WTC2(0x1ac21200), WTC2(0x1b857b40), 
  WTC2(0x1c4a1d40), WTC2(0x1d0fe600), WTC2(0x1dd6c3e0), WTC2(0x1e9ea4e0), WTC2(0x1f677740), WTC2(0x20312940), WTC2(0x20fba8c0), WTC2(0x21c6e440), 
  WTC2(0x2292c9c0), WTC2(0x235f4780), WTC2(0x242c4b80), WTC2(0x24f9c400), WTC2(0x25c79f40), WTC2(0x2695cb40), WTC2(0x27643680), WTC2(0x2832cec0), 
  WTC2(0x29018240), WTC2(0x29d03f80), WTC2(0x2a9ef480), WTC2(0x2b6d8f00), WTC2(0x2c3bfdc0), WTC2(0x2d0a2ec0), WTC2(0x2dd81000), WTC2(0x2ea58fc0), 
  WTC2(0x2f729c40), WTC2(0x303f2380), WTC2(0x310b1400), WTC2(0x31d65b80), WTC2(0x32a0e840), WTC2(0x336aa8c0), WTC2(0x34338ac0), WTC2(0x34fb7cc0), 
  WTC2(0x35c26cc0), WTC2(0x36884900), WTC2(0x374cff80), WTC2(0x38107e80), WTC2(0x38d2b440), WTC2(0x39938ec0), WTC2(0x3a52fc40), WTC2(0x3b10eb00), 
  WTC2(0x3bcd4900), WTC2(0x3c880480), WTC2(0x3d410bc0), WTC2(0x3df84d00), WTC2(0x3eadb600), WTC2(0x3f613540), WTC2(0x4012b8ff), WTC2(0x40c22eff), 
  WTC2(0x416f85ff), WTC2(0x421aab7f), WTC2(0x42c38e7f), WTC2(0x436a1c7f), WTC2(0x440e437f), WTC2(0x44aff27f), WTC2(0x454f167f), WTC2(0x45eb9eff), 
  WTC2(0x468578ff), WTC2(0x471c937f), WTC2(0x47b0dc7f), WTC2(0x484241ff), WTC2(0x48d0b1ff), WTC2(0x495c1a7f), WTC2(0x49e46a7f), WTC2(0x4a698f7f), 
  WTC2(0x4aeb77ff), WTC2(0x4b6a11ff), WTC2(0x4be54b7f), WTC2(0x4c5d12ff), WTC2(0x4cd155ff), WTC2(0x4d4203ff), WTC2(0x4daf09ff), WTC2(0x4e18567f), 
  WTC2(0x4e7dd77f), WTC2(0x4edf7b7f), WTC2(0x4f3d307f), WTC2(0x4f96e47f), WTC2(0x4fec85ff), WTC2(0x503e02ff), WTC2(0x508b497f), WTC2(0x50d447ff), 
  WTC2(0x5118ec7f), WTC2(0x515924ff), WTC2(0x5194dfff), WTC2(0x51cc0b7f), WTC2(0x51fe95ff), WTC2(0x522c6cff), WTC2(0x52557eff), WTC2(0x5279b9ff), 
  WTC2(0x52990c7f), WTC2(0x52b364ff), WTC2(0x52c8b07f), WTC2(0x52d8ddff), WTC2(0x52e3db7f), WTC2(0x52e996ff), WTC2(0x52e9ff7f), WTC2(0x52e501ff), 
  WTC2(0x52da8cff), WTC2(0x52ca8f7f), WTC2(0x52b4f67f), WTC2(0x5299b07f), WTC2(0x5278ac7f), WTC2(0x5251d77f), WTC2(0x52251fff), WTC2(0x51f274ff), 
  WTC2(0x51b9c37f), WTC2(0x517af9ff), WTC2(0x5136077f), WTC2(0x50ead8ff), WTC2(0x50995cff), WTC2(0x504181ff), WTC2(0x4fe335ff), WTC2(0x4f7e677f), 
  WTC2(0x4f1303ff), WTC2(0x4ea0f9ff), WTC2(0x4e2837ff), WTC2(0x4da8ab7f), WTC2(0x4d2242ff), WTC2(0x4c94ecff), WTC2(0x4c0096ff), WTC2(0x4b652f7f), 
  WTC2(0x4ac2a4ff), WTC2(0x4a18e4ff), WTC2(0x4967ddff), WTC2(0x48af7e7f), WTC2(0x47efb3ff), WTC2(0x47286cff), WTC2(0x4659ad7f), WTC2(0x45856f7f), 
  WTC2(0x44afa3ff), WTC2(0x43dc507f), WTC2(0x430f657f), WTC2(0x424ad47f), WTC2(0x418e927f), WTC2(0x40da7bff), WTC2(0x402e6f7f), WTC2(0x3f8a3100), 
  WTC2(0x3eed6f40), WTC2(0x3e57d700), WTC2(0x3dc914c0), WTC2(0x3d40cc40), WTC2(0x3cbe98c0), WTC2(0x3c421540), WTC2(0x3bcadbc0), WTC2(0x3b588880), 
  WTC2(0x3aeab780), WTC2(0x3a810540), WTC2(0x3a1b0e00), WTC2(0x39b86d00), WTC2(0x3958bcc0), WTC2(0x38fb9700), WTC2(0x38a095c0), WTC2(0x38473d80), 
  WTC2(0x37eeff40), WTC2(0x37974b40), WTC2(0x373f9500), WTC2(0x36e7ae00), WTC2(0x368fc4c0), WTC2(0x36380b80), WTC2(0x35e0b300), WTC2(0x3589c140), 
  WTC2(0x35331180), WTC2(0x34dc7c80), WTC2(0x3485dc80), WTC2(0x342f1600), WTC2(0x33d81780), WTC2(0x3380d0c0), WTC2(0x33293100), WTC2(0x32d11800), 
  WTC2(0x32785780), WTC2(0x321ec0c0), WTC2(0x31c42680), WTC2(0x316885c0), WTC2(0x310c0580), WTC2(0x30aecec0), WTC2(0x30510940), WTC2(0x2ff2b8c0), 
  WTC2(0x2f93bf40), WTC2(0x2f33fc00), WTC2(0x2ed350c0), WTC2(0x2e71ba80), WTC2(0x2e0f5340), WTC2(0x2dac35c0), WTC2(0x2d487c80), WTC2(0x2ce431c0), 
  WTC2(0x2c7f4fc0), WTC2(0x2c19d080), WTC2(0x2bb3ad80), WTC2(0x2b4ce080), WTC2(0x2ae56340), WTC2(0x2a7d2f80), WTC2(0x2a143f00), WTC2(0x29aa8b40) 
};

const FIXP_WTB ELDAnalysis480[1440] = {
  WTC0(0xfacfbef0), WTC0(0xfab88c18), WTC0(0xfaa0e520), WTC0(0xfa88d110), WTC0(0xfa7056e8), WTC0(0xfa577db0), WTC0(0xfa3e4c70), WTC0(0xfa24ca28), 
  WTC0(0xfa0afde0), WTC0(0xf9f0eea0), WTC0(0xf9d6a2c8), WTC0(0xf9bc1ab8), WTC0(0xf9a15230), WTC0(0xf9864510), WTC0(0xf96af058), WTC0(0xf94f55c0), 
  WTC0(0xf93378e0), WTC0(0xf9175d80), WTC0(0xf8fb0468), WTC0(0xf8de68b8), WTC0(0xf8c18438), WTC0(0xf8a450d8), WTC0(0xf886cde8), WTC0(0xf8690148), 
  WTC0(0xf84af148), WTC0(0xf82ca410), WTC0(0xf80e1e18), WTC0(0xf7ef62a0), WTC0(0xf7d074e0), WTC0(0xf7b15870), WTC0(0xf7921240), WTC0(0xf772a7a0), 
  WTC0(0xf7531e50), WTC0(0xf7337820), WTC0(0xf713afd0), WTC0(0xf6f3bea0), WTC0(0xf6d39dc0), WTC0(0xf6b352e0), WTC0(0xf692f280), WTC0(0xf6729250), 
  WTC0(0xf65247a0), WTC0(0xf63224c0), WTC0(0xf6123a00), WTC0(0xf5f297c0), WTC0(0xf5d34dd0), WTC0(0xf5b46b10), WTC0(0xf595fd90), WTC0(0xf5781390), 
  WTC0(0xf55abba0), WTC0(0xf53e0510), WTC0(0xf521ff70), WTC0(0xf506ba30), WTC0(0xf4ec4330), WTC0(0xf4d2a680), WTC0(0xf4b9efe0), WTC0(0xf4a22ac0), 
  WTC0(0xf48b5f70), WTC0(0xf4759310), WTC0(0xf460cde0), WTC0(0xf44cfcc0), WTC0(0xf439aff0), WTC0(0xf4264e00), WTC0(0xf4123d90), WTC0(0xf3fd1370), 
  WTC0(0xf3e6be00), WTC0(0xf3cf41a0), WTC0(0xf3b6a030), WTC0(0xf39cdd60), WTC0(0xf381fe00), WTC0(0xf3660760), WTC0(0xf348fe70), WTC0(0xf32ae820), 
  WTC0(0xf30bc940), WTC0(0xf2eba690), WTC0(0xf2ca8480), WTC0(0xf2a86670), WTC0(0xf2854f40), WTC0(0xf2614190), WTC0(0xf23c41e0), WTC0(0xf21657a0), 
  WTC0(0xf1ef8ae0), WTC0(0xf1c7e3e0), WTC0(0xf19f63d0), WTC0(0xf1760450), WTC0(0xf14bbdf0), WTC0(0xf1208960), WTC0(0xf0f45cd0), WTC0(0xf0c72ce0), 
  WTC0(0xf098ee00), WTC0(0xf06996f0), WTC0(0xf0392620), WTC0(0xf0079e10), WTC0(0xefd4ffc0), WTC0(0xefa15ca0), WTC0(0xef6ce600), WTC0(0xef37d460), 
  WTC0(0xef025f80), WTC0(0xeecca2c0), WTC0(0xee969760), WTC0(0xee603440), WTC0(0xee296d20), WTC0(0xedf21c00), WTC0(0xedba07e0), WTC0(0xed80f640), 
  WTC0(0xed46bf40), WTC0(0xed0b7b00), WTC0(0xeccf5fc0), WTC0(0xec92a120), WTC0(0xec556d60), WTC0(0xec17e700), WTC0(0xebda2d40), WTC0(0xeb9c5fa0), 
  WTC0(0xeb5e7040), WTC0(0xeb201b20), WTC0(0xeae117c0), WTC0(0xeaa12000), WTC0(0xea600180), WTC0(0xea1d9940), WTC0(0xe9d9c160), WTC0(0xe99468a0), 
  WTC0(0xe94dc040), WTC0(0xe9061940), WTC0(0xe8bdc140), WTC0(0xe8750ae0), WTC0(0xe82c4fa0), WTC0(0xe7e3ea40), WTC0(0xe79c35e0), WTC0(0xe7554ca0), 
  WTC0(0xe70efc00), WTC0(0xe6c90c20), WTC0(0xe6833f00), WTC0(0xe63d2300), WTC0(0xe5f620a0), WTC0(0xe5ad9dc0), WTC0(0xe5632080), WTC0(0xe5169da0), 
  WTC0(0xe4c83e60), WTC0(0xe4782400), WTC0(0xe4269840), WTC0(0xe3d42dc0), WTC0(0xe38188c0), WTC0(0xe32f4be0), WTC0(0xe2ddeea0), WTC0(0xe28db520), 
  WTC0(0xe23ee000), WTC0(0xe1f1a580), WTC0(0xe1a5e3a0), WTC0(0xe15b35a0), WTC0(0xe1113860), WTC0(0xe0c78a00), WTC0(0xe07dd0e0), WTC0(0xe033b7c0), 
  WTC0(0xdfe8e680), WTC0(0xdf9d1fc0), WTC0(0xdf5055c0), WTC0(0xdf0287c0), WTC0(0xdeb3b340), WTC0(0xde63e7c0), WTC0(0xde134a00), WTC0(0xddc20000), 
  WTC0(0xdd703180), WTC0(0xdd1e1280), WTC0(0xdccbe080), WTC0(0xdc79d980), WTC0(0xdc283600), WTC0(0xdbd71e00), WTC0(0xdb86b140), WTC0(0xdb3710c0), 
  WTC0(0xdae850c0), WTC0(0xda9a6bc0), WTC0(0xda4d5640), WTC0(0xda010640), WTC0(0xd9b56640), WTC0(0xd96a5700), WTC0(0xd91fb700), WTC0(0xd8d56600), 
  WTC0(0xd88b4a40), WTC0(0xd8414f00), WTC0(0xd7f75f80), WTC0(0xd7ad6740), WTC0(0xd76352c0), WTC0(0xd7191040), WTC0(0xd6ce8c80), WTC0(0xd683bd00), 
  WTC0(0xd638a5c0), WTC0(0xd5ed4f80), WTC0(0xd5a1c240), WTC0(0xd5562b80), WTC0(0xd50ae500), WTC0(0xd4c04c80), WTC0(0xd476bb40), WTC0(0xd42e62c0), 
  WTC0(0xd3e75680), WTC0(0xd3a1ad00), WTC0(0xd35d6780), WTC0(0xd31a4300), WTC0(0xd2d7dc00), WTC0(0xd295d080), WTC0(0xd253d8c0), WTC0(0xd211df40), 
  WTC0(0xd1cfdbc0), WTC0(0xd18dc480), WTC0(0xd14b9dc0), WTC0(0xd1097c80), WTC0(0xd0c77700), WTC0(0xd085a500), WTC0(0xd0442f40), WTC0(0xd0034a80), 
  WTC0(0xcfc32c00), WTC0(0xcf840400), WTC0(0xcf45f400), WTC0(0xcf0913c0), WTC0(0xcecd8000), WTC0(0xce932c80), WTC0(0xce59bf40), WTC0(0xce20cd40), 
  WTC0(0xcde7ec40), WTC0(0xcdaeedc0), WTC0(0xcd75ea00), WTC0(0xcd3cfec0), WTC0(0xcd044b40), WTC0(0xcccbff00), WTC0(0xcc945480), WTC0(0xcc5d8780), 
  WTC0(0xcc27c3c0), WTC0(0xcbf2fc40), WTC0(0xcbbf0a00), WTC0(0xcb8bc7c0), WTC0(0xcb591880), WTC0(0xcb26f0c0), WTC0(0xcaf54980), WTC0(0xcac41ac0), 
  WTC0(0xca936440), WTC0(0xca632d80), WTC0(0xca337f00), WTC0(0xca046180), WTC0(0xc9d5dd40), WTC0(0xc9a7fa80), WTC0(0xc97ac200), WTC0(0xc94e3c00), 
  WTC0(0xc91d1840), WTC0(0xc8f15980), WTC0(0xc8c52340), WTC0(0xc8988100), WTC0(0xc86b7f00), WTC0(0xc83e28c0), WTC0(0xc8108a80), WTC0(0xc7e2afc0), 
  WTC0(0xc7b4a480), WTC0(0xc7867480), WTC0(0xc7582b40), WTC0(0xc729cc80), WTC0(0xc6fb5700), WTC0(0xc6ccca40), WTC0(0xc69e2180), WTC0(0xc66f49c0), 
  WTC0(0xc64029c0), WTC0(0xc610a740), WTC0(0xc5e0bfc0), WTC0(0xc5b09e80), WTC0(0xc5807900), WTC0(0xc5508440), WTC0(0xc520e840), WTC0(0xc4f1bdc0), 
  WTC0(0xc4c31d00), WTC0(0xc4951780), WTC0(0xc4678a00), WTC0(0xc43a28c0), WTC0(0xc40ca800), WTC0(0xc3deccc0), WTC0(0xc3b09940), WTC0(0xc3822c00), 
  WTC0(0xc353a0c0), WTC0(0xc3251740), WTC0(0xc2f6b500), WTC0(0xc2c8a140), WTC0(0xc29b02c0), WTC0(0xc26df5c0), WTC0(0xc2418940), WTC0(0xc215cbc0), 
  WTC0(0xc1eaca00), WTC0(0xc1c08680), WTC0(0xc196fb00), WTC0(0xc16e22c0), WTC0(0xc145f040), WTC0(0xc11e3a80), WTC0(0xc0f6cc00), WTC0(0xc0cf6ec0), 
  WTC0(0xc0a802c0), WTC0(0xc0809280), WTC0(0xc0593340), WTC0(0xc031f880), WTC0(0xc00b04c0), WTC0(0xbfe48981), WTC0(0xbfbebb81), WTC0(0xbf99cb01), 
  WTC0(0xbf75cc81), WTC0(0xbf52c101), WTC0(0xbf30a901), WTC0(0xbf0f8301), WTC0(0xbeef4601), WTC0(0xbecfe601), WTC0(0xbeb15701), WTC0(0xbe938c81), 
  WTC0(0xbe767e81), WTC0(0xbe5a2301), WTC0(0xbe3e7201), WTC0(0xbe236001), WTC0(0xbe08e181), WTC0(0xbdeee981), WTC0(0xbdd56b81), WTC0(0xbdbc6381), 
  WTC0(0xbda3d081), WTC0(0xbd8bb281), WTC0(0xbd740b81), WTC0(0xbd5ce281), WTC0(0xbd464281), WTC0(0xbd303581), WTC0(0xbd1ac801), WTC0(0xbd060c81), 
  WTC0(0xbcf21601), WTC0(0xbcdef701), WTC0(0xbcccbd01), WTC0(0xbcbb7001), WTC0(0xbcab1781), WTC0(0xbc9bb901), WTC0(0xbc8d5101), WTC0(0xbc7fd301), 
  WTC0(0xbc733401), WTC0(0xbc676501), WTC0(0xbc5c4c81), WTC0(0xbc51cb01), WTC0(0xbc47c281), WTC0(0xbc3e1981), WTC0(0xbc34c081), WTC0(0xbc2bab01), 
  WTC0(0xbc22cd81), WTC0(0xbc1a2401), WTC0(0xbc11b681), WTC0(0xbc098d81), WTC0(0xbc01b381), WTC0(0xbbfa3c01), WTC0(0xbbf34281), WTC0(0xbbece281), 
  WTC0(0xbbe73201), WTC0(0xbbe23281), WTC0(0xbbdddb01), WTC0(0xbbda2501), WTC0(0xbbd70201), WTC0(0xbbd45601), WTC0(0xbbd20301), WTC0(0xbbcfea81), 
  WTC0(0xbbce0601), WTC0(0xbbcc6b01), WTC0(0xbbcb3201), WTC0(0xbbca7481), WTC0(0xbbca5d01), WTC0(0xbbcb2281), WTC0(0xbbccfc81), WTC0(0xbbd01301), 
  WTC0(0xbbd45881), WTC0(0xbbd9a781), WTC0(0xbbdfdb81), WTC0(0xbbe6c801), WTC0(0xbbee2f81), WTC0(0xbbf5d181), WTC0(0xbbfd6c01), WTC0(0xbc04e381), 
  WTC0(0xbc0c4581), WTC0(0xbc13a481), WTC0(0xbc1b1081), WTC0(0xbc228f01), WTC0(0xbc2a1a81), WTC0(0xbc31af01), WTC0(0xbc394901), WTC0(0xbc40e881), 
  WTC0(0xbc488e81), WTC0(0xbc503b81), WTC0(0xbc57f101), WTC0(0xbc5fae81), WTC0(0xbc677501), WTC0(0xbc6f4401), WTC0(0xbc771c01), WTC0(0xbc7efc81), 
  WTC0(0xbc86e581), WTC0(0xbc8ed701), WTC0(0xbc96d101), WTC0(0xbc9ed481), WTC0(0xbca6e101), WTC0(0xbcaef701), WTC0(0xbcb71701), WTC0(0xbcbf4001), 
  WTC0(0xbcc77181), WTC0(0xbccfac01), WTC0(0xbcd7ef01), WTC0(0xbce03b81), WTC0(0xbce89281), WTC0(0xbcf0f381), WTC0(0xbcf95e81), WTC0(0xbd01d281), 
  WTC0(0xbd0a4f81), WTC0(0xbd12d581), WTC0(0xbd1b6501), WTC0(0xbd23ff01), WTC0(0xbd2ca281), WTC0(0xbd355081), WTC0(0xbd3e0801), WTC0(0xbd46c801), 
  WTC0(0xbd4f9101), WTC0(0xbd586281), WTC0(0xbd613d81), WTC0(0xbd6a2201), WTC0(0xbd731081), WTC0(0xbd7c0781), WTC0(0xbd850701), WTC0(0xbd8e0e01), 
  WTC0(0xbd971c81), WTC0(0xbda03381), WTC0(0xbda95301), WTC0(0xbdb27b01), WTC0(0xbdbbab01), WTC0(0xbdc4e301), WTC0(0xbdce2181), WTC0(0xbdd76701), 
  WTC0(0xbde0b301), WTC0(0xbdea0681), WTC0(0xbdf36101), WTC0(0xbdfcc301), WTC0(0xbe062b81), WTC0(0xbe0f9a01), WTC0(0xbe190d81), WTC0(0xbe228681), 
  WTC0(0xbe2c0501), WTC0(0xbe358901), WTC0(0xbe3f1381), WTC0(0xbe48a301), WTC0(0xbe523781), WTC0(0xbe5bd001), WTC0(0xbe656c01), WTC0(0xbe6f0c01), 
  WTC0(0xbe78b001), WTC0(0xbe825801), WTC0(0xbe8c0501), WTC0(0xbe95b581), WTC0(0xbe9f6901), WTC0(0xbea91f01), WTC0(0xbeb2d681), WTC0(0xbebc9181), 
  WTC0(0xbec64e81), WTC0(0xbed00f81), WTC0(0xbed9d281), WTC0(0xbee39801), WTC0(0xbeed5f01), WTC0(0xbef72681), WTC0(0xbf00ef81), WTC0(0xbf0aba01), 
  WTC0(0xbf148681), WTC0(0xbf1e5501), WTC0(0xbf282501), WTC0(0xbf31f501), WTC0(0xbf3bc601), WTC0(0xbf459681), WTC0(0xbf4f6801), WTC0(0xbf593a01), 
  WTC0(0xbf630d81), WTC0(0xbf6ce201), WTC0(0xbf76b701), WTC0(0xbf808b81), WTC0(0xbf8a5f81), WTC0(0xbf943301), WTC0(0xbf9e0701), WTC0(0xbfa7dc01), 
  WTC0(0xbfb1b101), WTC0(0xbfbb8701), WTC0(0xbfc55c81), WTC0(0xbfcf3181), WTC0(0xbfd90601), WTC0(0xbfe2d901), WTC0(0xbfecaa81), WTC0(0xbff67a01), 
  /* part 1 */
  WTC1(0x80130981), WTC1(0x80269f81), WTC1(0x803a3381), WTC1(0x804dc481), WTC1(0x80615281), WTC1(0x8074dc01), WTC1(0x80886081), WTC1(0x809bdf01), 
  WTC1(0x80af5701), WTC1(0x80c2c781), WTC1(0x80d63101), WTC1(0x80e99401), WTC1(0x80fcf181), WTC1(0x81104a01), WTC1(0x81239d81), WTC1(0x8136ea01), 
  WTC1(0x814a2f81), WTC1(0x815d6c01), WTC1(0x8170a181), WTC1(0x8183cf81), WTC1(0x8196f781), WTC1(0x81aa1981), WTC1(0x81bd3401), WTC1(0x81d04681), 
  WTC1(0x81e34f81), WTC1(0x81f64f01), WTC1(0x82094581), WTC1(0x821c3401), WTC1(0x822f1b01), WTC1(0x8241fa01), WTC1(0x8254cf01), WTC1(0x82679901), 
  WTC1(0x827a5801), WTC1(0x828d0b01), WTC1(0x829fb401), WTC1(0x82b25301), WTC1(0x82c4e801), WTC1(0x82d77201), WTC1(0x82e9ef01), WTC1(0x82fc5f01), 
  WTC1(0x830ec081), WTC1(0x83211501), WTC1(0x83335c81), WTC1(0x83459881), WTC1(0x8357c701), WTC1(0x8369e781), WTC1(0x837bf801), WTC1(0x838df801), 
  WTC1(0x839fe801), WTC1(0x83b1c881), WTC1(0x83c39a81), WTC1(0x83d55d01), WTC1(0x83e70f01), WTC1(0x83f8b001), WTC1(0x840a3e81), WTC1(0x841bb981), 
  WTC1(0x842d2281), WTC1(0x843e7a81), WTC1(0x844fc081), WTC1(0x8460f581), WTC1(0x84721701), WTC1(0x84832481), WTC1(0x84941d81), WTC1(0x84a50201), 
  WTC1(0x84b5d301), WTC1(0x84c69101), WTC1(0x84d73c01), WTC1(0x84e7d381), WTC1(0x84f85581), WTC1(0x8508c181), WTC1(0x85191801), WTC1(0x85295881), 
  WTC1(0x85398481), WTC1(0x85499d01), WTC1(0x8559a081), WTC1(0x85698e81), WTC1(0x85796601), WTC1(0x85892681), WTC1(0x8598d081), WTC1(0x85a86581), 
  WTC1(0x85b7e601), WTC1(0x85c75201), WTC1(0x85d6a981), WTC1(0x85e5eb81), WTC1(0x85f51681), WTC1(0x86042c01), WTC1(0x86132c01), WTC1(0x86221801), 
  WTC1(0x8630f181), WTC1(0x863fb701), WTC1(0x864e6901), WTC1(0x865d0581), WTC1(0x866b8d81), WTC1(0x867a0081), WTC1(0x86886001), WTC1(0x8696ad01), 
  WTC1(0x86a4e781), WTC1(0x86b30f01), WTC1(0x86c12401), WTC1(0x86cf2601), WTC1(0x86dd1481), WTC1(0x86eaf081), WTC1(0x86f8ba81), WTC1(0x87067281), 
  WTC1(0x87141b01), WTC1(0x8721b481), WTC1(0x872f4201), WTC1(0x873cc201), WTC1(0x874a2f01), WTC1(0x87578181), WTC1(0x8764b101), WTC1(0x8771c601), 
  WTC1(0x877ede01), WTC1(0x878c1881), WTC1(0x87998f01), WTC1(0x87a70e81), WTC1(0x87b42481), WTC1(0x87c05e81), WTC1(0x87cb5101), WTC1(0x87d4ac81), 
  WTC1(0x87e73d81), WTC1(0x88124281), WTC1(0x88353501), WTC1(0x885f8481), WTC1(0x888d3181), WTC1(0x88be1681), WTC1(0x88f13801), WTC1(0x8925f101), 
  WTC1(0x895bcd01), WTC1(0x89925a81), WTC1(0x89c92f81), WTC1(0x8a001f01), WTC1(0x8a372881), WTC1(0x8a6e4a01), WTC1(0x8aa58681), WTC1(0x8adcee01), 
  WTC1(0x8b149701), WTC1(0x8b4c9701), WTC1(0x8b850281), WTC1(0x8bbde981), WTC1(0x8bf75b01), WTC1(0x8c316681), WTC1(0x8c6c1b01), WTC1(0x8ca78781), 
  WTC1(0x8ce3ba81), WTC1(0x8d20c301), WTC1(0x8d5eaa01), WTC1(0x8d9d7781), WTC1(0x8ddd3201), WTC1(0x8e1de001), WTC1(0x8e5f8881), WTC1(0x8ea23201), 
  WTC1(0x8ee5e301), WTC1(0x8f2aa101), WTC1(0x8f706f01), WTC1(0x8fb74f81), WTC1(0x8fff4601), WTC1(0x90485401), WTC1(0x90927b81), WTC1(0x90ddc001), 
  WTC1(0x912a2201), WTC1(0x9177a301), WTC1(0x91c64301), WTC1(0x92160301), WTC1(0x9266e281), WTC1(0x92b8e101), WTC1(0x930bff81), WTC1(0x93603d01), 
  WTC1(0x93b59901), WTC1(0x940c1281), WTC1(0x9463a881), WTC1(0x94bc5981), WTC1(0x95162381), WTC1(0x95710601), WTC1(0x95ccff01), WTC1(0x962a0c81), 
  WTC1(0x96882e01), WTC1(0x96e76101), WTC1(0x9747a481), WTC1(0x97a8f681), WTC1(0x980b5501), WTC1(0x986ebd81), WTC1(0x98d32d81), WTC1(0x9938a281), 
  WTC1(0x999f1981), WTC1(0x9a069001), WTC1(0x9a6f0381), WTC1(0x9ad87081), WTC1(0x9b42d581), WTC1(0x9bae2f81), WTC1(0x9c1a7c81), WTC1(0x9c87ba81), 
  WTC1(0x9cf5e701), WTC1(0x9d650081), WTC1(0x9dd50481), WTC1(0x9e45f081), WTC1(0x9eb7c101), WTC1(0x9f2a7281), WTC1(0x9f9e0301), WTC1(0xa0127081), 
  WTC1(0xa087b981), WTC1(0xa0fddd81), WTC1(0xa174da81), WTC1(0xa1ecae01), WTC1(0xa2655581), WTC1(0xa2dece81), WTC1(0xa3591801), WTC1(0xa3d43001), 
  WTC1(0xa4501601), WTC1(0xa4ccc901), WTC1(0xa54a4701), WTC1(0xa5c89001), WTC1(0xa647a301), WTC1(0xa6c77e01), WTC1(0xa7482101), WTC1(0xa7c98b01), 
  WTC1(0xa84bbb81), WTC1(0xa8ceb201), WTC1(0xa9526d81), WTC1(0xa9d6ef01), WTC1(0xaa5c3601), WTC1(0xaae24301), WTC1(0xab691681), WTC1(0xabf0b181), 
  WTC1(0xac791401), WTC1(0xad023f01), WTC1(0xad8c3301), WTC1(0xae16f001), WTC1(0xaea27681), WTC1(0xaf2ec901), WTC1(0xafbbe801), WTC1(0xb049d601), 
  WTC1(0xb0d89401), WTC1(0xb1682281), WTC1(0xb1f88181), WTC1(0xb289b181), WTC1(0xb31bb301), WTC1(0xb3ae8601), WTC1(0xb4422b81), WTC1(0xb4d6a381), 
  WTC1(0x4a5a327f), WTC1(0x49c4adff), WTC1(0x492e637f), WTC1(0x48974f7f), WTC1(0x47ff6d7f), WTC1(0x4766baff), WTC1(0x46cd35ff), WTC1(0x4632dd7f), 
  WTC1(0x4597b0ff), WTC1(0x44fbb1ff), WTC1(0x445eeaff), WTC1(0x43c165ff), WTC1(0x4323227f), WTC1(0x4284277f), WTC1(0x41e48aff), WTC1(0x4144557f), 
  WTC1(0x40a3867f), WTC1(0x4001f5ff), WTC1(0x3f5f5d80), WTC1(0x3ebbad00), WTC1(0x3e16ee40), WTC1(0x3d713d00), WTC1(0x3ccab700), WTC1(0x3c236500), 
  WTC1(0x3b7b5800), WTC1(0x3ad2ecc0), WTC1(0x3a2a6540), WTC1(0x3981b7c0), WTC1(0x38d8ba00), WTC1(0x382f01c0), WTC1(0x37846240), WTC1(0x36d8eb00), 
  WTC1(0x362c9ec0), WTC1(0x357f7a00), WTC1(0x34d18340), WTC1(0x3422c900), WTC1(0x33736c40), WTC1(0x32c39040), WTC1(0x32134280), WTC1(0x31629280), 
  WTC1(0x30b1a000), WTC1(0x30008380), WTC1(0x2f4f4240), WTC1(0x2e9df180), WTC1(0x2decc780), WTC1(0x2d3bd640), WTC1(0x2c8b0cc0), WTC1(0x2bda3080), 
  WTC1(0x2b28ec80), WTC1(0x2a773500), WTC1(0x29c51b40), WTC1(0x291293c0), WTC1(0x285f9280), WTC1(0x27ac35c0), WTC1(0x26f8ab40), WTC1(0x26454c00), 
  WTC1(0x25925600), WTC1(0x24dfd580), WTC1(0x242ddd40), WTC1(0x237c87c0), WTC1(0x22cbe240), WTC1(0x221bef40), WTC1(0x216cb040), WTC1(0x20be2800), 
  WTC1(0x20105c80), WTC1(0x1f6352a0), WTC1(0x1eb71240), WTC1(0x1e0ba140), WTC1(0x1d60fe40), WTC1(0x1cb723e0), WTC1(0x1c0e0300), WTC1(0x1b6596c0), 
  WTC1(0x1abde8a0), WTC1(0x1a16fbe0), WTC1(0x1970c680), WTC1(0x18cb4840), WTC1(0x18268e20), WTC1(0x1782a0c0), WTC1(0x16df8960), WTC1(0x163d6300), 
  WTC1(0x159c52c0), WTC1(0x14fc87e0), WTC1(0x145e2c80), WTC1(0x13c15b60), WTC1(0x13263240), WTC1(0x128cd9a0), WTC1(0x11f562a0), WTC1(0x115fc1c0), 
  WTC1(0x10cbf160), WTC1(0x1039f200), WTC1(0x0fa9a080), WTC1(0x0f1abd90), WTC1(0x0e8d01d0), WTC1(0x0e003330), WTC1(0x0d743590), WTC1(0x0ce8ef40), 
  WTC1(0x0c5e1900), WTC1(0x0bd35d70), WTC1(0x0b488eb0), WTC1(0x0abd8410), WTC1(0x0a320a00), WTC1(0x09a60e70), WTC1(0x0919ab00), WTC1(0x088d0de0), 
  WTC1(0x080065e0), WTC1(0x07739710), WTC1(0x06e65808), WTC1(0x06588348), WTC1(0x05ca0ae0), WTC1(0x053aaaf8), WTC1(0x04a9faf0), WTC1(0x0417f698), 
  WTC1(0x03859ff4), WTC1(0x02f49be4), WTC1(0x0266b668), WTC1(0x01de554e), WTC1(0x015f50ca), WTC1(0x00eb7e5d), WTC1(0x00904f24), WTC1(0x00212889), 
  WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), 
  WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), 
  WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), 
  WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), 
  WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), 
  WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), 
  WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), 
  WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), 
  WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), 
  WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), 
  WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), 
  WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), 
  WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), 
  WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), 
  WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), WTC1(0x00000000), 
  /* part 2 */
  WTC2(0xfffece02), WTC2(0xffe4c3df), WTC2(0xffcaaa55), WTC2(0xffb087d1), WTC2(0xff9662bf), WTC2(0xff7c418b), WTC2(0xff622aa0), WTC2(0xff48246c), 
  WTC2(0xff2e355a), WTC2(0xff1463db), WTC2(0xfefab608), WTC2(0xfee12f0a), WTC2(0xfec7cfd2), WTC2(0xfeae995a), WTC2(0xfe958cc4), WTC2(0xfe7cabce), 
  WTC2(0xfe63f882), WTC2(0xfe4b74e0), WTC2(0xfe3322f6), WTC2(0xfe1b04dc), WTC2(0xfe031ccc), WTC2(0xfdeb6cf0), WTC2(0xfdd3ff7c), WTC2(0xfdbce834), 
  WTC2(0xfda63bb8), WTC2(0xfd900c68), WTC2(0xfd7a590c), WTC2(0xfd6511b4), WTC2(0xfd5026c0), WTC2(0xfd3b8954), WTC2(0xfd272df0), WTC2(0xfd130adc), 
  WTC2(0xfcff15ac), WTC2(0xfceb4a68), WTC2(0xfcd7b110), WTC2(0xfcc454d0), WTC2(0xfcb14064), WTC2(0xfc9e896c), WTC2(0xfc8c5264), WTC2(0xfc7abef0), 
  WTC2(0xfc69f078), WTC2(0xfc59f5e8), WTC2(0xfc4acfec), WTC2(0xfc3c8060), WTC2(0xfc2f0264), WTC2(0xfc223b7c), WTC2(0xfc160714), WTC2(0xfc0a4150), 
  WTC2(0xfbfec920), WTC2(0xfbf38320), WTC2(0xfbe855d0), WTC2(0xfbdd2740), WTC2(0xfbd1fc68), WTC2(0xfbc6fea0), WTC2(0xfbbc5a48), WTC2(0xfbb23b48), 
  WTC2(0xfba8ca78), WTC2(0xfba02e50), WTC2(0xfb988de0), WTC2(0xfb920b40), WTC2(0xfb8cb870), WTC2(0xfb889f68), WTC2(0xfb85cbe8), WTC2(0xfb843dd0), 
  WTC2(0xfb83df78), WTC2(0xfb8495d0), WTC2(0xfb864660), WTC2(0xfb88d4a8), WTC2(0xfb8c21e8), WTC2(0xfb900f28), WTC2(0xfb947dc0), WTC2(0xfb9950c0), 
  WTC2(0xfb9e6d08), WTC2(0xfba3b658), WTC2(0xfba91908), WTC2(0xfbae9e08), WTC2(0xfbb45bd0), WTC2(0xfbba66f8), WTC2(0xfbc0dcf0), WTC2(0xfbc7ead8), 
  WTC2(0xfbcfc200), WTC2(0xfbd89330), WTC2(0xfbe294d0), WTC2(0xfbee03d0), WTC2(0xfbfb1de8), WTC2(0xfc0a1da4), WTC2(0xfc1b22e0), WTC2(0xfc2e38f0), 
  WTC2(0xfc436d48), WTC2(0xfc5abf7c), WTC2(0xfc74024c), WTC2(0xfc8ef2e8), WTC2(0xfcab51ac), WTC2(0xfcc8d024), WTC2(0xfce704f0), WTC2(0xfd0580cc), 
  WTC2(0xfd23d4d0), WTC2(0xfd41ce40), WTC2(0xfd5f81b0), WTC2(0xfd7d08f0), WTC2(0xfd9a8560), WTC2(0xfdb85938), WTC2(0xfdd71798), WTC2(0xfdf753b8), 
  WTC2(0xfe1993ee), WTC2(0xfe3e30f8), WTC2(0xfe656cba), WTC2(0xfe8f8fdc), WTC2(0xfebca8a4), WTC2(0xfeec590e), WTC2(0xff1e285c), WTC2(0xff51a0b7), 
  WTC2(0xff866330), WTC2(0xffbc2cbb), WTC2(0xfff2bbff), WTC2(0x0029d79d), WTC2(0x00618a22), WTC2(0x009a1185), WTC2(0x00d3aa8c), WTC2(0x010e8ff6), 
  WTC2(0x014af29e), WTC2(0x0188fe56), WTC2(0x01c8e108), WTC2(0x020ab3c4), WTC2(0x024e68a8), WTC2(0x0293e824), WTC2(0x02db1bc8), WTC2(0x0323f1a4), 
  WTC2(0x036e5d6c), WTC2(0x03ba5320), WTC2(0x0407c938), WTC2(0x0456cad0), WTC2(0x04a77288), WTC2(0x04f9db88), WTC2(0x054e1888), WTC2(0x05a41ef0), 
  WTC2(0x05fbd6e0), WTC2(0x065528c0), WTC2(0x06b00838), WTC2(0x070c7ee0), WTC2(0x076a9bb0), WTC2(0x07ca6d10), WTC2(0x082c08e0), WTC2(0x088f8da0), 
  WTC2(0x08f51ac0), WTC2(0x095ccc20), WTC2(0x09c69f70), WTC2(0x0a327b40), WTC2(0x0aa046d0), WTC2(0x0b0febb0), WTC2(0x0b815dd0), WTC2(0x0bf49600), 
  WTC2(0x0c698c50), WTC2(0x0ce03ba0), WTC2(0x0d58a380), WTC2(0x0dd2c510), WTC2(0x0e4ea110), WTC2(0x0ecc3dd0), WTC2(0x0f4ba800), WTC2(0x0fcced10), 
  WTC2(0x10501960), WTC2(0x10d532a0), WTC2(0x115c39c0), WTC2(0x11e52fa0), WTC2(0x12701560), WTC2(0x12fcef20), WTC2(0x138bc200), WTC2(0x141c9300), 
  WTC2(0x14af64a0), WTC2(0x154434e0), WTC2(0x15db0020), WTC2(0x1673c360), WTC2(0x170e7e80), WTC2(0x17ab35e0), WTC2(0x1849ee40), WTC2(0x18eaaba0), 
  WTC2(0x198d6f00), WTC2(0x1a3236a0), WTC2(0x1ad90080), WTC2(0x1b81cc60), WTC2(0x1c2c9da0), WTC2(0x1cd97980), WTC2(0x1d8865c0), WTC2(0x1e396540), 
  WTC2(0x1eec7700), WTC2(0x1fa198c0), WTC2(0x2058c840), WTC2(0x21120640), WTC2(0x21cd5700), WTC2(0x228abec0), WTC2(0x234a4180), WTC2(0x240bdf80), 
  WTC2(0x24cf95c0), WTC2(0x259561c0), WTC2(0x265d4200), WTC2(0x27273840), WTC2(0x27f348c0), WTC2(0x28c17700), WTC2(0x2991c500), WTC2(0x2a643080), 
  WTC2(0x2b38b680), WTC2(0x2c0f53c0), WTC2(0x2ce80840), WTC2(0x2dc2d680), WTC2(0x2e9fc100), WTC2(0x2f7ecac0), WTC2(0x305ff280), WTC2(0x314334c0), 
  WTC2(0x32288e00), WTC2(0x330ffb80), WTC2(0x33f97d80), WTC2(0x34e515c0), WTC2(0x35d2c5c0), WTC2(0x36c28d00), WTC2(0x37b467c0), WTC2(0x38a85080), 
  WTC2(0x399e4240), WTC2(0x3a963a00), WTC2(0x3b903600), WTC2(0x3c8c3480), WTC2(0x3d8a3380), WTC2(0x3e8a2dc0), WTC2(0x3f8c1b40), WTC2(0x408ff2ff), 
  WTC2(0x4195ae7f), WTC2(0x429d477f), WTC2(0x43a6b87f), WTC2(0x44b1fdff), WTC2(0x45bf11ff), WTC2(0x46cdee7f), WTC2(0x47de8cff), WTC2(0x48f0e77f), 
  WTC2(0x4a050eff), WTC2(0x4b1b2dff), WTC2(0x4c3372ff), WTC2(0x4d4e0bff), WTC2(0x4e6b257f), WTC2(0x4f8aedff), WTC2(0x50ad92ff), WTC2(0x51d341ff), 
  WTC2(0x002006a9), WTC2(0x000bfb36), WTC2(0xfffe45ac), WTC2(0xfff6d064), WTC2(0xfff585bc), WTC2(0xfffa500d), WTC2(0x000519b4), WTC2(0x0015cd0c), 
  WTC2(0x002c5470), WTC2(0x00489a3b), WTC2(0x006a88c8), WTC2(0x00920a74), WTC2(0x00bf0999), WTC2(0x00f17092), WTC2(0x012929bc), WTC2(0x01661f70), 
  WTC2(0x01a83c0c), WTC2(0x01ef69e8), WTC2(0x023b9364), WTC2(0x028ca2d4), WTC2(0x02e2829c), WTC2(0x033d1d10), WTC2(0x039c5c90), WTC2(0x04002b78), 
  WTC2(0x04687418), WTC2(0x04d520e0), WTC2(0x05461c18), WTC2(0x05bb5020), WTC2(0x0634a758), WTC2(0x06b20c20), WTC2(0x073368c8), WTC2(0x07b8a7b0), 
  WTC2(0x0841b340), WTC2(0x08ce75b0), WTC2(0x095ed980), WTC2(0x09f2c900), WTC2(0x0a8a2e80), WTC2(0x0b24f470), WTC2(0x0bc30510), WTC2(0x0c644ad0), 
  WTC2(0x0d08b010), WTC2(0x0db01f10), WTC2(0x0e5a8250), WTC2(0x0f07c400), WTC2(0x0fb7cea0), WTC2(0x106a8c80), WTC2(0x111fe800), WTC2(0x11d7cb60), 
  WTC2(0x12922120), WTC2(0x134ed3a0), WTC2(0x140dcd00), WTC2(0x14cef7e0), WTC2(0x15923e60), WTC2(0x16578b00), WTC2(0x171ec820), WTC2(0x17e7e020), 
  WTC2(0x18b2bd20), WTC2(0x197f49c0), WTC2(0x1a4d7040), WTC2(0x1b1d1b00), WTC2(0x1bee3460), WTC2(0x1cc0a6a0), WTC2(0x1d945c40), WTC2(0x1e693f80), 
  WTC2(0x1f3f3ac0), WTC2(0x20163880), WTC2(0x20ee22c0), WTC2(0x21c6e440), WTC2(0x22a06740), WTC2(0x237a9600), WTC2(0x24555ac0), WTC2(0x2530a040), 
  WTC2(0x260c5080), WTC2(0x26e85600), WTC2(0x27c49b00), WTC2(0x28a10a00), WTC2(0x297d8d80), WTC2(0x2a5a0f80), WTC2(0x2b367a80), WTC2(0x2c12b8c0), 
  WTC2(0x2ceeb500), WTC2(0x2dca5940), WTC2(0x2ea58fc0), WTC2(0x2f804340), WTC2(0x305a5dc0), WTC2(0x3133ca00), WTC2(0x320c7200), WTC2(0x32e44000), 
  WTC2(0x33bb1ec0), WTC2(0x3490f880), WTC2(0x3565b7c0), WTC2(0x36394640), WTC2(0x370b8f00), WTC2(0x37dc7c00), WTC2(0x38abf7c0), WTC2(0x3979ecc0), 
  WTC2(0x3a464500), WTC2(0x3b10eb00), WTC2(0x3bd9c940), WTC2(0x3ca0c9c0), WTC2(0x3d65d740), WTC2(0x3e28dc00), WTC2(0x3ee9c240), WTC2(0x3fa87480), 
  WTC2(0x4064dcff), WTC2(0x411ee67f), WTC2(0x41d67a7f), WTC2(0x428b847f), WTC2(0x433ded7f), WTC2(0x43eda0ff), WTC2(0x449a887f), WTC2(0x45448f7f), 
  WTC2(0x45eb9eff), WTC2(0x468fa1ff), WTC2(0x473082ff), WTC2(0x47ce2c7f), WTC2(0x4868887f), WTC2(0x48ff80ff), WTC2(0x499300ff), WTC2(0x4a22f2ff), 
  WTC2(0x4aaf407f), WTC2(0x4b37d47f), WTC2(0x4bbc997f), WTC2(0x4c3d78ff), WTC2(0x4cba5e7f), WTC2(0x4d33337f), WTC2(0x4da7e27f), WTC2(0x4e18567f), 
  WTC2(0x4e8478ff), WTC2(0x4eec347f), WTC2(0x4f4f737f), WTC2(0x4fae20ff), WTC2(0x500825ff), WTC2(0x505d6dff), WTC2(0x50ade37f), WTC2(0x50f96f7f), 
  WTC2(0x513ffdff), WTC2(0x518177ff), WTC2(0x51bdc87f), WTC2(0x51f4d9ff), WTC2(0x5226967f), WTC2(0x5252e87f), WTC2(0x5279b9ff), WTC2(0x529af5ff), 
  WTC2(0x52b6867f), WTC2(0x52cc55ff), WTC2(0x52dc4eff), WTC2(0x52e65aff), WTC2(0x52ea657f), WTC2(0x52e857ff), WTC2(0x52e01d7f), WTC2(0x52d19fff), 
  WTC2(0x52bcc9ff), WTC2(0x52a1857f), WTC2(0x527fbd7f), WTC2(0x52575b7f), WTC2(0x52284a7f), WTC2(0x51f274ff), WTC2(0x51b5c47f), WTC2(0x5172247f), 
  WTC2(0x51277dff), WTC2(0x50d5bc7f), WTC2(0x507cc9ff), WTC2(0x501c90ff), WTC2(0x4fb4fb7f), WTC2(0x4f45f3ff), WTC2(0x4ecf64ff), WTC2(0x4e5138ff), 
  WTC2(0x4dcb597f), WTC2(0x4d3db1ff), WTC2(0x4ca82bff), WTC2(0x4c0ab27f), WTC2(0x4b652f7f), WTC2(0x4ab78d7f), WTC2(0x4a01b67f), WTC2(0x4943957f), 
  WTC2(0x487d12ff), WTC2(0x47ae1f7f), WTC2(0x46d68f7f), WTC2(0x45f7187f), WTC2(0x4513597f), WTC2(0x4430467f), WTC2(0x4352d2ff), WTC2(0x427e6bff), 
  WTC2(0x41b390ff), WTC2(0x40f2077f), WTC2(0x4039a87f), WTC2(0x3f8a3100), WTC2(0x3ee33e00), WTC2(0x3e446ac0), WTC2(0x3dad5180), WTC2(0x3d1d7fc0), 
  WTC2(0x3c947b00), WTC2(0x3c11c7c0), WTC2(0x3b94ebc0), WTC2(0x3b1d6dc0), WTC2(0x3aaad480), WTC2(0x3a3ca740), WTC2(0x39d26c40), WTC2(0x396ba8c0), 
  WTC2(0x3907e080), WTC2(0x38a69800), WTC2(0x38473d80), WTC2(0x37e923c0), WTC2(0x378b9b80), WTC2(0x372e0380), WTC2(0x36d03a80), WTC2(0x36727f00), 
  WTC2(0x36150e40), WTC2(0x35b81540), WTC2(0x355b8000), WTC2(0x34ff1dc0), WTC2(0x34a2bfc0), WTC2(0x34463e80), WTC2(0x33e982c0), WTC2(0x338c7880), 
  WTC2(0x332f0bc0), WTC2(0x32d11800), WTC2(0x327265c0), WTC2(0x3212bbc0), WTC2(0x31b1e740), WTC2(0x314fef00), WTC2(0x30ed0540), WTC2(0x30895c80), 
  WTC2(0x30251880), WTC2(0x2fc02880), WTC2(0x2f5a6480), WTC2(0x2ef3a480), WTC2(0x2e8bd640), WTC2(0x2e231100), WTC2(0x2db97680), WTC2(0x2d4f2700), 
  WTC2(0x2ce431c0), WTC2(0x2c789080), WTC2(0x2c0c3bc0), WTC2(0x2b9f2bc0), WTC2(0x2b315940), WTC2(0x2ac2bc00), WTC2(0x2a534cc0), WTC2(0x29e303c0)
};


