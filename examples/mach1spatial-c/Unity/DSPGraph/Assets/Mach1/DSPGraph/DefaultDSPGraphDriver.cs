using System;
using System.IO;
using Unity.Audio;
using Unity.Burst;
using Unity.Collections;

[BurstCompile]
public struct DefaultDSPGraphDriver : IAudioOutput
{
    public DSPGraph Graph;
    private int m_ChannelCount;

    public void Initialize(int channelCount, SoundFormat format, int sampleRate, long dspBufferSize)
    {
        m_ChannelCount = channelCount;
    }

    public void BeginMix(int frameCount)
    {
        Graph.OutputMixer.BeginMix(frameCount);
    }

    public void EndMix(NativeArray<float> output, int frames)
    {
        Graph.OutputMixer.ReadMix(output, frames, m_ChannelCount);
    }

    public void Dispose()
    {
        Graph.Dispose();
    }
}
