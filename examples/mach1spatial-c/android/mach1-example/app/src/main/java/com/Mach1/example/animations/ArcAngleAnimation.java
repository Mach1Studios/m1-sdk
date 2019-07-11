package com.Mach1.example.animations;
import com.Mach1.example.custom_views.AngleView;

import android.view.animation.Animation;
import android.view.animation.Transformation;

public class ArcAngleAnimation extends Animation {

    AngleView roundChartView;

    private float oldAngle;
    private float newAngle;

    public ArcAngleAnimation(AngleView roundChartView, float newAngle) {

        this.roundChartView = roundChartView;
        this.oldAngle = roundChartView.getAngle();
        this.newAngle = newAngle;
    }

    @Override
    protected void applyTransformation(float interpolatedTime, Transformation transformation) {

        float angle = 0 + ((newAngle - oldAngle) * interpolatedTime);

        roundChartView.setAngle((int)angle);
        roundChartView.requestLayout();
    }
}
