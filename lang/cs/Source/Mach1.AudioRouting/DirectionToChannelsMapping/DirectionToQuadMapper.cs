using System;
using CSCore.DSP;

namespace Mach1.AudioRouting.DirectionToChannelsMapping
{
	class DirectionToQuadMapper : DirectionToChannelsMapper
	{
		public DirectionToQuadMapper(DmoChannelResampler resampler) : base(resampler)
		{
		}

		public override void ApplyHorizontalAngle(float angle)
		{
			float pair1Coeff = 1f - Math.Min(1f, Math.Min(360f - angle, angle) / 90f);
			float pair2Coeff = 1f - Math.Min(1f, Math.Abs(90f - angle) / 90f);
			float pair3Coeff = 1f - Math.Min(1f, Math.Abs(180f - angle) / 90f);
			float pair4Coeff = 1f - Math.Min(1f, Math.Abs(270f - angle) / 90f);
			float[,] channelMatrix =
				{
					{ pair1Coeff, pair4Coeff },
					{ pair2Coeff, pair1Coeff },
					{ pair4Coeff, pair3Coeff },
					{ pair3Coeff, pair2Coeff }
				};
			_resampler.ChannelMatrix.SetMatrix(channelMatrix);
			try
			{
				_resampler.CommitChannelMatrixChanges();
			}
			catch
			{
			}
		}

		public override void ApplyVerticalAngle(float angle)
		{
			// Nothing to do
		}
	}
}
