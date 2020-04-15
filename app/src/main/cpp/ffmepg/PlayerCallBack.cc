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
    this->onResumeMethodId = jniEnv->GetMethodID(clazz, "onResume", "()V");
    this->onPauseMethodId = jniEnv->GetMethodID(clazz, "onPause", "()V");
    this->captureImageMethodId = jniEnv->GetMethodID(clazz, "captureImage", "([BII)V");
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
    callJavaVoidMethod(threadId, onSuccessMethodId);
}
void PlayerCallBack::onPrepare(int threadId) {
    callJavaVoidMethod(threadId, onPrepareMethodId);
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

void PlayerCallBack::onResume(int threadId) {
    callJavaVoidMethod(threadId, onResumeMethodId);
}

void PlayerCallBack::onPause(int threadId) {
    callJavaVoidMethod(threadId, onPauseMethodId);
}

void PlayerCallBack::callJavaVoidMethod(int threadId, jmethodID methodId) {
    if (threadId == THREAD_CHILD) {
        int status;
        bool isAttached = false;
        JNIEnv *env;

        status = javaVm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6);

        if (status < 0) {
            if (javaVm->AttachCurrentThread(&env, 0)) {
                return;
            }
            isAttached = true;
        }

        // 获取当前线程的JNIEnv
        env->CallVoidMethod(instance, methodId);
        if (isAttached) {
            javaVm->DetachCurrentThread();
        }
    } else {
        jniEnv->CallVoidMethod(instance, methodId);
    }
}

void PlayerCallBack::captureImage(uint8_t *data, int width, int height) {
    int status;
    bool isAttached = false;
    JNIEnv *env;

    status = javaVm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6);

    if (status < 0) {
        if (javaVm->AttachCurrentThread(&env, 0)) {
            return;
        }
        isAttached = true;
    }

    jbyteArray imageByteArray = env->NewByteArray(width * height * 4);
    env->SetByteArrayRegion(imageByteArray, 0, width * height * 4, (jbyte *) data);

    // 获取当前线程的JNIEnv
    env->CallVoidMethod(instance, captureImageMethodId, imageByteArray, width, height);

    if (isAttached) {
        javaVm->DetachCurrentThread();
    }
}

