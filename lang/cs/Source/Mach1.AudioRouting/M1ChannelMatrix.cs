using CSCore;
using CSCore.DSP;

namespace Mach1.AudioRouting
{
	class M1ChannelMatrix
	{
		public static readonly ChannelMatrix SevenOneToStereo;
		public static readonly ChannelMatrix FiveOneToStereo;
		public static readonly ChannelMatrix QuadToStereo;

		static M1ChannelMatrix()
		{
			// 7.1
			SevenOneToStereo = new ChannelMatrix(
				ChannelMask.SpeakerFrontLeft | ChannelMask.SpeakerFrontRight |
				ChannelMask.SpeakerFrontCenter | ChannelMask.SpeakerLowFrequency |
				ChannelMask.SpeakerSideLeft | ChannelMask.SpeakerSideRight |
				ChannelMask.SpeakerBackLeft | ChannelMask.SpeakerBackRight,
				ChannelMask.SpeakerFrontLeft | ChannelMask.SpeakerFrontRight);
			SevenOneToStereo.SetMatrix(
				new [,]
				{
					{ 0.125f, 0.125f },
					{ 0.125f, 0.125f },
					{ 0.125f, 0.125f },
					{ 0.125f, 0.125f },
					{ 0.125f, 0.125f },
					{ 0.125f, 0.125f },
					{ 0.125f, 0.125f },
					{ 0.125f, 0.125f }
				});

			// 5.1
			FiveOneToStereo = new ChannelMatrix(
				ChannelMask.SpeakerFrontLeft | ChannelMask.SpeakerFrontRight |
				ChannelMask.SpeakerFrontCenter | ChannelMask.SpeakerLowFrequency |
				ChannelMask.SpeakerBackLeft | ChannelMask.SpeakerBackRight,
				ChannelMask.SpeakerFrontLeft | ChannelMask.SpeakerFrontRight);
			FiveOneToStereo.SetMatrix(
				new [,]
				{
					{ 0.167f, 0.167f },
					{ 0.167f, 0.167f },
					{ 0.167f, 0.167f },
					{ 0.167f, 0.167f },
					{ 0.167f, 0.167f },
					{ 0.167f, 0.167f }
				});

			// 4.0
			QuadToStereo = new ChannelMatrix(
				ChannelMask.SpeakerFrontLeft | ChannelMask.SpeakerFrontRight |
				ChannelMask.SpeakerBackLeft | ChannelMask.SpeakerBackRight,
				ChannelMask.SpeakerFrontLeft | ChannelMask.SpeakerFrontRight);
			QuadToStereo.SetMatrix(
				new [,]
				{
					{ 0.25f, 0.25f },
					{ 0.25f, 0.25f },
					{ 0.25f, 0.25f },
					{ 0.25f, 0.25f }
				});
		}
	}
}
