package com.nelson.player.player;

public interface FFmpegCallBack {

    void onProgress(String msg);

    void onError(int errorCode);

    void onSuccess();

    void onPrepare();
}
