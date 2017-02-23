//import android.util.Log;

class mPoint {
	float x;
	float y;
	float z;
	
	public mPoint() {
		x = 0;
		y = 0;
		z = 0;
	}
	
	public mPoint(float x, float y) {
		this.x = x;
		this.y = y;
		this.z = 0;
	}
	
	public mPoint(float x, float y, float z) {
		this.x = x;
		this.y = y;
		this.z = z;
	}
	
	public mPoint add(mPoint pnt) {
		return new mPoint(x+pnt.x, y+pnt.y, z+pnt.z);
	}
	
	public mPoint multiply(float f) {
		return new mPoint(x*f, y*f, z*f);
	}
	
	public mPoint multiply(mPoint pnt) {
		return new mPoint(x*pnt.x, y*pnt.y, z*pnt.z);
	}
	
	public mPoint subtract(mPoint pnt) {
		return new mPoint(x-pnt.x, y-pnt.y, z-pnt.z);
	}
	
	public float length() {
		return (float)Math.sqrt(x*x + y*y + z*z);
	}
	
	public float get(int index) {
		float[] arr = {x, y, z};
		return arr[index];
	}
	
    public mPoint rotate(float angle, mPoint axis) {
        mPoint ax = axis.getNormalized();
        float a = (float)(angle*3.14159265358979323846f/180.0f);
        float sina = (float)Math.sin( a );
        float cosa = (float)Math.cos( a );
        float cosb = 1.0f - cosa;
        
        float nx = x*(ax.x*ax.x*cosb + cosa)
        + y*(ax.x*ax.y*cosb - ax.z*sina)
        + z*(ax.x*ax.z*cosb + ax.y*sina);
        float ny = x*(ax.y*ax.x*cosb + ax.z*sina)
        + y*(ax.y*ax.y*cosb + cosa)
        + z*(ax.y*ax.z*cosb - ax.x*sina);
        float nz = x*(ax.z*ax.x*cosb - ax.y*sina)
        + y*(ax.z*ax.y*cosb + ax.x*sina)
        + z*(ax.z*ax.z*cosb + cosa);
        x = nx; y = ny; z = nz;
        
        return this;
    }
    
    public mPoint normalize() {
        float length = (float)Math.sqrt(x*x + y*y + z*z);
        if( length > 0 ) {
            x /= length;
            y /= length;
            z /= length;
        }
        return this;
    }
    
    public mPoint getNormalized() {
        float length = (float)Math.sqrt(x*x + y*y + z*z);
        if( length > 0 ) {
            return new mPoint( x/length, y/length, z/length );
        } else {
            return new mPoint();
        }
    }
    
    public mPoint getRotated(float angle, mPoint axis) {
        mPoint ax = axis.getNormalized();
        float a = (float)(angle*3.14159265358979323846f/180.0f);
        float sina = (float)Math.sin( a );
        float cosa = (float)Math.cos( a );
        float cosb = 1.0f - cosa;
        
        return new mPoint( x*(ax.x*ax.x*cosb + cosa)
                      + y*(ax.x*ax.y*cosb - ax.z*sina)
                      + z*(ax.x*ax.z*cosb + ax.y*sina),
                      x*(ax.y*ax.x*cosb + ax.z*sina)
                      + y*(ax.y*ax.y*cosb + cosa)
                      + z*(ax.y*ax.z*cosb - ax.x*sina),
                      x*(ax.z*ax.x*cosb - ax.y*sina)
                      + y*(ax.z*ax.y*cosb + ax.x*sina)
                      + z*(ax.z*ax.z*cosb + cosa) );
    }
}

public class M1Audio {
	private static float mDegToRad(float degrees) {
	    return degrees * (3.14159265358979323846f/180.0f);
	}
	
	private static float mmap(float value, float inputMin, float inputMax, float outputMin, float outputMax, boolean clamp) {
	    if (Math.abs(inputMin - inputMax) < Float.MIN_VALUE){
	        return outputMin;
	    } else {
	        float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
	        
	        if( clamp ){
	            if(outputMax < outputMin){
	                if( outVal < outputMax )outVal = outputMax;
	                else if( outVal > outputMin )outVal = outputMin;
	            }else{
	                if( outVal > outputMax )outVal = outputMax;
	                else if( outVal < outputMin )outVal = outputMin;
	            }
	        }
	        return outVal;
	    }
	    
	}

	private static float clamp(float a, float min, float max )
	{
	    return (a < min) ? min : ((a > max) ? max : a);
	}

	private static float[] eightChannelsIsotropicAlgorithm(float yaw, float pitch, float roll) {
	    mPoint simulationAngles = new mPoint(yaw, pitch, roll);
	    
	    mPoint faceVector1 = (new mPoint((float)Math.cos(mDegToRad(simulationAngles.get(1))), (float)Math.sin(mDegToRad(simulationAngles.get(1))))).normalize();
	    
	    mPoint faceVector2 = (faceVector1.getRotated(simulationAngles.get(0),
	    		new mPoint((float)Math.cos(mDegToRad(simulationAngles.get(1) - 90)), (float)Math.sin(mDegToRad(simulationAngles.get(1) - 90))))).normalize();
	    
	    mPoint faceVector21 = (faceVector1.getRotated(simulationAngles.get(0) + 90,
	    		new mPoint((float)Math.cos(mDegToRad(simulationAngles.get(1) - 90)), (float)Math.sin(mDegToRad(simulationAngles.get(1) - 90))))).normalize();
	    
	    mPoint faceVectorLeft = faceVector21.getRotated(-simulationAngles.get(2) - 90, faceVector2);
	    mPoint faceVectorRight = faceVector21.getRotated(-simulationAngles.get(2) + 90, faceVector2);
	    
	    //mPoint faceVectorOffsetted = (new mPoint((float)Math.cos(mDegToRad(simulationAngles.get(1))),
	    //                                    (float)Math.sin(mDegToRad(simulationAngles.get(1))))).normalize().rotate(simulationAngles.get(0) + 10, (new mPoint((float)Math.cos(mDegToRad(simulationAngles.get(1) - 90)), (float)Math.sin(mDegToRad(simulationAngles.get(1) - 90))).normalize())).subtract(faceVector2);

	    //mPoint tiltSphereRotated = faceVectorOffsetted.rotate(-simulationAngles.get(2), faceVector2);
	    
	    // Drawing another 8 dots
	    
	    mPoint[] points = 
	    {   new mPoint(100, -100, -100),
	        new mPoint(100, 100, -100),
	        new mPoint(-100, -100, -100),
	        new mPoint(-100, 100, -100),
	        
	        new mPoint(100, -100, 100),
	        new mPoint(100, 100, 100),
	        new mPoint(-100, -100, 100),
	        new mPoint(-100, 100, 100)
	    };
	    
	    float[] qL = new float[8];
	    for (int i = 0; i < 8; i++) {
	        qL[i] = (faceVectorLeft.multiply(100.0f).add(faceVector2.multiply(100.0f)).subtract(points[i])).length();
	    }
	    
	    float[] qR = new float[8];
	    for (int i = 0; i < 8; i++) {
	        qR[i] = (faceVectorRight.multiply(100.0f).add(faceVector2.multiply(100.0f)).subtract(points[i])).length();
	    }
	    
	    float[] result = new float[16];
	    
	    for (int i = 0; i < 8; i++) {
	        float vL = clamp(mmap(qL[i] * 2, 250, 400, 1.0f, 0.0f, false), 0.0f, 1.0f) / 2;
	        float vR = clamp(mmap(qR[i] * 2, 250, 400, 1.0f, 0.0f, false), 0.0f, 1.0f) / 2;
	        
	        // TODO: why did I need to put / 2 here to match what I had with other
	        // algo? Isn't that other one normalized to max 1?
	        
	        result[i * 2] = vR;
	        result[i  * 2 + 1] = vL;
	        
	    }
	    
	    return result;
	}

	public static void interleavedM1SpatialFormat(int yaw, int pitch, int roll, int sampleCount, short data_in[], short data_out[]) {
		float[] coefficients;
		
		// yaw comes in as 0 front, 90 right, 180 back, 270 left
		// pitch comes in as 90 down, 0 straight, 270 up
		// roll comes in as 90 tilt left, 0 straight, 270 tilt right
		
		// pitch needs to be -90 down, 0 straight, 90 up
		pitch = 360 - pitch;
		
		// roll needs to be -90 tilt left, 0 straight, 90 tilt right
		roll = 360 - roll;
		
		coefficients = eightChannelsIsotropicAlgorithm(yaw, pitch, roll);
		
		//Log.i("BR", "yaw: " + yaw + " pitch: " + pitch + " roll: " + roll);
		//Log.i("BR", "coefficients even: [ " + coefficients[0] + ", " + coefficients[2] + ", " + coefficients[4] + ", " + coefficients[6] + ", " + coefficients[8] + ", " + coefficients[10] + ", " + coefficients[12] + ", " + coefficients[14] + "]");
		//Log.i("BR", "coefficients odd:  [ " + coefficients[1] + ", " + coefficients[3] + ", " + coefficients[5] + ", " + coefficients[7] + ", " + coefficients[9] + ", " + coefficients[11] + ", " + coefficients[13] + ", " + coefficients[15] + "]");
		
		int j = 0;
		for(int i=0;i<sampleCount;i+=8) {
			int left = 0;
			int right = 0;
			for(int k=0;k<8;k++) {
				left += data_in[i+k]*coefficients[2*k];
				right += data_in[i+k]*coefficients[2*k+1];
			}
			data_out[j] = (short)(left);
			data_out[j+1] = (short)(right);
			j += 2;
		}
	}
}
