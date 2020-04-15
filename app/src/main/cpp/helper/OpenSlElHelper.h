//
// Created by Administrator on 2020\4\8 0008.
//

#ifndef PLAYER_OPENSLELHELPER_H
#define PLAYER_OPENSLELHELPER_H
#include <SLES/OpenSLES_Android.h>
#include "SLES/OpenSLES.h"
#include "../ffmepg/GetPcmCallBack.h"


class OpenSlElHelper {
 public:
  OpenSlElHelper(GetPcmCallBack *callBack);
  ~OpenSlElHelper();
  GetPcmCallBack *callBack;
  void active();
  bool isActive = false;
  void pause();
  void resume();
 private:

  /**
      * OpenSL ES
      */
  // 引擎与引擎接口
  SLObjectItf engineObject = 0;
  SLEngineItf engineInterface = 0;
  //混音器
  SLObjectItf outputMixObject = 0;
  //播放器
  SLObjectItf bqPlayerObject = 0;
  //播放器接口
  SLPlayItf bqPlayerInterface = 0;

  SLAndroidSimpleBufferQueueItf bqPlayerBufferQueueInterface = 0;
};


#endif //PLAYER_OPENSLELHELPER_H
