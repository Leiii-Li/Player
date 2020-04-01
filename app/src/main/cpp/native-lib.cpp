#include <jni.h>
#include <string>

#include <android/log.h>
#define LOG_TAG "[nelson]"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))

extern "C"
JNIEXPORT void JNICALL
Java_com_nelson_player_player_PlayerNative_init(JNIEnv *env, jclass clazz, jobject call_back) {
    // TODO: implement init()
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nelson_player_player_PlayerNative_setDataSource(JNIEnv *env, jclass clazz) {
    // TODO: implement setDataSource()
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nelson_player_player_PlayerNative_start(JNIEnv *env, jclass clazz) {
    // TODO: implement start()
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nelson_player_player_PlayerNative_prepare(JNIEnv *env, jclass clazz) {
    // TODO: implement prepare()
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_nelson_player_player_PlayerNative_getVersion(JNIEnv *env, jclass clazz) {



    const char *dataSource = "http://ivi.bupt.edu.cn/hls/cctv1hd.m3u8";

//    int ret = avformat_open_input(&avFormatContext, dataSource, 0, 0);

//    LOGD("%d", ret);

    return env->NewStringUTF("FFmpegPLayer Version :1.2 ");
}