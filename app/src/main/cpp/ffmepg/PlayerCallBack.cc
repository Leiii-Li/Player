//
// Created by Administrator on 2020\4\2 0002.
//

#include "PlayerCallBack.h"
#include "../constant/Global.h"

PlayerCallBack::PlayerCallBack(JavaVM *javaVmm, JNIEnv *jniEnv, jobject instance) {
    this->javaVm = javaVmm;
    this->jniEnv = jniEnv;
    this->instance = jniEnv->NewGlobalRef(instance);
    jclass clazz = jniEnv->GetObjectClass(instance);
    this->onErrorMethodId = jniEnv->GetMethodID(clazz, "onError", "(I)V");
    this->onSuccessMethodId = jniEnv->GetMethodID(clazz, "onSuccess", "()V");
    this->onPrepareMethodId = jniEnv->GetMethodID(clazz, "onPrepare", "()V");
    this->onProgressMethodId = jniEnv->GetMethodID(clazz, "onProgress", "(Ljava/lang/String;)V");
}
PlayerCallBack::~PlayerCallBack() {
    this->jniEnv->DeleteGlobalRef(instance);
}

void PlayerCallBack::onError(int threadId, int errorCode) {
    if (threadId == THREAD_CHILD) {
        JNIEnv *env;
        // 获取当前线程的JNIEnv
        javaVm->AttachCurrentThread(&env, 0);
        env->CallVoidMethod(instance, onErrorMethodId, errorCode);
        javaVm->DetachCurrentThread();
    } else {
        jniEnv->CallVoidMethod(instance, onErrorMethodId, errorCode);
    }
}
void PlayerCallBack::onSuccess(int threadId) {
    if (threadId == THREAD_CHILD) {
        JNIEnv *env;
        // 获取当前线程的JNIEnv
        javaVm->AttachCurrentThread(&env, 0);
        env->CallVoidMethod(instance, onSuccessMethodId);
        javaVm->DetachCurrentThread();
    } else {
        jniEnv->CallVoidMethod(instance, onSuccessMethodId);
    }
}
void PlayerCallBack::onPrepare(int threadId) {

}
void PlayerCallBack::onProgress(int threadId, const char *msg) {
    if (threadId == THREAD_CHILD) {
        JNIEnv *env;
        // 获取当前线程的JNIEnv
        javaVm->AttachCurrentThread(&env, 0);
        jstring jMsg = env->NewStringUTF(msg);
        env->CallVoidMethod(instance, onProgressMethodId, jMsg);
        env->ReleaseStringUTFChars(jMsg, msg);
        env->DeleteGlobalRef(jMsg);
        javaVm->DetachCurrentThread();
    } else {
        jstring jMsg = jniEnv->NewStringUTF(msg);
        jniEnv->CallVoidMethod(instance, onProgressMethodId, jMsg);
        jniEnv->ReleaseStringUTFChars(jMsg, msg);
        jniEnv->DeleteGlobalRef(jMsg);
    }
}