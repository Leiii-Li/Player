package com.nelson.player.player;

public interface PlayerCallBack {

    void onProgress(String msg);

    void onError(int errorCode);

    void onSuccess();

    void onPrepare();
}
