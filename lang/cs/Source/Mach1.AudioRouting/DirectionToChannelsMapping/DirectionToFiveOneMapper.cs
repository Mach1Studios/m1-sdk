using System;
using CSCore.DSP;

namespace Mach1.AudioRouting.DirectionToChannelsMapping
{
	class DirectionToFiveOneMapper : DirectionToChannelsMapper
	{
		public DirectionToFiveOneMapper(DmoChannelResampler resampler) : base(resampler)
		{
		}

		public override void ApplyHorizontalAngle(float angle)
		{
			float pair1Coeff = (1f - Math.Min(1f, Math.Min(360f - angle, angle) / 90f)) * 0.5f;
			float pair2Coeff = (1f - Math.Min(1f, Math.Abs(90f - angle) / 90f)) * 0.5f;
			float pair3Coeff = (1f - Math.Min(1f, Math.Abs(180f - angle) / 90f)) * 0.5f;
			float pair4Coeff = (1f - Math.Min(1f, Math.Abs(270f - angle) / 90f)) * 0.5f;
			float pair5Coeff = 0.25f;
			float[,] channelMatrix =
				{
					{ pair1Coeff, pair4Coeff },
					{ pair2Coeff, pair1Coeff },
					{ pair5Coeff, pair5Coeff },
					{ pair5Coeff, pair5Coeff },
					{ pair4Coeff, pair3Coeff },
					{ pair3Coeff, pair2Coeff }
				};
			_resampler.ChannelMatrix.SetMatrix(channelMatrix);
			_resampler.CommitChannelMatrixChanges();
		}

		public override void ApplyVerticalAngle(float angle)
		{
			// Nothing to do
		}
	}
}
