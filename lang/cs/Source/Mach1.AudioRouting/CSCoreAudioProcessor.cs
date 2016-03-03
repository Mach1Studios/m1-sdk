using System;
using CSCore;
using CSCore.Codecs;
using CSCore.SoundOut;
using CSCore.Streams;

namespace Mach1.AudioRouting
{
	public class CSCoreAudioProcessor : IAudioProcessor
	{
		public event EventHandler<float> MultiPeakCalculated;
		public event EventHandler<float> OmniPeakCalculated;

		private readonly ISoundOut _multiOut;
		private readonly ISoundOut _omniOut;

		private float _multiVolume;
		private float _omniVolume;
		private PeakMeter _multiPeakMeter;
		private PeakMeter _omniPeakMeter;

		public CSCoreAudioProcessor()
		{
			_multiOut = new WasapiOut();
			_omniOut = new WasapiOut();
		}

		public void PlayMulti(string filePath)
		{
			StopMulti();
			_multiPeakMeter = new PeakMeter(CodecFactory.Instance.GetCodec(filePath).ToSampleSource());
			Play(_multiOut, _multiPeakMeter, _multiVolume);
			SubscribeToPeakCalculated(_multiPeakMeter, MultiPeakCalculated);
		}

		public void PlayOmni(string filePath)
		{
			StopOmni();
			_omniPeakMeter = new PeakMeter(CodecFactory.Instance.GetCodec(filePath).ToSampleSource());
			Play(_omniOut, _omniPeakMeter, _omniVolume);
			SubscribeToPeakCalculated(_omniPeakMeter, OmniPeakCalculated);
		}

		private void Play(ISoundOut soundOut, PeakMeter peakMeter, float volume)
		{
			peakMeter.Interval = 200;
			soundOut.Initialize(peakMeter.ToWaveSource());
			soundOut.Volume = volume;
			soundOut.Play();
		}

		private void SubscribeToPeakCalculated(PeakMeter peakMeter, EventHandler<float> onPeakCalculated)
		{
			peakMeter.PeakCalculated += (sender, args) => onPeakCalculated(this, args.PeakValue);
		}

		public void StopMulti()
		{
			Stop(_multiOut);
			_multiPeakMeter?.Dispose();
		}

		public void StopOmni()
		{
			Stop(_omniOut);
			_omniPeakMeter?.Dispose();
		}

		private void Stop(ISoundOut soundOut)
		{
			if (soundOut.PlaybackState == PlaybackState.Playing)
			{
				soundOut.Stop();
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
			SetVolume(_multiVolume, _multiOut);
		}

		public void SetOmniVolume(double volume)
		{
			_omniVolume = (float)volume;
			SetVolume(_omniVolume, _omniOut);
		}

		private void SetVolume(float volume, ISoundOut soundOut)
		{
			if (soundOut.PlaybackState == PlaybackState.Playing)
			{
				soundOut.Volume = volume;
			}
		}

		public void Dispose()
		{
			_omniPeakMeter?.Dispose();
			_multiPeakMeter?.Dispose();
		}
	}
}
