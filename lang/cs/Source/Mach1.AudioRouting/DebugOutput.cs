using System;
using System.Collections.Generic;

namespace Mach1.AudioRouting
{
	static class DebugOutput
	{
		public static void ShowPairVolume(List<float> volumes)
		{
			for (int i = 0; i < volumes.Count; i++)
			{
				float multiVolume = volumes[i];
				Console.Write("[P{0}: {1}] ", i + 1, multiVolume);
			}
			Console.WriteLine();
		}

		public static void ShowChannelMatrix(float[,] channelMatrix)
		{
			Console.WriteLine();
			for (int i = 0; i < channelMatrix.GetLength(0); i++)
			{
				Console.Write("[ ");
				for (int j = 0; j < channelMatrix.GetLength(1); j++)
				{
					Console.Write("{0} ", channelMatrix[i, j]);
				}
				Console.Write("]");
				Console.WriteLine();
			}
			Console.WriteLine();
		}
	}
}
