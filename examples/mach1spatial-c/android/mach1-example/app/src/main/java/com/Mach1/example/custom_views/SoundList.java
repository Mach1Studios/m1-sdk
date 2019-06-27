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

    View oldSelected = null;

    public SoundList(Context context, AttributeSet attrs) {
        super(context, attrs);

        LayoutInflater layoutInflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        View view = layoutInflater.inflate(R.layout.sime_list_layout, this, true);

        LinearLayout llListOfImage = (LinearLayout) view.findViewById(R.id.ll_smile_list);
        int countChild = llListOfImage.getChildCount();

        for (int i=0; i< countChild; i++){

            View viewChild = llListOfImage.getChildAt(i);
            if (viewChild instanceof ImageView){
                viewChild.setTag(""+i);
                viewChild.setOnClickListener(this);
            }
        }
    }

    @Override
    public void onClick(View v) {

        if (oldSelected!=null){
            oldSelected.setBackground(null);
        }

        Log.d("kkk", "" + v.getTag());
        v.setBackgroundResource(R.drawable.sime_border_item);
        oldSelected = v;
    }
}
