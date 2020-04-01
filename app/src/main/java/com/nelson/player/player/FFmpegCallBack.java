package com.nelson.player.player;

public interface FFmpegCallBack {

    void onError(int errorCode);

    void onSuccess();

    void onPrepare();
}
