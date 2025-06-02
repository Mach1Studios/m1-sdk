//  Mach1 SDK
//  Copyright © 2017-2021 Mach1. All rights reserved.

using System;
using System.Runtime.InteropServices;

namespace Mach1
{
    public enum Mach1EncodeInputMode
    {
        Mono = 0,
        Stereo,
        LCR,
        Quad,
        LCRS,
        AFormat,
        FiveDotZero,
        FiveDotOneFilm,
        FiveDotOneDTS,
        FiveDotOneSMTPE,
        BFOAACN,
        BFOAFUMA,
        B2OAACN,
        B2OAFUMA,
        B3OAACN,
        B3OAFUMA
    }

    public enum Mach1EncodeOutputMode
    {
        M1Spatial_4 = 0,
        M1Spatial_8,
        M1Spatial_14
    }

    public enum Mach1EncodePannerMode
    {
        IsotropicLinear = 0,
        IsotropicEqualPower,
        PeriphonicLinear
    }

    public class Mach1Encode : IDisposable
    {
#if UNITY_IOS && !UNITY_EDITOR
        internal const string libname = "__Internal";
#elif UNITY_STANDALONE_WIN || UNITY_EDITOR_WIN
        internal const string libname = "Mach1EncodeCAPI";
#else
        internal const string libname = "libMach1EncodeCAPI";
#endif

        [DllImport(libname)]
        internal static extern IntPtr Mach1EncodeCAPI_create();

        [DllImport(libname)]
        internal static extern void Mach1EncodeCAPI_copy(IntPtr M1str, IntPtr M1dst);

        [DllImport(libname)]
        internal static extern void Mach1EncodeCAPI_delete(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern IntPtr Mach1EncodeCAPI_getPoints(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern IntPtr Mach1EncodeCAPI_getGains(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern IntPtr Mach1EncodeCAPI_getPointsNames(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern IntPtr Mach1EncodeCAPI_getGainsForInputChannelNamed(IntPtr M1obj, string pointName);

        [DllImport(libname)]
        internal static extern void Mach1EncodeCAPI_generatePointResults(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern int Mach1EncodeCAPI_getPointsCount(IntPtr M1obj);

#if M1ENCODE_INLINE_DECODE
        [DllImport(libname)]
        internal static extern IntPtr Mach1EncodeCAPI_getResultingCoeffsDecoded(IntPtr M1obj, int decodeMode, IntPtr decodeResult);
#endif

        [DllImport(libname)]
        internal static extern Mach1EncodeInputMode Mach1EncodeCAPI_getInputMode(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern Mach1EncodeOutputMode Mach1EncodeCAPI_getOutputMode(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern Mach1EncodePannerMode Mach1EncodeCAPI_getPannerMode(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern bool Mach1EncodeCAPI_getAutoOrbit(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern int Mach1EncodeCAPI_getInputChannelsCount(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern int Mach1EncodeCAPI_getOutputChannelsCount(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern void Mach1EncodeCAPI_setInputMode(IntPtr M1obj, Mach1EncodeInputMode inputMode);

        [DllImport(libname)]
        internal static extern void Mach1EncodeCAPI_setOutputMode(IntPtr M1obj, Mach1EncodeOutputMode outputMode);

        [DllImport(libname)]
        internal static extern void Mach1EncodeCAPI_setAzimuth(IntPtr M1obj, float azimuthFromMinus1To1);

        [DllImport(libname)]
        internal static extern void Mach1EncodeCAPI_setAzimuthDegrees(IntPtr M1obj, float azimuthDegrees);

        [DllImport(libname)]
        internal static extern void Mach1EncodeCAPI_setAzimuthRadians(IntPtr M1obj, float azimuthRadians);

        [DllImport(libname)]
        internal static extern void Mach1EncodeCAPI_setDiverge(IntPtr M1obj, float divergeFromMinus1To1);

        [DllImport(libname)]
        internal static extern void Mach1EncodeCAPI_setElevation(IntPtr M1obj, float elevationFromMinus1to1);

        [DllImport(libname)]
        internal static extern void Mach1EncodeCAPI_setElevationDegrees(IntPtr M1obj, float elevationFromMinus90to90);

        [DllImport(libname)]
        internal static extern void Mach1EncodeCAPI_setElevationRadians(IntPtr M1obj, float elevationFromMinusHalfPItoHalfPI);

        [DllImport(libname)]
        internal static extern void Mach1EncodeCAPI_setPannerMode(IntPtr M1obj, Mach1EncodePannerMode pannerMode);

        [DllImport(libname)]
        internal static extern void Mach1EncodeCAPI_setFrontSurroundPerspective(IntPtr M1obj, bool frontSurroundPerspective);

        [DllImport(libname)]
        internal static extern float Mach1EncodeCAPI_getOutputGain(IntPtr M1obj, bool isDecibel);

        [DllImport(libname)]
        internal static extern void Mach1EncodeCAPI_setOutputGain(IntPtr M1obj, float outputGainMultipler, bool isDecibel);

        [DllImport(libname)]
        internal static extern float Mach1EncodeCAPI_getGainCompensation(IntPtr M1obj, bool isDecibel);

        [DllImport(libname)]
        internal static extern bool Mach1EncodeCAPI_getGainCompensationActive(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern void Mach1EncodeCAPI_setGainCompensationActive(IntPtr M1obj, bool active);

        [DllImport(libname)]
        internal static extern void Mach1EncodeCAPI_setAutoOrbit(IntPtr M1obj, bool autoOrbit);

        [DllImport(libname)]
        internal static extern void Mach1EncodeCAPI_setOrbitRotation(IntPtr M1obj, float orbitRotationFromMinusOnetoOne);

        [DllImport(libname)]
        internal static extern void Mach1EncodeCAPI_setOrbitRotationDegrees(IntPtr M1obj, float orbitRotationDegrees);

        [DllImport(libname)]
        internal static extern void Mach1EncodeCAPI_setOrbitRotationRadians(IntPtr M1obj, float orbitRotationRadians);

        [DllImport(libname)]
        internal static extern void Mach1EncodeCAPI_setStereoSpread(IntPtr M1obj, float sSpreadFrom0to1);

        [DllImport(libname)]
        internal static extern long Mach1EncodeCAPI_getLastCalculationTime(IntPtr M1obj);

        internal IntPtr M1obj;

        public Mach1Encode()
        {
            M1obj = Mach1EncodeCAPI_create();
        }

        public void Dispose()
        {
            Mach1EncodeCAPI_delete(M1obj);
        }

        public void copy(Mach1Encode destination)
        {
            if (destination != null)
            {
                Mach1EncodeCAPI_copy(M1obj, destination.M1obj);
            }
            /// Copy the current encoder state to another encoder instance
        }

        public Mach1Point3D[] getPoints()
        {
            IntPtr pointsPtr = Mach1EncodeCAPI_getPoints(M1obj);
            int count = getPointsCount();
            
            Mach1Point3D[] points = new Mach1Point3D[count];
            int size = Marshal.SizeOf<Mach1Point3D>();
            
            for (int i = 0; i < count; i++)
            {
                IntPtr elementPtr = new IntPtr(pointsPtr.ToInt64() + i * size);
                points[i] = Marshal.PtrToStructure<Mach1Point3D>(elementPtr);
            }
            
            return points;
            /// Returns the array of point positions
        }

        public float[] getGains()
        {
            IntPtr gainsPtr = Mach1EncodeCAPI_getGains(M1obj);
            int count = getPointsCount();
            
            float[] gains = new float[count];
            Marshal.Copy(gainsPtr, gains, 0, count);
            
            return gains;
            /// Returns the array of gain values for each point
        }

        public string[] getPointsNames()
        {
            IntPtr namesPtr = Mach1EncodeCAPI_getPointsNames(M1obj);
            int count = getPointsCount();
            
            string[] names = new string[count];
            for (int i = 0; i < count; i++)
            {
                IntPtr strPtr = Marshal.ReadIntPtr(namesPtr, i * IntPtr.Size);
                names[i] = Marshal.PtrToStringAnsi(strPtr);
            }
            
            return names;
            /// Returns the names of all points
        }

        public float[] getGainsForInputChannelNamed(string pointName)
        {
            IntPtr gainsPtr = Mach1EncodeCAPI_getGainsForInputChannelNamed(M1obj, pointName);
            int count = getOutputChannelsCount();
            
            float[] gains = new float[count];
            Marshal.Copy(gainsPtr, gains, 0, count);
            
            return gains;
            /// Returns the gain values for a specific input channel by name
        }

        public void generatePointResults()
        {
            Mach1EncodeCAPI_generatePointResults(M1obj);
            /// Generate point results based on current settings
        }

        public int getPointsCount()
        {
            return Mach1EncodeCAPI_getPointsCount(M1obj);
            /// Returns the number of points
        }

#if M1ENCODE_INLINE_DECODE
        public float[] getResultingCoeffsDecoded(int decodeMode, float[] decodeResult)
        {
            IntPtr resultPtr = Marshal.AllocHGlobal(decodeResult.Length * sizeof(float));
            try
            {
                Marshal.Copy(decodeResult, 0, resultPtr, decodeResult.Length);
                
                IntPtr coeffsPtr = Mach1EncodeCAPI_getResultingCoeffsDecoded(M1obj, decodeMode, resultPtr);
                
                int count = getOutputChannelsCount();
                float[] coeffs = new float[count];
                Marshal.Copy(coeffsPtr, coeffs, 0, count);
                
                return coeffs;
            }
            finally
            {
                Marshal.FreeHGlobal(resultPtr);
            }
            /// Returns the decoded coefficients for a specific decode mode
        }
#endif

        public Mach1EncodeInputMode getInputMode()
        {
            return Mach1EncodeCAPI_getInputMode(M1obj);
            /// Returns the current input mode
        }

        public Mach1EncodeOutputMode getOutputMode()
        {
            return Mach1EncodeCAPI_getOutputMode(M1obj);
            /// Returns the current output mode
        }

        public Mach1EncodePannerMode getPannerMode()
        {
            return Mach1EncodeCAPI_getPannerMode(M1obj);
            /// Returns the current panner mode
        }

        public bool getAutoOrbit()
        {
            return Mach1EncodeCAPI_getAutoOrbit(M1obj);
            /// Returns whether auto orbit is enabled
        }

        public int getInputChannelsCount()
        {
            return Mach1EncodeCAPI_getInputChannelsCount(M1obj);
            /// Returns the number of input channels
        }

        public int getOutputChannelsCount()
        {
            return Mach1EncodeCAPI_getOutputChannelsCount(M1obj);
            /// Returns the number of output channels
        }

        public void setInputMode(Mach1EncodeInputMode inputMode)
        {
            Mach1EncodeCAPI_setInputMode(M1obj, inputMode);
            /// Set the input mode
        }

        public void setOutputMode(Mach1EncodeOutputMode outputMode)
        {
            Mach1EncodeCAPI_setOutputMode(M1obj, outputMode);
            /// Set the output mode
        }

        public void setAzimuth(float azimuthFromMinus1To1)
        {
            Mach1EncodeCAPI_setAzimuth(M1obj, azimuthFromMinus1To1);
            /// Set the azimuth in normalized range [-1, 1]
        }

        public void setAzimuthDegrees(float azimuthDegrees)
        {
            Mach1EncodeCAPI_setAzimuthDegrees(M1obj, azimuthDegrees);
            /// Set the azimuth in degrees
        }

        public void setAzimuthRadians(float azimuthRadians)
        {
            Mach1EncodeCAPI_setAzimuthRadians(M1obj, azimuthRadians);
            /// Set the azimuth in radians
        }

        public void setDiverge(float divergeFromMinus1To1)
        {
            Mach1EncodeCAPI_setDiverge(M1obj, divergeFromMinus1To1);
            /// Set the diverge value in normalized range [-1, 1]
        }

        public void setElevation(float elevationFromMinus1to1)
        {
            Mach1EncodeCAPI_setElevation(M1obj, elevationFromMinus1to1);
            /// Set the elevation in normalized range [-1, 1]
        }

        public void setElevationDegrees(float elevationFromMinus90to90)
        {
            Mach1EncodeCAPI_setElevationDegrees(M1obj, elevationFromMinus90to90);
            /// Set the elevation in degrees [-90, 90]
        }

        public void setElevationRadians(float elevationFromMinusHalfPItoHalfPI)
        {
            Mach1EncodeCAPI_setElevationRadians(M1obj, elevationFromMinusHalfPItoHalfPI);
            /// Set the elevation in radians [-PI/2, PI/2]
        }

        public void setPannerMode(Mach1EncodePannerMode pannerMode)
        {
            Mach1EncodeCAPI_setPannerMode(M1obj, pannerMode);
            /// Set the panner mode
        }

        public void setFrontSurroundPerspective(bool frontSurroundPerspective)
        {
            Mach1EncodeCAPI_setFrontSurroundPerspective(M1obj, frontSurroundPerspective);
            /// Set front surround perspective
        }

        public float getOutputGain(bool isDecibel = false)
        {
            return Mach1EncodeCAPI_getOutputGain(M1obj, isDecibel);
            /// Get the output gain (in decibels if isDecibel is true)
        }

        public void setOutputGain(float outputGainMultipler, bool isDecibel = false)
        {
            Mach1EncodeCAPI_setOutputGain(M1obj, outputGainMultipler, isDecibel);
            /// Set the output gain (in decibels if isDecibel is true)
        }

        public float getGainCompensation(bool isDecibel = false)
        {
            return Mach1EncodeCAPI_getGainCompensation(M1obj, isDecibel);
            /// Get the gain compensation (in decibels if isDecibel is true)
        }

        public bool getGainCompensationActive()
        {
            return Mach1EncodeCAPI_getGainCompensationActive(M1obj);
            /// Returns whether gain compensation is active
        }

        public void setGainCompensationActive(bool active)
        {
            Mach1EncodeCAPI_setGainCompensationActive(M1obj, active);
            /// Set whether gain compensation is active
        }

        public void setAutoOrbit(bool autoOrbit)
        {
            Mach1EncodeCAPI_setAutoOrbit(M1obj, autoOrbit);
            /// Set whether auto orbit is enabled
        }

        public void setOrbitRotation(float orbitRotationFromMinusOnetoOne)
        {
            Mach1EncodeCAPI_setOrbitRotation(M1obj, orbitRotationFromMinusOnetoOne);
            /// Set the orbit rotation in normalized range [-1, 1]
        }

        public void setOrbitRotationDegrees(float orbitRotationDegrees)
        {
            Mach1EncodeCAPI_setOrbitRotationDegrees(M1obj, orbitRotationDegrees);
            /// Set the orbit rotation in degrees
        }

        public void setOrbitRotationRadians(float orbitRotationRadians)
        {
            Mach1EncodeCAPI_setOrbitRotationRadians(M1obj, orbitRotationRadians);
            /// Set the orbit rotation in radians
        }

        public void setStereoSpread(float sSpreadFrom0to1)
        {
            Mach1EncodeCAPI_setStereoSpread(M1obj, sSpreadFrom0to1);
            /// Set the stereo spread in normalized range [0, 1]
        }

        public long getLastCalculationTime()
        {
            return Mach1EncodeCAPI_getLastCalculationTime(M1obj);
            /// Returns the last calculation time in milliseconds
        }
    }
} 