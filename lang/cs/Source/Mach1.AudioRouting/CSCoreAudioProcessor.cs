using System;
using System.Collections.Generic;
using System.Linq;
using CSCore;
using CSCore.Codecs;
using CSCore.DSP;
using CSCore.SoundOut;
using CSCore.Streams;
using Mach1.AudioRouting.CustomSources;
using Mach1.AudioRouting.DirectionToChannelsMapping;
using Mach1.AudioRouting.DirectionToPairsVolumeMapping;

namespace Mach1.AudioRouting
{
	enum MultiInputType
	{
		SingleFile,
		MultipleFiles
	}

	public class CSCoreAudioProcessor : IAudioProcessor
	{
		public event EventHandler<PeakEventArgs> MasterPeakCalculated;
		public event EventHandler<PeakEventArgs> OmniPeakCalculated;

		private readonly ISoundOut _soundOut;

		private float _multiVolume = 1;
		private float _omniVolume = 1;
		private float _horizontalAngle;
		private float _verticalAngle;
		private PeakMeter _masterPeakMeter;
		private PeakMeter _omniPeakMeter;
		private DmoChannelResampler _channelResampler;
		private DirectionToChannelsMapper _directionToChannelsMapper;
		private DirectionToPairsVolumeMapper _directionToPairsVolumeMapper;
		private Mixer _mixer;
		private string _omniFilePath;
		private MultiInputType _multiInputType;
		public static bool DebugModeEnabled;

		public CSCoreAudioProcessor(bool debugModeEnabled = false)
		{
			DebugModeEnabled = debugModeEnabled;
			_soundOut = new WasapiOut();
		}

		public void LoadMultiSource(IReadOnlyList<string> multiFilePaths)
		{
			Stop();
			_mixer?.Dispose();
			_mixer = null;
			_multiInputType = multiFilePaths.Count == 1
				? MultiInputType.SingleFile
				: MultiInputType.MultipleFiles;
			IWaveSource[] waveSources = multiFilePaths.Select(
				filePath => CodecFactory.Instance.GetCodec(filePath)).ToArray();
			for (int i = 0; i < waveSources.Length; i++)
			{
				IWaveSource waveSource = waveSources[i];
				if (_mixer == null)
				{
					_mixer = new Mixer(2, waveSource.WaveFormat.SampleRate);
				}
				if (_multiInputType == MultiInputType.SingleFile)
				{
					_mixer.AddSource(ApplyChannelMatrix(waveSource).ToSampleSource());
				}
				else if (_multiInputType == MultiInputType.MultipleFiles)
				{
					_mixer.AddSource(waveSource.ToSampleSource());
				}
			}
			if (_multiInputType == MultiInputType.MultipleFiles)
			{
				_directionToPairsVolumeMapper = DirectionToPairsVolumeMapper
					.CreateMapper(_mixer);
				_directionToPairsVolumeMapper.ApplyVerticalAngle(_verticalAngle);
				_directionToPairsVolumeMapper.ApplyHorizontalAngle(_horizontalAngle);
			}
			_masterPeakMeter = new PeakMeter(_mixer);
			_masterPeakMeter.Interval = 50;
			SubscribeToPeakCalculated(_masterPeakMeter, MasterPeakCalculated);
			if (_omniPeakMeter != null)
			{
				LoadOmniSource(_omniFilePath);
			}
			_soundOut.Initialize(_masterPeakMeter.ToWaveSource());
		}

		private IWaveSource ApplyChannelMatrix(IWaveSource waveSource)
		{
			ChannelMatrix matrix = null;
			switch (waveSource.WaveFormat.Channels)
			{
				case 8:
					matrix = M1ChannelMatrix.SevenOneToStereo;
					break;
				case 6:
					matrix = M1ChannelMatrix.FiveOneToStereo;
					break;
				case 4:
					matrix = M1ChannelMatrix.QuadToStereo;
					break;
			}
			if (matrix == null)
			{
				_channelResampler = null;
				_directionToChannelsMapper = null;
				return waveSource;
			}
			var source = waveSource.AppendSource(
				s => new DmoChannelResampler(s, matrix), out _channelResampler);
			_directionToChannelsMapper = DirectionToChannelsMapper.CreateMapper(_channelResampler);
			_directionToChannelsMapper.ApplyHorizontalAngle(_horizontalAngle);
			_directionToChannelsMapper.ApplyVerticalAngle(_verticalAngle);
			return source;
		}

		public void LoadOmniSource(string omniFilePath)
		{
			Stop();
			_omniFilePath = omniFilePath;
			if (string.IsNullOrEmpty(omniFilePath) && _omniPeakMeter != null)
			{
				_mixer.RemoveOmniSource();
				_omniPeakMeter.Dispose();
				_omniPeakMeter = null;
			}
			else
			{
				_mixer.RemoveOmniSource();
				_omniPeakMeter?.Dispose();
				IWaveSource waveSource = CodecFactory.Instance.GetCodec(omniFilePath);
				_omniPeakMeter = new PeakMeter(waveSource.ToSampleSource());
				_omniPeakMeter.Interval = 50;
				_mixer.AddOmniSource(_omniPeakMeter);
				_mixer.OmniVolume = _omniVolume;
				SubscribeToPeakCalculated(_omniPeakMeter, OmniPeakCalculated);
			}
		}

		public void Play()
		{
			_soundOut.Volume = _multiVolume;
			_soundOut.Play();
		}

		private void SubscribeToPeakCalculated(PeakMeter peakMeter,
			EventHandler<PeakEventArgs> onPeakCalculated)
		{
			peakMeter.PeakCalculated += (sender, args) => onPeakCalculated(this, new PeakEventArgs
			{
				ChannelPeakValues = args.ChannelPeakValues,
				PeakValue = args.PeakValue
			});
		}

		public void Stop()
		{
			if (_soundOut.PlaybackState == PlaybackState.Playing)
			{
				_soundOut.Stop();
			}
		}

		public string[] GetSupportedExtensions()
		{
			return CodecFactory.Instance.GetSupportedFileExtensions();
		}

		public string GetSupportedExtensionsFilterString()
		{
			return CodecFactory.SupportedFilesFilterEn;
		}

		public void SetMultiVolume(double volume)
		{
			_multiVolume = (float)volume;
			if (_soundOut.PlaybackState == PlaybackState.Playing)
			{
				_soundOut.Volume = _multiVolume;
			}
		}

		public void SetOmniVolume(double volume)
		{
			_omniVolume = (float)volume;
			_mixer.OmniVolume = _omniVolume;
		}

		public void SetHorizontalAngle(double angle)
		{
			_horizontalAngle = (float)angle;
			if (_multiInputType == MultiInputType.SingleFile)
			{
				_directionToChannelsMapper?.ApplyHorizontalAngle(_horizontalAngle);
				if (DebugModeEnabled)
				{
					_directionToChannelsMapper?.ShowDebugInfo();
				}
			}
			else if (_multiInputType == MultiInputType.MultipleFiles)
			{
				_directionToPairsVolumeMapper?.ApplyHorizontalAngle(_horizontalAngle);
				if (DebugModeEnabled)
				{
					_mixer.ShowDebugInfo();
				}
			}
		}

		public void SetVerticalAngle(double angle)
		{
			_verticalAngle = (float)angle;
			if (_multiInputType == MultiInputType.SingleFile)
			{
				_directionToChannelsMapper?.ApplyVerticalAngle(_verticalAngle);
				if (DebugModeEnabled)
				{
					_directionToChannelsMapper?.ShowDebugInfo();
				}
			}
			else if (_multiInputType == MultiInputType.MultipleFiles)
			{
				_directionToPairsVolumeMapper?.ApplyVerticalAngle(_verticalAngle);
				if (DebugModeEnabled)
				{
					_mixer.ShowDebugInfo();
				}
			}
		}

		public void SetTiltAngle(double angle)
		{
			if (_multiInputType == MultiInputType.SingleFile 
				&& _directionToChannelsMapper is DirectionToSevenOneMapper)
			{
				((DirectionToSevenOneMapper)_directionToChannelsMapper).ApplyTiltAngle((float)angle);
				if (DebugModeEnabled)
				{
					_directionToChannelsMapper.ShowDebugInfo();
				}
			}
		}

		public void Dispose()
		{
			_mixer?.Dispose();
		}
	}
}
