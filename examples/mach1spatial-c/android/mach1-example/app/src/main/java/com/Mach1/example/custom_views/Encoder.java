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
import com.Mach1.example.MainActivity;
import com.Mach1.example.R;

public class Encoder {

    public float volume;
    public float height;
    public float stereoSpread;

    float x;
    float y;
    boolean selected;
    float radiusPoint;
    Context context;
    MediaPlayer[] players;
    int parentHeight;
    int parentWidth;
    boolean isMono;

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

        this.volume = 1.0f;
        this.height = 0.5f;
        this.stereoSpread = 1.0f;

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

        play(  new String[]{ "m1_sdkdemo_electronic_stereo_l", "m1_sdkdemo_electronic_stereo_r" });
    }

    public void play(String[] filenames) {

        this.isMono = filenames.length == 1;

        type = isMono ? Mach1EncodeInputModeType.Mach1EncodeInputModeMono : Mach1EncodeInputModeType.Mach1EncodeInputModeStereo;

        stop();

        players = new MediaPlayer[2];

        players[0] = MediaPlayer.create(context, Uri.parse("android.resource://" + context.getPackageName() + "/raw/" + (isMono ? filenames[0] : filenames[0])));
        players[0].setVolume(0,0);
        players[0].setLooping(true);
        players[0].start();

        players[1] = MediaPlayer.create(context, Uri.parse("android.resource://" + context.getPackageName() + "/raw/" + (isMono ? filenames[0] : filenames[1])));
        players[1].setVolume(0,0);
        players[1].setLooping(true);
        players[1].start();
    }

    public void stop() {
        if(players != null) {
            players[0].stop();
            players[1].stop();
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

        //Use each coeff to decode multichannel Mach1 Spatial mix
        float[] volumes = m1Encode.getResultingVolumesDecoded(decodeType, decodeArray);

        for (int i = 0; i < players.length; i++) {
            players[i].setVolume(volumes[2*i+0] * volume,volumes[2*i+1] * volume);
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
