package com.example.user.myapplication;

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

public class MainActivity extends Activity implements SensorEventListener {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private static final String LOG_TAG = "MyTag";
    private SensorManager mSensorManager;
    private Sensor mAccelerometer;
    private Sensor mMagnetometer;

    private TextView textView;

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

        // mRotationVectorSensor = mSensorManager.getDefaultSensor(Sensor.TYPE_ROTATION_VECTOR );//TYPE_GAME_ROTATION_VECTOR);//TYPE_ROTATION_VECTOR);
        //   mGravity = mSensorManager.getDefaultSensor(Sensor.TYPE_GRAVITY);
        mAccelerometer = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        mMagnetometer = mSensorManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD);

        // mSensorManager.registerListener(this, mGravity, SensorManager.SENSOR_DELAY_FASTEST);
        mSensorManager.registerListener(this, mAccelerometer, SensorManager.SENSOR_DELAY_FASTEST);
        mSensorManager.registerListener(this, mMagnetometer, SensorManager.SENSOR_DELAY_FASTEST);

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

    float[] mGravity;
    float[] mGeomagnetic;

    public void onSensorChanged(SensorEvent event) {

        if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER)
            mGravity = event.values;
        if (event.sensor.getType() == Sensor.TYPE_MAGNETIC_FIELD)
            mGeomagnetic = event.values;
        if (mGravity != null && mGeomagnetic != null) {
            float R[] = new float[9];
            float I[] = new float[9];
            boolean success = SensorManager.getRotationMatrix(R, I, mGravity, mGeomagnetic);
            if (success) {
                float mOrientation[] = new float[3];
                SensorManager.getOrientation(R, mOrientation);

                float mAzimut = (float)( Math.toDegrees( mOrientation[0]) + 0 ) % 360; // orientation contains: azimut, pitch and roll
                float mPitch = (float)( Math.toDegrees( mOrientation[2]) + 0 ) % 360;
                float mRoll = (float)( Math.toDegrees( mOrientation[1]) + 0 ) % 360;

                setAudioAngles(mAzimut, mPitch, mRoll);

                textView.setText(
                        "angles: \r\n" +
                                "Yaw: " + Integer.toString((int)mAzimut) + "\r\n" +
                                "Pitch: " + Integer.toString((int)mPitch) + "\r\n" +
                                "Roll: " + Integer.toString((int)mRoll));

                //Log.v(LOG_TAG, "SENSOR: " + mAzimut + " , " + mPitch + " , " + mRoll + " , " );

            }
        }
    }

    @Override
    protected void onDestroy() {
        //shutdownAudio();
        super.onDestroy();
    }

    protected void Play()
    {
        //Guitar-8ch.aac
        //out.aac
        //Guitar-8ch.m4a
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
