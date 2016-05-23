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

		public override void ApplyHorizontalAngle(float angle)
		{
			_horizontalAngle = angle;
			ApplyAngles();
		}

		public override void ApplyVerticalAngle(float angle)
		{
			_verticalAngle = angle;
			ApplyAngles();
		}

		public void ApplyTiltAngle(float angle)
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
			float topCoeff = 2f - (90f - _verticalAngle) / 90f;
			float bottomCoeff = 2f - topCoeff;
			_coefficients[0] = _coefficients[4] * topCoeff;
			_coefficients[1] = _coefficients[5] * topCoeff;
			_coefficients[2] = _coefficients[6] * topCoeff;
			_coefficients[3] = _coefficients[7] * topCoeff;
			_coefficients[4] *= bottomCoeff;
			_coefficients[5] *= bottomCoeff;
			_coefficients[6] *= bottomCoeff;
			_coefficients[7] *= bottomCoeff;
			float tiltRCoeff = 2f - (90f - _tiltAngle) / 90f;
			float tiltLCoeff = 2f - tiltRCoeff;
			_channelMatrix = new [,]
				{
					{ tiltRCoeff * _coefficients[0], _coefficients[3] * tiltLCoeff },	// {P1, P4} L Front
					{ tiltRCoeff * _coefficients[2], _coefficients[1] * tiltLCoeff },	// {P3, P2} R Front
					{ tiltRCoeff * _coefficients[1], _coefficients[0] * tiltLCoeff },	// {P2, P1} Center
					{ tiltLCoeff * _coefficients[7], _coefficients[6] * tiltRCoeff },	// {P8, P7} Low Freq
					{ tiltRCoeff * _coefficients[3], _coefficients[2] * tiltLCoeff },	// {P4, P3} L Side
					{ tiltLCoeff * _coefficients[4], _coefficients[7] * tiltRCoeff },	// {P5, P8} R Side
					{ tiltLCoeff * _coefficients[5], _coefficients[4] * tiltRCoeff },	// {P6, P5} L Rear
					{ tiltLCoeff * _coefficients[6], _coefficients[5] * tiltRCoeff }	// {P7, P6} R Rear
				};
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
	}
}
