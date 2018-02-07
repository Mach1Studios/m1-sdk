//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.NativeLong;
import com.sun.jna.Platform;
import com.sun.jna.*;

public interface Mach1DecodeCAPI extends Library {
  Pointer Mach1DecodeCAPI_create();
  void Mach1DecodeCAPI_delete(Pointer M1obj);

  Pointer Mach1DecodeCAPI_horizonAlgo(Pointer M1obj, float Yaw, float Pitch, float Roll, boolean smoothAngles);
  Pointer Mach1DecodeCAPI_horizonPairsAlgo(Pointer M1obj, float Yaw, float Pitch, float Roll, boolean smoothAngles);
  Pointer Mach1DecodeCAPI_spatialAlgo(Pointer M1obj, float Yaw, float Pitch, float Roll, boolean smoothAngles);
  Pointer Mach1DecodeCAPI_spatialAltAlgo(Pointer M1obj, float Yaw, float Pitch, float Roll, boolean smoothAngles);
  Pointer Mach1DecodeCAPI_spatialPairsAlgo(Pointer M1obj, float Yaw, float Pitch, float Roll, boolean smoothAngles);

  long Mach1DecodeCAPI_getCurrentTime(Pointer M1obj);
}

public static class Mach1Decode {
  private Pointer M1obj;
  private Mach1DecodeCAPI M1API;

  Mach1Decode(String dir)
  {
    if (M1API == null)
    {
      M1API = (Mach1DecodeCAPI)Native.loadLibrary(dir + (Platform.isWindows() ? "/Mach1DecodeCAPI.dll" : "/Mach1DecodeCAPI.so"), Mach1DecodeCAPI.class);
    }
    M1obj = M1API.Mach1DecodeCAPI_create();
  }

  public float[] horizonAlgo(float Yaw, float Pitch, float Roll, boolean smoothAngles)
  {
    Pointer p = M1API.Mach1DecodeCAPI_horizonAlgo(M1obj, Yaw, Pitch, Roll, smoothAngles);
    return p.getFloatArray(0, 10);
  }

  public float[] horizonPairsAlgo(float Yaw, float Pitch, float Roll, boolean smoothAngles)
  {
    Pointer p = M1API.Mach1DecodeCAPI_horizonPairsAlgo(M1obj, Yaw, Pitch, Roll, smoothAngles);
    return p.getFloatArray(0, 8);
  }


  public float[] spatialAlgo(float Yaw, float Pitch, float Roll, boolean smoothAngles)
  {
    Pointer p = M1API.Mach1DecodeCAPI_spatialAlgo(M1obj, Yaw, Pitch, Roll, smoothAngles);
    return p.getFloatArray(0, 18);
  }

  public float[] spatialAltAlgo(float Yaw, float Pitch, float Roll, boolean smoothAngles)
  {
    Pointer p = M1API.Mach1DecodeCAPI_spatialAltAlgo(M1obj, Yaw, Pitch, Roll, smoothAngles);
    return p.getFloatArray(0, 18);
  }

  public float[] spatialPairsAlgo(float Yaw, float Pitch, float Roll, boolean smoothAngles)
  {
    Pointer p = M1API.Mach1DecodeCAPI_spatialPairsAlgo(M1obj, Yaw, Pitch, Roll, smoothAngles);
    return p.getFloatArray(0, 16);
  }

  public long getCurrentTime()
  {
    return M1API.Mach1DecodeCAPI_getCurrentTime(M1obj);
  }

  protected void finalize ( ) {
    M1API.Mach1DecodeCAPI_delete(M1obj);
  }
}