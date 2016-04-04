using System;
using CSCore;

namespace Mach1.AudioRouting.CustomSources
{
	class VolumeControlSource : ISampleSource
	{
		public float Volume { get; set; }

		public bool CanSeek { get; }
		public WaveFormat WaveFormat => _baseSource.WaveFormat;
		public long Position { get; set; }
		public long Length { get; }

		private ISampleSource _baseSource;

		public VolumeControlSource(ISampleSource source)
		{
			_baseSource = source;
			Volume = 1.0f;
			if (source.WaveFormat.Channels > 2)
				throw new ArgumentException("Source must have one or two channels.", nameof(source));
		}

		public int Read(float[] buffer, int offset, int count)
		{
			int read = _baseSource.Read(buffer, offset, count);
			for (int i = 0; i < read; i += 2)
			{
				// 2 output channels should be available in any case
				buffer[i] *= Volume;
				buffer[i + 1] *= Volume;
			}

			return read;
		}

		public void Dispose()
		{
			
		}
	}
}
