using System;
using Mach1.AudioRouting.CustomSources;

namespace Mach1.AudioRouting.DirectionToPairsVolumeMapping
{
	class DirectionToFourPairsVolumeMapper : DirectionToPairsVolumeMapper
	{
		public DirectionToFourPairsVolumeMapper(Mixer mixer) : base(mixer)
		{
		}

		public override void ApplyYawAngle(float angle)
		{
			Mixer.MultiVolumes[0] = 1f - Math.Min(1f, Math.Min(360f - angle, angle) / 90f);
			Mixer.MultiVolumes[1] = 1f - Math.Min(1f, Math.Abs(90f - angle) / 90f);
			Mixer.MultiVolumes[2] = 1f - Math.Min(1f, Math.Abs(180f - angle) / 90f);
			Mixer.MultiVolumes[3] = 1f - Math.Min(1f, Math.Abs(270f - angle) / 90f);
		}

		public override void ApplyPitchAngle(float angle)
		{
			// Nothing to do
		}
	}
}
