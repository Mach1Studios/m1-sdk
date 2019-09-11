using System;
using Unity.Audio;
using UnityEngine;

public struct Conversion
{
    public static SoundFormat ConvertSpeakerMode(AudioSpeakerMode mode)
    {
        switch (mode)
        {
            case AudioSpeakerMode.Mono:
                return SoundFormat.Mono;
            case AudioSpeakerMode.Stereo:
                return SoundFormat.Stereo;
            case AudioSpeakerMode.Quad:
                return SoundFormat.Quad;
            case AudioSpeakerMode.Surround:
                return SoundFormat.Surround;
            case AudioSpeakerMode.Mode5point1:
                return SoundFormat.FiveDot1;
            case AudioSpeakerMode.Mode7point1:
                return SoundFormat.SevenDot1;
            case AudioSpeakerMode.Prologic:
                return SoundFormat.Raw;
            default:
                throw new ArgumentException($"Invalid speaker mode {mode}", nameof(mode));
        }
    }

    public static int ConvertSoundFormatToChannels(SoundFormat format)
    {
        switch (format)
        {
            case SoundFormat.Raw:
                return 2;
            case SoundFormat.Mono:
                return 1;
            case SoundFormat.Stereo:
                return 2;
            case SoundFormat.Quad:
                return 4;
            case SoundFormat.Surround:
                return 5;
            case SoundFormat.FiveDot1:
                return 6;
            case SoundFormat.SevenDot1:
                return 8;
            default:
                throw new ArgumentException($"Invalid sound format {format}", nameof(format));
        }
    }
}
