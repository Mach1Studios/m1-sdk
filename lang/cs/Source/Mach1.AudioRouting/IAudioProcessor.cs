using System;

namespace Mach1.AudioRouting
{
	public interface IAudioProcessor : IDisposable
	{
		event EventHandler<float> MultiPeakCalculated;
		event EventHandler<float> OmniPeakCalculated;

		void PlayMulti(string filePath);
		void PlayOmni(string filePath);

		void StopMulti();
		void StopOmni();

		string[] GetSupportedExtensions();

		string GetSupportedExtensionsFilterString();

		void SetMultiVolume(double volume);
		void SetOmniVolume(double volume);
	}
}
