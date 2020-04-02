//
// Created by Administrator on 2020\4\2 0002.
//

#ifndef PLAYER_PLAYERCALLBACK_H
#define PLAYER_PLAYERCALLBACK_H

#include <jni.h>

class PlayerCallBack {
 public:
  PlayerCallBack(JavaVM *javaVmm, JNIEnv *jniEnv, jobject instance);
  ~PlayerCallBack();
 private:
  JavaVM *javaVm;
  JNIEnv *jniEnv;
  jobject instance;
  jmethodID onErrorMethodId;
  jmethodID onSuccessMethodId;
  jmethodID onPrepareMethodId;
};


#endif //PLAYER_PLAYERCALLBACK_H
