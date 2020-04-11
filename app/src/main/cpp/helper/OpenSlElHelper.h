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
 private:
  // PCM播放器
  SLObjectItf playerObject = 0;
  // 混音器引擎
  SLObjectItf outputMixObject = 0;
  // SLEL 引擎
  SLObjectItf audioEngine = 0;
  //
  SLEngineItf audioEngineInterface = 0;
  //
  SLPlayItf playerInterface = 0;

  // 音频解码后的队列
  SLAndroidSimpleBufferQueueItf bufferQueue = 0;
};


#endif //PLAYER_OPENSLELHELPER_H
