using System;

namespace Mach1.AudioRouting
{
	public class PeakEventArgs : EventArgs
	{
		public float[] ChannelPeakValues { get; set; }
		public float PeakValue { get; set; }
	}
}
