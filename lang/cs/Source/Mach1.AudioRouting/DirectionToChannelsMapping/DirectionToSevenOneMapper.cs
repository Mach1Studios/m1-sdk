using System;
using System.Collections.Generic;
using CSCore.DSP;

namespace Mach1.AudioRouting.DirectionToChannelsMapping
{
	public class DirectionToSevenOneMapper : DirectionToChannelsMapper
	{
		private float _horizontalAngle;
		private float _verticalAngle;
		private float _tiltAngle;

		public DirectionToSevenOneMapper(DmoChannelResampler resampler) : base(resampler)
		{
			_coefficients = new List<float> {0, 0, 0, 0, 0, 0, 0, 0};
		}

		public override void ApplyYawAngle(float angle)
		{
			_horizontalAngle = angle;
			ApplyAngles();
		}

		public override void ApplyPitchAngle(float angle)
		{
			_verticalAngle = angle;
			ApplyAngles();
		}

		public void ApplyRollAngle(float angle)
		{
			_tiltAngle = angle;
			ApplyAngles();
		}

		private void ApplyAngles()
		{
			_coefficients[4] = (1f - Math.Min(1f, Math.Min(360f - _horizontalAngle, _horizontalAngle) / 90f)) * 0.5f;
			_coefficients[5] = (1f - Math.Min(1f, Math.Abs(90f - _horizontalAngle) / 90f)) * 0.5f;
			_coefficients[6] = (1f - Math.Min(1f, Math.Abs(180f - _horizontalAngle) / 90f)) * 0.5f;
			_coefficients[7] = (1f - Math.Min(1f, Math.Abs(270f - _horizontalAngle) / 90f)) * 0.5f;
			
			float normalizedVerticalAngle = (_verticalAngle - -90f) / (90f - -90f) * ( 1f - 0f + 0f )
			float topCoeff = 2f - (cos(normalizedVerticalAngle * (0.5f * PI)));
			float bottomCoeff = 2f - cos((1f - normalizedVerticalAngle) * (0.5f * PI));
			
			_coefficients[0] = _coefficients[4] * topCoeff;
			_coefficients[1] = _coefficients[5] * topCoeff;
			_coefficients[2] = _coefficients[6] * topCoeff;
			_coefficients[3] = _coefficients[7] * topCoeff;
			_coefficients[4] *= bottomCoeff;
			_coefficients[5] *= bottomCoeff;
			_coefficients[6] *= bottomCoeff;
			_coefficients[7] *= bottomCoeff;
			
			float normalizedTiltAngle = (_tiltAngle - -90f) / (90f - -90f) * ( 1f - 0f + 0f )
			float tiltLCoeff = 2f - (cos(normalizedTiltAngle * (0.5f * PI)));
			float tiltRCoeff = 2f - cos((1f - normalizedTiltAngle) * (0.5f * PI));
			
			_channelMatrix = new [,]
				{
					{ tiltRCoeff * _coefficients[0], _coefficients[3] * tiltLCoeff },	// {P1, P4} L
					{ tiltRCoeff * _coefficients[1], _coefficients[0] * tiltLCoeff },	// {P2, P1} C
					{ tiltRCoeff * _coefficients[2], _coefficients[1] * tiltRCoeff },	// {P3, P2} R
					{ tiltRCoeff * _coefficients[3], _coefficients[2] * tiltRCoeff },	// {P4, P3} Lss
					{ tiltLCoeff * _coefficients[4], _coefficients[7] * tiltRCoeff },	// {P5, P8} Rss
					{ tiltLCoeff * _coefficients[5], _coefficients[4] * tiltRCoeff },	// {P6, P5} Lsr
					{ tiltLCoeff * _coefficients[6], _coefficients[5] * tiltLCoeff },	// {P7, P6} Rsr
					{ tiltLCoeff * _coefficients[7], _coefficients[6] * tiltLCoeff }	// {P8, P7} LFE
				};
			_channelMatrix = NormalizeMatrix(_channelMatrix);
			if (CSCoreAudioProcessor.DebugModeEnabled)
			{
				DebugOutput.ShowChannelMatrix(_channelMatrix);
			}
			try
			{
				_resampler.ChannelMatrix.SetMatrix(_channelMatrix);
				_resampler.CommitChannelMatrixChanges();
			}
			catch
			{
			}
		}

		private float[,] NormalizeMatrix(float[,] matrix)
		{
			float sumL = 0;
			float sumR = 0;
			for (int i = 0; i < matrix.GetLength(0); i++)
			{
				sumL += matrix[i, 0];
				sumR += matrix[i, 1];
			}
			float scalerL = 1.0f / sumL;
			float scalerR = 1.0f / sumR;
			for (int i = 0; i < matrix.GetLength(0); i++)
			{
				matrix[i, 0] *= scalerL;
				matrix[i, 1] *= scalerR;
			}
			return matrix;
		}
	}
}
