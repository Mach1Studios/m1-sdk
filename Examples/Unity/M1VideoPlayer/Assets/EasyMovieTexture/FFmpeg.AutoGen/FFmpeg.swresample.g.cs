#if UNITY_EDITOR || UNITY_STANDALONE
using System;
using System.Runtime.InteropServices;

namespace FFmpeg.AutoGen
{
    public unsafe partial struct AVBuffer
    {
    }
    
    public unsafe partial struct AVBufferPool
    {
    }
    
    public unsafe partial struct AVDictionary
    {
    }
    
    public unsafe partial struct SwrContext
    {
    }
    
    public enum SwrDitherType : int
    {
        @SWR_DITHER_NONE = 0,
        @SWR_DITHER_RECTANGULAR = 1,
        @SWR_DITHER_TRIANGULAR = 2,
        @SWR_DITHER_TRIANGULAR_HIGHPASS = 3,
        @SWR_DITHER_NS = 64,
        @SWR_DITHER_NS_LIPSHITZ = 65,
        @SWR_DITHER_NS_F_WEIGHTED = 66,
        @SWR_DITHER_NS_MODIFIED_E_WEIGHTED = 67,
        @SWR_DITHER_NS_IMPROVED_E_WEIGHTED = 68,
        @SWR_DITHER_NS_SHIBATA = 69,
        @SWR_DITHER_NS_LOW_SHIBATA = 70,
        @SWR_DITHER_NS_HIGH_SHIBATA = 71,
        @SWR_DITHER_NB = 72,
    }
    
    public enum SwrEngine : int
    {
        @SWR_ENGINE_SWR = 0,
        @SWR_ENGINE_SOXR = 1,
        @SWR_ENGINE_NB = 2,
    }
    
    public enum SwrFilterType : int
    {
        @SWR_FILTER_TYPE_CUBIC = 0,
        @SWR_FILTER_TYPE_BLACKMAN_NUTTALL = 1,
        @SWR_FILTER_TYPE_KAISER = 2,
    }
    
    public unsafe static partial class ffmpeg
    {
        public const int LIBSWRESAMPLE_VERSION_MAJOR = 2;
        public const int LIBSWRESAMPLE_VERSION_MINOR = 0;
        public const int LIBSWRESAMPLE_VERSION_MICRO = 101;
        public const int SWR_FLAG_RESAMPLE = 1;
#if UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX  
		private const string libswresample = "EasyMovieTexture";
#else
        private const string libswresample = "swresample-2";
#endif
        
        [DllImport(libswresample, EntryPoint = "swr_get_class", CallingConvention = CallingConvention.Cdecl)]
        public static extern AVClass* swr_get_class();
        
        [DllImport(libswresample, EntryPoint = "swr_alloc", CallingConvention = CallingConvention.Cdecl)]
        public static extern SwrContext* swr_alloc();
        
        [DllImport(libswresample, EntryPoint = "swr_init", CallingConvention = CallingConvention.Cdecl)]
        public static extern int swr_init(SwrContext* @s);
        
        [DllImport(libswresample, EntryPoint = "swr_is_initialized", CallingConvention = CallingConvention.Cdecl)]
        public static extern int swr_is_initialized(SwrContext* @s);
        
        [DllImport(libswresample, EntryPoint = "swr_alloc_set_opts", CallingConvention = CallingConvention.Cdecl)]
        public static extern SwrContext* swr_alloc_set_opts(SwrContext* @s, long @out_ch_layout, AVSampleFormat @out_sample_fmt, int @out_sample_rate, long @in_ch_layout, AVSampleFormat @in_sample_fmt, int @in_sample_rate, int @log_offset, void* @log_ctx);
        
        [DllImport(libswresample, EntryPoint = "swr_free", CallingConvention = CallingConvention.Cdecl)]
        public static extern void swr_free(SwrContext** @s);
        
        [DllImport(libswresample, EntryPoint = "swr_close", CallingConvention = CallingConvention.Cdecl)]
        public static extern void swr_close(SwrContext* @s);
        
        [DllImport(libswresample, EntryPoint = "swr_convert", CallingConvention = CallingConvention.Cdecl)]
        public static extern int swr_convert(SwrContext* @s, sbyte** @out, int @out_count, sbyte** @in, int @in_count);
        
        [DllImport(libswresample, EntryPoint = "swr_next_pts", CallingConvention = CallingConvention.Cdecl)]
        public static extern long swr_next_pts(SwrContext* @s, long @pts);
        
        [DllImport(libswresample, EntryPoint = "swr_set_compensation", CallingConvention = CallingConvention.Cdecl)]
        public static extern int swr_set_compensation(SwrContext* @s, int @sample_delta, int @compensation_distance);
        
        [DllImport(libswresample, EntryPoint = "swr_set_channel_mapping", CallingConvention = CallingConvention.Cdecl)]
        public static extern int swr_set_channel_mapping(SwrContext* @s, int* @channel_map);
        
        [DllImport(libswresample, EntryPoint = "swr_set_matrix", CallingConvention = CallingConvention.Cdecl)]
        public static extern int swr_set_matrix(SwrContext* @s, double* @matrix, int @stride);
        
        [DllImport(libswresample, EntryPoint = "swr_drop_output", CallingConvention = CallingConvention.Cdecl)]
        public static extern int swr_drop_output(SwrContext* @s, int @count);
        
        [DllImport(libswresample, EntryPoint = "swr_inject_silence", CallingConvention = CallingConvention.Cdecl)]
        public static extern int swr_inject_silence(SwrContext* @s, int @count);
        
        [DllImport(libswresample, EntryPoint = "swr_get_delay", CallingConvention = CallingConvention.Cdecl)]
        public static extern long swr_get_delay(SwrContext* @s, long @base);
        
        [DllImport(libswresample, EntryPoint = "swr_get_out_samples", CallingConvention = CallingConvention.Cdecl)]
        public static extern int swr_get_out_samples(SwrContext* @s, int @in_samples);
        
        [DllImport(libswresample, EntryPoint = "swresample_version", CallingConvention = CallingConvention.Cdecl)]
        public static extern uint swresample_version();
        
        [DllImport(libswresample, EntryPoint = "swresample_configuration", CallingConvention = CallingConvention.Cdecl)]
        public static extern string swresample_configuration();
        
        [DllImport(libswresample, EntryPoint = "swresample_license", CallingConvention = CallingConvention.Cdecl)]
        public static extern string swresample_license();
        
        [DllImport(libswresample, EntryPoint = "swr_convert_frame", CallingConvention = CallingConvention.Cdecl)]
        public static extern int swr_convert_frame(SwrContext* @swr, AVFrame* @output, AVFrame* @input);
        
        [DllImport(libswresample, EntryPoint = "swr_config_frame", CallingConvention = CallingConvention.Cdecl)]
        public static extern int swr_config_frame(SwrContext* @swr, AVFrame* @out, AVFrame* @in);
        
    }
}

#endif
