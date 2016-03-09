using System;
using CSCore;
using CSCore.Codecs;
using CSCore.DSP;
using CSCore.SoundOut;
using CSCore.Streams;

namespace Mach1.AudioRouting
{
	public class CSCoreAudioProcessor : IAudioProcessor
	{
		public event EventHandler<PeakEventArgs> MasterPeakCalculated;
		public event EventHandler<PeakEventArgs> OmniPeakCalculated;

		private readonly ISoundOut _soundOut;

		private float _multiVolume;
		private float _omniVolume;
		private PeakMeter _masterPeakMeter;
		private PeakMeter _omniPeakMeter;
		private VolumeControlSource _omniVolumeControl;

		private Mixer _mixer;
		private string _omniFilePath;

		public CSCoreAudioProcessor()
		{
			_soundOut = new WasapiOut();
		}

		public void InitializeMultiSource(string multiFilePath)
		{
			Stop();
			_mixer?.Dispose();
			IWaveSource waveSource = CodecFactory.Instance.GetCodec(multiFilePath);
			waveSource = ApplyChannelMatrix(waveSource);
			_mixer = new Mixer(waveSource.WaveFormat.Channels, waveSource.WaveFormat.SampleRate);
			_mixer.AddSource(waveSource.ToSampleSource());
			_masterPeakMeter = new PeakMeter(_mixer);
			_masterPeakMeter.Interval = 50;
			SubscribeToPeakCalculated(_masterPeakMeter, MasterPeakCalculated);
			if (_omniPeakMeter != null)
			{
				InitializeOmniSource(_omniFilePath);
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
				return waveSource;
			}
			return waveSource.AppendSource(s => new DmoChannelResampler(s, matrix));
		}

		public void InitializeOmniSource(string omniFilePath)
		{
			Stop();
			_omniFilePath = omniFilePath;
			if (string.IsNullOrEmpty(omniFilePath) && _omniPeakMeter != null)
			{
				_mixer.RemoveSource(_omniPeakMeter);
				_omniPeakMeter.Dispose();
				_omniPeakMeter = null;
				_omniVolumeControl = null;
			}
			else
			{
				_mixer.RemoveSource(_omniPeakMeter);
				_omniPeakMeter?.Dispose();
				IWaveSource waveSource = CodecFactory.Instance.GetCodec(omniFilePath);
				_omniPeakMeter = new PeakMeter(waveSource.ToSampleSource());
				_omniPeakMeter.Interval = 50;
				_omniVolumeControl = new VolumeControlSource(_omniPeakMeter)
				{
					Volume = _omniVolume
				};
				_mixer.AddSource(_omniVolumeControl);
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
			if (_omniVolumeControl != null)
			{
				_omniVolumeControl.Volume = _omniVolume;
			}
		}

		public void Dispose()
		{
			_mixer?.Dispose();
		}
	}
}
