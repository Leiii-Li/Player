package com.nelson.player.player;

class PlayerNative {

    static {
        System.loadLibrary("native-lib");

        System.loadLibrary("avcodec-57");
        System.loadLibrary("avdevice-57");
        System.loadLibrary("avfilter-6");
        System.loadLibrary("avformat-57");
        System.loadLibrary("avutil-55");
        System.loadLibrary("postproc-54");
        System.loadLibrary("swresample-2");
        System.loadLibrary("swscale-4");
    }

    public static native String getVersion();

    public static native void init(FFmpegCallBack callBack);

    public static native void setDataSource(String dataSource);

    public static native void start();

    public static native void prepare();
}
