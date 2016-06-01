using System;
using System.Collections.Generic;
using CSCore.DSP;

namespace Mach1.AudioRouting.DirectionToChannelsMapping
{
	public class DirectionToFiveOneMapper : DirectionToChannelsMapper
	{

		public DirectionToFiveOneMapper(DmoChannelResampler resampler) : base(resampler)
		{
			_coefficients = new List<float> {0, 0, 0, 0, 0};
		}

		public override void ApplyYawAngle(float angle)
		{
			_coefficients[0] = (1f - Math.Min(1f, Math.Min(360f - angle, angle) / 90f)) * 0.5f;
			_coefficients[1] = (1f - Math.Min(1f, Math.Abs(90f - angle) / 90f)) * 0.5f;
			_coefficients[2] = (1f - Math.Min(1f, Math.Abs(180f - angle) / 90f)) * 0.5f;
			_coefficients[3] = (1f - Math.Min(1f, Math.Abs(270f - angle) / 90f)) * 0.5f;
			_coefficients[4] = 0.25f;
			_channelMatrix = new [,]
				{
					{ _coefficients[0], _coefficients[3] },
					{ _coefficients[1], _coefficients[0] },
					{ _coefficients[4], _coefficients[4] },
					{ _coefficients[4], _coefficients[4] },
					{ _coefficients[3], _coefficients[2] },
					{ _coefficients[2], _coefficients[1] }
				};
			try
			{
				_resampler.ChannelMatrix.SetMatrix(_channelMatrix);
				_resampler.CommitChannelMatrixChanges();
			}
			catch
			{
			}
		}

		public override void ApplyPitchAngle(float angle)
		{
			// Nothing to do
		}
	}
}
