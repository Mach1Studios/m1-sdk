using System;

namespace Mach1.AudioRouting
{
	public interface IAudioProcessor : IDisposable
	{
		event EventHandler<PeakEventArgs> MasterPeakCalculated;
		event EventHandler<PeakEventArgs> OmniPeakCalculated;

		void InitializeOmniSource(string omniFilePath);
		void InitializeMultiSource(string multiFilePath);

		void Play();
		void Stop();

		string[] GetSupportedExtensions();
		string GetSupportedExtensionsFilterString();

		void SetMultiVolume(double volume);
		void SetOmniVolume(double volume);

		void SetHorizontalAngle(double angle);
		void SetVerticalAngle(double angle);
	}
}
