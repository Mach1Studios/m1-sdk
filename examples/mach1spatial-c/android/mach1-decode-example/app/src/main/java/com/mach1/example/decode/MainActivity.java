package com.mach1.example.decode;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

import java.lang.Math;

import android.media.AudioManager;
import android.view.View;
import android.widget.Button;

import android.content.res.AssetManager;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

import android.content.res.Configuration;
import android.view.Surface;
import android.content.pm.ActivityInfo;
import android.util.DisplayMetrics;

public class MainActivity extends Activity implements SensorEventListener {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private static final String LOG_TAG = "MyTag";
    private SensorManager mSensorManager;
    private Sensor mRotationSensor;

    private TextView textView;
    /*
    private int getScreenOrientation() {
        int rotation = getWindowManager().getDefaultDisplay().getRotation();
        DisplayMetrics dm = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(dm);
        int width = dm.widthPixels;
        int height = dm.heightPixels;
        int orientation;
        // if the device's natural orientation is portrait:
        if ((rotation == Surface.ROTATION_0
                || rotation == Surface.ROTATION_180) && height > width ||
                (rotation == Surface.ROTATION_90
                        || rotation == Surface.ROTATION_270) && width > height) {
            switch(rotation) {
                case Surface.ROTATION_0:
                    orientation = ActivityInfo.SCREEN_ORIENTATION_PORTRAIT;
                    break;
                case Surface.ROTATION_90:
                    orientation = ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE;
                    break;
                case Surface.ROTATION_180:
                    orientation = ActivityInfo.SCREEN_ORIENTATION_REVERSE_PORTRAIT;
                    break;
                case Surface.ROTATION_270:
                    orientation = ActivityInfo.SCREEN_ORIENTATION_REVERSE_LANDSCAPE;
                    break;
                default:
                    orientation = ActivityInfo.SCREEN_ORIENTATION_PORTRAIT;
                    break;
            }
        }
        // if the device's natural orientation is landscape or if the device
        // is square:
        else {
            switch(rotation) {
                case Surface.ROTATION_0:
                    orientation = ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE;
                    break;
                case Surface.ROTATION_90:
                    orientation = ActivityInfo.SCREEN_ORIENTATION_PORTRAIT;
                    break;
                case Surface.ROTATION_180:
                    orientation = ActivityInfo.SCREEN_ORIENTATION_REVERSE_LANDSCAPE;
                    break;
                case Surface.ROTATION_270:
                    orientation = ActivityInfo.SCREEN_ORIENTATION_REVERSE_PORTRAIT;
                    break;
                default:
                    orientation = ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE;
                    break;
            }
        }

        return orientation;
    }
    */

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        AudioManager am = (AudioManager) getSystemService(AUDIO_SERVICE);
        String framesPerBuffer = am.getProperty(AudioManager.PROPERTY_OUTPUT_FRAMES_PER_BUFFER);
        int framesPerBufferInt = Integer.parseInt(framesPerBuffer);
        if (framesPerBufferInt == 0) framesPerBufferInt = 512; // Use default
        initAudio(framesPerBufferInt);

        // Get an instance of the SensorManager
        mSensorManager = (SensorManager)getSystemService(SENSOR_SERVICE);

        mRotationSensor = mSensorManager.getDefaultSensor(Sensor.TYPE_ROTATION_VECTOR);

        mSensorManager.registerListener(this, mRotationSensor, SensorManager.SENSOR_DELAY_GAME);

        ((Button) findViewById(R.id.buttonPlay)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Play();
                //Log.d("mytag","play");
            }
        });

        ((Button) findViewById(R.id.buttonStop)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Stop();
            }
        });


        textView = (TextView) findViewById(R.id.textView);

        Play();
    }

    public void onAccuracyChanged(Sensor sensor, int accuracy) {  }


    public void onSensorChanged(SensorEvent event) {
        if (event.sensor.getType() == Sensor.TYPE_ROTATION_VECTOR ) {
            // get the rotation matrix
            float[] temp_matrix = new float[9];
            SensorManager.getRotationMatrixFromVector(temp_matrix, event.values);

            float[] mOrientation = new float[3];
            SensorManager.getOrientation(temp_matrix, mOrientation);

            // orientation contains: azimut, pitch and roll
            float mYaw = (float)( Math.toDegrees( mOrientation[0]) );// % 360;
            float mPitch = (float)( Math.toDegrees( mOrientation[2]) + 90 );// % 360;
            float mRoll = (float)( Math.toDegrees( mOrientation[1]) + 0 );// % 360;

            setAudioAngles(mYaw, mPitch, mRoll);

            textView.setText(
                    "angles: \r\n" +
                            "Yaw: " + Integer.toString((int)mYaw) + "\r\n" +
                            "Pitch: " + Integer.toString((int)mPitch) + "\r\n" +
                            "Roll: " + Integer.toString((int)mRoll));
            //Log.v(LOG_TAG, "SENSOR: " + mYaw + " , " + mPitch + " , " + mRoll + " , " );
        }
    }

    @Override
    protected void onDestroy() {
        //shutdownAudio();
        super.onDestroy();
    }

    protected void Play()
    {
        playAudio(getAssets(), "Guitar-8ch.aac");
    }

    protected void Stop()
    {
        stopAudio();
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */

    public native void initAudio(int framesPerBufferInt);
    public native void shutdownAudio();

    public native void setAudioAngles(float Yaw, float Pitch, float Roll);

    public native void playAudio(AssetManager assetsManager, String name );
    public native void stopAudio();
}
