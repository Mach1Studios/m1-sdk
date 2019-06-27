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
import android.view.View;
import android.view.animation.Animation;

import com.Mach1.example.R;

public class YawView extends View implements AngleView {

    private int STROKE_WIDTH = 0;
    private int STROKE_WIDTH_1 = 5;
    private Paint mBasePaint,
            mDegreesPaint,
            mDegreesPaint1,
            mDegreesPaint2,
            mCenterPaint,
            mRectPaint,
            mTextPaint,
            mTextPaint1,
            mTextPaint2;

    private RectF mRect;
    private RectF mRect1;
    private float centerX, centerY, radius, radius1;

    float textSizeBig = 34;
    float textSizeSmall = 13;
    float textSizeMidle = 20;
    int longLineSizeDP = 10;
    int shortLinetSizeDP = 5;

    int deepBreathValue;
    int angle = -70;

    public void setAngle(int angle) {

        this.angle = angle;
        invalidate();
    }

    public int getAngle(){

        return angle;
    }

    public void setDeepBreathValue(int deepBreathValue) {
        this.deepBreathValue = deepBreathValue;
        invalidate();
    }


    public YawView(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    public void invalidateAngleText(int angle) {

        this.angle = angle;
        invalidate();
    }

    @Override
    protected void onDraw(Canvas canvas) {

        float a = (Math.min(getMeasuredWidth(), getMeasuredWidth())) *0.60f;

        centerX = getMeasuredWidth() / 2;
        centerY = getMeasuredWidth() / 2;

        radius = (Math.min(centerX, centerY))* 0.55f;
        radius1 = (Math.min(centerX, centerY)) * 0.50f;

        float startTop = centerY - radius * 1.2f;
        float startLeft = centerX -radius * 1.2f;
        float endBottom = centerY + radius * 1.2f;
        float endRight = centerX + radius * 1.2f;

        mRect = new RectF(startTop, startLeft, endRight, endBottom);
        canvas.drawCircle(centerX, centerY, radius, mBasePaint);
        //canvas.drawRect(mRect, mBasePaint);

        canvas.drawLine(centerX, centerY - radius, centerX, centerY - radius + toPx(longLineSizeDP, getContext()), mCenterPaint);
        canvas.drawLine(centerX, centerY + radius, centerX, centerY + radius - toPx(longLineSizeDP, getContext()), mCenterPaint);

        canvas.drawLine(centerX - radius, centerY, centerX - radius + toPx(longLineSizeDP, getContext()), centerY, mCenterPaint);
        canvas.drawLine(centerX + radius, centerY, centerX + radius - toPx(longLineSizeDP, getContext()), centerY, mCenterPaint);

        //scanvas.drawArc(mRect, 270, timeValue, false, mDegreesPaint);
        // canvas.drawRect(0, 0, getMeasuredWidth(), getMeasuredHeight(), mRectPaint);
        //canvas.drawRect(mRect1, mRectPaint);
        /*rx = x0 - x;
        ry = y0 - y;
        c = Math.cos(alpha);
        s = Math.sin(alpha);
        x1 = x + rx * c - ry * s;
        y1 = y + rx * s + ry * c;*/

        float x0 = centerX;
        float y0 = centerY;

        float x = 0;
        float y = 0;

        float x1 = 0;
        float y1 = 0;

        for (int i = 0; i < 32; i++) {

            if (i == 0 || i == 8 || i == 16 || i==24 || i == 32) continue;

            float angleRad = (11.25f * i + 1) * 3.14f / 180;

            x = (float) (x0 + radius * Math.cos(angleRad));
            y = (float) (y0 + radius * Math.sin(angleRad));

            x1 = (float) (x0 + radius1 * Math.cos(angleRad));
            y1 = (float) (y0 + radius1 * Math.sin(angleRad));

            canvas.drawLine(x1, y1, x, y, mCenterPaint);
        }

        canvas.drawArc(mRect, 270, angle, false, mDegreesPaint1);
        //canvas.drawArc(mRect1, 270, deepBreathValue, false, mDegreesPaint2);
        //+10 т.к. мы хотим, чтобы надпись вдох была по центру относительно своего центра
        canvas.drawText(angle+"º", centerX, centerY , mTextPaint);
        // canvas.drawText(timeText, centerX, centerY - toPx(40, getContext()), mTextPaint2);
        //canvas.drawText("Низкая", centerX - radius1, centerY - radius1 / 1.3f, mTextPaint1);
        //canvas.drawText("Отличная", centerX + radius1, centerY - radius1 / 1.3f, mTextPaint1);
        //canvas.drawText("Хорошая", centerX, startTop1 - 30, mTextPaint1);
        //canvas.drawText("Глубина дыхания", centerX , startTop1  -50, mTextPaint2);

        super.onDraw(canvas);
    }

    private void init() {

        mRectPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        mRectPaint.setColor(ContextCompat.getColor(getContext(), R.color.green));
        mRectPaint.setStrokeWidth(2);
        mRectPaint.setStyle(Paint.Style.STROKE);

        mCenterPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        mCenterPaint.setColor(ContextCompat.getColor(getContext(), R.color.white));
        mCenterPaint.setStrokeWidth(toPx(2, getContext()));

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

    @Override
    public void setOnActionEventListener(OnActionEventListener onActionEventListener) {

    }

    @Override
    public void setScrollView(FixedFocusScrollView sv) {

    }
}
