//
// Created by Administrator on 2020\4\2 0002.
//

#include "PlayerCallBack.h"
PlayerCallBack::PlayerCallBack(JavaVM *javaVmm, JNIEnv *jniEnv, jobject instance) {
    this->javaVm = javaVmm;
    this->jniEnv = jniEnv;
    this->instance = jniEnv->NewGlobalRef(instance);
    jclass clazz = jniEnv->GetObjectClass(instance);
    this->onErrorMethodId = jniEnv->GetMethodID(clazz, "onError", "(I)V");
    this->onSuccessMethodId = jniEnv->GetMethodID(clazz, "onSuccess", "()V");
    this->onPrepareMethodId = jniEnv->GetMethodID(clazz, "onPrepare", "()V");
}
PlayerCallBack::~PlayerCallBack() {
    this->jniEnv->DeleteGlobalRef(instance);
}