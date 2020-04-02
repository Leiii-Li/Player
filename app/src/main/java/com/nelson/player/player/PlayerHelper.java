package com.nelson.player.player;

import android.util.Log;

public class PlayerHelper {

    private static final String TAG = PlayerHelper.class.getSimpleName();
    private String mDataSource;

    public PlayerHelper() {

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
        }
    };
}
