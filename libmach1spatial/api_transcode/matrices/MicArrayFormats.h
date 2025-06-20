//  Mach1 Spatial SDK
//  Copyright © 2017-2022 Mach1. All rights reserved.
//
// References:
//

#pragma once

#include "Mach1TranscodeConstants.h"

/*
 * Description of matrices
 * - Inputs are rows
 * - Outputs are columns
 *
 *  I1 -> | I1 | I2 | I3 | I4 |
 *  I2 -> | I1 | I2 | I3 | I4 |
 *  I3 -> | I1 | I2 | I3 | I4 |
 *          ↓    ↓    ↓    ↓
 *          O1   O2   O3   O4
 */

/*
 * Multichannel Format/Configuration Naming Conventions:
 * - SIM or Simulated      = `Simulated Room Mix` (Lessens the divergence of virtual speakers to quickly simulate hearing front/back soundfield within a real world listening environment)
 */

namespace Mach1TranscodeConstants {
class MatricesMicArray {
  public:
    static std::vector<Mach1TranscodeMatrix> getData() {
        return {
            {"1.0", "M1Spatial-8", {
                                       Mach1TranscodeChannel::Panner(0, 0, 1, {-6, true}),
                                   }},
            {"Ambeo", "M1Spatial-8", {
                                         Mach1TranscodeChannel::Coeffs({1.70710599f, -0.001728f, 0.000685f, -0.31546f, 0.005525f, -0.317307f, -0.311014f, -0.037532f}),
                                         Mach1TranscodeChannel::Coeffs({-0.31979799f, 0.024362f, -0.083181f, -0.31374699f, -0.004715f, 1.77277803f, -0.32243899f, -0.001391f}),
                                         Mach1TranscodeChannel::Coeffs({-0.17735f, -0.32358f, -0.19385999f, -0.18116499f, -0.219145f, -0.18212f, 1.787938f, -0.223065f}),
                                         Mach1TranscodeChannel::Coeffs({-0.32679999f, -0.002112f, 0.003135f, 1.71765006f, -0.03372f, -0.32499999f, -0.31977001f, 0.0062f}),
                                     }},
            {"SPS-200", "M1Spatial-8", {
                                           Mach1TranscodeChannel::Coeffs({1.70000899f, 0.000097f, -0.005204f, -0.36458799f, 0.022792f, -0.360383f, -0.36231801f, -0.037654f}),
                                           Mach1TranscodeChannel::Coeffs({-0.36043099f, 0.022792f, -0.037529f, -0.36238801f, 0.000104f, 1.70005906f, -0.36461499f, -0.005212f}),
                                           Mach1TranscodeChannel::Coeffs({-0.199072f, -0.33234701f, -0.218069f, -0.19542401f, -0.24213f, -0.199532f, 1.70079505f, -0.23848f}),
                                           Mach1TranscodeChannel::Coeffs({-0.198948f, -0.238601f, -0.243719f, 1.70087302f, -0.33212301f, -0.196025f, -0.198909f, -0.21687099f}),
                                       }},
            {"TetraMic", "M1Spatial-8", {
                                            Mach1TranscodeChannel::Coeffs({2.36442399f, -0.000757f, -0.005645f, -0.52798003f, 0.033468f, -0.51843101f, -0.52326399f, -0.087658f}),
                                            Mach1TranscodeChannel::Coeffs({-0.52781701f, 0.031688f, -0.087271f, -0.51984698f, -0.005087f, 2.36920309f, -0.52968502f, -0.002606f}),
                                            Mach1TranscodeChannel::Coeffs({-0.27798f, -0.53587401f, -0.308456f, -0.27288699f, -0.339082f, -0.28054601f, 2.35262704f, -0.33451f}),
                                            Mach1TranscodeChannel::Coeffs({-0.706792f, -0.008215f, -0.000956f, 2.53077793f, 0.246269f, -0.69186598f, -0.68480098f, 0.035837f}),
                                        }},
            {"NT-SF1", "M1Spatial-8", {
                                          Mach1TranscodeChannel::Coeffs({1.61973f, -0.000089f, -0.003503f, -0.32746801f, 0.022924f, -0.32106999f, -0.32451501f, -0.03195f}),
                                          Mach1TranscodeChannel::Coeffs({-0.32611799f, 0.021812f, -0.033303f, -0.321188f, -0.003148f, 1.62159598f, -0.32668999f, -0.001212f}),
                                          Mach1TranscodeChannel::Coeffs({-0.178378f, -0.29723999f, -0.19407f, -0.174988f, -0.217002f, -0.178848f, 1.62242603f, -0.213596f}),
                                          Mach1TranscodeChannel::Coeffs({-0.178344f, -0.21367501f, -0.21852399f, 1.62255704f, -0.296904f, -0.175521f, -0.178372f, -0.193075f}),
                                      }},
            {"CoreSound-OctoMic", "M1Spatial-14", {
                                                      Mach1TranscodeChannel::Coeffs({0.436369f, 0.436369f, 0.000554f, 0.000554f, 0.002443f, 0.002443f, 0.000031f, 0.000031f, 0.107056f, 0.000510f, 0.000022f, 0.000510f, 0.013067f, 0.000041f}),
                                                      Mach1TranscodeChannel::Coeffs({0.020883f, 0.000114f, 0.000114f, 0.000007f, 0.798890f, 0.020860f, 0.020860f, 0.000114f, 0.046047f, 0.000030f, 0.000030f, 0.046047f, 0.000030f, 0.045976f}),
                                                      Mach1TranscodeChannel::Coeffs({0.436369f, 0.000554f, 0.436369f, 0.000554f, 0.002443f, 0.000031f, 0.002443f, 0.000031f, 0.000510f, 0.000022f, 0.000510f, 0.107056f, 0.013067f, 0.000041f}),
                                                      Mach1TranscodeChannel::Coeffs({0.000114f, 0.000007f, 0.020883f, 0.000114f, 0.020860f, 0.000114f, 0.798890f, 0.020860f, 0.000030f, 0.000030f, 0.046047f, 0.046047f, 0.000030f, 0.045976f}),
                                                      Mach1TranscodeChannel::Coeffs({0.000554f, 0.000554f, 0.436369f, 0.436369f, 0.000031f, 0.000031f, 0.002443f, 0.002443f, 0.000022f, 0.000510f, 0.107056f, 0.000510f, 0.013067f, 0.000041f}),
                                                      Mach1TranscodeChannel::Coeffs({0.000007f, 0.000114f, 0.000114f, 0.020883f, 0.000114f, 0.020860f, 0.020860f, 0.798890f, 0.000030f, 0.046047f, 0.046047f, 0.000030f, 0.000030f, 0.045976f}),
                                                      Mach1TranscodeChannel::Coeffs({0.000554f, 0.436369f, 0.000554f, 0.436369f, 0.000031f, 0.002443f, 0.000031f, 0.002443f, 0.000510f, 0.107056f, 0.000510f, 0.000022f, 0.013067f, 0.000041f}),
                                                      Mach1TranscodeChannel::Coeffs({0.000114f, 0.020883f, 0.000007f, 0.000114f, 0.020860f, 0.798890f, 0.000114f, 0.020860f, 0.046047f, 0.046047f, 0.000030f, 0.000030f, 0.000030f, 0.045976f}),
                                                  }},
            {"CoreSound-OctoMic_SIM", "M1Spatial-14", {
                                                          Mach1TranscodeChannel::Coeffs({0.294030f, 0.294030f, 0.005892f, 0.005892f, 0.018085f, 0.018085f, 0.000757f, 0.000757f, 0.293813f, 0.006858f, 0.000572f, 0.006858f, 0.053373f, 0.001001f}),
                                                          Mach1TranscodeChannel::Coeffs({0.061595f, 0.004814f, 0.004814f, 0.000464f, 0.369136f, 0.061569f, 0.061569f, 0.004812f, 0.142172f, 0.001598f, 0.001598f, 0.142172f, 0.001598f, 0.142086f}),
                                                          Mach1TranscodeChannel::Coeffs({0.294030f, 0.005892f, 0.294030f, 0.005892f, 0.018085f, 0.000757f, 0.018085f, 0.000757f, 0.006858f, 0.000572f, 0.006858f, 0.293813f, 0.053373f, 0.001001f}),
                                                          Mach1TranscodeChannel::Coeffs({0.004814f, 0.000464f, 0.061595f, 0.004814f, 0.061569f, 0.004812f, 0.369136f, 0.061569f, 0.001598f, 0.001598f, 0.142172f, 0.142172f, 0.001598f, 0.142086f}),
                                                          Mach1TranscodeChannel::Coeffs({0.005892f, 0.005892f, 0.294030f, 0.294030f, 0.000757f, 0.000757f, 0.018085f, 0.018085f, 0.000572f, 0.006858f, 0.293813f, 0.006858f, 0.053373f, 0.001001f}),
                                                          Mach1TranscodeChannel::Coeffs({0.000464f, 0.004814f, 0.004814f, 0.061595f, 0.004812f, 0.061569f, 0.061569f, 0.369136f, 0.001598f, 0.142172f, 0.142172f, 0.001598f, 0.001598f, 0.142086f}),
                                                          Mach1TranscodeChannel::Coeffs({0.005892f, 0.294030f, 0.005892f, 0.294030f, 0.000757f, 0.018085f, 0.000757f, 0.018085f, 0.006858f, 0.293813f, 0.006858f, 0.000572f, 0.053373f, 0.001001f}),
                                                          Mach1TranscodeChannel::Coeffs({0.004814f, 0.061595f, 0.000464f, 0.004814f, 0.061569f, 0.369136f, 0.004812f, 0.061569f, 0.142172f, 0.142172f, 0.001598f, 0.001598f, 0.001598f, 0.142086f}),
                                                      }},
            {// Zylia ZM-1
             "ZM-1",
             "M1Spatial-14",
             {
                 Mach1TranscodeChannel::Coeffs({0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f}),
                 Mach1TranscodeChannel::Coeffs({0.495617f, 0.495617f, 0.000043f, 0.000043f, 0.000030f, 0.000030f, 0.000001f, 0.000001f, 0.002965f, 0.000019f, 0.000001f, 0.000019f, 0.005612f, 0.000001f}),
                 Mach1TranscodeChannel::Coeffs({0.105631f, 0.000060f, 0.770623f, 0.001142f, 0.000022f, 0.000002f, 0.000338f, 0.000007f, 0.000011f, 0.000005f, 0.000461f, 0.003073f, 0.118622f, 0.000003f}),
                 Mach1TranscodeChannel::Coeffs({0.000060f, 0.105631f, 0.001142f, 0.770623f, 0.000002f, 0.000022f, 0.000007f, 0.000338f, 0.000011f, 0.003073f, 0.000461f, 0.000005f, 0.118622f, 0.000003f}),
                 Mach1TranscodeChannel::Coeffs({0.000017f, 0.188361f, 0.000007f, 0.002682f, 0.000003f, 0.010365f, 0.000002f, 0.000643f, 0.000040f, 0.797773f, 0.000012f, 0.000001f, 0.000090f, 0.000005f}),
                 Mach1TranscodeChannel::Coeffs({0.000882f, 0.772087f, 0.000006f, 0.000267f, 0.000051f, 0.112119f, 0.000001f, 0.000019f, 0.111638f, 0.002559f, 0.000002f, 0.000004f, 0.000355f, 0.000010f}),
                 Mach1TranscodeChannel::Coeffs({0.772086f, 0.000882f, 0.000267f, 0.000006f, 0.112119f, 0.000051f, 0.000019f, 0.000001f, 0.111638f, 0.000004f, 0.000002f, 0.002559f, 0.000355f, 0.000010f}),
                 Mach1TranscodeChannel::Coeffs({0.188361f, 0.000017f, 0.002682f, 0.000007f, 0.010365f, 0.000003f, 0.000643f, 0.000002f, 0.000040f, 0.000001f, 0.000012f, 0.797773f, 0.000090f, 0.000005f}),
                 Mach1TranscodeChannel::Coeffs({0.000002f, 0.000000f, 0.451252f, 0.000397f, 0.000000f, 0.000000f, 0.000237f, 0.000007f, 0.000000f, 0.000000f, 0.548086f, 0.000009f, 0.000008f, 0.000000f}),
                 Mach1TranscodeChannel::Coeffs({0.000000f, 0.000002f, 0.000397f, 0.451252f, 0.000000f, 0.000000f, 0.000007f, 0.000237f, 0.000000f, 0.000009f, 0.548086f, 0.000000f, 0.000008f, 0.000000f}),
                 Mach1TranscodeChannel::Coeffs({0.000001f, 0.000019f, 0.000051f, 0.112119f, 0.000006f, 0.000267f, 0.000882f, 0.772087f, 0.000002f, 0.002559f, 0.111638f, 0.000004f, 0.000010f, 0.000355f}),
                 Mach1TranscodeChannel::Coeffs({0.000002f, 0.000643f, 0.000003f, 0.010365f, 0.000007f, 0.002682f, 0.000017f, 0.188361f, 0.000012f, 0.797773f, 0.000040f, 0.000001f, 0.000005f, 0.000090f}),
                 Mach1TranscodeChannel::Coeffs({0.000007f, 0.000237f, 0.000000f, 0.000000f, 0.000397f, 0.451252f, 0.000000f, 0.000002f, 0.548086f, 0.000009f, 0.000000f, 0.000000f, 0.000000f, 0.000008f}),
                 Mach1TranscodeChannel::Coeffs({0.000237f, 0.000007f, 0.000000f, 0.000000f, 0.451252f, 0.000397f, 0.000002f, 0.000000f, 0.548087f, 0.000000f, 0.000000f, 0.000009f, 0.000000f, 0.000008f}),
                 Mach1TranscodeChannel::Coeffs({0.000643f, 0.000002f, 0.010365f, 0.000003f, 0.002682f, 0.000007f, 0.188361f, 0.000017f, 0.000012f, 0.000001f, 0.000040f, 0.797773f, 0.000005f, 0.000090f}),
                 Mach1TranscodeChannel::Coeffs({0.000019f, 0.000001f, 0.112119f, 0.000051f, 0.000267f, 0.000006f, 0.772086f, 0.000882f, 0.000002f, 0.000004f, 0.111638f, 0.002559f, 0.000010f, 0.000355f}),
                 Mach1TranscodeChannel::Coeffs({0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f}),
                 Mach1TranscodeChannel::Coeffs({0.000007f, 0.000338f, 0.000002f, 0.000022f, 0.001142f, 0.770623f, 0.000060f, 0.105631f, 0.000461f, 0.003073f, 0.000011f, 0.000005f, 0.000003f, 0.118622f}),
                 Mach1TranscodeChannel::Coeffs({0.000338f, 0.000007f, 0.000022f, 0.000002f, 0.770623f, 0.001142f, 0.105631f, 0.000060f, 0.000461f, 0.000005f, 0.000011f, 0.003073f, 0.000003f, 0.118622f}),
             }},
            {// Zylia ZM-1 diverged each capsule at 70.7%
             "ZM-1_SIM",
             "M1Spatial-14",
             {
                 Mach1TranscodeChannel::Coeffs({0.061538f, 0.061538f, 0.061538f, 0.061538f, 0.000455f, 0.000455f, 0.000455f, 0.000455f, 0.001442f, 0.001442f, 0.001442f, 0.001442f, 0.746158f, 0.000102f}),
                 Mach1TranscodeChannel::Coeffs({0.342439f, 0.342439f, 0.011556f, 0.011556f, 0.007815f, 0.007815f, 0.000754f, 0.000754f, 0.096228f, 0.007030f, 0.000825f, 0.007030f, 0.163066f, 0.000695f}),
                 Mach1TranscodeChannel::Coeffs({0.128826f, 0.008407f, 0.389634f, 0.033111f, 0.003985f, 0.000450f, 0.016948f, 0.001614f, 0.002326f, 0.001140f, 0.028162f, 0.089888f, 0.294761f, 0.000749f}),
                 Mach1TranscodeChannel::Coeffs({0.008407f, 0.128826f, 0.033111f, 0.389634f, 0.000450f, 0.003985f, 0.001614f, 0.016948f, 0.002326f, 0.089888f, 0.028162f, 0.001140f, 0.294761f, 0.000749f}),
                 Mach1TranscodeChannel::Coeffs({0.001873f, 0.189507f, 0.001062f, 0.071863f, 0.000515f, 0.031812f, 0.000330f, 0.020782f, 0.004416f, 0.666868f, 0.001772f, 0.000196f, 0.008100f, 0.000904f}),
                 Mach1TranscodeChannel::Coeffs({0.031877f, 0.389950f, 0.001564f, 0.016472f, 0.008498f, 0.132451f, 0.000453f, 0.004024f, 0.293925f, 0.089732f, 0.000740f, 0.001123f, 0.026816f, 0.002374f}),
                 Mach1TranscodeChannel::Coeffs({0.389950f, 0.031877f, 0.016472f, 0.001564f, 0.132451f, 0.008498f, 0.004024f, 0.000453f, 0.293925f, 0.001123f, 0.000740f, 0.089732f, 0.026816f, 0.002374f}),
                 Mach1TranscodeChannel::Coeffs({0.189507f, 0.001873f, 0.071863f, 0.001062f, 0.031812f, 0.000515f, 0.020782f, 0.000330f, 0.004416f, 0.000196f, 0.001772f, 0.666868f, 0.008100f, 0.000904f}),
                 Mach1TranscodeChannel::Coeffs({0.004346f, 0.001018f, 0.320145f, 0.036181f, 0.001119f, 0.000312f, 0.042726f, 0.012302f, 0.000305f, 0.001208f, 0.550472f, 0.015027f, 0.013516f, 0.001324f}),
                 Mach1TranscodeChannel::Coeffs({0.001018f, 0.004346f, 0.036181f, 0.320145f, 0.000312f, 0.001119f, 0.012302f, 0.042725f, 0.000305f, 0.015027f, 0.550472f, 0.001208f, 0.013516f, 0.001324f}),
                 Mach1TranscodeChannel::Coeffs({0.000453f, 0.004024f, 0.008498f, 0.132451f, 0.001564f, 0.016472f, 0.031877f, 0.389950f, 0.000740f, 0.089732f, 0.293925f, 0.001123f, 0.002374f, 0.026816f}),
                 Mach1TranscodeChannel::Coeffs({0.000330f, 0.020782f, 0.000515f, 0.031812f, 0.001062f, 0.071863f, 0.001873f, 0.189507f, 0.001772f, 0.666868f, 0.004416f, 0.000196f, 0.000904f, 0.008100f}),
                 Mach1TranscodeChannel::Coeffs({0.012302f, 0.042725f, 0.000312f, 0.001119f, 0.036181f, 0.320145f, 0.001018f, 0.004346f, 0.550472f, 0.015027f, 0.000305f, 0.001208f, 0.001324f, 0.013516f}),
                 Mach1TranscodeChannel::Coeffs({0.042725f, 0.012302f, 0.001119f, 0.000312f, 0.320145f, 0.036181f, 0.004346f, 0.001018f, 0.550472f, 0.001208f, 0.000305f, 0.015027f, 0.001324f, 0.013516f}),
                 Mach1TranscodeChannel::Coeffs({0.020782f, 0.000330f, 0.031812f, 0.000515f, 0.071863f, 0.001062f, 0.189507f, 0.001873f, 0.001772f, 0.000196f, 0.004416f, 0.666868f, 0.000904f, 0.008100f}),
                 Mach1TranscodeChannel::Coeffs({0.004024f, 0.000453f, 0.132451f, 0.008498f, 0.016472f, 0.001564f, 0.389950f, 0.031877f, 0.000740f, 0.001123f, 0.293925f, 0.089732f, 0.002374f, 0.026816f}),
                 Mach1TranscodeChannel::Coeffs({0.000754f, 0.000754f, 0.007815f, 0.007815f, 0.011556f, 0.011556f, 0.342439f, 0.342439f, 0.000825f, 0.007030f, 0.096228f, 0.007030f, 0.000695f, 0.163065f}),
                 Mach1TranscodeChannel::Coeffs({0.001614f, 0.016948f, 0.000450f, 0.003985f, 0.033111f, 0.389634f, 0.008407f, 0.128826f, 0.028161f, 0.089888f, 0.002326f, 0.001140f, 0.000749f, 0.294761f}),
                 Mach1TranscodeChannel::Coeffs({0.016948f, 0.001614f, 0.003985f, 0.000450f, 0.389634f, 0.033111f, 0.128826f, 0.008407f, 0.028161f, 0.001140f, 0.002326f, 0.089888f, 0.000749f, 0.294761f}),
             }},
        };
    }
};
}; // namespace Mach1TranscodeConstants
