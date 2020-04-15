package com.nelson.player.player;

import static android.media.AudioManager.FLAG_SHOW_UI;

import android.content.Context;
import android.media.AudioManager;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceHolder.Callback;
import android.view.SurfaceView;
import android.view.View;
import android.view.View.OnTouchListener;

public class PlayerHelper implements Callback {

    private static final String TAG = PlayerHelper.class.getSimpleName();
    private String mDataSource;
    private SurfaceView mSurfaceView;
    private PlayerCallBack mCallBack;
    private Context mContext;
    private final AudioManager mAudioManager;
    private float mVolumeStartY;

    public PlayerHelper(SurfaceView surfaceView) {
        mSurfaceView = surfaceView;
        mContext = mSurfaceView.getContext();
        mSurfaceView.getHolder().addCallback(this);
        mSurfaceView.setOnTouchListener(mSurfaceViewTouchListener);
        mAudioManager = (AudioManager) mContext.getSystemService(Context.AUDIO_SERVICE);
    }

    public void setDataSource(String dataSource) {
        mDataSource = dataSource;
        PlayerNative.setDataSource(dataSource);
    }

    public void init() {
        PlayerNative.init(mCallBackWrapper);
    }

    private PlayerCallBack mCallBackWrapper = new PlayerCallBack() {
        @Override
        public void onProgress(String msg) {
            Log.i(TAG, "onProgress: " + msg);
            if (mCallBack != null) {
                mCallBack.onProgress(msg);
            }
        }

        @Override
        public void onError(int errorCode) {
            Log.i(TAG, "onError: " + errorCode);
            if (mCallBack != null) {
                mCallBack.onError(errorCode);
            }
        }

        @Override
        public void onSuccess() {
            Log.i(TAG, "onSuccess: ");
            if (mCallBack != null) {
                mCallBack.onSuccess();
            }
        }

        @Override
        public void onPrepare() {
            Log.i(TAG, "onPrepare: ");
            PlayerNative.start();
            if (mCallBack != null) {
                mCallBack.onPrepare();
            }
        }

        @Override
        public void onPause() {
            if (mCallBack != null) {
                mCallBack.onPause();
            }
        }

        @Override
        public void onResume() {
            if (mCallBack != null) {
                mCallBack.onResume();
            }
        }

        @Override
        public void captureImage(byte[] data, int width, int height) {
            if(mCallBack!=null){
                mCallBack.captureImage(data,width,height);
            }
        }
    };

    @Override
    public void surfaceCreated(SurfaceHolder holder) {

    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        PlayerNative.setSurface(holder.getSurface(), width, height);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }

    public void setCallBack(PlayerCallBack playerCallBack) {
        mCallBack = playerCallBack;
    }

    public int getTotalDuration() {
        return PlayerNative.getTotalDuration();
    }

    public int getCurrentDuration() {
        return PlayerNative.getCurrentDuration();
    }

    public void pause() {
        PlayerNative.pause();
    }

    public void resume() {
        PlayerNative.resume();
    }


    private OnTouchListener mSurfaceViewTouchListener = new OnTouchListener() {
        @Override
        public boolean onTouch(View v, MotionEvent event) {
            switch (event.getAction()) {
                case MotionEvent.ACTION_DOWN:
                    mVolumeStartY = event.getY();
                    break;
                case MotionEvent.ACTION_MOVE:
                    handVolumeSet(event.getY());
                    break;
            }
            return true;
        }
    };

    private void handVolumeSet(float y) {
        float distance = mVolumeStartY - y;

        distance /= 2f;

        mVolumeStartY = y;

        int streamVolume = mAudioManager.getStreamVolume(AudioManager.STREAM_MUSIC);

        streamVolume += distance;

        mAudioManager.setStreamVolume(AudioManager.STREAM_MUSIC, streamVolume, FLAG_SHOW_UI);
    }

    public void captureImage() {
        PlayerNative.captureImage();
    }
}
