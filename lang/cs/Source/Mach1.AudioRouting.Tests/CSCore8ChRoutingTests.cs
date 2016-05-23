using System;
using System.Collections.Generic;
using System.Linq;
using Mach1.AudioRouting.DirectionToChannelsMapping;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace Mach1.AudioRouting.Tests
{
	public class FloatComparerWithTolerance : EqualityComparer<float>
	{
		private readonly float _tolerance;

		public FloatComparerWithTolerance(float tolerance)
		{
			_tolerance = tolerance;
		}

		public override bool Equals(float x, float y)
		{
			return Math.Abs(x - y) <= _tolerance;
		}

		public override int GetHashCode(float number)
		{
			return number.GetHashCode();
		}
	}

	[TestClass]
	public class CSCore8ChRoutingTests
	{
		private static DirectionToSevenOneMapper _directionMapper;

		private const float Tolerance = 0.001f;

		[ClassInitialize]
		public static void ClassInitialize(TestContext context)
		{
			_directionMapper = new DirectionToSevenOneMapper(null);
		}

		// Yaw=0, Pitch=0, Roll=0
		// [p1: 0.5, 0.5][p2: 0.0, 0.0][p3: 0.0, 0.0][p4: 0.0, 0.0]
		// [p5: 0.5, 0.5][p6: 0.0, 0.0][p7: 0.0, 0.0][p8: 0.0, 0.0]
		[TestMethod]
		public void DirectionToSevenOneMapper_Yaw0Pitch0Roll0_ChMatrixCoeffs()
		{
			_directionMapper.ApplyHorizontalAngle(0);
			_directionMapper.ApplyVerticalAngle(0);
			_directionMapper.ApplyTiltAngle(0);

			float[,] expected =
			{
				{ 0.5f,	0f },	// {P1, P4} L Front
				{ 0f,	0f },	// {P3, P2} R Front
				{ 0f,	0.5f },	// {P2, P1} Center
				{ 0f,	0f },	// {P8, P7} Low Freq
				{ 0f,	0f },	// {P4, P3} L Side
				{ 0.5f,	0f },	// {P5, P8} R Side
				{ 0f,	0.5f },	// {P6, P5} L Rear
				{ 0f,	0f }	// {P7, P6} R Rear
			};

			Assert.IsTrue(AreEqual(_directionMapper.ChannelMatrix, expected, Tolerance));
		}

		// Yaw=0, Pitch=0, Roll=45
		// [p1: 0.75, 0.25][p2: 0.0, 0.0][p3: 0.0, 0.0][p4: 0.0, 0.0]
		// [p5: 0.25, 0.75][p6: 0.0, 0.0][p7: 0.0, 0.0][p8: 0.0, 0.0]
		[TestMethod]
		public void DirectionToSevenOneMapper_Yaw0Pitch0Roll45_ChMatrixCoeffs()
		{
			_directionMapper.ApplyHorizontalAngle(0);
			_directionMapper.ApplyVerticalAngle(0);
			_directionMapper.ApplyTiltAngle(45);

			float[,] expected =
			{
				{ 0.75f,	0f },	// {P1, P4} L Front
				{ 0f,		0f },	// {P3, P2} R Front
				{ 0f,		0.25f },// {P2, P1} Center
				{ 0f,		0f },	// {P8, P7} Low Freq
				{ 0f,		0f },	// {P4, P3} L Side
				{ 0.25f,	0f },	// {P5, P8} R Side
				{ 0f,		0.75f },// {P6, P5} L Rear
				{ 0f,		0f }	// {P7, P6} R Rear
			};

			Assert.IsTrue(AreEqual(_directionMapper.ChannelMatrix, expected, Tolerance));
		}

		// Yaw=0, Pitch=0, Roll=-90
		// [p1: 0.0, 1.0][p2: 0.0, 0.0][p3: 0.0, 0.0][p4: 0.0, 0.0]
		// [p5: 1.0, 0.0][p6: 0.0, 0.0][p7: 0.0, 0.0][p8: 0.0, 0.0]
		[TestMethod]
		public void DirectionToSevenOneMapper_Yaw0Pitch0RollNeg90_ChMatrixCoeffs()
		{
			_directionMapper.ApplyHorizontalAngle(0);
			_directionMapper.ApplyVerticalAngle(0);
			_directionMapper.ApplyTiltAngle(-90);

			float[,] expected =
			{
				{ 0f,	0f },	// {P1, P4} L Front
				{ 0f,   0f },	// {P3, P2} R Front
				{ 0f,   1f },	// {P2, P1} Center
				{ 0f,   0f },	// {P8, P7} Low Freq
				{ 0f,   0f },	// {P4, P3} L Side
				{ 1f,	0f },	// {P5, P8} R Side
				{ 0f,   0f },	// {P6, P5} L Rear
				{ 0f,   0f }	// {P7, P6} R Rear
			};

			Assert.IsTrue(AreEqual(_directionMapper.ChannelMatrix, expected, Tolerance));
		}

		// Yaw=45, Pitch=0, Roll=45
		// [p1: 0.1875, 0.0625][p2: 0.1875, 0.0625][p3: 0.0, 0.0][p4: 0.0, 0.0]
		// [p5: 0.0625, 0.1875][p6: 0.0625, 0.1875][p7: 0.0, 0.0][p8: 0.0, 0.0]
		[TestMethod]
		public void DirectionToSevenOneMapper_Yaw45Pitch0Roll45_ChMatrixCoeffs()
		{
			_directionMapper.ApplyHorizontalAngle(45);
			_directionMapper.ApplyVerticalAngle(0);
			_directionMapper.ApplyTiltAngle(45);

			float[,] expected =
			{
				{ 0.1875f,  0f },		// {P1, P4} L Front
				{ 0f,		0.0625f },	// {P3, P2} R Front
				{ 0.1875f,	0.0625f },	// {P2, P1} Center
				{ 0f,		0f },		// {P8, P7} Low Freq
				{ 0f,		0f },		// {P4, P3} L Side
				{ 0.0625f,	0f },		// {P5, P8} R Side
				{ 0.0625f,	0.1875f },	// {P6, P5} L Rear
				{ 0f,		0.1875f }	// {P7, P6} R Rear
			};

			Assert.IsTrue(AreEqual(_directionMapper.ChannelMatrix, expected, Tolerance));
		}

		// Yaw=45, Pitch=45, Roll=45
		// [p1: 0.28125, 0.09375][p2: 0.28125, 0.09375][p3: 0.0, 0.0][p4: 0.0, 0.0]
		// [p5: 0.03125, 0.09375][p6: 0.03125, 0.09375][p7: 0.0, 0.0][p8: 0.0, 0.0]
		[TestMethod]
		public void DirectionToSevenOneMapper_Yaw45Pitch45Roll45_ChMatrixCoeffs()
		{
			_directionMapper.ApplyHorizontalAngle(45);
			_directionMapper.ApplyVerticalAngle(45);
			_directionMapper.ApplyTiltAngle(45);

			float[,] expected =
			{
				{ 0.28125f, 0f },		// {P1, P4} L Front
				{ 0f,       0.09375f },	// {P3, P2} R Front
				{ 0.28125f, 0.09375f },	// {P2, P1} Center
				{ 0f,       0f },		// {P8, P7} Low Freq
				{ 0f,       0f },		// {P4, P3} L Side
				{ 0.03125f, 0f },		// {P5, P8} R Side
				{ 0.03125f, 0.09375f },	// {P6, P5} L Rear
				{ 0f,		0.09375f }	// {P7, P6} R Rear
			};

			Assert.IsTrue(AreEqual(_directionMapper.ChannelMatrix, expected, Tolerance));
		}

		private static bool AreEqual(float[,] a, float[,] b, float tolerance)
		{
			return a.Rank == b.Rank &&
				Enumerable.Range(0, a.Rank).All(dim => a.GetLength(dim) == b.GetLength(dim)) &&
				a.Cast<float>().SequenceEqual(b.Cast<float>(), new FloatComparerWithTolerance(tolerance));
		}
	}
}
