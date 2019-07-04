package com.Mach1.example;

import com.Mach1.*;
import com.Mach1.Mach1DecodeAlgoType;

import com.Mach1.example.custom_views.AngleView;
import com.Mach1.example.custom_views.SoundList;
import com.Mach1.example.custom_views.SoundMap;
import com.Mach1.example.custom_views.YawView;
import com.Mach1.example.custom_views.FixedFocusScrollView;
import com.Mach1.example.custom_views.OnSoundListSelectedItemChangedListener;

import android.content.Context;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import android.widget.LinearLayout;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.TextView;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

public class MainActivity extends AppCompatActivity implements SensorEventListener, OnSeekBarChangeListener {

    private Mach1Decode m1Decode;

    private AngleView arcView;
    private SoundMap soundMap;
    private LinearLayout llMainContainer;
    private SoundList soundList;

    private FixedFocusScrollView sv;
    boolean isBlockedScrollView;
    boolean isGoneFromChart = false;

    private SeekBar sbRoll;
    private TextView sbRollText;
    private SeekBar sbPitch;
    private TextView sbPitchText;
    private YawView yawView;

    private SeekBar sbVolume;
    private SeekBar sbHeight;
    private SeekBar sbStereo;

    private SensorManager sensorManager;
    private Sensor rotationSensor;

    private float yaw;
    private float pitch;
    private float roll;

    private final static float PI = (float) Math.PI;
    private final static float TWO_PI = PI*2;
    private Context context;

    public static String[][] audioFiles = {
            { "nature_mono01" },
            { "nature_mono02" },
            { "nature_mono03" },
            { "scifi_mono01" },
            { "scifi_mono02" },
            { "scifi_mono03" },
            { "scifi_mono01" },
            { "m1_sdkdemo_electronic_stereo_l", "m1_sdkdemo_electronic_stereo_r" },
            { "m1_sdkdemo_orchestral_stereo_l", "m1_sdkdemo_orchestral_stereo_r" },
    };

    public static int selectedSoundItem = 0;

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
        soundMap = (SoundMap)findViewById(R.id.main_chart_view);

        sv = findViewById(R.id.sv);
        soundMap.setScrollView(sv);

        // angles
        sbPitch = (SeekBar)findViewById(R.id.sbPitch);
        sbPitch.setEnabled(false);
        sbPitchText = (TextView) findViewById(R.id.sbPitchText);

        sbRoll = (SeekBar)findViewById(R.id.sbRoll);
        sbRoll.setEnabled(false);
        sbRollText = (TextView) findViewById(R.id.sbRollText);

        yawView = (YawView)findViewById(R.id.round_chart_view);

        // sound list
        soundList = (SoundList)findViewById(R.id.sound_list);
        soundList.setSoundListSelectedItemChangedListener(new OnSoundListSelectedItemChangedListener() {
            @Override
            public void OnSoundListSelectedItemChanged(int idx) {
                if(soundMap.selectedEncoder != null) {
                    soundMap.selectedEncoder.play(idx, audioFiles[idx]);
                }
            }
        });

        llMainContainer = findViewById(R.id.main_container);

        sbVolume = (SeekBar)findViewById(R.id.sbVolume);
        sbVolume.setOnSeekBarChangeListener(this);

        sbHeight = (SeekBar)findViewById(R.id.sbHeight);
        sbHeight.setOnSeekBarChangeListener(this);

        sbStereo = (SeekBar)findViewById(R.id.sbStereo);
        sbStereo.setOnSeekBarChangeListener(this);


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
    public void onStopTrackingTouch(SeekBar seekBar) {
        // TODO Auto-generated method stub
    }

    @Override
    public void onStartTrackingTouch(SeekBar seekBar) {
        // TODO Auto-generated method stub
    }

    @Override
    public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
        if(soundMap.selectedEncoder != null) {
            if(seekBar == sbVolume) {
                soundMap.selectedEncoder.volume = 1.0f * progress / 100;
            }
            else  if(seekBar == sbHeight) {
                soundMap.selectedEncoder.height = 1.0f * progress / 100;
            }
            else  if(seekBar == sbStereo) {
                soundMap.selectedEncoder.stereoSpread = 1.0f * progress / 100;
            }
        }
    }

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
            sbPitchText.setText("" + (int)pitch + "º");

            sbRoll.setProgress((int)(map(roll, -90, 90, 0, 100)));
            sbRollText.setText("" + (int)roll + "º");

            m1Decode.beginBuffer();
            float decoded[] = new float[18];
            m1Decode.decode(yaw, pitch, roll, decoded, 0, 0);
            m1Decode.endBuffer();

            soundMap.update(decoded, Mach1DecodeAlgoType.Mach1DecodeAlgoSpatial);

            if(soundMap.selectedEncoder != null) {
                sbVolume.setProgress((int)(soundMap.selectedEncoder.volume * 100));
                sbHeight.setProgress((int)(soundMap.selectedEncoder.height * 100));
                sbStereo.setProgress((int)(soundMap.selectedEncoder.stereoSpread * 100));

                sbStereo.setEnabled(!soundMap.selectedEncoder.isMono);

                soundList.selectIndex(soundMap.selectedEncoder.indexSound);
            }

            // Log.v("Mach1",  "yaw: " + yaw + " , " + "pitch: " + pitch + " , " + "roll: " + roll);
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
