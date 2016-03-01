using CSCore.Codecs;
using CSCore.SoundOut;

namespace Mach1.AudioRouting
{
	public class CSCoreAudioProcessor : IAudioProcessor
	{
		private WasapiOut _wasapiOut = new WasapiOut();

		public void Play(string filePath)
		{
			Stop();
			_wasapiOut.Initialize(CodecFactory.Instance.GetCodec(filePath));
			_wasapiOut.Play();
		}

		public void Stop()
		{
			if (_wasapiOut.PlaybackState == PlaybackState.Playing)
			{
				_wasapiOut.Stop();
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

		public void Dispose()
		{
			Stop();
			_wasapiOut.Dispose();
		}
	}
}
