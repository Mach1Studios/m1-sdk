package com.Mach1.example;

import com.Mach1.*;

import com.Mach1.example.animations.ArcAngleAnimation;
import com.Mach1.example.custom_views.AngleView;
import com.Mach1.example.custom_views.FixedFocusScrollView;

import android.animation.ValueAnimator;
import android.content.Context;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.LinearLayout;
import android.widget.SeekBar;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.view.Surface;

public class MainActivity extends AppCompatActivity implements SensorEventListener {

    private AngleView arcView;
    private AngleView mainView;
    private LinearLayout llMainContainer;

    private FixedFocusScrollView sv;
    boolean isBlockedScrollView;
    boolean isGoneFromChart = false;

    private SeekBar sbRoll;
    private SeekBar sbPitch;

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
        Mach1Decode mach1Decode = new Mach1Decode();
        mach1Decode.setDecodeAlgoType(Mach1DecodeAlgoType.Mach1DecodeAlgoAltSpatial);

        mach1Decode.beginBuffer();
        float flt[] = new float[18];
        mach1Decode.decode(40, 100, 30, flt, 0, 0);
        mach1Decode.endBuffer();

        mach1Decode.beginBuffer();
        flt = new float[18];
        mach1Decode.decode(40, 100, 30, flt, 0, 0);
        mach1Decode.endBuffer();

        mach1Decode.beginBuffer();
        flt = new float[18];
        mach1Decode.decode(40, 100, 30, flt, 0, 0);
        mach1Decode.endBuffer();


        mach1Decode.beginBuffer();
        flt = new float[18];
        mach1Decode.decode(40, 100, 30, flt, 0, 0);
        mach1Decode.endBuffer();


        Mach1Point3D point = mach1Decode.getCurrentAngle();
        float x = point.getX();
        float y = point.getY();
        float z = point.getZ();

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        arcView = findViewById(R.id.round_chart_view);
        mainView = findViewById(R.id.main_chart_view);

        sv = findViewById(R.id.sv);
        mainView.setScrollView(sv);

        sbPitch = (SeekBar)findViewById(R.id.sbPitch);
        sbPitch.setEnabled(false);

        sbRoll = (SeekBar)findViewById(R.id.sbRoll);
        sbRoll.setEnabled(false);

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
        demonAnimationAngle(mainView);
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

            SensorManager.getRotationMatrixFromVector(rotMatrix, event.values);

            int screenRotation =  this.getWindowManager().getDefaultDisplay().getRotation();
            int axisX = SensorManager.AXIS_X;
            int axisY = SensorManager.AXIS_Y;

            switch (screenRotation) {
                case Surface.ROTATION_0:
                    axisX = SensorManager.AXIS_X;
                    axisY = SensorManager.AXIS_Y;
                    break;
                case Surface.ROTATION_90:
                    axisX = SensorManager.AXIS_Y;
                    axisY = SensorManager.AXIS_MINUS_X;
                    break;
                case Surface.ROTATION_180:
                    axisX = SensorManager.AXIS_MINUS_X;
                    axisY = SensorManager.AXIS_MINUS_Y;
                    break;
                case Surface.ROTATION_270:
                    axisX = SensorManager.AXIS_MINUS_Y;
                    axisY = SensorManager.AXIS_X;
                    break;
                default:
                    break;
            }

            SensorManager.remapCoordinateSystem(rotMatrix, axisX, axisY, rotMatrix);
            SensorManager.getOrientation(rotMatrix, rotVals);

            yaw = (float) Math.toDegrees(rotVals[0]);
            pitch = (float) Math.toDegrees(rotVals[1]);
            roll = (float) Math.toDegrees(rotVals[2]);

            sbPitch.setProgress((int)(map(pitch, -90, 90, 0, 100)));
            sbRoll.setProgress((int)(map(roll, -180, 180, 0, 100)));

            Log.v("MYTAG",  "yaw: " + yaw + " , " + "pitch: " + pitch + " , " + "roll: " + roll);
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
        mainView.requestLayout();
        sensorManager.registerListener(this, rotationSensor, SensorManager.SENSOR_DELAY_GAME);
    }
}
