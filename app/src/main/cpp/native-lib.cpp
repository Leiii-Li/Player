#include <jni.h>
#include <string>

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
    return env->NewStringUTF("1.0");
}