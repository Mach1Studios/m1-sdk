package com.Mach1.example.custom_views;

import android.content.Context;
import android.content.res.Resources;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.RectF;
import android.graphics.Typeface;
import android.support.annotation.Nullable;
import android.support.v4.content.ContextCompat;
import android.util.AttributeSet;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.animation.Animation;

import java.util.ArrayList;
import java.util.List;

import com.Mach1.example.R;

public class SoundMap extends View implements AngleView {

    private int STROKE_WIDTH_1 = 5;
    private Paint mBasePaint,
            mDegreesPaint1,
            mDegreesPaint2,
            mCenterPaint,
            mRectPaint,
            mTextPaint,
            mTextPaint1,
            mTextPaint2;

    float textSizeSmall = 13;
    float textSizeMidle = 20;
    int wight;
    int height;
    List<Encoder> listEncoders = new ArrayList<>();
    OnActionEventListener onActionEventListener;
    FixedFocusScrollView fixedFocusScrollView;

    public void setOnActionEventListener(OnActionEventListener onActionEventListener) {
        this.onActionEventListener = onActionEventListener;
    }

    @Override
    public void setScrollView(FixedFocusScrollView sv) {
        fixedFocusScrollView = sv;
    }

    int angle = -70;

    public void setAngle(int angle) {
        this.angle = angle;
        invalidate();
    }

    public int getAngle() {
        return angle;
    }

    public SoundMap(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    public void invalidateAngleText(int angle) {
        this.angle = angle;
        invalidate();
    }

    int countLines = 50;
    float next = 0;
    float radius = 0;
    float radius1 = 0;
    float centerX;
    float centerY;

    @Override
    protected void onDraw(Canvas canvas) {

        super.onDraw(canvas);

        float a = (Math.min(getMeasuredWidth(), getMeasuredWidth())) * 0.60f;

        wight = getMeasuredWidth();
        height = getMeasuredHeight();
        radius = (float) ((height / 2));
        radius1 = (float) ((height / 2) * 0.47);
        next = 0;

        centerX = wight / 2;
        centerY = height / 2;

        float step = wight / countLines;
        float step1 = height / countLines;

        for (int i = 0; i <= countLines + 1; i++) {

            canvas.drawLine(next, 0, next, height, mBasePaint);
            canvas.drawLine(0, next, wight, next, mBasePaint);
            next += step;

        }

        step = wight / 3;
        next = step;

        for (int i = 0; i <= 3 + 1; i++) {

            canvas.drawLine(next, 0, next, height, mBasePaint);
            canvas.drawLine(0, next, wight, next, mBasePaint);
            next += step;
        }

        canvas.drawLine(0, 0, wight, height, mCenterPaint);
        canvas.drawLine(wight, 0, 0, height, mCenterPaint);
        canvas.drawCircle(wight / 2, height / 2, radius, mCenterPaint);
        canvas.drawCircle(wight / 2, height / 2, radius1, mCenterPaint);

        radius = toPx(50, getContext());

        RectF rect = new RectF(centerX - radius, centerY - radius, centerX + radius, centerY + radius);
        canvas.drawArc(rect, 250 - angle, 40, true, mRectPaint);

        //drawPoints

        for (Encoder encoder : listEncoders) {
            encoder.onDraw(canvas);
        }

        //Rect rect = new Rect(0,0, wight, height);
        //canvas.drawRect(rect, mRectPaint);
    }

    private void init() {


        mRectPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        mRectPaint.setColor(ContextCompat.getColor(getContext(), R.color.gray));
        mRectPaint.setStrokeWidth(6);
        mRectPaint.setStyle(Paint.Style.STROKE);

        mCenterPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        mCenterPaint.setColor(ContextCompat.getColor(getContext(), R.color.gray2));
        mCenterPaint.setStyle(Paint.Style.STROKE);
        mCenterPaint.setStrokeWidth(toPx(1, getContext()));

        //Основной круг - background
        mBasePaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        mBasePaint.setStyle(Paint.Style.FILL);
        mBasePaint.setColor(ContextCompat.getColor(getContext(), R.color.gray2));

        //Дуга над основным кругом - background
        mDegreesPaint1 = new Paint(Paint.ANTI_ALIAS_FLAG);
        mDegreesPaint1.setStyle(Paint.Style.STROKE);
        mDegreesPaint1.setStrokeWidth(toPx(STROKE_WIDTH_1, getContext()));
        mDegreesPaint1.setColor(ContextCompat.getColor(getContext(), R.color.yellow));

        mDegreesPaint1.setStrokeCap(Paint.Cap.ROUND);

        //Дуга над основным кругом - foreground
        mDegreesPaint2 = new Paint(Paint.ANTI_ALIAS_FLAG);
        mDegreesPaint2.setStyle(Paint.Style.STROKE);
        mDegreesPaint2.setStrokeWidth(toPx(STROKE_WIDTH_1, getContext()));

        mDegreesPaint2.setColor(ContextCompat.getColor(getContext(), R.color.blue1));
        mDegreesPaint2.setStrokeCap(Paint.Cap.ROUND);

        mTextPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        mTextPaint.setColor(ContextCompat.getColor(getContext(), R.color.yellow));

        mTextPaint.setTextSize(toPx(textSizeSmall, getContext()));
        mTextPaint.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.BOLD));

        mTextPaint.setTextAlign(Paint.Align.CENTER);
        mTextPaint.setStyle(Paint.Style.FILL);

        mTextPaint1 = new Paint(Paint.ANTI_ALIAS_FLAG);
        mTextPaint1.setColor(ContextCompat.getColor(getContext(), R.color.blue3));

        mTextPaint1.setTextSize(toPx(textSizeSmall, getContext()));
        mTextPaint1.setTextAlign(Paint.Align.CENTER);
        mTextPaint1.setStyle(Paint.Style.FILL);

        mTextPaint2 = new Paint(Paint.ANTI_ALIAS_FLAG);

        mTextPaint2.setColor(ContextCompat.getColor(getContext(), R.color.blue3));

        mTextPaint2.setTextSize(toPx(textSizeMidle, getContext()));
        mTextPaint2.setTextAlign(Paint.Align.CENTER);
        mTextPaint2.setStyle(Paint.Style.FILL);

    }

    public static float toPx(float dp, Context context) {
        Resources resources = context.getResources();
        DisplayMetrics metrics = resources.getDisplayMetrics();
        float px = dp * ((float) metrics.densityDpi / DisplayMetrics.DENSITY_DEFAULT);
        return px;
    }

    @Override
    public void startAnimation(Animation animation) {
        super.startAnimation(animation);
    }

    boolean firstTouch = false;
    long time = 0;

    Encoder isEncoderExist(float x, float y) {
        for (Encoder encoder : listEncoders) {
            if ( encoder.isEncoderExist(x , y)) {
                return encoder;
            }
        }
        return null;
    }

    void resetPointSelected() {
        for (Encoder encoder : listEncoders) {
            encoder.setSelected(false);
        }
    }

    private boolean isActivatedActionMove = false;
    private long timeDown = 0;
    private boolean isPointSelected = false;
    Encoder selectedEncoder = null;
    Encoder newEncoder;

    @Override
    public boolean onTouchEvent(MotionEvent event) {

        if (event.getAction() == event.ACTION_MOVE) {

            isActivatedActionMove = true;

            fixedFocusScrollView.setEnableScrolling(false);
            if (firstTouch) {
                Log.d("ffff", "x:" + event.getX() + ", y:" + event.getY());

                if (selectedEncoder != null) {
                    selectedEncoder.setX(event.getX());
                    selectedEncoder.setY(event.getY());

                    invalidate();
                }
            }

        } else if (event.getAction() == event.ACTION_DOWN) {

            fixedFocusScrollView.setEnableScrolling(false);
            isActivatedActionMove = false;
            timeDown = System.currentTimeMillis();
            long aa = System.currentTimeMillis() - time;

            selectedEncoder = isEncoderExist(event.getX(), event.getY());

            if (firstTouch && aa <= 300) {
                //do stuff here for double tap
                firstTouch = false;
                resetPointSelected();

                if (selectedEncoder == null) {
                    newEncoder = new Encoder(event.getX(), event.getY(), getContext(), true);
                    listEncoders.add(newEncoder);

                } else {

                    selectedEncoder.setSelected(true);
                }

                invalidate();

            } else {

                if (selectedEncoder != null) {
                    resetPointSelected();
                    selectedEncoder.setSelected(true);
                }

                firstTouch = true;
                time = System.currentTimeMillis();
                invalidate();
            }
        } else if (event.getAction() == event.ACTION_UP) {

            fixedFocusScrollView.setEnableScrolling(true);

            long aa = System.currentTimeMillis() - timeDown;
            if (aa > 500 && !isActivatedActionMove) {

                Encoder myEvent1 = isEncoderExist(event.getX(), event.getY());
                if (myEvent1 != null) {
                    listEncoders.remove(myEvent1);
                    invalidate();
                }
            }

            isActivatedActionMove = false;

            Log.d("log", "aa1" + aa);
        }

        return true;
    }


    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
    }


}
