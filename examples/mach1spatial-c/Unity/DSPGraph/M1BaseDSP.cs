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
struct M1PlayerNode : IAudioKernel<M1PlayerNode.Parameters, M1PlayerNode.Providers>
{
    // Parameters are currently defined with enumerations. Each enum value corresponds to
    // a parameter within the node. Setting a value for a parameter uses these enum values.
    public enum Parameters { Rate0, Rate1, Rate2, Rate3, Rate4, Rate5, Rate6, Rate7, DSPBufferLength }

    // Sample providers are defined with enumerations. Each enum value defines a slot where
    // a sample provider can live on a IAudioKernel. Sample providers are used to get samples from
    // AudioClips and VideoPlayers. They will eventually be able to pull samples from microphones and other concepts.
    public enum Providers { Slot0, Slot1, Slot2, Slot3, Slot4, Slot5, Slot6, Slot7 }

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
        dspBufferLength = 2048;
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

    public void Execute(ref ExecuteContext<Parameters, Providers> context)
    {
        dspBufferLength = (int)context.Parameters.GetFloat(Parameters.DSPBufferLength, 0);

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
                        output[j * 8 + i] = OutBuffers[OutBuffersFrom + j * 2 + 0]; // left rasampled channel
                    }
                }
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
struct M1DecodeNode : IAudioKernel<M1DecodeNode.Parameters, M1DecodeNode.Providers>
{
    // Parameters are currently defined with enumerations. Each enum value corresponds to
    // a parameter within the node. Setting a value for a parameter uses these enum values.
    public enum Parameters
    {
        ChannelL0, ChannelL1, ChannelL3, ChannelL4, ChannelL5, ChannelL6, ChannelL7, ChannelL8,
        ChannelR0, ChannelR1, ChannelR3, ChannelR4, ChannelR5, ChannelR6, ChannelR7, ChannelR8,
        DSPBufferLength
    }

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

        for (var i = 0; i < outputBuffer.Length / 2; i++)
        {
            outputBuffer[i * 2 + 0] += 3 * inputBuff[i * 8 + 0];
            outputBuffer[i * 2 + 1] += 3 * inputBuff[i * 8 + 1];
        }
    }

    public void Dispose()
    {

    }
}


[BurstCompile(CompileSynchronously = true)]
struct PlayClipKernel : IAudioKernelUpdate<M1PlayerNode.Parameters, M1PlayerNode.Providers, M1PlayerNode>
{
    // This update job is used to kick off playback of the node.
    public void Update(ref M1PlayerNode audioKernel)
    {
        audioKernel.Playing = true;
    }
}

// Token Event that indicates that playback has finished
struct ClipStopped { }

// Bootstrap MonoBehaviour to get the example running.
public class M1BaseDSP : MonoBehaviour
{
    public AudioClip[] ClipToPlay;

    AudioOutputHandle output;
    DSPGraph dpsGraph;
    DSPNode nodePlayer;
    DSPNode nodeDecode;
    DSPConnection connectionDecodeNode;
    DSPConnection connectionPlayerNode;
 
    int handlerID;

    void Start()
    {
        // 
        var format = Conversion.ConvertSpeakerMode(AudioSettings.speakerMode);
        var channels = Conversion.ConvertSoundFormatToChannels(format);
        AudioSettings.GetDSPBufferSize(out var bufferLength, out var numBuffers);

        var sampleRate = AudioSettings.outputSampleRate;

        dpsGraph = DSPGraph.Create(format, channels, bufferLength, sampleRate);

        var driver = new DefaultDSPGraphDriver { Graph = dpsGraph };
        output = driver.AttachToDefaultOutput();

        Debug.Log("bufferLength: " + bufferLength);

        // Add an event handler delegate to the graph for ClipStopped. So we are notified
        // of when a clip is stopped in the node and can handle the resources on the main thread.
        handlerID = dpsGraph.AddNodeEventHandler<ClipStopped>((node, evt) =>
        {
            Debug.Log("Received ClipStopped event on main thread, cleaning resources");
        });

        // All async interaction with the graph must be done through a DSPCommandBlock.
        // Create it here and complete it once all commands are added.
        var block = dpsGraph.CreateCommandBlock();

        nodePlayer = block.CreateDSPNode<M1PlayerNode.Parameters, M1PlayerNode.Providers, M1PlayerNode>();
        block.SetFloat<M1PlayerNode.Parameters, M1PlayerNode.Providers, M1PlayerNode>(nodePlayer, M1PlayerNode.Parameters.DSPBufferLength, bufferLength);
        block.AddOutletPort(nodePlayer, 8, SoundFormat.SevenDot1);

        nodeDecode = block.CreateDSPNode<M1DecodeNode.Parameters, M1DecodeNode.Providers, M1DecodeNode>();  
        block.SetFloat<M1DecodeNode.Parameters, M1DecodeNode.Providers, M1DecodeNode>(nodeDecode, M1DecodeNode.Parameters.DSPBufferLength, bufferLength);
        block.AddInletPort(nodeDecode, 8, SoundFormat.SevenDot1);
        block.AddOutletPort(nodeDecode, 2, SoundFormat.Stereo);

        // Connect the node to the root of the graph.
        connectionPlayerNode = block.Connect(nodePlayer, 0, nodeDecode, 0);
        connectionDecodeNode = block.Connect(nodeDecode, 0, dpsGraph.RootDSP, 0);

        // We are done, fire off the command block atomically to the mixer thread.
        block.Complete();
    }

    void Update()
    {
        using (var block = dpsGraph.CreateCommandBlock())
        {
            block.SetFloat<M1DecodeNode.Parameters, M1DecodeNode.Providers, M1DecodeNode>(nodeDecode, M1DecodeNode.Parameters.ChannelL0, 0);
            block.SetFloat<M1DecodeNode.Parameters, M1DecodeNode.Providers, M1DecodeNode>(nodeDecode, M1DecodeNode.Parameters.ChannelL1, 0);
            block.SetFloat<M1DecodeNode.Parameters, M1DecodeNode.Providers, M1DecodeNode>(nodeDecode, M1DecodeNode.Parameters.ChannelL2, 0);
        }
        
        dpsGraph.Update();
    }

    void OnDisable()
    {
        // Command blocks can also be completed via the C# 'using' construct for convenience
        using (var block = dpsGraph.CreateCommandBlock())
        {
            block.Disconnect(connectionPlayerNode);
            block.Disconnect(connectionDecodeNode);
            block.ReleaseDSPNode(nodePlayer);
            block.ReleaseDSPNode(nodeDecode);
        }

        dpsGraph.RemoveNodeEventHandler(handlerID);

        output.Dispose();
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

            using (var block = dpsGraph.CreateCommandBlock())
            {
                for (var i = 0; i < ClipToPlay.Length; i++)
                {
                    // Decide on playback rate here by taking the provider input rate and the output settings of the system
                    var resampleRate = (float)ClipToPlay[i].frequency / AudioSettings.outputSampleRate;
                    Debug.Log("Clip freq:" + resampleRate);

                    block.SetFloat<M1PlayerNode.Parameters, M1PlayerNode.Providers, M1PlayerNode>(nodePlayer, (M1PlayerNode.Parameters)i, resampleRate);

                    // Assign the sample provider to the slot of the node.
                    block.SetSampleProvider<M1PlayerNode.Parameters, M1PlayerNode.Providers, M1PlayerNode>(ClipToPlay[i], nodePlayer, (M1PlayerNode.Providers)i);
                }

                // Kick off playback. This will be done in a better way in the future.
                block.UpdateAudioKernel<PlayClipKernel, M1PlayerNode.Parameters, M1PlayerNode.Providers, M1PlayerNode>(new PlayClipKernel(), nodePlayer);
            }
        }
    }
}