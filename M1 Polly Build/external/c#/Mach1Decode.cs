using System;
using System.Runtime.InteropServices;

namespace Mach1
{
    class M1DSPAlgorithms : IDisposable
    {
        internal const string libname = "M1DSPAlgorithmsCAPI";

        [DllImport(libname)]
        internal static extern IntPtr M1DSPAlgorithmsCAPI_create();

        [DllImport(libname)]
        internal static extern void M1DSPAlgorithmsCAPI_delete(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern IntPtr M1DSPAlgorithmsCAPI_fourChannelAlgorithm(IntPtr M1obj, float Yaw, float Pitch, float Roll, bool smoothAngles);

        [DllImport(libname)]
        internal static extern IntPtr M1DSPAlgorithmsCAPI_fourPairsAlgorithm(IntPtr M1obj, float Yaw, float Pitch, float Roll, bool smoothAngles);

        [DllImport(libname)]
        internal static extern IntPtr M1DSPAlgorithmsCAPI_eightChannelsIsotropicAlgorithm(IntPtr M1obj, float Yaw, float Pitch, float Roll, bool smoothAngles);

        [DllImport(libname)]
        internal static extern IntPtr M1DSPAlgorithmsCAPI_eightChannelsAlgorithm(IntPtr M1obj, float Yaw, float Pitch, float Roll, bool smoothAngles);

        [DllImport(libname)]
        internal static extern IntPtr M1DSPAlgorithmsCAPI_eightPairsAlgorithm(IntPtr M1obj, float Yaw, float Pitch, float Roll, bool smoothAngles);

        [DllImport(libname)]
        internal static extern long M1DSPAlgorithmsCAPI_getCurrentTime(IntPtr M1obj);

        internal IntPtr M1obj;

        public M1DSPAlgorithms()
        {
            M1obj = M1DSPAlgorithmsCAPI_create();
        }

        public void Dispose()
        {
            M1DSPAlgorithmsCAPI_delete(M1obj);
        }

        public float[] fourChannelAlgorithm(float Yaw, float Pitch, float Roll, bool smoothAngles)
        {
            IntPtr ptr = M1DSPAlgorithmsCAPI_fourChannelAlgorithm(M1obj, Yaw, Pitch, Roll, smoothAngles);

            float[] data = new float[10];
            Marshal.Copy(ptr, data, 0, data.Length);
            return data;
        }

        public float[] fourPairsAlgorithm(float Yaw, float Pitch, float Roll, bool smoothAngles)
        {
            IntPtr ptr = M1DSPAlgorithmsCAPI_fourPairsAlgorithm(M1obj, Yaw, Pitch, Roll, smoothAngles);

            float[] data = new float[6];
            Marshal.Copy(ptr, data, 0, data.Length);
            return data;
        }


        public float[] eightChannelsIsotropicAlgorithm(float Yaw, float Pitch, float Roll, bool smoothAngles)
        {
            IntPtr ptr = M1DSPAlgorithmsCAPI_eightChannelsIsotropicAlgorithm(M1obj, Yaw, Pitch, Roll, smoothAngles);

            float[] data = new float[18];
            Marshal.Copy(ptr, data, 0, data.Length);
            return data;
        }

        public float[] eightChannelsAlgorithm(float Yaw, float Pitch, float Roll, bool smoothAngles)
        {
            IntPtr ptr = M1DSPAlgorithmsCAPI_eightChannelsAlgorithm(M1obj, Yaw, Pitch, Roll, smoothAngles);

            float[] data = new float[18];
            Marshal.Copy(ptr, data, 0, data.Length);
            return data;
        }

        public float[] eightPairsAlgorithm(float Yaw, float Pitch, float Roll, bool smoothAngles)
        {
            IntPtr ptr = M1DSPAlgorithmsCAPI_eightPairsAlgorithm(M1obj, Yaw, Pitch, Roll, smoothAngles);

            float[] data = new float[10];
            Marshal.Copy(ptr, data, 0, data.Length);
            return data;
        }

        public long getCurrentTime()
        {
            return M1DSPAlgorithmsCAPI_getCurrentTime(M1obj);
        }
    }
}
