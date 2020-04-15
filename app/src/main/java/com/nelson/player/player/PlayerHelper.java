package com.nelson.player.player;

import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceHolder.Callback;
import android.view.SurfaceView;
import com.nelson.player.MainActivity;

public class PlayerHelper implements Callback {

    private static final String TAG = PlayerHelper.class.getSimpleName();
    private String mDataSource;
    private SurfaceView mSurfaceView;
    private PlayerCallBack mCallBack;

    public PlayerHelper(SurfaceView surfaceView) {
        mSurfaceView = surfaceView;
        mSurfaceView.getHolder().addCallback(this);
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
}
