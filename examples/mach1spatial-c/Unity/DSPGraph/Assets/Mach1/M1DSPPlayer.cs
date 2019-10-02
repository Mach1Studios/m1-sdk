using System;
using UnityEngine;
using Unity.Audio;
using Unity.Collections;
using Unity.Collections.LowLevel.Unsafe;
using Unity.Burst;
using System.Collections.Generic;

// The 'audio job'. This is the kernel that defines a running DSP node inside the
// DSPGraph. It is a struct that implements the IAudioKernel interface. It can contain
// internal state, and will have the Execute function called as part of the graph
// traversal during an audio frame.
[BurstCompile(CompileSynchronously = true)]
struct M1DSPPlayerNode : IAudioKernel<M1DSPPlayerNode.Parameters, M1DSPPlayerNode.Providers>
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
                if (provider.Valid)
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
struct M1DSPWriterNode : IAudioKernel<M1DSPWriterNode.Parameters, M1DSPWriterNode.Providers>
{
    public static List<float[]> writerData = new List<float[]>();
    
    // Parameters are currently defined with enumerations. Each enum value corresponds to
    // a parameter within the node. Setting a value for a parameter uses these enum values.
    public enum Parameters
    {
        WriterIndex
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

        var inputBuffer = context.Inputs.GetSampleBuffer(0).Buffer;

        float[] arr = new float[inputBuffer.Length];

        for (var i = 0; i < inputBuffer.Length; i++)
        {
            outputBuffer[i] = inputBuffer[i];

            arr[i] = inputBuffer[i];
        }

        writerData.Add(arr);
        //Debug.Log(">> " + writerData.Count);
    }

    public void Dispose()
    {
    }
}

[BurstCompile(CompileSynchronously = true)]
struct M1DSPDecodeNode : IAudioKernel<M1DSPDecodeNode.Parameters, M1DSPDecodeNode.Providers>
{
    // Parameters are currently defined with enumerations. Each enum value corresponds to
    // a parameter within the node. Setting a value for a parameter uses these enum values.
    public enum Parameters
    {
        Coeff0,
        Coeff1,
        Coeff2,
        Coeff3,
        Coeff4,
        Coeff5,
        Coeff6,
        Coeff7,
        Coeff8,
        Coeff9,
        Coeff10,
        Coeff11,
        Coeff12,
        Coeff13,
        Coeff14,
        Coeff15
    }

    // Sample providers are defined with enumerations. Each enum value defines a slot where
    // a sample provider can live on a IAudioKernel. Sample providers are used to get samples from
    // AudioClips and VideoPlayers. They will eventually be able to pull samples from microphones and other concepts.
    public enum Providers { }

    // The clip sample rate might be different to the output rate used by the system. Therefore we use a resampler
    // here.

    [NativeDisableContainerSafetyRestriction]
    public NativeArray<float> coeffs;

    public void Initialize()
    {
        coeffs = new NativeArray<float>(16, Allocator.AudioKernel);
    }

    public void Execute(ref ExecuteContext<Parameters, Providers> context)
    {
        var outputBuffer = context.Outputs.GetSampleBuffer(0).Buffer;

        var inputBuffer = context.Inputs.GetSampleBuffer(0).Buffer;

        for (var i = 0; i < 16; i++)
        {
            coeffs[i] = context.Parameters.GetFloat((Parameters)i, 0);
        }

        for (var c = 0; c < 8; c++)
        {
            int channel0 = 2 * c + 0;
            int channel1 = 2 * c + 1;

            for (var i = 0; i < outputBuffer.Length / 2; i++)
            {
                outputBuffer[i * 2 + 0] += 1.0f * inputBuffer[i * 8 + c] * coeffs[channel0];
                outputBuffer[i * 2 + 1] += 1.0f * inputBuffer[i * 8 + c] * coeffs[channel1];
            }
        }
    }

    public void Dispose()
    {
        if (coeffs.IsCreated)
            coeffs.Dispose();
    }
}


[BurstCompile(CompileSynchronously = true)]
struct M1DSPEncodeNode : IAudioKernel<M1DSPEncodeNode.Parameters, M1DSPEncodeNode.Providers>
{
    // Parameters are currently defined with enumerations. Each enum value corresponds to
    // a parameter within the node. Setting a value for a parameter uses these enum values.
    public enum Parameters
    {
        inputChannels,
        Point0_Gain0,
        Point0_Gain1,
        Point0_Gain2,
        Point0_Gain3,
        Point0_Gain4,
        Point0_Gain5,
        Point0_Gain6,
        Point0_Gain7,
        Point1_Gain0,
        Point1_Gain1,
        Point1_Gain2,
        Point1_Gain3,
        Point1_Gain4,
        Point1_Gain5,
        Point1_Gain6,
        Point1_Gain7,
        Point2_Gain0,
        Point2_Gain1,
        Point2_Gain2,
        Point2_Gain3,
        Point2_Gain4,
        Point2_Gain5,
        Point2_Gain6,
        Point2_Gain7,
        Point3_Gain0,
        Point3_Gain1,
        Point3_Gain2,
        Point3_Gain3,
        Point3_Gain4,
        Point3_Gain5,
        Point3_Gain6,
        Point3_Gain7,
    }

    // Sample providers are defined with enumerations. Each enum value defines a slot where
    // a sample provider can live on a IAudioKernel. Sample providers are used to get samples from
    // AudioClips and VideoPlayers. They will eventually be able to pull samples from microphones and other concepts.
    public enum Providers { }

    // The clip sample rate might be different to the output rate used by the system. Therefore we use a resampler
    // here.

    [NativeDisableContainerSafetyRestriction]
    public NativeArray<float> gains;

    public void Initialize()
    {
        gains = new NativeArray<float>(4 * 8, Allocator.AudioKernel);
    }

    public void Execute(ref ExecuteContext<Parameters, Providers> context)
    {
        var outputBuffer = context.Outputs.GetSampleBuffer(0).Buffer;

        var inputBuffer = context.Inputs.GetSampleBuffer(0).Buffer;

        int length = outputBuffer.Length / 8;
        int inputChannels = (int)context.Parameters.GetFloat(Parameters.inputChannels, 0);

        for (var i = 0; i < inputChannels; i++)
        {
            for (var j = 0; j < 8; j++)
            {
                gains[i * 8 + j] = context.Parameters.GetFloat((Parameters)(1 + i * 8 + j), 0);
            }
        }

        for (var i = 0; i < length; i++)
        {
            for (var j = 0; j < inputChannels; j++)
            {
                float input = inputBuffer[i * 8 + j];
                for (var c = 0; c < 8; c++)
                {
                    outputBuffer[i * 8 + c] += 1.0f * input * gains[j * 8 + c];
                }

            }
        }
    }

    public void Dispose()
    {
        if (gains.IsCreated)
            gains.Dispose();
    }
}



[BurstCompile(CompileSynchronously = true)]
struct PlayClipKernel : IAudioKernelUpdate<M1DSPPlayerNode.Parameters, M1DSPPlayerNode.Providers, M1DSPPlayerNode>
{
    // This update job is used to kick off playback of the node.
    public void Update(ref M1DSPPlayerNode audioKernel)
    {
        audioKernel.Playing = true;
    }
}

[BurstCompile(CompileSynchronously = true)]
struct StopClipKernel : IAudioKernelUpdate<M1DSPPlayerNode.Parameters, M1DSPPlayerNode.Providers, M1DSPPlayerNode>
{
    // This update job is used to kick off playback of the node.
    public void Update(ref M1DSPPlayerNode audioKernel)
    {
        audioKernel.Playing = false;
    }
}

// Token Event that indicates that playback has finished
struct ClipStopped { }

// Bootstrap MonoBehaviour to get the example running.
public class M1DSPPlayer// : MonoBehaviour
{
    public AudioClip[] audioClips;
    public float[] coeffs;
    public float[][] gains;

    public bool isPlaying;
    public bool useEncode;
    public bool useWriter;

    AudioOutputHandle output;
    DSPGraph dpsGraph;
    DSPNode nodePlayer;
    DSPNode nodeWriter;
    DSPNode nodeEncode;
    DSPNode nodeDecode;
    DSPConnection connectionDecodeNode;
    DSPConnection connectionEncodeNode;
    DSPConnection connectionPlayerNode;
    DSPConnection connectionWriterNode;

    int handlerID;

    public void Start()
    {
        // init
        if (audioClips == null)
        {
            audioClips = new AudioClip[8];
        }
        coeffs = new float[16];
        isPlaying = false;

        // DSPGraph init
        var format = Conversion.ConvertSpeakerMode(AudioSettings.speakerMode);
        var channels = Conversion.ConvertSoundFormatToChannels(format);
        AudioSettings.GetDSPBufferSize(out var bufferLength, out var numBuffers);

        var sampleRate = AudioSettings.outputSampleRate;

        dpsGraph = DSPGraph.Create(format, channels, bufferLength, sampleRate);

        var driver = new DefaultDSPGraphDriver { Graph = dpsGraph };
        output = driver.AttachToDefaultOutput();

        Debug.Log("dsp bufferLength: " + bufferLength);

        // Add an event handler delegate to the graph for ClipStopped. So we are notified
        // of when a clip is stopped in the node and can handle the resources on the main thread.
        handlerID = dpsGraph.AddNodeEventHandler<ClipStopped>((node, evt) =>
        {
            Debug.Log("Received ClipStopped event on main thread, cleaning resources");
            isPlaying = false;
        });

        // All async interaction with the graph must be done through a DSPCommandBlock.
        // Create it here and complete it once all commands are added.
        var block = dpsGraph.CreateCommandBlock();

        nodePlayer = block.CreateDSPNode<M1DSPPlayerNode.Parameters, M1DSPPlayerNode.Providers, M1DSPPlayerNode>();
        block.SetFloat<M1DSPPlayerNode.Parameters, M1DSPPlayerNode.Providers, M1DSPPlayerNode>(nodePlayer, M1DSPPlayerNode.Parameters.DSPBufferLength, bufferLength);
        block.AddOutletPort(nodePlayer, 8, SoundFormat.SevenDot1);

        if(useWriter)
        { 
            nodeWriter = block.CreateDSPNode<M1DSPWriterNode.Parameters, M1DSPWriterNode.Providers, M1DSPWriterNode>();
            block.SetFloat<M1DSPWriterNode.Parameters, M1DSPWriterNode.Providers, M1DSPWriterNode>(nodeWriter, M1DSPWriterNode.Parameters.WriterIndex, 0);
            block.AddInletPort(nodeWriter, 8, SoundFormat.SevenDot1);
            block.AddOutletPort(nodeWriter, 8, SoundFormat.SevenDot1);
        }

        if (useEncode)
        {
            nodeEncode = block.CreateDSPNode<M1DSPEncodeNode.Parameters, M1DSPEncodeNode.Providers, M1DSPEncodeNode>();
            block.AddInletPort(nodeEncode, 8, SoundFormat.SevenDot1);
            block.AddOutletPort(nodeEncode, 8, SoundFormat.SevenDot1);
        }

        nodeDecode = block.CreateDSPNode<M1DSPDecodeNode.Parameters, M1DSPDecodeNode.Providers, M1DSPDecodeNode>();
        block.AddInletPort(nodeDecode, 8, SoundFormat.SevenDot1);
        block.AddOutletPort(nodeDecode, 2, SoundFormat.Stereo);

        // Connect the node to the root of the graph.
        if (!useEncode)
        {
            if (useWriter)
            {
                connectionWriterNode = block.Connect(nodePlayer, 0, nodeWriter, 0);
                connectionPlayerNode = block.Connect(nodeWriter, 0, nodeDecode, 0);
            }
            else
            {
                connectionPlayerNode = block.Connect(nodePlayer, 0, nodeDecode, 0);
            }
        }
        else
        {
            if (useWriter)
            {
                connectionPlayerNode = block.Connect(nodePlayer, 0, nodeEncode, 0);
                connectionWriterNode = block.Connect(nodeEncode, 0, nodeWriter, 0);
                connectionEncodeNode = block.Connect(nodeWriter, 0, nodeDecode, 0);
            }
            else
            {
                connectionPlayerNode = block.Connect(nodePlayer, 0, nodeEncode, 0);
                connectionEncodeNode = block.Connect(nodeEncode, 0, nodeDecode, 0);
            }
        }
        connectionDecodeNode = block.Connect(nodeDecode, 0, dpsGraph.RootDSP, 0);

        // We are done, fire off the command block atomically to the mixer thread.
        block.Complete();
    }

    public void Update()
    {
        //Debug.Log("dsp update: " + coeffs.Length);

        if (coeffs != null && coeffs.Length >= 16)
        {
            using (var block = dpsGraph.CreateCommandBlock())
            {
                for (var i = 0; i < 16; i++)
                {
                    block.SetFloat<M1DSPDecodeNode.Parameters, M1DSPDecodeNode.Providers, M1DSPDecodeNode>(nodeDecode, (M1DSPDecodeNode.Parameters)i, coeffs[i]);
                }

                if (useEncode)
                {
                    block.SetFloat<M1DSPEncodeNode.Parameters, M1DSPEncodeNode.Providers, M1DSPEncodeNode>(nodeEncode, M1DSPEncodeNode.Parameters.inputChannels, gains.Length);
                    for (var i = 0; i < gains.Length; i++)
                    {
                        for (var j = 0; j < gains[i].Length; j++)
                        {
                            block.SetFloat<M1DSPEncodeNode.Parameters, M1DSPEncodeNode.Providers, M1DSPEncodeNode>(nodeEncode, (M1DSPEncodeNode.Parameters)(1 + i * 8 + j), gains[i][j]);
                        }
                    }
                }

            }
            dpsGraph.Update();
        }
    }

    public void OnDisable()
    {
        // Command blocks can also be completed via the C# 'using' construct for convenience
        using (var block = dpsGraph.CreateCommandBlock())
        {
            block.Disconnect(connectionPlayerNode);
            block.Disconnect(connectionDecodeNode);
            if (useEncode)
            {
                block.Disconnect(connectionEncodeNode);
            }
            if (useWriter)
            {
                block.Disconnect(connectionWriterNode);
            }

            block.ReleaseDSPNode(nodePlayer);
            block.ReleaseDSPNode(nodeDecode);
            if (useEncode)
            {
                block.ReleaseDSPNode(nodeEncode);
            }
            if (useWriter)
            {
                block.ReleaseDSPNode(nodeWriter);
            }
        }

        dpsGraph.RemoveNodeEventHandler(handlerID);

        output.Dispose();
    }

    public void Stop()
    {
        Debug.Log("dsp stop");

        using (var block = dpsGraph.CreateCommandBlock())
        {
            block.UpdateAudioKernel<StopClipKernel, M1DSPPlayerNode.Parameters, M1DSPPlayerNode.Providers, M1DSPPlayerNode>(new StopClipKernel(), nodePlayer);
            isPlaying = false;
        }
    }

    public void Play()
    {
        Debug.Log("dsp play: " + audioClips.Length);

        using (var block = dpsGraph.CreateCommandBlock())
        {
            for (var i = 0; i < audioClips.Length; i++)
            {
                // Decide on playback rate here by taking the provider input rate and the output settings of the system
                var resampleRate = (float)audioClips[i].frequency / AudioSettings.outputSampleRate;
                Debug.Log("dsp resampleRate: " + resampleRate);

                block.SetFloat<M1DSPPlayerNode.Parameters, M1DSPPlayerNode.Providers, M1DSPPlayerNode>(nodePlayer, (M1DSPPlayerNode.Parameters)i, resampleRate);

                // Assign the sample provider to the slot of the node.
                block.SetSampleProvider<M1DSPPlayerNode.Parameters, M1DSPPlayerNode.Providers, M1DSPPlayerNode>(audioClips[i], nodePlayer, (M1DSPPlayerNode.Providers)i);
            }

            // Kick off playback. This will be done in a better way in the future.
            block.UpdateAudioKernel<PlayClipKernel, M1DSPPlayerNode.Parameters, M1DSPPlayerNode.Providers, M1DSPPlayerNode>(new PlayClipKernel(), nodePlayer);

            isPlaying = true;
        }
    }

    
    void OnGUI()
    {
        if (GUI.Button(new Rect(10, 10, 150, 100), "Play Clips!"))
        {
            Play();
        }
    }
    
}