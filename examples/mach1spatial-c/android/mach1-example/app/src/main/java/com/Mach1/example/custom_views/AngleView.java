package com.Mach1.example.custom_views;

import android.view.animation.Animation;

public interface AngleView {

    int getAngle();
    void setAngle(int angle);
    void requestLayout();
    void startAnimation(Animation animation);
    void setOnActionEventListener(OnActionEventListener onActionEventListener);

    void setScrollView(FixedFocusScrollView sv);
}
