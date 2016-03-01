using System;

namespace Mach1.AudioRouting
{
	public interface IAudioProcessor : IDisposable
	{
		void Play(string filePath);

		void Stop();

		string[] GetSupportedExtensions();

		string GetSupportedExtensionsFilterString();
	}
}
