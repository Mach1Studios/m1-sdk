package com.Mach1.example;

import com.Mach1.*;
import com.Mach1.Mach1DecodeAlgoType;

import com.Mach1.example.custom_views.AngleView;
import com.Mach1.example.custom_views.SoundMap;
import com.Mach1.example.custom_views.YawView;
import com.Mach1.example.custom_views.FixedFocusScrollView;

import android.content.Context;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.LinearLayout;
import android.widget.SeekBar;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

public class MainActivity extends AppCompatActivity implements SensorEventListener {

    private Mach1Decode m1Decode;

    private AngleView arcView;
    private SoundMap soundMap;
    private LinearLayout llMainContainer;

    private FixedFocusScrollView sv;
    boolean isBlockedScrollView;
    boolean isGoneFromChart = false;

    private SeekBar sbRoll;
    private SeekBar sbPitch;
    private YawView yawView;

    private SensorManager sensorManager;
    private Sensor rotationSensor;

    private float yaw;
    private float pitch;
    private float roll;

    private final static float PI = (float) Math.PI;
    private final static float TWO_PI = PI*2;
    private Context context;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


        m1Decode = new Mach1Decode();
        //Mach1 Decode Setup
        //Setup the correct angle convention for orientation Euler input angles
        //m1Decode.setPlatformType(Mach1PlatformType.Mach1PlatformAndroid);
        //Setup the expected spatial audio mix format for decoding
        m1Decode.setDecodeAlgoType(Mach1DecodeAlgoType.Mach1DecodeAlgoAltSpatial);
        //Setup for the safety filter speed:
        //1.0 = no filter | 0.1 = slow filter
        m1Decode.setFilterSpeed(1.0f);


        arcView = findViewById(R.id.round_chart_view);
        soundMap = findViewById(R.id.main_chart_view);

        sv = findViewById(R.id.sv);
        soundMap.setScrollView(sv);

        sbPitch = (SeekBar)findViewById(R.id.sbPitch);
        sbPitch.setEnabled(false);

        sbRoll = (SeekBar)findViewById(R.id.sbRoll);
        sbRoll.setEnabled(false);

        yawView = (YawView) findViewById(R.id.round_chart_view);

        llMainContainer = findViewById(R.id.main_container);

        /*
        ValueAnimator anim = ValueAnimator.ofInt(0, sbPitch.getMax());
        anim.setDuration(1000);
        anim.addUpdateListener(new ValueAnimator.AnimatorUpdateListener() {
            @Override
            public void onAnimationUpdate(ValueAnimator animation) {
                int animProgress = (Integer) animation.getAnimatedValue();
                sbPitch.setProgress(animProgress);
                sbRoll.setProgress(animProgress);
            }
        });
        anim.start();

        demonAnimationAngle(arcView);
        demonAnimationAngle(soundMap);
        */

        sensorManager = (SensorManager)getSystemService(SENSOR_SERVICE);
        rotationSensor = sensorManager.getDefaultSensor(Sensor.TYPE_GAME_ROTATION_VECTOR);
    }

    /*
    void demonAnimationAngle(AngleView angleView){
        angleView.setAngle(0);
        ArcAngleAnimation animation = new ArcAngleAnimation(angleView, -90);
        animation.setDuration(1000);
        angleView.startAnimation(animation);
    }
    */

    @Override
    public final void onAccuracyChanged(Sensor sensor, int accuracy) {

    }

    public float map(float input, float input_start, float input_end, float output_start, float output_end)
    {
        float input_range = input_end - input_start;
        float output_range = output_end - output_start;
        return (input - input_start)*output_range / input_range + output_start;
    }

    private double mod(double a, double b){ //functions calculates the mod
        return a % b;
    }

    @Override
    public final void onSensorChanged(SensorEvent event) {
        if(event.sensor.getType() ==  Sensor.TYPE_GAME_ROTATION_VECTOR){
            float[] rotMatrix = new float[9];
            float[] rotVals = new float[3];
            float[] remapedMatrix = new float[9];

            SensorManager.getRotationMatrixFromVector(rotMatrix, event.values);
            SensorManager.remapCoordinateSystem(rotMatrix, SensorManager.AXIS_X, SensorManager.AXIS_Z, remapedMatrix);
            SensorManager.getOrientation(remapedMatrix, rotVals);

            yaw = (float) Math.toDegrees(rotVals[0]);
            pitch = (float) Math.toDegrees(rotVals[1]);
            roll = (float) Math.toDegrees(rotVals[2]);

            soundMap.setAngle((int)yaw);
            yawView.setAngle((int)yaw);
            sbPitch.setProgress((int)(map(pitch, -90, 90, 0, 100)));
            sbRoll.setProgress((int)(map(roll, -90, 90, 0, 100)));

            m1Decode.beginBuffer();
            float decoded[] = new float[18];
            m1Decode.decode(yaw, pitch, roll, decoded, 0, 0);
            m1Decode.endBuffer();

            soundMap.update(decoded, Mach1DecodeAlgoType.Mach1DecodeAlgoAltSpatial);
            // Log.v("MYTAG",  "yaw: " + yaw + " , " + "pitch: " + pitch + " , " + "roll: " + roll);
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        sensorManager.unregisterListener(this);
    }

    @Override
    protected void onResume() {
        super.onResume();
        soundMap.requestLayout();
        sensorManager.registerListener(this, rotationSensor, SensorManager.SENSOR_DELAY_GAME);
    }
}
