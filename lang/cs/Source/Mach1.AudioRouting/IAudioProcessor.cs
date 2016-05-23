using System;
using System.Collections.Generic;

namespace Mach1.AudioRouting
{
	public interface IAudioProcessor : IDisposable
	{
		event EventHandler<PeakEventArgs> MasterPeakCalculated;
		event EventHandler<PeakEventArgs> OmniPeakCalculated;

		void LoadOmniSource(string omniFilePath);
		void LoadMultiSource(IReadOnlyList<string> multiFilePaths);

		void Play();
		void Stop();

		string[] GetSupportedExtensions();
		string GetSupportedExtensionsFilterString();

		void SetMultiVolume(double volume);
		void SetOmniVolume(double volume);

		void SetHorizontalAngle(double angle);
		void SetVerticalAngle(double angle);
		void SetTiltAngle(double angle);
	}
}
