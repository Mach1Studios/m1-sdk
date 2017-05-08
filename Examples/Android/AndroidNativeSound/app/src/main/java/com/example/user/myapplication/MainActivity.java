package com.example.user.myapplication;

import android.app.Activity;
import android.content.res.AssetFileDescriptor;
import android.os.Bundle;
import android.os.Process;
import android.widget.TextView;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.ShortBuffer;
import java.lang.Math;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.media.MediaCodec;
import android.media.MediaCodec.BufferInfo;
import android.media.MediaCodecInfo;
import android.media.MediaExtractor;
import android.media.MediaFormat;
import android.util.Log;


import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageView;

import android.content.res.AssetManager;

import android.content.pm.PackageManager;
import android.media.AudioManager;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import android.app.Activity;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.opengl.GLSurfaceView;
import android.os.Bundle;

public class MainActivity extends Activity implements SensorEventListener {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private static final String LOG_TAG = "MyTag";
    private SensorManager mSensorManager;
    private Sensor mAccelerometer;
    private Sensor mMagnetometer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        AudioManager am = (AudioManager) getSystemService(AUDIO_SERVICE);
        String framesPerBuffer = am.getProperty(AudioManager.PROPERTY_OUTPUT_FRAMES_PER_BUFFER);
        int framesPerBufferInt = Integer.parseInt(framesPerBuffer);
        if (framesPerBufferInt == 0) framesPerBufferInt = 1024; // Use default
        initAudio(framesPerBufferInt * 2);

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
            }
        });

        ((Button) findViewById(R.id.buttonStop)).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Stop();
            }
        });
    }
    /*
        @Override
        public void onSensorChanged(SensorEvent event) {
            if (event.sensor.getType() == Sensor.TYPE_MAGNETIC_FIELD)
                mMagnetometer = event.values;
            if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER)
                mGravity = event.values;
            if ((mGravity == null) || (mMagnetometer == null))
                return;

            float R[] = new float[9];
            float I[] = new float[9];
            boolean success = SensorManager.getRotationMatrix(R, I, mGravity, mMagnetometer);
            if (success) {
                mOrientation = new float[3];
                mQuaternion = new float[4];
                SensorManager.getOrientation(R, mOrientation);
                mAzimut = mOrientation[0]; // orientation contains: azimut, pitch and roll
                mPitch = mOrientation[1];
                mRoll = mOrientation[2];

                SensorManager.getQuaternionFromVector(mQuaternion, mOrientation);

                if(mAzimut - mQuaternion[1] == 0 && mPitch - mQuaternion[2] == 0 && mRoll - mQuaternion[3] == 0){
                    Log.d("ORIENTATION", "QUATERNION IS THE SAME");
                }
                notifyObservers();
            }
        }
    */
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
                float mPitch = (float)( Math.toDegrees( mOrientation[1]) + 0 ) % 360;
                float mRoll = (float)( Math.toDegrees( mOrientation[2]) + 0 ) % 360;

                setAudioAngles(mAzimut, mPitch, mRoll);

                Log.v(LOG_TAG, "SENSOR: " + mAzimut + " , " + mPitch + " , " + mRoll + " , " );

            }
        }

        /*
        i
        f (event.sensor.getType() == Sensor.TYPE_ROTATION_VECTOR) {


            float R[] = new float[9];
            float mOrientation[] = new float[3];
            SensorManager.getOrientation(R, mOrientation);
            float mAzimut = mOrientation[0]; // orientation contains: azimut, pitch and roll
            float mPitch = mOrientation[1];
            float mRoll = mOrientation[2];

            Log.v(LOG_TAG, "SENSOR: " + mAzimut + " , " + mPitch + " , " + mRoll + " , " );
        }
        */
    }

    @Override
    protected void onDestroy() {
        shutdownAudio();
        super.onDestroy();
    }

    protected void Play()
    {
        //Guitar-8ch.aac
        //out.aac
        //Guitar-8ch.m4a

        playAudio(getAssets(), "Guitar-8ch.aac");
/*
        new Thread(new Runnable() {
            @Override

            public void run() {

                 android.os.Process.setThreadPriority(Process.THREAD_PRIORITY_AUDIO);//.THREAD_PRIORITY_URGENT_AUDIO);
            //    setPriority(Thread.MAX_PRIORITY);

                mShouldContinue = true;

                mAudioTrack.play();

                Log.v(LOG_TAG, "Audio streaming started");

                short[] buffer = new short[mBufferSize];
                long totalWritten = 0;
                //while (mShouldContinue) {
                double phase = (2.0*Math.PI) * (440.0/44100.0);

                while (mAudioTrack.getPlayState() != AudioTrack.PLAYSTATE_STOPPED) {
                    Log.v(LOG_TAG, "totalWritten: " + totalWritten);

                    // Short.MAX_VALUE

                     for (int i = 0; i < mBufferSize/CHANNELS; i++) {
                        buffer[i*2] =  (short)(Math.sin(phase * totalWritten) * Short.MAX_VALUE);
                        buffer[i*2+1] = 0;
                        totalWritten++;
                    }
                    //    totalWritten += mBufferSize;
                    mAudioTrack.write(buffer, 0, mBufferSize);

                }


                if (!mShouldContinue) {
                 //   mAudioTrack.release();
                }


                Log.v(LOG_TAG, "Audio streaming finished. Samples written: " + totalWritten);
            }
        }).start();
        */
    }

    protected void Stop()
    {
        stopAudio();
/*
        mAudioTrack.stop();
        mShouldContinue = false;
        */
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
