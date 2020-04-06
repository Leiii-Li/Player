//
// Created by Administrator on 2020\4\2 0002.
//

#ifndef PLAYER_AUDIOCHANNEL_H
#define PLAYER_AUDIOCHANNEL_H
#include "BaseChannel.h"

class AudioChannel: public BaseChannel {
 public:
  AudioChannel(int streamId, AVCodecContext *pContext);

  //进行一些准备工作
  void start();
  void stop();


  // 解码线程
  void runDecodeTask();
  //渲染线程
  void runRenderTask();
};


#endif //PLAYER_AUDIOCHANNEL_H
