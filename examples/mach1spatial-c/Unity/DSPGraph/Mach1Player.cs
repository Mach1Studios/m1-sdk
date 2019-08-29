using System;
using UnityEngine;
using Unity.Audio;
using Unity.Collections;
using Unity.Collections.LowLevel.Unsafe;
using Unity.Burst;

// The 'audio job'. This is the kernel that defines a running DSP node inside the
// DSPGraph. It is a struct that implements the IAudioKernel interface. It can contain
// internal state, and will have the Execute function called as part of the graph
// traversal during an audio frame.
[BurstCompile(CompileSynchronously = true)]
struct Mach1PlayerNode : IAudioKernel<Mach1PlayerNode.Parameters, Mach1PlayerNode.SampleProviders>
{
    // Parameters are currently defined with enumerations. Each enum value corresponds to
    // a parameter within the node. Setting a value for a parameter uses these enum values.
    public enum Parameters { Rate0, Rate1, Rate2, Rate3, Rate4, Rate5, Rate6, Rate7 }

    // Sample providers are defined with enumerations. Each enum value defines a slot where
    // a sample provider can live on a IAudioKernel. Sample providers are used to get samples from
    // AudioClips and VideoPlayers. They will eventually be able to pull samples from microphones and other concepts.
    public enum SampleProviders { Slot0, Slot1, Slot2, Slot3, Slot4, Slot5, Slot6, Slot7 }

    // The clip sample rate might be different to the output rate used by the system. Therefore we use a resampler
    // here.
    [NativeDisableContainerSafetyRestriction]
    public NativeArray<float> ResampleBuffers;

    [NativeDisableContainerSafetyRestriction]
    public NativeArray<float> OutBuffers;

    [NativeDisableContainerSafetyRestriction]
    public NativeArray<double> ResamplerPositions;

    private int dspBufferLength;
    private int inputChannels;

    public bool Playing;

    // Resampler with linear interpolation, pulling directly from the provider
    public bool ResampleLerpRead<TParameters>(ParameterData<TParameters> parameterData, TParameters parameter,
        NativeArray<float> output, int outputFrom, int outputLength, NativeArray<float> input, int inputFrom, int inputLength, SampleProvider provider, long dspClock, ref double Position)
        where TParameters : unmanaged, Enum
    {
        var finishedSampleProvider = false;

        for (var i = 0; i < outputLength / 2; i++)
        {
            var factor = parameterData.GetFloat(parameter, i);
            Position += factor;

            var length = inputLength / 2 - 1;

            // sometimes need to jump more than one buffer, hence the while loop
            while (Position >= length)
            {
                // move the last sample to the start of buffer
                input[inputFrom + 0] = input[inputFrom + inputLength - 2];
                input[inputFrom + 1] = input[inputFrom + inputLength - 1];

                // read N-1 samples starting at buffer index 1 - this allows us to save the previous sample (last sample of previous buffer).
                finishedSampleProvider |= ReadSamples(provider, new NativeSlice<float>(input, inputFrom + 2, inputLength - 2));

                Position -= inputLength / 2 - 1;
            }

            var positionFloor = Math.Floor(Position);
            var positionFraction = Position - positionFloor; // fractional part, used for interpolation
            var previousSampleIndex = (int)positionFloor;
            var nextSampleIndex = previousSampleIndex + 1;

            var prevSampleL = input[inputFrom + previousSampleIndex * 2 + 0];
            var prevSampleR = input[inputFrom + previousSampleIndex * 2 + 1];
            var sampleL = input[inputFrom + nextSampleIndex * 2 + 0];
            var sampleR = input[inputFrom + nextSampleIndex * 2 + 1];

            output[outputFrom + i * 2 + 0] = (float)(prevSampleL + (sampleL - prevSampleL) * positionFraction);
            output[outputFrom + i * 2 + 1] = (float)(prevSampleR + (sampleR - prevSampleR) * positionFraction);
        }

        return finishedSampleProvider;
    }

    // read either mono or stereo, always convert to stereo interleaved
    static bool ReadSamples(SampleProvider provider, NativeSlice<float> destination)
    {
        if (!provider.Valid)
            return true;

        var finished = false;

        // Read from SampleProvider and convert to interleaved stereo if needed
        if (provider.ChannelCount == 2)
        {
            var read = provider.Read(destination.Slice(0, destination.Length));
            if (read < destination.Length / 2)
            {
                for (var i = read * 2; i < destination.Length; i++)
                    destination[i] = 0;
                return true;
            }
        }
        else
        {
            var n = destination.Length / 2;
            var buffer = destination.Slice(0, n);
            var read = provider.Read(buffer);

            if (read < n)
            {
                for (var i = read; i < n; i++)
                    destination[i] = 0;

                finished = true;
            }

            for (var i = n - 1; i >= 0; i--)
            {
                destination[i * 2 + 0] = destination[i];
                destination[i * 2 + 1] = destination[i];
            }
        }

        return finished;
    }

    public void Initialize()
    {
        dspBufferLength = 1024;
        inputChannels = 8; // samples providers

        ResamplerPositions = new NativeArray<double>(inputChannels, Allocator.AudioKernel);
        ResampleBuffers = new NativeArray<float>(inputChannels * (dspBufferLength + 1) * 2, Allocator.AudioKernel);
        OutBuffers = new NativeArray<float>(inputChannels * dspBufferLength * 2, Allocator.AudioKernel);

        for (int i = 0; i < inputChannels; i++)
        {
            // set position to "end of buffer", to force pulling data on first iteration
            ResamplerPositions[i] = (double)(dspBufferLength + 1);
        }
    }

    public void Execute(ref ExecuteContext<Parameters, SampleProviders> context)
    {
        if (Playing)
        {
            var buffer = context.Outputs.GetSampleBuffer(0);
            NativeArray<float> output = buffer.Buffer;

            bool finished = false;
            for (int i = 0; i < inputChannels; i++)
            {
                var provider = context.Providers.GetSampleProvider(i);
                if(provider.Valid)
                {
                    double position = ResamplerPositions[i];

                    finished |= ResampleLerpRead(context.Parameters, (Parameters)i, OutBuffers, i * dspBufferLength * 2, dspBufferLength * 2, ResampleBuffers, i * (dspBufferLength + 1) * 2, (dspBufferLength + 1) * 2, provider, context.DSPClock, ref position);

                    ResamplerPositions[i] = position;
                }
            }

            int OutBuffersLength = dspBufferLength * 2;
            for (int i = 0; i < inputChannels; i++)
            {
                var provider = context.Providers.GetSampleProvider(i);
                if (provider.Valid)
                {
                    int OutBuffersFrom = i * dspBufferLength * 2;

                    for (var j = 0; j < OutBuffersLength / 2; j++)
                    {
                        output[j * 2 + 0] += OutBuffers[OutBuffersFrom + j * 2 + 0];
                        output[j * 2 + 1] += OutBuffers[OutBuffersFrom + j * 2 + 1];
                    }
                }
            }

            for (var i = 0; i < OutBuffersLength / 2; i++)
            {
                // 0.3f * (float)Math.Sin(0.02 * j);
                output[i * 2 + 0] /= 8;
                output[i * 2 + 1] /= 8;
            }

            if (finished)
            {
                context.PostEvent(new ClipStopped());
                Playing = false;
            }
        }
    }

    public void Dispose()
    {
        if (ResampleBuffers.IsCreated)
            ResampleBuffers.Dispose();

        if (OutBuffers.IsCreated)
            OutBuffers.Dispose();

        if (ResamplerPositions.IsCreated)
            ResamplerPositions.Dispose();
    }
}

[BurstCompile(CompileSynchronously = true)]
struct PlayClipNode2 : IAudioKernel<PlayClipNode2.Parameters, PlayClipNode2.Providers>
{
    // Parameters are currently defined with enumerations. Each enum value corresponds to
    // a parameter within the node. Setting a value for a parameter uses these enum values.
    public enum Parameters { }

    // Sample providers are defined with enumerations. Each enum value defines a slot where
    // a sample provider can live on a IAudioKernel. Sample providers are used to get samples from
    // AudioClips and VideoPlayers. They will eventually be able to pull samples from microphones and other concepts.
    public enum Providers { }

    // The clip sample rate might be different to the output rate used by the system. Therefore we use a resampler
    // here.

    public void Initialize()
    {

    }

    public void Execute(ref ExecuteContext<Parameters, Providers> context)
    {
        var outputBuffer = context.Outputs.GetSampleBuffer(0).Buffer;

        var inputBuff = context.Inputs.GetSampleBuffer(0).Buffer;
        //for (var s = 0; s < outputBuffer.Length; s++) outputBuffer[s] += 3.2f * inputBuff[s];
        for (var i = 0; i < outputBuffer.Length / 2; i++)
        {
            outputBuffer[i * 2 + 0] += 3 * inputBuff[i * 8 + 2];
            outputBuffer[i * 2 + 1] += 3 * inputBuff[i * 8 + 3];
        }
        //var read = provider1.Read(arr);//.Slice(0, arr.Length));

        /*
        NativeArray<float> output = buffer.Buffer;
        for (var i = 0; i < output.Length / 2; i++)
        {
            output[i * 2 + 0] += arr[i * 2 + 0];
            output[i * 2 + 1] += arr[i * 2 + 1];
        }
        */


    }

    public void Dispose()
    {

    }
}


[BurstCompile(CompileSynchronously = true)]
struct PlayClipKernel : IAudioKernelUpdate<Mach1PlayerNode.Parameters, Mach1PlayerNode.SampleProviders, Mach1PlayerNode>
{
    // This update job is used to kick off playback of the node.
    public void Update(ref Mach1PlayerNode audioKernel)
    {
        audioKernel.Playing = true;
    }
}

// Token Event that indicates that playback has finished
struct ClipStopped { }

// Bootstrap MonoBehaviour to get the example running.
public class Mach1Player : MonoBehaviour
{
    public AudioClip[] ClipToPlay;

    AudioOutputHandle m_Output;
    DSPGraph m_Graph;
    DSPNode m_Node;
    // DSPNode m_Node2;
    DSPNode m_NoiseFilter;
    DSPConnection m_Connection;
    //DSPConnection m_Connection1;

    int m_HandlerID;

    void Start()
    {
        var format = Conversion.ConvertSpeakerMode(AudioSettings.speakerMode);
        var channels = Conversion.ConvertSoundFormatToChannels(format);
        AudioSettings.GetDSPBufferSize(out var bufferLength, out var numBuffers);

        var sampleRate = AudioSettings.outputSampleRate;

        m_Graph = DSPGraph.Create(format, channels, bufferLength, sampleRate);

        var driver = new DefaultDSPGraphDriver { Graph = m_Graph };
        m_Output = driver.AttachToDefaultOutput();

        Debug.Log("bufferLength: " + bufferLength);

        // Add an event handler delegate to the graph for ClipStopped. So we are notified
        // of when a clip is stopped in the node and can handle the resources on the main thread.
        m_HandlerID = m_Graph.AddNodeEventHandler<ClipStopped>((node, evt) =>
        {
            Debug.Log("Received ClipStopped event on main thread, cleaning resources");
        });

        // All async interaction with the graph must be done through a DSPCommandBlock.
        // Create it here and complete it once all commands are added.
        var block = m_Graph.CreateCommandBlock();

        m_Node = block.CreateDSPNode<Mach1PlayerNode.Parameters, Mach1PlayerNode.SampleProviders, Mach1PlayerNode>();
        block.AddOutletPort(m_Node, 2, SoundFormat.Stereo);
        //block.AddOutletPort(m_Node, 8, SoundFormat.SevenDot1);

        /*
        m_Node2 = block.CreateDSPNode<PlayClipNode2.Parameters, PlayClipNode2.Providers, PlayClipNode2>(); // 
        block.AddInletPort(m_Node2, 8, SoundFormat.SevenDot1);
        block.AddOutletPort(m_Node2, 2, SoundFormat.Stereo);
        */

        // Currently input and output ports are dynamic and added via this API to a node.
        // This will change to a static definition of nodes in the future.
        // block.AddOutletPort(m_Node, 2, SoundFormat.Stereo);

        // Connect the node to the root of the graph.
        //m_Connection1 = block.Connect(m_Node, 0, m_Node2, 0);
        m_Connection = block.Connect(m_Node, 0, m_Graph.RootDSP, 0); // m_Node2

        // We are done, fire off the command block atomically to the mixer thread.
        block.Complete();
    }

    void Update()
    {
        m_Graph.Update();
    }

    void OnDisable()
    {
        // Command blocks can also be completed via the C# 'using' construct for convenience
        using (var block = m_Graph.CreateCommandBlock())
        {
            block.Disconnect(m_Connection);
            block.ReleaseDSPNode(m_Node);
        }

        m_Graph.RemoveNodeEventHandler(m_HandlerID);

        m_Output.Dispose();
    }

    void OnGUI()
    {
        if (GUI.Button(new Rect(10, 10, 150, 100), "Play Clips!"))
        {
            if (ClipToPlay.Length == 0)
            {
                Debug.Log("No clip assigned, not playing (" + gameObject.name + ")");
                return;
            }

            using (var block = m_Graph.CreateCommandBlock())
            {
                for (var i = 0; i < ClipToPlay.Length; i++)
                {
                    // Decide on playback rate here by taking the provider input rate and the output settings of the system
                    var resampleRate = (float)ClipToPlay[i].frequency / AudioSettings.outputSampleRate;
                    Debug.Log("Clip freq:" + resampleRate);

                    block.SetFloat<Mach1PlayerNode.Parameters, Mach1PlayerNode.SampleProviders, Mach1PlayerNode>(m_Node, (Mach1PlayerNode.Parameters)i, resampleRate);

                    // Assign the sample provider to the slot of the node.
                    block.SetSampleProvider<Mach1PlayerNode.Parameters, Mach1PlayerNode.SampleProviders, Mach1PlayerNode>(ClipToPlay[i], m_Node, (Mach1PlayerNode.SampleProviders)i);
                }

                // Kick off playback. This will be done in a better way in the future.
                block.UpdateAudioKernel<PlayClipKernel, Mach1PlayerNode.Parameters, Mach1PlayerNode.SampleProviders, Mach1PlayerNode>(new PlayClipKernel(), m_Node);
            }
        }
    }
}