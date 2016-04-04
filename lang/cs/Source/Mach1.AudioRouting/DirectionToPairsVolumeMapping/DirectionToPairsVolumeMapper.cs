using System;
using Mach1.AudioRouting.CustomSources;

namespace Mach1.AudioRouting.DirectionToPairsVolumeMapping
{
	abstract class DirectionToPairsVolumeMapper
	{
		protected readonly VolumeControlSource[] _volumeSources;

		protected DirectionToPairsVolumeMapper(VolumeControlSource[] volumeSources)
		{
			_volumeSources = volumeSources;
		}

		public static DirectionToPairsVolumeMapper CreateMapper(VolumeControlSource[] volumeSources)
		{
			switch (volumeSources.Length)
			{
				case 4:
					return new DirectionToFourPairsVolumeMapper(volumeSources);
				case 8:
					return new DirectionToEightPairsVolumeMapper(volumeSources);
				default:
					throw new ArgumentException(nameof(volumeSources));
			}
		}

		public abstract void ApplyHorizontalAngle(float angle);

		public abstract void ApplyVerticalAngle(float angle);
	}
}
