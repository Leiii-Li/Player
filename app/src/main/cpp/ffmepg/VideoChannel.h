//
// Created by Administrator on 2020\4\2 0002.
//

#ifndef PLAYER_VIDEOCHANNEL_H
#define PLAYER_VIDEOCHANNEL_H


#include "BaseChannel.h"
#include "pthread.h"

class VideoChannel: public BaseChannel {
 public:
  VideoChannel(int streamId, AVCodecContext *pContext);

  //进行一些准备工作
  void init();
  // 读流线程
  void readTask();
  // 解码线程
  void decodeTask();
  //渲染线程
  void renderTask();
 private:
  pthread_t decodeThreadId;
};


#endif //PLAYER_VIDEOCHANNEL_H
