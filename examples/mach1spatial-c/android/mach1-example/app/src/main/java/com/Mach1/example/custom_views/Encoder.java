package com.Mach1.example.custom_views;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.support.v4.content.ContextCompat;
import android.media.MediaPlayer;
import android.net.Uri;
import android.util.Log;

import com.Mach1.Mach1Encode;
import com.Mach1.Mach1EncodeInputModeType;
import com.Mach1.Mach1DecodeAlgoType;
import com.Mach1.Mach1Point3DArray;
import com.Mach1.example.MainActivity;
import com.Mach1.example.R;

public class Encoder {

    public float volume;
    public float height;
    public float stereoSpread;
    public boolean isMono;
    public int indexSound;

    float x;
    float y;
    boolean selected;
    float radiusPoint;
    Context context;
    MediaPlayer[] players;
    int parentHeight;
    int parentWidth;

    private final static float PI = (float) Math.PI;

    private Mach1Encode m1Encode;
    private Mach1EncodeInputModeType type;

    private Paint mBasePaint,
            mDegreesPaint1,
            mDegreesPaint2,
            mCenterPaint,
            mCirclePaint,
            mCirclePaint1,
            mCirclePaint1Selected,
            mCirclePaint2,
            mCirclePaint2Selected,
            mRectPaint,
            mTextPaint,
            mTextPaint1,
            mTextPaint2;

    public boolean isSelected() {
        return selected;
    }

    public Encoder(float x, float y, int parentHeight, int parentWidth, Context context, boolean selected) {
        this.x = x;
        this.y = y;
        this.parentHeight = parentHeight;
        this.parentWidth = parentWidth;
        this.context = context;
        this.selected = selected;
        this.radiusPoint =  SoundMap.toPx(15, context);

        // default values
        this.volume = 1.0f;
        this.height = 1.0f;
        this.stereoSpread = 0.25f;

        this.isMono = true;
        this.indexSound = -1;
        this.type = Mach1EncodeInputModeType.Mach1EncodeInputModeMono;

        m1Encode = new Mach1Encode();

        mCirclePaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        mCirclePaint.setStyle(Paint.Style.FILL);
        mCirclePaint.setColor(ContextCompat.getColor(context, R.color.gray2));

        mCirclePaint1 = new Paint(Paint.ANTI_ALIAS_FLAG);
        mCirclePaint1.setStyle(Paint.Style.STROKE);
        mCirclePaint1.setColor(ContextCompat.getColor(context, R.color.white));

        mCirclePaint1Selected = new Paint(Paint.ANTI_ALIAS_FLAG);
        mCirclePaint1Selected.setStyle(Paint.Style.STROKE);
        mCirclePaint1Selected.setColor(ContextCompat.getColor(context, R.color.yellow));

        mCirclePaint2 = new Paint(Paint.ANTI_ALIAS_FLAG);
        mCirclePaint2.setStyle(Paint.Style.FILL);
        mCirclePaint2.setColor(ContextCompat.getColor(context, R.color.gray));

        mCirclePaint2Selected = new Paint(Paint.ANTI_ALIAS_FLAG);
        mCirclePaint2Selected.setStyle(Paint.Style.FILL);
        mCirclePaint2Selected.setColor(ContextCompat.getColor(context, R.color.yellow));

        //play( 0, new String[]{ "m1_sdkdemo_electronic_stereo_l", "m1_sdkdemo_electronic_stereo_r" });
    }

    public void play(int indexSound, String[] filenames) {
        if(this.indexSound != indexSound) {
            this.indexSound = indexSound;
            this.isMono = filenames.length == 1;

            type = isMono ? Mach1EncodeInputModeType.Mach1EncodeInputModeMono : Mach1EncodeInputModeType.Mach1EncodeInputModeStereo;

            stop();

            players = new MediaPlayer[isMono ? 1 : 2];
            for (int i = 0; i < players.length; i++) {
                players[i] = MediaPlayer.create(context, Uri.parse("android.resource://" + context.getPackageName() + "/raw/" + filenames[i]));
                players[i].setVolume(0,0);
                players[i].setLooping(true);
                players[i].start();
            }
        }
    }

    public void stop() {
        if(players != null) {
            for (int i = 0; i < players.length; i++) {
                players[i].stop();
            }
        }
    }

    public void update(float[] decodeArray, Mach1DecodeAlgoType decodeType) {
        float xInternal = 2 * (x / parentHeight - 0.5f);
        float yInternal = 2 * (y / parentWidth - 0.5f);

        float rotation = (float)(Math.atan2(-xInternal,yInternal) / (2 * PI) + 0.5) % 1.0f; // 0 - 1
        float diverge = (float)(Math.sqrt(Math.pow(xInternal, 2) + Math.pow(yInternal, 2)) / Math.sqrt(2)); // diagonal

        m1Encode.setRotation(rotation);
        m1Encode.setDiverge(diverge);
        m1Encode.setPitch(height);
        m1Encode.setAutoOrbit(true);
        m1Encode.setIsotropicEncode(true);
        m1Encode.setInputMode(type);
        m1Encode.setStereoSpread(stereoSpread);
        m1Encode.generatePointResults();

        //Log.v("Mach1",  "diverge: " + diverge + " , " + "rotation: " + rotation );

        //Use each coeff to decode multichannel Mach1 Spatial mix
        float[] volumes = m1Encode.getResultingVolumesDecoded(decodeType, decodeArray);

        if(players != null) {
            for (int i = 0; i < players.length; i++) {
                players[i].setVolume(volumes[2 * i + 0] * volume, volumes[2 * i + 1] * volume);
            }
        }
    }

    public float getX() {
        return x;
    }

    public float getY() {
        return y;
    }

    public void setSelected(boolean selected) {
        this.selected = selected;
    }

    public void setX(float x) {
        this.x = x;
    }

    public void setY(float y) {
        this.y = y;
    }

    public void onDraw(Canvas canvas) {

        canvas.drawCircle(x, y, radiusPoint, mCirclePaint);
        canvas.drawCircle(x, y, radiusPoint, (selected) ? mCirclePaint1Selected : mCirclePaint1);
        canvas.drawCircle(x, y, radiusPoint - SoundMap.toPx(10, context), (selected) ? mCirclePaint2Selected : mCirclePaint2);

        if(m1Encode.getPointsCount() == 2) {
            Mach1Point3DArray points = m1Encode.getPoints();

            // Further Debug functions
            /*
            Log.v("Mach1", "points names: " + m1Encode.getPointsNames());
            Log.v("Mach1", "getGains: " + m1Encode.getGains());
            Log.v("Mach1", "getPointsCount: " + m1Encode.getPointsCount());
            Log.v("Mach1", "getGainsForInputChannelNamed: " + m1Encode.getGainsForInputChannelNamed("R"));
            */

            canvas.drawCircle(x + (points.getitem(0).getZ()-0.5f) * parentWidth, y + (1-points.getitem(0).getX()-0.5f) * parentHeight, radiusPoint * 0.25f, (selected) ? mCirclePaint1Selected : mCirclePaint1);
            canvas.drawCircle(x + (points.getitem(1).getZ()-0.5f) * parentWidth, y + (1-points.getitem(1).getX()-0.5f) * parentHeight, radiusPoint * 0.25f, (selected) ? mCirclePaint1Selected : mCirclePaint1);

            points.delete();
        }
    }

    public boolean isEncoderExist(float pointX, float pointY) {
        float left = getX() - radiusPoint;
        float top = getY() - radiusPoint;
        float right = getX() + radiusPoint;
        float bottom = getY() + radiusPoint;

        if (pointX >= left && pointX <= right && pointY >= top && pointY <= bottom) {
            return true;
        }
        return false;
    }

}
