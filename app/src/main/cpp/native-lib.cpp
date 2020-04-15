#include <jni.h>
#include <string>
#include <pthread.h>
#include <android/log.h>

#include "ffmepg/Player.h"
#include "ffmepg/PlayerCallBack.h"
#include <android/native_window_jni.h>
#include "../utils/RenderCallBack.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#define LOG_TAG "[nelson]"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))

void *loadResource(void *args);
void *printCodeFormat(void *args);

JavaVM *javaVm;
Player *player;
ANativeWindow *window;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    javaVm = vm;
    return JNI_VERSION_1_6;
}

static void render(uint8_t *data, int lineSize, int width, int height) {
    pthread_mutex_lock(&mutex);
    if (!window) {
        pthread_mutex_unlock(&mutex);
        return;
    }
    //设置窗口属性
    ANativeWindow_setBuffersGeometry(window, width,
                                     height,
                                     WINDOW_FORMAT_RGBA_8888);

    ANativeWindow_Buffer window_buffer;
    if (ANativeWindow_lock(window, &window_buffer, 0)) {
        ANativeWindow_release(window);
        window = 0;
        pthread_mutex_unlock(&mutex);
        return;
    }
    uint8_t *dst_data = static_cast<uint8_t *>(window_buffer.bits);
    //一行需要多少像素 * 4(RGBA)
    int dst_linesize = window_buffer.stride * 4;
    uint8_t *src_data = data;
    int src_linesize = lineSize;
    //一次拷贝一行
    for (int i = 0; i < window_buffer.height; ++i) {
        memcpy(dst_data + i * dst_linesize, src_data + i * src_linesize, dst_linesize);
    }
    ANativeWindow_unlockAndPost(window);
    pthread_mutex_unlock(&mutex);

}

extern "C"
JNIEXPORT void JNICALL
Java_com_nelson_player_player_PlayerNative_init(JNIEnv *env, jclass clazz, jobject call_back) {
    PlayerCallBack *callBack = new PlayerCallBack(javaVm, env, call_back);
    player = new Player(callBack);
    player->setRenderFrameCallBack(render);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nelson_player_player_PlayerNative_setDataSource(JNIEnv *env,
                                                         jclass clazz,
                                                         jstring data_source) {
    const char *dataSource = env->GetStringUTFChars(data_source, 0);
    player->setDataSource(dataSource);
    player->prepare();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nelson_player_player_PlayerNative_start(JNIEnv *env, jclass clazz) {
    player->start();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nelson_player_player_PlayerNative_prepare(JNIEnv *env, jclass clazz) {
    // TODO: implement prepare()
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_nelson_player_player_PlayerNative_getVersion(JNIEnv *env, jclass clazz) {

//    pthread_t threadId;
//    pthread_create(&threadId, 0, loadResource, env);

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

    AVFormatContext *avFormatContext = avformat_alloc_context();;
    avcodec_register_all();
    av_register_all();
    avformat_network_init();

    int ret = avformat_open_input(&avFormatContext, dataSource, NULL, NULL);

    LOGD("Return : %d", ret);

    return NULL;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nelson_player_player_PlayerNative_setSurface(JNIEnv *env,
                                                      jclass clazz,
                                                      jobject surface,
                                                      jint width,
                                                      jint height) {
    if (window) {
        //释放之前的window
        ANativeWindow_release(window);
    }
    window = ANativeWindow_fromSurface(env, surface);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_nelson_player_player_PlayerNative_getTotalDuration(JNIEnv *env, jclass clazz) {
    int totalDuration = player->getTotalDuration();
    return totalDuration;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_nelson_player_player_PlayerNative_getCurrentDuration(JNIEnv *env, jclass clazz) {
    int currentDuration = player->getCurrentDuration();
    return currentDuration;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nelson_player_player_PlayerNative_pause(JNIEnv *env, jclass clazz) {
    player->pause();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nelson_player_player_PlayerNative_resume(JNIEnv *env, jclass clazz) {
    // TODO: implement resume()
}