using System;
using System.Collections.Generic;
using CSCore.DSP;

namespace Mach1.AudioRouting.DirectionToChannelsMapping
{
	abstract class DirectionToChannelsMapper
	{
		protected readonly DmoChannelResampler _resampler;

		protected List<float> _coefficients;

		protected DirectionToChannelsMapper(DmoChannelResampler resampler)
		{
			_resampler = resampler;
		}

		public static DirectionToChannelsMapper CreateMapper(DmoChannelResampler resampler)
		{
			switch (resampler.ChannelMatrix.InputChannelCount)
			{
				case 4:
					return new DirectionToQuadMapper(resampler);
				case 6:
					return new DirectionToFiveOneMapper(resampler);
				case 8:
					return new DirectionToSevenOneMapper(resampler);
				default:
					throw new ArgumentException(nameof(resampler));
			}
		}

		public abstract void ApplyHorizontalAngle(float angle);

		public abstract void ApplyVerticalAngle(float angle);

		public void ShowDebugInfo()
		{
			DebugOutput.ShowPairVolume(_coefficients);
		}
	}
}
