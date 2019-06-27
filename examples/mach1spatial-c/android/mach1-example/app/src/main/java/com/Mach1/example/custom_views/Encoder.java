package com.Mach1.example.custom_views;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.support.v4.content.ContextCompat;

import com.Mach1.example.R;

public class Encoder {

    float x;
    float y;
    boolean selected;
    float radiusPoint;
    Context context;

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

    public Encoder(float x, float y, Context context, boolean selected) {
        this.x = x;
        this.y = y;
        this.context = context;
        this.selected = selected;
        this.radiusPoint =  SoundMap.toPx(15, context);

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
