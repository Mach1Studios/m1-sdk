import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.NativeLong;
import com.sun.jna.Platform;
import com.sun.jna.*;

public interface M1DSPAlgorithmsCAPI extends Library {
  Pointer M1DSPAlgorithmsCAPI_create();
  void M1DSPAlgorithmsCAPI_delete(Pointer M1obj);

  Pointer M1DSPAlgorithmsCAPI_fourChannelAlgorithm(Pointer M1obj, float Yaw, float Pitch, float Roll, boolean smoothAngles);
  Pointer M1DSPAlgorithmsCAPI_fourPairsAlgorithm(Pointer M1obj, float Yaw, float Pitch, float Roll, boolean smoothAngles);
  Pointer M1DSPAlgorithmsCAPI_eightChannelsIsotropicAlgorithm(Pointer M1obj, float Yaw, float Pitch, float Roll, boolean smoothAngles);
  Pointer M1DSPAlgorithmsCAPI_eightChannelsAlgorithm(Pointer M1obj, float Yaw, float Pitch, float Roll, boolean smoothAngles);
  Pointer M1DSPAlgorithmsCAPI_eightPairsAlgorithm(Pointer M1obj, float Yaw, float Pitch, float Roll, boolean smoothAngles);

  long M1DSPAlgorithmsCAPI_getCurrentTime(Pointer M1obj);
}

public static class M1DSPAlgorithms {
  private Pointer M1obj;
  private M1DSPAlgorithmsCAPI M1API;

  M1DSPAlgorithms(String dir)
  {
    if (M1API == null)
    {
      M1API = (M1DSPAlgorithmsCAPI)Native.loadLibrary(dir + (Platform.isWindows() ? "/M1DSPAlgorithmsCAPI.dll" : "/M1DSPAlgorithmsCAPI.so"), M1DSPAlgorithmsCAPI.class);
    }
    M1obj = M1API.M1DSPAlgorithmsCAPI_create();
  }

  public float[] fourChannelAlgorithm(float Yaw, float Pitch, float Roll, boolean smoothAngles)
  {
    Pointer p = M1API.M1DSPAlgorithmsCAPI_fourChannelAlgorithm(M1obj, Yaw, Pitch, Roll, smoothAngles);
    return p.getFloatArray(0, 10);
  }

  public float[] fourPairsAlgorithm(float Yaw, float Pitch, float Roll, boolean smoothAngles)
  {
    Pointer p = M1API.M1DSPAlgorithmsCAPI_fourPairsAlgorithm(M1obj, Yaw, Pitch, Roll, smoothAngles);
    return p.getFloatArray(0, 6);
  }


  public float[] eightChannelsIsotropicAlgorithm(float Yaw, float Pitch, float Roll, boolean smoothAngles)
  {
    Pointer p = M1API.M1DSPAlgorithmsCAPI_eightChannelsIsotropicAlgorithm(M1obj, Yaw, Pitch, Roll, smoothAngles);
    return p.getFloatArray(0, 18);
  }

  public float[] eightChannelsAlgorithm(float Yaw, float Pitch, float Roll, boolean smoothAngles)
  {
    Pointer p = M1API.M1DSPAlgorithmsCAPI_eightChannelsAlgorithm(M1obj, Yaw, Pitch, Roll, smoothAngles);
    return p.getFloatArray(0, 18);
  }

  public float[] eightPairsAlgorithm(float Yaw, float Pitch, float Roll, boolean smoothAngles)
  {
    Pointer p = M1API.M1DSPAlgorithmsCAPI_eightPairsAlgorithm(M1obj, Yaw, Pitch, Roll, smoothAngles);
    return p.getFloatArray(0, 10);
  }

  public long getCurrentTime()
  {
    return M1API.M1DSPAlgorithmsCAPI_getCurrentTime(M1obj);
  }

  protected void finalize ( ) {
    M1API.M1DSPAlgorithmsCAPI_delete(M1obj);
  }
}