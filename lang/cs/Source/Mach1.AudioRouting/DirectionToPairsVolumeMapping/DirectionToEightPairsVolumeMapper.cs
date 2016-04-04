using System;
using Mach1.AudioRouting.CustomSources;

namespace Mach1.AudioRouting.DirectionToPairsVolumeMapping
{
	class DirectionToEightPairsVolumeMapper : DirectionToPairsVolumeMapper
	{
		private float _horizontalAngle;
		private float _verticalAngle;

		public DirectionToEightPairsVolumeMapper(VolumeControlSource[] volumeSources) : base(volumeSources)
		{
		}

		public override void ApplyHorizontalAngle(float angle)
		{
			_horizontalAngle = angle;
			ApplyAngles();
		}

		public override void ApplyVerticalAngle(float angle)
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
			_volumeSources[0].Volume = pairB1Coeff * topCoeff;
			_volumeSources[1].Volume = pairB2Coeff * topCoeff;
			_volumeSources[2].Volume = pairB3Coeff * topCoeff;
			_volumeSources[3].Volume = pairB4Coeff * topCoeff;
			_volumeSources[4].Volume = pairB1Coeff * bottomCoeff;
			_volumeSources[5].Volume = pairB2Coeff * bottomCoeff;
			_volumeSources[6].Volume = pairB3Coeff * bottomCoeff;
			_volumeSources[7].Volume = pairB4Coeff * bottomCoeff;
		}
	}
}
