#pragma once

extern "C"
{
#include <libavutil/imgutils.h>
#include <libavutil/samplefmt.h>
#include <libavutil/timestamp.h>
#include <libavformat/avformat.h>

#include <libavutil/opt.h>

}

#include <thread>
#include <unistd.h>

#define AUDIO_PLAYER_BUFFERSIZE (4 * 1024)
#define AUDIO_PLAYER_CHANNELS 8

class AudioPlayer
{
	AVFormatContext *fmt_ctx = NULL;
	AVCodecContext *video_dec_ctx = NULL, *audio_dec_ctx;
	AVStream *video_stream = NULL, *audio_stream = NULL;
	const char *src_filename = NULL;
	uint8_t *video_dst_data[4] = { NULL };
	int video_dst_linesize[4];
	int video_dst_bufsize;
	int video_stream_idx = -1, audio_stream_idx = -1;
	AVFrame *frame = NULL;
	AVPacket pkt;

	int video_frame_count = 0;
	int audio_frame_count = 0;

	int decode_packet(int *got_frame, int cached);
	int open_codec_context(int *stream_idx, AVFormatContext *fmt_ctx, enum AVMediaType type);
	int get_format_from_sample_fmt(const char **fmt, enum AVSampleFormat sample_fmt);

	int play(uint8_t *buffer ,  size_t buffer_size);


public:
	float buffer[AUDIO_PLAYER_CHANNELS][AUDIO_PLAYER_BUFFERSIZE];
	long bufferWrite = 0;
	long bufferRead = 0;
	bool ready = false;
	bool running;

	AudioPlayer();
	~AudioPlayer();

	void Play(uint8_t *buffer ,  size_t buffer_size);
	void Stop();
	bool Get(float * buf, int samples);
};