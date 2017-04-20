

#if UNITY_EDITOR || UNITY_STANDALONE
using System;
using System.Runtime.InteropServices;

namespace FFmpeg.AutoGen
{
    public unsafe partial struct SwsVector
    {
        public double* @coeff;
        public int @length;
    }
    
    public unsafe partial struct SwsFilter
    {
        public SwsVector* @lumH;
        public SwsVector* @lumV;
        public SwsVector* @chrH;
        public SwsVector* @chrV;
    }
    
    public unsafe partial struct SwsContext
    {
    }
    
    public unsafe static partial class ffmpeg
    {
        public const int LIBSWSCALE_VERSION_MAJOR = 4;
        public const int LIBSWSCALE_VERSION_MINOR = 0;
        public const int LIBSWSCALE_VERSION_MICRO = 100;
        public const int SWS_FAST_BILINEAR = 1;
        public const int SWS_BILINEAR = 2;
        public const int SWS_BICUBIC = 4;
        public const int SWS_X = 8;
        public const int SWS_POINT = 0x10;
        public const int SWS_AREA = 0x20;
        public const int SWS_BICUBLIN = 0x40;
        public const int SWS_GAUSS = 0x80;
        public const int SWS_SINC = 0x100;
        public const int SWS_LANCZOS = 0x200;
        public const int SWS_SPLINE = 0x400;
        public const int SWS_SRC_V_CHR_DROP_MASK = 0x30000;
        public const int SWS_SRC_V_CHR_DROP_SHIFT = 16;
        public const int SWS_PARAM_DEFAULT = 123456;
        public const int SWS_PRINT_INFO = 0x1000;
        public const int SWS_FULL_CHR_H_INT = 0x2000;
        public const int SWS_FULL_CHR_H_INP = 0x4000;
        public const int SWS_DIRECT_BGR = 0x8000;
        public const int SWS_ACCURATE_RND = 0x40000;
        public const int SWS_BITEXACT = 0x80000;
        public const int SWS_ERROR_DIFFUSION = 0x800000;
        public const double SWS_MAX_REDUCE_CUTOFF = 0.002;
        public const int SWS_CS_ITU709 = 1;
        public const int SWS_CS_FCC = 4;
        public const int SWS_CS_ITU601 = 5;
        public const int SWS_CS_ITU624 = 5;
        public const int SWS_CS_SMPTE170M = 5;
        public const int SWS_CS_SMPTE240M = 7;
        public const int SWS_CS_DEFAULT = 5;
#if UNITY_EDITOR_OSX  || UNITY_STANDALONE_OSX 
		private const string libswscale = "EasyMovieTexture";
#else
        private const string libswscale = "swscale-4";
#endif
        
        [DllImport(libswscale, EntryPoint = "swscale_version", CallingConvention = CallingConvention.Cdecl)]
        public static extern uint swscale_version();
        
        [DllImport(libswscale, EntryPoint = "swscale_configuration", CallingConvention = CallingConvention.Cdecl)]
        public static extern string swscale_configuration();
        
        [DllImport(libswscale, EntryPoint = "swscale_license", CallingConvention = CallingConvention.Cdecl)]
        public static extern string swscale_license();
        
        [DllImport(libswscale, EntryPoint = "sws_getCoefficients", CallingConvention = CallingConvention.Cdecl)]
        public static extern int* sws_getCoefficients(int @colorspace);
        
        [DllImport(libswscale, EntryPoint = "sws_isSupportedInput", CallingConvention = CallingConvention.Cdecl)]
        public static extern int sws_isSupportedInput(AVPixelFormat @pix_fmt);
        
        [DllImport(libswscale, EntryPoint = "sws_isSupportedOutput", CallingConvention = CallingConvention.Cdecl)]
        public static extern int sws_isSupportedOutput(AVPixelFormat @pix_fmt);
        
        [DllImport(libswscale, EntryPoint = "sws_isSupportedEndiannessConversion", CallingConvention = CallingConvention.Cdecl)]
        public static extern int sws_isSupportedEndiannessConversion(AVPixelFormat @pix_fmt);
        
        [DllImport(libswscale, EntryPoint = "sws_alloc_context", CallingConvention = CallingConvention.Cdecl)]
        public static extern SwsContext* sws_alloc_context();
        
        [DllImport(libswscale, EntryPoint = "sws_init_context", CallingConvention = CallingConvention.Cdecl)]
        public static extern int sws_init_context(SwsContext* @sws_context, SwsFilter* @srcFilter, SwsFilter* @dstFilter);
        
        [DllImport(libswscale, EntryPoint = "sws_freeContext", CallingConvention = CallingConvention.Cdecl)]
        public static extern void sws_freeContext(SwsContext* @swsContext);
        
        [DllImport(libswscale, EntryPoint = "sws_getContext", CallingConvention = CallingConvention.Cdecl)]
        public static extern SwsContext* sws_getContext(int @srcW, int @srcH, AVPixelFormat @srcFormat, int @dstW, int @dstH, AVPixelFormat @dstFormat, int @flags, SwsFilter* @srcFilter, SwsFilter* @dstFilter, double* @param);
        
        [DllImport(libswscale, EntryPoint = "sws_scale", CallingConvention = CallingConvention.Cdecl)]
        public static extern int sws_scale(SwsContext* @c, sbyte** @srcSlice, int* @srcStride, int @srcSliceY, int @srcSliceH, sbyte** @dst, int* @dstStride);
        
        [DllImport(libswscale, EntryPoint = "sws_setColorspaceDetails", CallingConvention = CallingConvention.Cdecl)]
        public static extern int sws_setColorspaceDetails(SwsContext* @c, [MarshalAs(UnmanagedType.LPArray, SizeConst=4)] int[] @inv_table, int @srcRange, [MarshalAs(UnmanagedType.LPArray, SizeConst=4)] int[] @table, int @dstRange, int @brightness, int @contrast, int @saturation);
        
        [DllImport(libswscale, EntryPoint = "sws_getColorspaceDetails", CallingConvention = CallingConvention.Cdecl)]
        public static extern int sws_getColorspaceDetails(SwsContext* @c, int** @inv_table, int* @srcRange, int** @table, int* @dstRange, int* @brightness, int* @contrast, int* @saturation);
        
        [DllImport(libswscale, EntryPoint = "sws_allocVec", CallingConvention = CallingConvention.Cdecl)]
        public static extern SwsVector* sws_allocVec(int @length);
        
        [DllImport(libswscale, EntryPoint = "sws_getGaussianVec", CallingConvention = CallingConvention.Cdecl)]
        public static extern SwsVector* sws_getGaussianVec(double @variance, double @quality);
        
        [DllImport(libswscale, EntryPoint = "sws_getConstVec", CallingConvention = CallingConvention.Cdecl)]
        public static extern SwsVector* sws_getConstVec(double @c, int @length);
        
        [DllImport(libswscale, EntryPoint = "sws_getIdentityVec", CallingConvention = CallingConvention.Cdecl)]
        public static extern SwsVector* sws_getIdentityVec();
        
        [DllImport(libswscale, EntryPoint = "sws_scaleVec", CallingConvention = CallingConvention.Cdecl)]
        public static extern void sws_scaleVec(SwsVector* @a, double @scalar);
        
        [DllImport(libswscale, EntryPoint = "sws_normalizeVec", CallingConvention = CallingConvention.Cdecl)]
        public static extern void sws_normalizeVec(SwsVector* @a, double @height);
        
        [DllImport(libswscale, EntryPoint = "sws_convVec", CallingConvention = CallingConvention.Cdecl)]
        public static extern void sws_convVec(SwsVector* @a, SwsVector* @b);
        
        [DllImport(libswscale, EntryPoint = "sws_addVec", CallingConvention = CallingConvention.Cdecl)]
        public static extern void sws_addVec(SwsVector* @a, SwsVector* @b);
        
        [DllImport(libswscale, EntryPoint = "sws_subVec", CallingConvention = CallingConvention.Cdecl)]
        public static extern void sws_subVec(SwsVector* @a, SwsVector* @b);
        
        [DllImport(libswscale, EntryPoint = "sws_shiftVec", CallingConvention = CallingConvention.Cdecl)]
        public static extern void sws_shiftVec(SwsVector* @a, int @shift);
        
        [DllImport(libswscale, EntryPoint = "sws_cloneVec", CallingConvention = CallingConvention.Cdecl)]
        public static extern SwsVector* sws_cloneVec(SwsVector* @a);
        
        [DllImport(libswscale, EntryPoint = "sws_printVec2", CallingConvention = CallingConvention.Cdecl)]
        public static extern void sws_printVec2(SwsVector* @a, AVClass* @log_ctx, int @log_level);
        
        [DllImport(libswscale, EntryPoint = "sws_freeVec", CallingConvention = CallingConvention.Cdecl)]
        public static extern void sws_freeVec(SwsVector* @a);
        
        [DllImport(libswscale, EntryPoint = "sws_getDefaultFilter", CallingConvention = CallingConvention.Cdecl)]
        public static extern SwsFilter* sws_getDefaultFilter(float @lumaGBlur, float @chromaGBlur, float @lumaSharpen, float @chromaSharpen, float @chromaHShift, float @chromaVShift, int @verbose);
        
        [DllImport(libswscale, EntryPoint = "sws_freeFilter", CallingConvention = CallingConvention.Cdecl)]
        public static extern void sws_freeFilter(SwsFilter* @filter);
        
        [DllImport(libswscale, EntryPoint = "sws_getCachedContext", CallingConvention = CallingConvention.Cdecl)]
        public static extern SwsContext* sws_getCachedContext(SwsContext* @context, int @srcW, int @srcH, AVPixelFormat @srcFormat, int @dstW, int @dstH, AVPixelFormat @dstFormat, int @flags, SwsFilter* @srcFilter, SwsFilter* @dstFilter, double* @param);
        
        [DllImport(libswscale, EntryPoint = "sws_convertPalette8ToPacked32", CallingConvention = CallingConvention.Cdecl)]
        public static extern void sws_convertPalette8ToPacked32(sbyte* @src, sbyte* @dst, int @num_pixels, sbyte* @palette);
        
        [DllImport(libswscale, EntryPoint = "sws_convertPalette8ToPacked24", CallingConvention = CallingConvention.Cdecl)]
        public static extern void sws_convertPalette8ToPacked24(sbyte* @src, sbyte* @dst, int @num_pixels, sbyte* @palette);
        
        [DllImport(libswscale, EntryPoint = "sws_get_class", CallingConvention = CallingConvention.Cdecl)]
        public static extern AVClass* sws_get_class();
        
    }
}
#endif
