/************************************************************************************

Copyright   :   Copyright 2014 Oculus VR, LLC. All Rights reserved.

Licensed under the Oculus VR Rift SDK License Version 3.2 (the "License");
you may not use the Oculus VR Rift SDK except in compliance with the License,
which is provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

You may obtain a copy of the License at

http://www.oculusvr.com/licenses/LICENSE-3.2

Unless required by applicable law or agreed to in writing, the Oculus VR SDK
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

************************************************************************************/

using UnityEngine;
using System.Collections.Generic;
using System.Runtime.InteropServices;

public static class OVRMathUtils
{
	public const float Cosine5Degrees = 0.996194698f;
	public const float Cosine22Pt5Degrees = 0.9238795325f;
	public const float Cosine30Degrees = 0.86602540378f;
	public const float Cosine45Degrees = 0.7071067812f;
	public const float Cosine60Degrees = 0.5f;
	public const float Rad2Deg = 180.0f / Mathf.PI;
	public const float Deg2Rad = Mathf.PI / 180.0f;

	public const float FltSmallNumber = 1.0842021724855044e-019f;
	public const float FltSmallestNonDenormal = 1.1754943508222875e-038f;

	/// <summary>
	/// Derives sine from a cosine using trigonometric identities. Note that
	/// the dot product of two normalized vectors is the cosine of the angle
	/// between them -- so this can also derive a sine given a dot product.
	/// </summary>
	public static float SineFromCosine(float cosine)
	{
		return Mathf.Sqrt(Mathf.Abs(1.0f - (cosine * cosine)));
	}

	/// <summary>
	/// Given the cosine of an angle, this will return the cosine of 1/2 the angle.
	/// </summary>
	public static float CosineOfHalfAngleFromCosine(float cosine)
	{
		return Mathf.Sqrt((1.0f + cosine) * 0.5f);
	}

	/// <summary>
	/// Returns the cosine of the angle which is 2x the angle represented by cosine.
	/// </summary>
	public static float CosineOfDoubleAngleFromCosine(float cosine)
	{
		return 2.0f * (cosine * cosine) - 1.0f;
	}

	/// <summary>
	/// Normalizes a quaternion.  This can be necessary to repair floating point error.
	/// </summary>
	public static void Normalize(ref Quaternion q)
	{
		float mag = Mathf.Sqrt( q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w );
		// if the magnitude is becoming very small, let us know that we're risking a denormal
		OVRDebugUtils.Assert( mag > FltSmallestNonDenormal );
		float inverseMag = 1.0f /  mag;
		q.x *= inverseMag;
		q.y *= inverseMag;
		q.z *= inverseMag;
		q.w *= inverseMag;
	}

	/// <summary>
	/// Constructs a quaternion from the axis and a dot product representing 
	/// the rotation around the axis.  This will always construct a rotation
	/// in the positive direction.
	/// </summary>
	public static Quaternion QuaternionFromAxisAndDotProduct(Vector3 axis, float dot)
	{
		Quaternion q;
		q.w = CosineOfHalfAngleFromCosine(dot);
		float halfAngleSine = SineFromCosine(q.w);
		q.x = axis.x * halfAngleSine;
		q.y = axis.y * halfAngleSine;
		q.z = axis.z * halfAngleSine;
		return q;
	}

	/// <summary>
	/// Constructs a quaternion rotation from a forward and up axis.
	/// </summary>
	public static Quaternion QuaternionFromFwd(Vector3 fwd, Vector3 up)
	{
		return Quaternion.LookRotation(fwd, up);
	}

	/// <summary>
	/// Constructs a quaternion that will rotate the from vector to the to vector.
	/// This assumes 'from' and 'to' are normalized.
	/// </summary>
	public static Quaternion RotationBetweenTwoVectors(Vector3 from, Vector3 to)
	{
/*
		float dot = Vector3.Dot( from, to );
		if ( dot > 0.9999f )
		{
			OVRDebugUtils.Assert( dot <= 0.9999f );
			return Quaternion.identity;
		}

		Quaternion q;
		q.w = CosineOfHalfAngleFromCosine( dot );
		float sineHalfAngle = SineFromCosine( q.w );

		// get a vector orthogonal to both from and to
		Vector3 axis = Vector3.Cross( from, to );
		OVRDebugUtils.Assert( axis.magnitude > 0.0001f );
		q.x = axis.x * sineHalfAngle;
		q.y = axis.y * sineHalfAngle;
		q.z = axis.z * sineHalfAngle;

		return q;
*/
		Quaternion q;
		q.x = from.y * to.z - from.z * to.y;
		q.y = from.z * to.x - from.x * to.z;
		q.z = from.x * to.y - from.y * to.x;
		q.w = to.x * from.x + to.y * from.y + to.z * from.z + 1.0f;
		OVRDebugUtils.Assert(q.w > 1e-18f);
		Normalize(ref q);
		return q;
	}

	/// <summary>
	/// Returns vector v projected onto a plane with the specified normal.
	/// </summary>
	public static Vector3 ProjectOntoPlane(Vector3 v, Vector3 normal)
	{
		float dot = Vector3.Dot(v, normal);
		return v - (normal * dot);
	}

	/// <summary>
	/// Returns a normalized vector v projected onto a plane with the specified normal.
	/// </summary>
	public static Vector3 ProjectOntoPlaneAndNormalize(Vector3 v, Vector3 normal)
	{
		float dot = Vector3.Dot(v, normal);
		Vector3 outv = v - (normal * dot);
		outv.Normalize();
		return outv;
	}

	/// <summary>
	/// Returns true if the quaternion is normalized within epsilon.
	/// </summary>
	public static bool QuaternionIsNormalized(Quaternion a, float epsilon)
	{
		float len = (a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
		return Mathf.Abs(1.0f - len) < epsilon;
	}

	/// <summary>
	/// Returns the dot product of two quaternions.  Note this is not the cosine
	/// of the angle between the quaternions' forward vectors, but the cosine of
	/// half the angle between them.
	/// </summary>
	public static float QuaternionDot(Quaternion a, Quaternion b)
	{
		OVRDebugUtils.Assert(QuaternionIsNormalized(a, 0.001f));
		OVRDebugUtils.Assert(QuaternionIsNormalized(b, 0.001f));
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

	/// <summary>
	/// Returns the cosine of the angle between quaternions.  This just finds
	/// the double angle for the quaternion dot product.
	/// </summary>
	public static float CosineOfAngleBetweenQuaternions(Quaternion a, Quaternion b)
	{
		float dot = QuaternionDot(a, b);
		return CosineOfDoubleAngleFromCosine(dot);
	}

	/// <summary>
	/// Clamps a quaternion rotation so that it remains within the specified angle
	/// from identity.
	/// </summary>
	public static bool Clamp(ref Quaternion q, float cosineOfClampAngle)
	{
		float cosineOfHalfClampAngle = CosineOfHalfAngleFromCosine(cosineOfClampAngle);
		if (q.w >= cosineOfHalfClampAngle)
		{
			return false; // already inside of the clamp
		}
		if (q.w > 0.99999f)
		{
			q = Quaternion.identity;
			return true;
		}

		float s = SineFromCosine(q.w);
		OVRDebugUtils.Assert(s > OVRMathUtils.FltSmallestNonDenormal);

		Vector3 axis;
		axis.x = q.x / s;
		axis.y = q.y / s;
		axis.z = q.z / s;
		axis.Normalize();

		float sineOfHalfClampAngle = SineFromCosine(cosineOfHalfClampAngle);
		q.x = axis.x * sineOfHalfClampAngle;
		q.y = axis.y * sineOfHalfClampAngle;
		q.z = axis.z * sineOfHalfClampAngle;
		q.w = cosineOfHalfClampAngle;
		return true;
	}

	/// <summary>
	/// Returns the forward axis of the quaternion.
	/// This is just a helper to get the forward axis of a quaternion.
	/// </summary>
	public static Vector3 Forward(Quaternion q)
	{
		return q * Vector3.forward;
	}

	/// <summary>
	/// Clamps the passed quaternion to a maximum angle on the specified axis.  Returns true
	/// if the quaternion was outside of the max angle (i.e. was clamped). This is useful for
	/// AI to know when to turn their body, vs. head.
	/// </summary>
	public static bool ClampOnAxis(ref Quaternion q, Vector3 axis, float cosineOfClampAngle)
	{
		Vector3 qfwd = Forward(q);
		qfwd = ProjectOntoPlaneAndNormalize(qfwd, axis);
		float dot = Vector3.Dot(Vector3.forward, qfwd);
		if (dot >= cosineOfClampAngle)
		{
			return false; // already inside of the constraint angle on this axis
		}
		// construct a quaternion that is the clamped rotation on the correct side
		Quaternion clampRot = QuaternionFromAxisAndDotProduct(axis, cosineOfClampAngle);
		float rightDot = Vector3.Dot(Vector3.right, qfwd);
		if (rightDot < 0.0f)
		{
			clampRot = Quaternion.Inverse(clampRot);
		}
		// find the difference between the quaternion (projected onto axis) and the
		// clamped rotation, then rotate the full (unprojected) quaternion back by
		// this difference.
		Quaternion projected = QuaternionFromAxisAndDotProduct(axis, dot);
		if (rightDot < 0.0f)
		{
			projected = Quaternion.Inverse(projected);
		}
		Quaternion deltaq = Quaternion.Inverse(clampRot) * projected;
		q = Quaternion.Inverse(deltaq) * q;

		return true; // clamped
	}
}
