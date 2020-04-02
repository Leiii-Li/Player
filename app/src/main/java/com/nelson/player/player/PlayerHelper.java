package com.nelson.player.player;

public class PlayerHelper {

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
        public void onError(int errorCode) {

        }

        @Override
        public void onSuccess() {

        }

        @Override
        public void onPrepare() {

        }
    };
}
