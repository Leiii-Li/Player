package com.nelson.player.player;

public interface PlayerCallBack {

    void onProgress(String msg);

    void onError(int errorCode);

    void onSuccess();

    void onPrepare();

    void onPause();

    void onResume();

    void captureImage(byte[] data, int width, int height);
}
