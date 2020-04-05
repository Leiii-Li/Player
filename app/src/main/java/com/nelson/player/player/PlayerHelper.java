package com.nelson.player.player;

import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceHolder.Callback;
import android.view.SurfaceView;

public class PlayerHelper implements Callback {

    private static final String TAG = PlayerHelper.class.getSimpleName();
    private String mDataSource;
    private SurfaceView mSurfaceView;

    public PlayerHelper(SurfaceView surfaceView) {
        mSurfaceView = surfaceView;
        mSurfaceView.getHolder().addCallback(this);
    }

    public void setDataSource(String dataSource) {
        mDataSource = dataSource;
        PlayerNative.setDataSource(dataSource);
    }

    public void init() {
        PlayerNative.init(mFFmpegCallBack);
    }

    private FFmpegCallBack mFFmpegCallBack = new FFmpegCallBack() {
        @Override
        public void onProgress(String msg) {
            Log.i(TAG, "onProgress: " + msg);
        }

        @Override
        public void onError(int errorCode) {
            Log.i(TAG, "onError: " + errorCode);
        }

        @Override
        public void onSuccess() {
            Log.i(TAG, "onSuccess: ");
        }

        @Override
        public void onPrepare() {
            Log.i(TAG, "onPrepare: ");
            PlayerNative.start();
        }
    };

    @Override
    public void surfaceCreated(SurfaceHolder holder) {

    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        PlayerNative.setSurface(holder.getSurface(),width,height);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }
}
