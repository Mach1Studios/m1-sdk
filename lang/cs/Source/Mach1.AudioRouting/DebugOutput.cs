using System;
using System.Collections.Generic;

namespace Mach1.AudioRouting
{
	static class DebugOutput
	{
		public static void ShowPairVolume(List<float> levels)
		{
			if (levels.Count == 8)
			{
				return;
			}
			for (int i = 0; i < levels.Count; i++)
			{
				float multiVolume = levels[i];
				Console.Write("P{0}: [{1:0.00000000}] ", i + 1, multiVolume);
			}
			Console.WriteLine();
		}

		public static void ShowChannelMatrix(float[,] channelMatrix)
		{
			if (channelMatrix.GetLength(0) == 8)
			{
				Console.WriteLine();
				Console.WriteLine("P1: [{0:0.00000000}] [{1:0.00000000}]", 
					channelMatrix[0, 0], channelMatrix[1, 1]);
				Console.WriteLine("P2: [{0:0.00000000}] [{1:0.00000000}]",
					channelMatrix[1, 0], channelMatrix[2, 1]);
				Console.WriteLine("P3: [{0:0.00000000}] [{1:0.00000000}]",
					channelMatrix[2, 0], channelMatrix[3, 1]);
				Console.WriteLine("P4: [{0:0.00000000}] [{1:0.00000000}]",
					channelMatrix[3, 0], channelMatrix[0, 1]);
				Console.WriteLine("P5: [{0:0.00000000}] [{1:0.00000000}]",
					channelMatrix[4, 0], channelMatrix[5, 1]);
				Console.WriteLine("P6: [{0:0.00000000}] [{1:0.00000000}]",
					channelMatrix[5, 0], channelMatrix[6, 1]);
				Console.WriteLine("P7: [{0:0.00000000}] [{1:0.00000000}]",
					channelMatrix[6, 0], channelMatrix[7, 1]);
				Console.WriteLine("P8: [{0:0.00000000}] [{1:0.00000000}]",
					channelMatrix[7, 0], channelMatrix[4, 1]);
				Console.WriteLine();
			}
			else
			{
				Console.WriteLine();
				for (int i = 0; i < channelMatrix.GetLength(0); i++)
				{
					Console.Write("[ ");
					for (int j = 0; j < channelMatrix.GetLength(1); j++)
					{
						Console.Write("[{0:0.00000000}] ", channelMatrix[i, j]);
					}
					Console.Write("]");
					Console.WriteLine();
				}
				Console.WriteLine();
			}
		}
	}
}
