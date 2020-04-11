//
// Created by Administrator on 2020\4\2 0002.
//

#ifndef PLAYER_AUDIOCHANNEL_H
#define PLAYER_AUDIOCHANNEL_H
#include "BaseChannel.h"
#include "../ffmepg/GetPcmCallBack.h"
#include "../helper/OpenSlElHelper.h"

class AudioChannel: public BaseChannel, GetPcmCallBack {
 public:
  AudioChannel(int streamId, AVCodecContext *pContext);

  //进行一些准备工作
  void start();
  void stop();

  PcmData *getPcmData();

  OpenSlElHelper *openSlElHelper;

  // 解码线程
  void runDecodeTask();
  //渲染线程
  void runRenderTask();
};


#endif //PLAYER_AUDIOCHANNEL_H
