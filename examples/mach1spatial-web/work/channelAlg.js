"use strict";

const Point = require('./mPoint');

//
// Map utility
//
const __FLT_EPSILON__ = 1e-10;

/**
 *
 * @param value number
 * @param inputMin number
 * @param inputMax number
 * @param outputMin number
 * @param outputMax number
 * @param isClamp bool
 * @return {number}
 */
function mmap(value, inputMin, inputMax, outputMin, outputMax, isClamp) {

	if (Math.abs(inputMin - inputMax) < __FLT_EPSILON__)
	{
		return outputMin;
	}
	else
	{
		let outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);

		if (isClamp)
		{
			if (outputMax < outputMin)
			{
				if (outVal < outputMax)
					outVal = outputMax;
				else if (outVal > outputMin)
					outVal = outputMin;
			}
			else
			{
				if (outVal > outputMax)
					outVal = outputMax;
				else if (outVal < outputMin)
					outVal = outputMin;
			}
		}
		return outVal;
	}

}
/**
 *
 * @param a number
 * @param min number
 * @param max number
 * @return {number}
 */
function clamp(a, min, max )
{
	return (a < min) ? min : ((a > max) ? max : a);
}

/**
 * @param a angle in deg
 * @param min
 * @param max
 * max - min == 360
 * @return {number} angle in [min...max]
 */
function alignAngle(a, min = -180, max = 180)
{
	while (a < min)
		a += 360;
	while (a > max)
		a -= 360;

	return a;
}

//
// End map utility
//

/**
 *  Eight channel audio format.
 *
 *  Order of input angles:
 * @param yaw  yaw in angles
 * @param pitch pitch in angles
 * @param roll  roll in angles (default = 0)
 * @return {Array} volumes[8]
 */
function eightChannelsAlgorithm(yaw, pitch, roll = 0)
{
	//Orientation input safety clamps/alignment
	pitch = alignAngle(pitch, -180, 180);
	pitch = clamp(pitch, -90, 90); // -90, 90

	yaw = alignAngle(yaw, 0, 360);

	roll = alignAngle(roll, -180, 180);
	roll = clamp(roll, -90, 90); // -90, 90

	let coefficients = [8];
	coefficients[0] = 1. - Math.min(1., Math.min(360. - yaw, yaw) / 90.);
	coefficients[1] = 1. - Math.min(1., Math.abs(90. - yaw) / 90.);
	coefficients[2] = 1. - Math.min(1., Math.abs(180. - yaw) / 90.);
	coefficients[3] = 1. - Math.min(1., Math.abs(270. - yaw) / 90.);

	let tiltHigh = mmap(roll, -90., 90., 0., 1., true);
	let tiltLow = 1. - tiltHigh;

	//ISSUE//
	//Able to kill stereo by making both pitch and tilt at max or min values together without proper clamps

	let result = [];
	result.length = 16;
	result[0] = coefficients[0] * tiltHigh * 2.0; // 1 left
	result[1] = coefficients[3] * tiltHigh * 2.0; //   right
	result[2] = coefficients[1] * tiltLow * 2.0; // 2 left
	result[3] = coefficients[0] * tiltLow * 2.0; //   right
	result[4] = coefficients[3] * tiltLow * 2.0; // 3 left
	result[5] = coefficients[2] * tiltLow * 2.0; //   right
	result[6] = coefficients[2] * tiltHigh * 2.0; // 4 left
	result[7] = coefficients[1] * tiltHigh * 2.0; //   right

	result[0 + 8] = coefficients[0] * tiltLow * 2.0; // 1 left
	result[1 + 8] = coefficients[3] * tiltLow * 2.0; //   right
	result[2 + 8] = coefficients[1] * tiltHigh * 2.0; // 2 left
	result[3 + 8] = coefficients[0] * tiltHigh * 2.0; //   right
	result[4 + 8] = coefficients[3] * tiltHigh * 2.0; // 3 left
	result[5 + 8] = coefficients[2] * tiltHigh * 2.0; //   right
	result[6 + 8] = coefficients[2] * tiltLow * 2.0; // 4 left
	result[7 + 8] = coefficients[1] * tiltLow * 2.0; //   right

	let pitchHigherHalf = mmap(pitch, 90., -90., 0., 1., true);
	let pitchLowerHalf = 1. - pitchHigherHalf;

	for (let i = 0; i < 8; ++i) {
		result[i] *= pitchLowerHalf;
		result[i + 8] *= pitchHigherHalf;
	}

	return result;
}

//
//  Eight pairs audio format.
//
//  Order of input angles:
//  Y = yaw in angles
//  P = pitch in angles
//  R = roll in angles
//

function eightPairsAlgorithm(yaw, pitch, roll) {

	//Orientation input safety clamps/alignment
	pitch = alignAngle(pitch, -180, 180);
	pitch = clamp(pitch, -90, 90); // -90, 90

	yaw = alignAngle(yaw, 0, 360);

	let volumes = [8];
	volumes[0] = 1. - Math.min(1., Math.min(360. - yaw, yaw) / 90.);
	volumes[1] = 1. - Math.min(1., Math.abs(90. - yaw) / 90.);
	volumes[2] = 1. - Math.min(1., Math.abs(180. - yaw) / 90.);
	volumes[3] = 1. - Math.min(1., Math.abs(270. - yaw) / 90.);

	let pitchHigherHalf = mmap(pitch, 90., -90., 0., 1., true);
	let pitchLowerHalf = 1. - pitchHigherHalf;

	let  result = [];
	result.push(volumes[0] * pitchHigherHalf);
	result.push(volumes[1] * pitchHigherHalf);
	result.push(volumes[2] * pitchHigherHalf);
	result.push(volumes[3] * pitchHigherHalf);
	result.push(volumes[4] * pitchLowerHalf);
	result.push(volumes[5] * pitchLowerHalf);
	result.push(volumes[6] * pitchLowerHalf);
	result.push(volumes[7] * pitchLowerHalf);

	return result;
}


module.exports.eightPairsAlgorithm = eightPairsAlgorithm;
module.exports.eightChannelsAlgorithm = eightChannelsAlgorithm;
