using System;
using CSCore.DSP;

namespace Mach1.AudioRouting.DirectionToChannelsMapping
{
	class DirectionToSevenOneMapper : DirectionToChannelsMapper
	{
		private float _horizontalAngle;
		private float _verticalAngle;

		public DirectionToSevenOneMapper(DmoChannelResampler resampler) : base(resampler)
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
			float pairT1Coeff = pairB1Coeff * topCoeff;
			float pairT2Coeff = pairB2Coeff * topCoeff;
			float pairT3Coeff = pairB3Coeff * topCoeff;
			float pairT4Coeff = pairB4Coeff * topCoeff;
			pairB1Coeff *= bottomCoeff;
			pairB2Coeff *= bottomCoeff;
			pairB3Coeff *= bottomCoeff;
			pairB4Coeff *= bottomCoeff;
			float[,] channelMatrix =
				{
					{ pairT1Coeff, pairT4Coeff },
					{ pairB1Coeff, pairB4Coeff },
					{ pairT2Coeff, pairT1Coeff },
					{ pairB4Coeff, pairB3Coeff },
					{ pairB2Coeff, pairB1Coeff },
					{ pairT3Coeff, pairT2Coeff },
					{ pairT4Coeff, pairT3Coeff },
					{ pairB3Coeff, pairB2Coeff }
				};
			_resampler.ChannelMatrix.SetMatrix(channelMatrix);
			_resampler.CommitChannelMatrixChanges();
		}
	}
}
