using System;
using Mach1.AudioRouting.CustomSources;

namespace Mach1.AudioRouting.DirectionToPairsVolumeMapping
{
	class DirectionToEightPairsVolumeMapper : DirectionToPairsVolumeMapper
	{
		private float _horizontalAngle;
		private float _verticalAngle;

		public DirectionToEightPairsVolumeMapper(Mixer mixer) : base(mixer)
		{
		}

		public override void ApplyYawAngle(float angle)
		{
			_horizontalAngle = angle;
			ApplyAngles();
		}

		public override void ApplyPitchAngle(float angle)
		{
			_verticalAngle = angle;
			ApplyAngles();
		}

		private void ApplyAngles()
		{
			float pairB1Coeff = (1f - Math.Min(1f, Math.Min(360f - _horizontalAngle, _horizontalAngle) / 90f)) * 0.5f;
			float pairB2Coeff = (1f - Math.Min(1f, Math.Abs(90f - _horizontalAngle) / 90f)) * 0.5f;
			float pairB3Coeff = (1f - Math.Min(1f, Math.Abs(180f - _horizontalAngle) / 90f)) * 0.5f;
			float pairB4Coeff = (1f - Math.Min(1f, Math.Abs(270f - _horizontalAngle) / 90f)) * 0.5f;
			float topCoeff = 2f - (90f - _verticalAngle) / 90f;
			float bottomCoeff = 2f - topCoeff;
			Mixer.MultiVolumes[0] = pairB1Coeff * topCoeff;
			Mixer.MultiVolumes[1] = pairB2Coeff * topCoeff;
			Mixer.MultiVolumes[2] = pairB3Coeff * topCoeff;
			Mixer.MultiVolumes[3] = pairB4Coeff * topCoeff;
			Mixer.MultiVolumes[4] = pairB1Coeff * bottomCoeff;
			Mixer.MultiVolumes[5] = pairB2Coeff * bottomCoeff;
			Mixer.MultiVolumes[6] = pairB3Coeff * bottomCoeff;
			Mixer.MultiVolumes[7] = pairB4Coeff * bottomCoeff;
		}
	}
}
