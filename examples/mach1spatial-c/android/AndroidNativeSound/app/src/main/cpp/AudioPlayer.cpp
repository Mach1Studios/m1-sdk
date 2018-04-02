#include "AudioPlayer.h"

AudioPlayer::AudioPlayer()
{
}

AudioPlayer::~AudioPlayer()
{
    if (video_dec_ctx)
        avcodec_close(video_dec_ctx);
    if (audio_dec_ctx)
        avcodec_close(audio_dec_ctx);
    av_free(frame);
    av_free(video_dst_data[0]);
}

float clamp(float x, float lower, float upper)
{
    return std::min(upper, std::max(x, lower));
}

static const char *get_error_text(const int error)
{
    static char error_buffer[255];
    av_strerror(error, error_buffer, sizeof(error_buffer));
    return error_buffer;
}

int AudioPlayer::decode_packet(int * got_frame, int cached)
{
    int ret = 0;
    int decoded = pkt.size;

    *got_frame = 0;
    if (pkt.stream_index == video_stream_idx) {

        // SKIP VIDEO
        return 0;

        /* decode video frame */
        ret = avcodec_decode_video2(video_dec_ctx, frame, got_frame, &pkt);
        if (ret < 0) {
            fprintf(stderr, "Error decoding video frame\n");
            return ret;
        }
        if (*got_frame) {
            char buf[AV_TS_MAX_STRING_SIZE] = { 0 };
            av_ts_make_time_string(buf, frame->pts, &video_dec_ctx->time_base);
            printf("video_frame%s n:%d coded_n:%d pts:%s\n",
                   cached ? "(cached)" : "",
                   video_frame_count++, frame->coded_picture_number,
                   buf);
            /* copy decoded frame to destination buffer:
            * this is required since rawvideo expects non aligned data */
            av_image_copy(video_dst_data, video_dst_linesize,
                          (const uint8_t **)(frame->data), frame->linesize,
                          video_dec_ctx->pix_fmt, video_dec_ctx->width, video_dec_ctx->height);
            /* write to rawvideo file */

            // RAW VIDEO
            // fwrite(video_dst_data[0], 1, video_dst_bufsize, video_dst_file);
        }
    }
    else if (pkt.stream_index == audio_stream_idx) {
        /* decode audio frame */
        ret =  avcodec_decode_audio4(audio_dec_ctx, frame, got_frame, &pkt);
        if (ret < 0) {

            //char s[1000];
            //sprintf(s, "Could not decode frame (error '%s')\n", get_error_text(ret));

            fprintf(stderr, "Error decoding audio frame\n");
            return ret;
        }
        /* Some audio decoders decode only part of the packet, and have to be
        * called again with the remainder of the packet data.
        * Sample: fate-suite/lossless-audio/luckynight-partial.shn
        * Also, some decoders might over-read the packet. */
        decoded = FFMIN(ret, pkt.size);
        if (*got_frame) {
            size_t unpadded_linesize = frame->nb_samples * av_get_bytes_per_sample((AVSampleFormat)frame->format);

            if (video_dec_ctx)
            {
                char buf[AV_TS_MAX_STRING_SIZE] = { 0 };
                av_ts_make_time_string(buf, frame->pts, &video_dec_ctx->time_base);
                printf("audio_frame%s n:%d nb_samples:%d pts:%s\n",
                       cached ? "(cached)" : "",
                       audio_frame_count++, frame->nb_samples,
                       buf);
            }

            /* Write the raw audio data samples of the first plane. This works
            * fine for packed formats (e.g. AV_SAMPLE_FMT_S16). However,
            * most audio decoders output planar audio, which uses a separate
            * plane of audio samples for each channel (e.g. AV_SAMPLE_FMT_S16P).
            * In other words, this code will write only the first audio channel
            * in these cases.
            * You should use libswresample or libavfilter to convert the frame
            * to packed data. */

            // RAW AUDIO
            int samples = unpadded_linesize / sizeof(float);
            while (running && ready && bufferWrite + samples >= bufferRead)
            {
                usleep(20);
            }

            // http://stackoverflow.com/questions/14989397/how-to-convert-sample-rate-from-av-sample-fmt-fltp-to-av-sample-fmt-s16
            //	memcpy(&buffer[bufferWrite % AUDIO_PLAYER_BUFFERSIZE], frame->extended_data[0], unpadded_linesize);
            for(int j = 0; j < AUDIO_PLAYER_CHANNELS && frame->extended_data[j] != nullptr; j++)
            {
                for(size_t i = 0; i < samples; i++)
                {
                    buffer[j][(bufferWrite + i) % AUDIO_PLAYER_BUFFERSIZE] = clamp(((float*)frame->extended_data[j])[i], -1, 1); // planar flt
                }
                //buffer[(bufferWrite + i) % AUDIO_PLAYER_BUFFERSIZE] = 1.0 * ((uint32_t*)frame->extended_data[0])[i * frame->channels + 1] / INT32_MAX; // interleaved 32
            }

            bufferWrite += samples;
            if (bufferWrite >= bufferRead)
            {
                ready = true;
            }


            //fwrite(frame->extended_data[0], 1, unpadded_linesize, audio_dst_file);
        }
    }
    return decoded;
}

int AudioPlayer::open_codec_context(int * stream_idx, AVFormatContext * fmt_ctx, AVMediaType type)
{
    int ret;
    AVStream *st;
    AVCodecContext *dec_ctx = NULL;
    AVCodec *dec = NULL;
    ret = av_find_best_stream(fmt_ctx, type, -1, -1, NULL, 0);
    if (ret < 0) {
        fprintf(stderr, "Could not find %s stream in input file '%s'\n",
                av_get_media_type_string(type), src_filename);
        return ret;
    }
    else {
        *stream_idx = ret;
        st = fmt_ctx->streams[*stream_idx];
        /* find decoder for the stream */
        dec_ctx = st->codec;
        dec = avcodec_find_decoder(dec_ctx->codec_id);
        if (!dec) {
            fprintf(stderr, "Failed to find %s codec\n",
                    av_get_media_type_string(type));
            return ret;
        }
        if ((ret = avcodec_open2(dec_ctx, dec, NULL)) < 0) {
            fprintf(stderr, "Failed to open %s codec\n",
                    av_get_media_type_string(type));
            return ret;
        }
    }
    return 0;
}

int AudioPlayer::get_format_from_sample_fmt(const char ** fmt, AVSampleFormat sample_fmt)
{
    int i;
    struct sample_fmt_entry {
        enum AVSampleFormat sample_fmt;
        const char *fmt_be, *fmt_le;
    } sample_fmt_entries[] = {
            { AV_SAMPLE_FMT_U8,  "u8",    "u8" },
            { AV_SAMPLE_FMT_S16, "s16be", "s16le" },
            { AV_SAMPLE_FMT_S32, "s32be", "s32le" },
            { AV_SAMPLE_FMT_FLT, "f32be", "f32le" },
            { AV_SAMPLE_FMT_DBL, "f64be", "f64le" },
    };
    *fmt = NULL;
    for (i = 0; i < FF_ARRAY_ELEMS(sample_fmt_entries); i++) {
        struct sample_fmt_entry *entry = &sample_fmt_entries[i];
        if (sample_fmt == entry->sample_fmt) {
            *fmt = AV_NE(entry->fmt_be, entry->fmt_le);
            return 0;
        }
    }
    fprintf(stderr,
            "sample format %s is not supported as output format\n",
            av_get_sample_fmt_name(sample_fmt));
    return -1;
}


struct buffer_data {
    uint8_t *ptr;
    size_t size; ///< size left in the buffer
};

static int read_packet(void *opaque, uint8_t *buf, int buf_size)
{
    struct buffer_data *bd = (struct buffer_data *)opaque;
    buf_size = FFMIN(buf_size, bd->size);
    //printf("ptr:%p size:%zu\n", bd->ptr, bd->size);
    /* copy internal buffer data to buf */
    memcpy(buf, bd->ptr, buf_size);
    bd->ptr  += buf_size;
    bd->size -= buf_size;
    return buf_size;
}


int AudioPlayer::play(uint8_t *buffer, size_t buffer_size)//char * _src_filename)
{
    if(running) return 0;

    ready = false;
    bufferWrite = 0;
    bufferRead = 0;

    int cnt = 0, got_frame;

    //src_filename = _src_filename;

    char path[256];
    //  sprintf(path, "pipe://%d", fd);

    /* register all formats and codecs */
    av_register_all();

    // fmt_ctx =  avformat_alloc_context();
    //   fmt_ctx->skip_initial_bytes = offset;
/*
    FILE *f = fopen(path, "r");
    if(f) {
        fprintf(stderr, "ok");

    }
    else {
        fprintf(stderr, "Could not open source file %s\n", path);

    }
*/
    uint8_t  *avio_ctx_buffer = NULL;
    size_t avio_ctx_buffer_size = AUDIO_PLAYER_BUFFERSIZE;
    AVIOContext *avio_ctx = NULL;

    struct buffer_data bd = { 0 };
    /* fill opaque structure used by the AVIOContext read callback */
    bd.ptr  = buffer;
    bd.size = buffer_size;

    if (!(fmt_ctx = avformat_alloc_context())) {
        fprintf(stderr, "error\n");
        return 0;
    }

    avio_ctx_buffer = (uint8_t*)av_malloc(avio_ctx_buffer_size);
    if (!avio_ctx_buffer) {
        fprintf(stderr, "error\n");
        return 0;
    }
    avio_ctx = avio_alloc_context(avio_ctx_buffer, avio_ctx_buffer_size, 0, &bd, &read_packet, NULL, NULL);
    if (!avio_ctx) {
        fprintf(stderr, "error\n");
        return 0;

    }
    fmt_ctx->pb = avio_ctx;
    if (avformat_open_input(&fmt_ctx, NULL, NULL, NULL) < 0) {
        fprintf(stderr, "Could not open input\n");
        return 0;
    }


    /* retrieve stream information */
    if (avformat_find_stream_info(fmt_ctx, NULL) < 0) {
        fprintf(stderr, "Could not find stream information\n");
        return 0;
    }

    if (open_codec_context(&video_stream_idx, fmt_ctx, AVMEDIA_TYPE_VIDEO) >= 0) {
        video_stream = fmt_ctx->streams[video_stream_idx];
        video_dec_ctx = video_stream->codec;
        /* allocate image where the decoded image will be put */
        cnt = av_image_alloc(video_dst_data, video_dst_linesize,
                             video_dec_ctx->width, video_dec_ctx->height,
                             video_dec_ctx->pix_fmt, 1);
        if (cnt < 0) {
            fprintf(stderr, "Could not allocate raw video buffer\n");
            return 0;
        }
        video_dst_bufsize = cnt;
    }
    if (open_codec_context(&audio_stream_idx, fmt_ctx, AVMEDIA_TYPE_AUDIO) >= 0) {
        audio_stream = fmt_ctx->streams[audio_stream_idx];
        audio_dec_ctx = audio_stream->codec;
    }

    // av_opt_set_int(audio_dec_ctx, "refcounted_frames", 1, 0);

    /* dump input information to stderr */
    av_dump_format(fmt_ctx, 0, src_filename, 0);
    if (!audio_stream && !video_stream) {
        fprintf(stderr, "Could not find audio or video stream in the input, aborting\n");

        return 0;
    }
    frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Could not allocate frame\n");

        return 0;
    }

    running = true;

    /* initialize packet, set data to NULL, let the demuxer fill it */
    av_init_packet(&pkt);
    pkt.data = NULL;
    pkt.size = 0;

    int size = 0;

    /* read frames from the file */
    while (running && av_read_frame(fmt_ctx, &pkt) >= 0 && size<=buffer_size) {
        AVPacket orig_pkt = pkt;
        do {
            cnt = decode_packet(&got_frame, 0);
            if (cnt < 0)
                break;

            size += cnt;

            pkt.data += cnt;
            pkt.size -= cnt;
        } while (pkt.size > 0);
    }

    /* flush cached frames */
    pkt.data = NULL;
    pkt.size = 0;
    do {
        decode_packet(&got_frame, 1);
    } while (got_frame);

    printf("Demuxing succeeded.\n");

    if (video_stream) {
        //video_dec_ctx->width
        //video_dec_ctx->height
    }
    if (audio_stream) {
        enum AVSampleFormat sfmt = audio_dec_ctx->sample_fmt;
        int n_channels = audio_dec_ctx->channels;

    }

    avformat_close_input(&fmt_ctx);
    running = false;

    return cnt < 0;
}

void AudioPlayer::Play(uint8_t *buffer ,  size_t buffer_size)
{
    std::thread(&AudioPlayer::play, this, buffer,buffer_size).detach();
}

void AudioPlayer::Stop()
{
    running = false;
}

bool AudioPlayer::Get(float * buf, int samples)
{
    if (ready && running)
    {
        for (size_t i = 0; i < samples; i++)
        {
            buf[i*2+0] = buffer[0][(bufferRead + i) % AUDIO_PLAYER_BUFFERSIZE];
            buf[i*2+1] = buffer[1][(bufferRead + i) % AUDIO_PLAYER_BUFFERSIZE];

            if ((buffer[0][(bufferRead + i) % AUDIO_PLAYER_BUFFERSIZE] == 0) || (buffer[0][(bufferRead + i) % AUDIO_PLAYER_BUFFERSIZE] == 1)
                    || (buffer[0][(bufferRead + i) % AUDIO_PLAYER_BUFFERSIZE] == -1)) {
                fprintf(stderr, "Buffer peaking? \n");
            }
        }
        bufferRead += samples;
        return true;
    }
    else
    {
        for (size_t i = 0; i < samples; i++)
        {
            buf[i*2+0] =  buf[i*2+1] = 0;
        }
        return false;
    }
}
