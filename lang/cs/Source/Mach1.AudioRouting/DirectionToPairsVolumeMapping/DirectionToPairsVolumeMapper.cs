using System;
using Mach1.AudioRouting.CustomSources;

namespace Mach1.AudioRouting.DirectionToPairsVolumeMapping
{
	abstract class DirectionToPairsVolumeMapper
	{
		protected readonly Mixer Mixer;

		protected DirectionToPairsVolumeMapper(Mixer mixer)
		{
			Mixer = mixer;
		}

		public static DirectionToPairsVolumeMapper CreateMapper(Mixer mixer)
		{
			if (mixer.MultiVolumes.Count >= 8)
			{
				return new DirectionToEightPairsVolumeMapper(mixer);
			}
			if (mixer.MultiVolumes.Count >= 4)
			{
				return new DirectionToFourPairsVolumeMapper(mixer);
			}
			throw new ArgumentException(nameof(mixer));
		}

		public abstract void ApplyYawAngle(float angle);

		public abstract void ApplyPitchAngle(float angle);
	}
}
