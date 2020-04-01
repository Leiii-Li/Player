#include <jni.h>
#include <string>
#include <pthread.h>
#include <android/log.h>
extern "C" {
#include <libavformat/avformat.h>
}

#define LOG_TAG "[nelson]"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))

void *loadResource(void *args);
void *printCodeFormat(void *args);

JavaVM *javaVm;

jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    javaVm = vm;
    return JNI_VERSION_1_6;
}

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

    pthread_t threadId;
    pthread_create(&threadId, 0, printCodeFormat, env);

    return env->NewStringUTF("FFmpegPLayer Version :1.12");
}

void *printCodeFormat(void *args) {
    av_register_all();
    AVCodec *c_temp = av_codec_next(NULL);
    while (c_temp != NULL) {
        switch (c_temp->type) {
            case AVMEDIA_TYPE_VIDEO:
                LOGD("[Video]:%s", c_temp->name);
                break;
            case AVMEDIA_TYPE_AUDIO:
                LOGD("[Audio]:%s", c_temp->name);
                break;
            default:
                LOGD("[Other]:%s", c_temp->name);
                break;
        }
        c_temp = c_temp->next;
    }
    return NULL;
}
void *loadResource(void *args) {

    JNIEnv *env = static_cast<JNIEnv *>(args);

    const char *dataSource = "http://ivi.bupt.edu.cn/hls/cctv1hd.m3u8";

    AVFormatContext *avFormatContext;

    av_register_all();
    avformat_network_init();

    int ret = avformat_open_input(&avFormatContext, dataSource, NULL, NULL);
    jthrowable exc = env->ExceptionOccurred();

    if (exc) {
        // 打印异常日志
        env->ExceptionDescribe();
        // 这行代码才是关键不让应用崩溃的代码，
        env->ExceptionClear();
    }

    LOGD("Return : %d", ret);

    return NULL;
}