package com.Mach1.example.custom_views;

import android.content.Context;
import android.util.AttributeSet;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.ImageView;
import android.widget.LinearLayout;

import com.Mach1.example.R;

public class SoundList extends LinearLayout implements View.OnClickListener {

    public int selectedItemIndex = 0;

    View selectedItem = null;

    LinearLayout llListOfImage;
    int countChild;
    OnSoundListSelectedItemChangedListener listener;

    public SoundList(Context context, AttributeSet attrs) {
        super(context, attrs);

        LayoutInflater layoutInflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        View view = layoutInflater.inflate(R.layout.sound_list_layout, this, true);

        llListOfImage = (LinearLayout) view.findViewById(R.id.ll_sound_list);
        countChild = llListOfImage.getChildCount();

        for (int i=0; i < countChild; i++){
            View viewChild = llListOfImage.getChildAt(i);
            if (viewChild instanceof ImageView){
                viewChild.setTag(i);
                viewChild.setOnClickListener(this);
            }
        }

        // selected first
        selectIndex(0);
    }

    public void setSoundListSelectedItemChangedListener(OnSoundListSelectedItemChangedListener listener) {
        this.listener = listener;
    }

    public void selectIndex(int idx) {
        if(idx >= 0 && idx < countChild) {
            selectedItem = llListOfImage.getChildAt(idx);
            selectedItem.setBackgroundResource(R.drawable.sound_border_item);
            selectedItemIndex = idx;
        }
    }

    @Override
    public void onClick(View v) {
        if (selectedItem !=null) {
            selectedItem.setBackground(null);
        }

        Log.d("Selected item: ", "" + v.getTag());
        v.setBackgroundResource(R.drawable.sound_border_item);
        selectedItem = v;

        if(listener != null) {
            listener.OnSoundListSelectedItemChanged((int)v.getTag());
        }
    }
}
