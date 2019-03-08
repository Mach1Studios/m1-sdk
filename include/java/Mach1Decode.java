//  Mach1 SDK //<>//
//  Copyright © 2017 Mach1. All rights reserved.

package com.Mach1;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.NativeLong;
import com.sun.jna.Platform;
import com.sun.jna.*;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class Mach1Decode {
  private Pointer M1obj;

  public static enum  Mach1PlatformType {
    Mach1PlatformDefault, Mach1PlatformUnity, Mach1PlatformUE, Mach1PlatformOfEasyCam, Mach1PlatformAndroid, Mach1PlatformiOS
  };

  public static enum Mach1DecodeAlgoType {
    Mach1DecodeAlgoSpatial, Mach1DecodeAlgoAltSpatial, Mach1DecodeAlgoHorizon, Mach1DecodeAlgoHorizonPairs, Mach1DecodeAlgoSpatialPairs
  };

  public static class Mach1Point3D extends Structure {
    public float x;
    public float y;
    public float z;

    protected List<String> getFieldOrder() {
      return Arrays.asList(new String[] { "x", "y", "z" });
    }
  }

  private interface Mach1DecodeCAPI extends Library {
    Pointer Mach1DecodeCAPI_create();
    void Mach1DecodeCAPI_delete(Pointer M1obj);

    void Mach1DecodeCAPI_setDecodeAlgoType(Pointer M1obj, int algorithmType);
    void Mach1DecodeCAPI_setPlatformType(Pointer M1obj, int platformType);

    void Mach1DecodeCAPI_decode(Pointer M1obj, float Yaw, float Pitch, float Roll, Pointer data, int bufferSize, int sampleIndex);

    void Mach1DecodeCAPI_setFilterSpeed(Pointer M1obj, float filterSpeed);
    void Mach1DecodeCAPI_beginBuffer(Pointer M1obj);
    void Mach1DecodeCAPI_endBuffer(Pointer M1obj);

    long Mach1DecodeCAPI_getCurrentTime(Pointer M1obj);
    String Mach1DecodeCAPI_getLog(Pointer M1obj);

    Mach1Point3D Mach1DecodeCAPI_getCurrentAngle(Pointer M1obj);
  }

  private Mach1DecodeCAPI M1API;

  public Mach1Decode(String dir)
  {
    if (M1API == null)
    {
      M1API = (Mach1DecodeCAPI)Native.loadLibrary(dir + (Platform.isWindows() ? "Mach1DecodeCAPI.dll" : "Mach1DecodeCAPI.so"), Mach1DecodeCAPI.class);
    }
    M1obj = M1API.Mach1DecodeCAPI_create();
  }

  protected void finalize ( ) {
    M1API.Mach1DecodeCAPI_delete(M1obj);
  }

  public void setDecodeAlgoType(Pointer M1obj, Mach1DecodeAlgoType algorithmType)
  {
    M1API.Mach1DecodeCAPI_setDecodeAlgoType(M1obj, algorithmType.ordinal());
  }
  public void setPlatformType(Pointer M1obj, Mach1PlatformType platformType)
  {
    M1API.Mach1DecodeCAPI_setPlatformType(M1obj, platformType.ordinal());
  }

  public void decode(float Yaw, float Pitch, float Roll, float[] data, int bufferSize, int sampleIndex)
  {
    Pointer ptr = new Memory(data.length * Float.SIZE);
    ptr.write(0, data, 0, data.length);

    M1API.Mach1DecodeCAPI_decode(M1obj, Yaw, Pitch, Roll, ptr, bufferSize, sampleIndex);

    System.arraycopy(ptr.getFloatArray(0, 18), 0, data, 0, 18);
  }

  public void setFilterSpeed(float filterSpeed)
  {
    M1API.Mach1DecodeCAPI_setFilterSpeed(M1obj, filterSpeed);
  }

  public void beginBuffer()
  {
    M1API.Mach1DecodeCAPI_beginBuffer(M1obj);
  }

  public void endBuffer()
  {
    M1API.Mach1DecodeCAPI_endBuffer(M1obj);
  }

  public long getCurrentTime()
  {
    return M1API.Mach1DecodeCAPI_getCurrentTime(M1obj);
  }

  public String getLog()
  {
    return M1API.Mach1DecodeCAPI_getLog(M1obj);
  } 

  public Mach1Point3D getCurrentAngle()
  {
    return M1API.Mach1DecodeCAPI_getCurrentAngle(M1obj);
  }
}
