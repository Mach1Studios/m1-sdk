using System;
using Mach1.AudioRouting.CustomSources;

namespace Mach1.AudioRouting.DirectionToPairsVolumeMapping
{
	class DirectionToFourPairsVolumeMapper : DirectionToPairsVolumeMapper
	{
		public DirectionToFourPairsVolumeMapper(VolumeControlSource[] volumeSources) : base(volumeSources)
		{
		}

		public override void ApplyHorizontalAngle(float angle)
		{
			_volumeSources[0].Volume = 1f - Math.Min(1f, Math.Min(360f - angle, angle) / 90f);
			_volumeSources[1].Volume = 1f - Math.Min(1f, Math.Abs(90f - angle) / 90f);
			_volumeSources[2].Volume = 1f - Math.Min(1f, Math.Abs(180f - angle) / 90f);
			_volumeSources[3].Volume = 1f - Math.Min(1f, Math.Abs(270f - angle) / 90f);
		}

		public override void ApplyVerticalAngle(float angle)
		{
			// Nothing to do
		}
	}
}
