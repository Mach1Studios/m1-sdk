//
//  Mach1 SDK
//
//  Multichannel audio format family
//
//  Mixing algorithms in Java
//
//  Updated to match: 0.9.8a

 
import java.util.*;

final static float __FLT_EPSILON__ = 1.19209290e-07F;

public class mPoint
{
  public float x;
  public float y;
  public float z;

  public mPoint()
  {
    x = 0F;
    y = 0F;
    z = 0F;
  }

  public mPoint(float X, float Y, float Z)
  {
    x = X;
    y = Y;
    z = Z;
  }

  public mPoint(float X, float Y)
  {
    x = X;
    y = Y;
    z = 0F;
  }

  public mPoint add(mPoint pnt)
  {
    return new mPoint(x + pnt.x, y + pnt.y, z + pnt.z);
  }

  public mPoint multiply(float f)
  {
    return new mPoint(x * f, y * f, z * f);
  }


  public mPoint multiply(mPoint vec)
  {
    return new mPoint(x * vec.x, y * vec.y, z * vec.z);
  }


  public mPoint subtract(mPoint vec)
  {
    return new mPoint(x - vec.x, y - vec.y, z - vec.z);
  }

  public final float length()
  {
    return (float)Math.sqrt(x * x + y * y + z * z);
  }


  public final float getItem (int index)
  {
    float[] arr = {x, y, z};
    return arr[index];
  }

  public final mPoint rotate(float angle, mPoint axis)
  {
    mPoint ax = axis.getNormalized();
    float a = (float)(angle * PI / 180.0);
    float sina = (float)Math.sin(a);
    float cosa = (float)Math.cos(a);
    float cosb = 1.0f - cosa;

    float nx = x * (ax.x * ax.x * cosb + cosa) + y * (ax.x * ax.y * cosb - ax.z * sina) + z * (ax.x * ax.z * cosb + ax.y * sina);
    float ny = x * (ax.y * ax.x * cosb + ax.z * sina) + y * (ax.y * ax.y * cosb + cosa) + z * (ax.y * ax.z * cosb - ax.x * sina);
    float nz = x * (ax.z * ax.x * cosb - ax.y * sina) + y * (ax.z * ax.y * cosb + ax.x * sina) + z * (ax.z * ax.z * cosb + cosa);
    x = nx;
    y = ny;
    z = nz;
    return this;
  }

  public final mPoint normalize()
  {
    float length = (float)Math.sqrt(x * x + y * y + z * z);
    if (length > 0)
    {
      x /= length;
      y /= length;
      z /= length;
    }
    return this;
  }


  public final mPoint getNormalized()
  {
    float length = (float)Math.sqrt(x * x + y * y + z * z);
    if (length > 0)
    {
      return new mPoint(x / length, y / length, z / length);
    } else
    {
      return new mPoint();
    }
  }

  public final mPoint getRotated(float angle, mPoint axis)
  {
    mPoint ax = axis.getNormalized();
    float a = (float)(angle * PI / 180.0);
    float sina = (float)Math.sin(a);
    float cosa = (float)Math.cos(a);
    float cosb = 1.0f - cosa;

    return new mPoint(x * (ax.x * ax.x * cosb + cosa) + y * (ax.x * ax.y * cosb - ax.z * sina) + z * (ax.x * ax.z * cosb + ax.y * sina), x * (ax.y * ax.x * cosb + ax.z * sina) + y * (ax.y * ax.y * cosb + cosa) + z * (ax.y * ax.z * cosb - ax.x * sina), x * (ax.z * ax.x * cosb - ax.y * sina) + y * (ax.z * ax.y * cosb + ax.x * sina) + z * (ax.z * ax.z * cosb + cosa));
  }
}


private static float mDegToRad(float degrees)
{
  return degrees * DEG_TO_RAD;
}

//Utility function for mapping values
private static float mmap(float value, float inputMin, float inputMax, float outputMin, float outputMax, boolean clamp)
{

  if (Math.abs(inputMin - inputMax) < __FLT_EPSILON__)
  {
    return outputMin;
  } else
  {
    float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);

    if (clamp)
    {
      if (outputMax < outputMin)
      {
        if (outVal < outputMax)
        {
          outVal = outputMax;
        } else if (outVal > outputMin)
        {
          outVal = outputMin;
        }
      } else
      {
        if (outVal > outputMax)
        {
          outVal = outputMax;
        } else if (outVal < outputMin)
        {
          outVal = outputMin;
        }
      }
    }
    return outVal;
  }
}

//Utility function for clamping values 
private static float clamp(float a, float min, float max)
{
  return (a < min) ? min : ((a > max) ? max : a);
}
private static float alignAngle(float a, float min)
{
  return alignAngle(a, min, 180);
}
private static float alignAngle(float a)
{
  return alignAngle(a, -180, 180);
}

//Utility function for ensuring angle is aligned to -180/180 range
private static float alignAngle(float a, float min, float max)
{
  a = (float)Math.IEEEremainder(a, 360);
 
  if (a < min) a += 360;
  if (a > max) a -= 360;
  
  return a;
}



private ArrayList<Float> fourChannelAlgorithm(float Yaw, float Pitch, float Roll)
{
  //Orientation input safety clamps/alignment
  Yaw = alignAngle(Yaw, 0, 360);
  
  float[] coefficients = new float[4];
  coefficients[0] = 1.0 - Math.min(1.0, Math.min((float)360.0 - Yaw, Yaw) / 90.0);
  coefficients[1] = 1.0 - Math.min(1.0, Math.abs((float)90.0 - Yaw) / 90.0);
  coefficients[2] = 1.0 - Math.min(1.0, Math.abs((float)180.0 - Yaw) / 90.0);
  coefficients[3] = 1.0 - Math.min(1.0, Math.abs((float)270.0 - Yaw) / 90.0);


  ArrayList<Float> result = new ArrayList<Float>();
  while(result.size() < 8) result.add(0F);

  result.set(0, coefficients[0]); // 1 left
  result.set(1, coefficients[3]); //   right
  result.set(2, coefficients[1]); // 2 left0
  result.set(3, coefficients[0]); //   right
  result.set(4, coefficients[3]); // 3 left
  result.set(5, coefficients[2]); //   right
  result.set(6, coefficients[2]); // 4 left
  result.set(7, coefficients[1]); //   right
  return result;
}


private ArrayList<Float> eightChannelsAlgorithm(float Yaw, float Pitch, float Roll)
{
  //ensure the angles are clamped and aligned on input
  Pitch = alignAngle(Pitch, -180, 180);
  Pitch = clamp(Pitch, -90, 90); // -90, 90

  Yaw = alignAngle(Yaw, 0, 360);

  Roll = alignAngle(Roll, -180, 180);
  Roll = clamp(Roll, -90, 90); // -90, 90


  //setup the yaw math
  float[] coefficients = new float[4]; //<>//
  coefficients[0] = 1.0 - Math.min(1.0, Math.min((float)360.0 - Yaw, Yaw) / 90.0);
  coefficients[1] = 1.0 - Math.min(1.0, Math.abs((float)90.0 - Yaw) / 90.0);
  coefficients[2] = 1.0 - Math.min(1.0, Math.abs((float)180.0 - Yaw) / 90.0);
  coefficients[3] = 1.0 - Math.min(1.0, Math.abs((float)270.0 - Yaw) / 90.0);

  float tiltAngle = mmap(Roll, -90, 90, 0.0, 1.0, true);
  //Equal Power crossfade if needed
  //float tiltHigh = cos(tiltAngle * (0.5 * PI));
  //float tiltLow = cos((1.0 - tiltAngle) * (0.5 * PI));
  float tiltHigh = tiltAngle;
  float tiltLow = 1. - tiltHigh;

  //ISSUE//
  //Able to kill stereo by making both pitch and tilt at max or min values together
  //With an HMD this is difficult for a user to exploit

  //routes the coeffs to all the needed stereo pairs and applies tilt multiplier
  ArrayList<Float> result = new ArrayList<Float>();
  while(result.size() < 16) result.add(0F);

  result.set(0, coefficients[0] * tiltHigh); // 1 left
  result.set(1, coefficients[3] * tiltHigh); //   right
  result.set(2, coefficients[1] * tiltLow); // 2 left
  result.set(3, coefficients[0] * tiltLow); //   right
  result.set(4, coefficients[3] * tiltLow); // 3 left
  result.set(5, coefficients[2] * tiltLow); //   right
  result.set(6, coefficients[2] * tiltHigh); // 4 left
  result.set(7, coefficients[1] * tiltHigh); //   right

  result.set(0 + 8, coefficients[0] * tiltLow); // 1 left
  result.set(1 + 8, coefficients[3] * tiltLow); //   right
  result.set(2 + 8, coefficients[1] * tiltHigh); // 2 left
  result.set(3 + 8, coefficients[0] * tiltHigh); //   right
  result.set(4 + 8, coefficients[3] * tiltHigh); // 3 left
  result.set(5 + 8, coefficients[2] * tiltHigh); //   right
  result.set(6 + 8, coefficients[2] * tiltLow); // 4 left
  result.set(7 + 8, coefficients[1] * tiltLow); //   right

  //Pitch orientation input
  float pitchAngle = mmap(Pitch, 90, -90, 0.0, 1.0, true);
  //Equal Power crossfade if needed
  //float pitchHigherHalf = cos(pitchAngle * (0.5*PI));
  //float pitchLowerHalf = cos((1.0 - pitchAngle) * (0.5*PI));
  float pitchHigherHalf = pitchAngle;
  float pitchLowerHalf = 1 - pitchHigherHalf;

  for (int i = 0; i < 8; i++)
  {
    result.set(i, result.get(i) * pitchLowerHalf);
    result.set(i + 8, result.get(i + 8) * pitchHigherHalf);
  }

  return result;
}


private ArrayList<Float> eightChannelsIsotropicAlgorithm(float Yaw, float Pitch, float Roll)
{
  mPoint simulationAngles =  new mPoint(-Pitch, Yaw, Roll);

  mPoint faceVector1 = new mPoint((float)Math.cos(mDegToRad(simulationAngles.getItem(1))), 
                                  (float)Math.sin(mDegToRad(simulationAngles.getItem(1)))).normalize();


  mPoint faceVector2 = faceVector1.getRotated(simulationAngles.getItem(0), 
                                              new mPoint((float)Math.cos(mDegToRad(simulationAngles.getItem(1) - 90)), 
                                                         (float)Math.sin(mDegToRad(simulationAngles.getItem(1) - 90))).normalize());


  mPoint faceVector21 = faceVector1.getRotated(simulationAngles.getItem(0) + 90, 
                                               new mPoint((float)Math.cos(mDegToRad(simulationAngles.getItem(1) - 90)), 
                                                          (float)Math.sin(mDegToRad(simulationAngles.getItem(1) - 90))).normalize());

  mPoint faceVectorLeft = faceVector21.getRotated(-simulationAngles.getItem(2) + 90, faceVector2);
  mPoint faceVectorRight = faceVector21.getRotated(-simulationAngles.getItem(2) - 90, faceVector2);


  mPoint faceVectorOffsetted = new mPoint((float)Math.cos(mDegToRad(simulationAngles.getItem(1))), 
                                          (float)Math.sin(mDegToRad(simulationAngles.getItem(1)))).normalize().rotate(
                                                        simulationAngles.getItem(0) + 10, 
                                                        new mPoint((float)Math.cos(mDegToRad(simulationAngles.getItem(1) - 90)), 
                                                                   (float)Math.sin(mDegToRad(simulationAngles.getItem(1) - 90))).normalize()).subtract(faceVector2);

  mPoint tiltSphereRotated = faceVectorOffsetted.rotate(-simulationAngles.getItem(2), faceVector2);

  

  mPoint[] points = { new mPoint(100, -100, -100), 
                      new mPoint(100, 100, -100), 
                      new mPoint(-100, -100, -100), 
                      new mPoint(-100, 100, -100), 
                      new mPoint(100, -100, 100), 
                      new mPoint(100, 100, 100), 
                      new mPoint(-100, -100, 100), 
                      new mPoint(-100, 100, 100)};

  float[] qL = new float[8];
  for (int i = 0; i < 8; i++)
  {
    qL[i] = (faceVectorLeft.multiply(100).add(faceVector2.multiply(100)).subtract(points[i])).length();
  }

  float[] qR = new float[8];
  for (int i = 0; i < 8; i++)
  {
    qR[i] = (faceVectorRight.multiply(100).add(faceVector2.multiply(100)).subtract(points[i])).length();
  }

  ArrayList<Float> result = new ArrayList<Float>();
  while(result.size() < 16) result.add(0F);

  for (int i = 0; i < 8; i++)
  {
    float vL = clamp(mmap(qL[i], 0, 223, 1.0, 0.0, false), 0, 1);
    float vR = clamp(mmap(qR[i], 0, 223, 1.0, 0.0, false), 0, 1);



    result.set(i * 2, vL);
    result.set(i * 2 + 1, vR);
    
    
  }
  
  // volume balancer v2.0
  
  float sumL = 0, sumR = 0;
  for (int i = 0; i < 8; i++) {
        sumL += result.get(i * 2);
        sumR += result.get(i * 2 + 1);
    }
    
  float[] multipliersL = new float[8];
  float[] multipliersR = new float[8];
  for (int i = 0; i < 8; i++) {
    
        multipliersL[i] = result.get(i * 2) / sumL;
        multipliersR[i] = result.get(i * 2 + 1) / sumR;
  }
  
  for (int i = 0; i < 8; i++) {
        multipliersL[i] = result.get(i * 2) / sumL;
        multipliersR[i] = result.get(i * 2 + 1) / sumR;
  }
  
  float sumDiffL = sumL - 1.;
  float sumDiffR = sumR - 1.;
  
  float[] correctedVolumesL = new float[8]; 
  float[] correctedVolumesR = new float[8];
  for (int i = 0; i < 8; i++) {
        correctedVolumesL[i] = result.get(i * 2) - sumDiffL * multipliersL[i];
        correctedVolumesR[i] = result.get(i * 2 + 1) - sumDiffR * multipliersR[i];
  }
  
  for (int i = 0; i < 8; i++) {
        result.set(i * 2, correctedVolumesL[i]);
        result.set(i * 2 + 1, correctedVolumesR[i]);
  }
  
  return result;
}