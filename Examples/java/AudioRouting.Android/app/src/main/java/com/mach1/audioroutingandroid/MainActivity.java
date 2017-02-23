package com.mach1.audioroutingandroid;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;

import java.io.DataInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

class PlayFilesTask extends AsyncTask<Void, Void, Void> {

    private float[] _levels;
    private InputStream[] _resources;

    public PlayFilesTask(float[] levels, InputStream[] resources)
    {
        _levels = levels;
        _resources = resources;
    }

    @Override
    protected Void doInBackground(Void...voids) {
        int minBufferSize = AudioTrack.getMinBufferSize(44100, AudioFormat.CHANNEL_OUT_STEREO,
                AudioFormat.ENCODING_PCM_16BIT);
        int bufferSize = 512;
        DataInputStream[] inputStreams = new DataInputStream[8];
        AudioTrack audioTrack = new AudioTrack(AudioManager.STREAM_MUSIC, 44100,
                AudioFormat.CHANNEL_OUT_STEREO, AudioFormat.ENCODING_PCM_16BIT, minBufferSize,
                AudioTrack.MODE_STREAM);
        byte[][] buffers = new byte[8][];
        int[] positions = new int[8];
        ByteBuffer outputBuffer = ByteBuffer.allocate(bufferSize);
        outputBuffer.order(ByteOrder.LITTLE_ENDIAN);
        for (int i = 0; i < 8; i++) {
            positions[i] = 0;
            buffers[i] = new byte[bufferSize];
            inputStreams[i] = new DataInputStream(_resources[i]);
        }
        audioTrack.play();
        try {
            boolean trackFinished = false;
            do {
                short[] values = new short[bufferSize];
                for (int i = 0; i < 8; i++){
                    if((positions[i] = inputStreams[i].read(buffers[i], 0, bufferSize)) > 1){
                        for (int j = 0; j < bufferSize; j+=2){
                            outputBuffer.put(j, buffers[i][j]);
                            outputBuffer.put(j+1, buffers[i][j+1]);
                            short value = outputBuffer.getShort(j);
                            value *= _levels[i];
                            values[j] += value;
                            outputBuffer.putShort(j, values[j]);
                        }
                    } else {
                        trackFinished = true;
                        break;
                    }
                }
                audioTrack.write(outputBuffer.array(), 0, positions[0]);
            } while(!trackFinished);
            audioTrack.stop();
            audioTrack.release();
            for (int i = 0; i < 8; i++){
                inputStreams[i].close();
                _resources[i].close();
            }
        } catch (IOException e){
            e.printStackTrace();
        }
        return null;
    }
}

public class MainActivity extends AppCompatActivity implements SeekBar.OnSeekBarChangeListener {

    private float[] _mAllProgress = { 0.5f, 0, 0, 0, 0, 0, 0, 0 };
    private InputStream[] _resources = new InputStream[8];

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ((SeekBar)findViewById(R.id.level1)).setProgress(50);
        ((SeekBar)findViewById(R.id.level1)).setOnSeekBarChangeListener(this);
        ((SeekBar)findViewById(R.id.level2)).setOnSeekBarChangeListener(this);
        ((SeekBar)findViewById(R.id.level3)).setOnSeekBarChangeListener(this);
        ((SeekBar)findViewById(R.id.level4)).setOnSeekBarChangeListener(this);
        ((SeekBar)findViewById(R.id.level5)).setOnSeekBarChangeListener(this);
        ((SeekBar)findViewById(R.id.level6)).setOnSeekBarChangeListener(this);
        ((SeekBar)findViewById(R.id.level7)).setOnSeekBarChangeListener(this);
        ((SeekBar)findViewById(R.id.level8)).setOnSeekBarChangeListener(this);
        _resources[0] = getResources().openRawResource(R.raw.metronome0);
        _resources[1] = getResources().openRawResource(R.raw.metronome1);
        _resources[2] = getResources().openRawResource(R.raw.noise0);
        _resources[3] = getResources().openRawResource(R.raw.noise1);
        _resources[4] = getResources().openRawResource(R.raw.noise2);
        _resources[5] = getResources().openRawResource(R.raw.noise3);
        _resources[6] = getResources().openRawResource(R.raw.noise4);
        _resources[7] = getResources().openRawResource(R.raw.speech0);
    }

    public void onPlayClick(View view) throws IOException {
        view.findViewById(R.id.buttonPlay).setClickable(false);
        new PlayFilesTask(_mAllProgress, _resources).execute();
    }

    private int getSeekBarIndex(int id) {
        switch (id){
            case R.id.level1:
                return 0;
            case R.id.level2:
                return 1;
            case R.id.level3:
                return 2;
            case R.id.level4:
                return 3;
            case R.id.level5:
                return 4;
            case R.id.level6:
                return 5;
            case R.id.level7:
                return 6;
            case R.id.level8:
                return 7;
            default:
                throw new IllegalStateException(
                        "No SeekBar with id(" + id + ")");
        }
    }

    @Override
    public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser){
        int index = getSeekBarIndex(seekBar.getId());
        _mAllProgress[index] = ((float)seekBar.getProgress() / 100.0f);
    }

    @Override
    public void onStartTrackingTouch(SeekBar seekBar) {

    }

    @Override
    public void onStopTrackingTouch(SeekBar seekBar) {

    }
}
