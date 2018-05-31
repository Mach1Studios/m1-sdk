//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.

package com.Mach1;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.NativeLong;
import com.sun.jna.Platform;
import com.sun.jna.*;

public class Mach1Decode {
  private Pointer M1obj;
  private interface Mach1DecodeCAPI extends Library {
    Pointer Mach1DecodeCAPI_create();
    void Mach1DecodeCAPI_delete(Pointer M1obj);

    void Mach1DecodeCAPI_decode(Pointer M1obj, float Yaw, float Pitch, float Roll, Pointer data, int bufferSize, int sampleIndex);

    long Mach1DecodeCAPI_getCurrentTime(Pointer M1obj);
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

  public void decode(float Yaw, float Pitch, float Roll, float[] data, int bufferSize, int sampleIndex)
  {
    Pointer ptr = new Memory(data.length * Float.SIZE);
    ptr.write(0, data, 0, data.length); //<>//

    M1API.Mach1DecodeCAPI_decode(M1obj, Yaw, Pitch, Roll, ptr, bufferSize, sampleIndex);

    System.arraycopy(ptr.getFloatArray(0, 18), 0, data, 0, 18);
  }
 
  public long getCurrentTime()
  {
    return M1API.Mach1DecodeCAPI_getCurrentTime(M1obj);
  }

}
