//
// Created by Administrator on 2020\4\2 0002.
//

#ifndef PLAYER_VIDEOCHANNEL_H
#define PLAYER_VIDEOCHANNEL_H

extern "C" {
#include <libswscale/swscale.h>
}
#include "BaseChannel.h"
#include "pthread.h"

class VideoChannel: public BaseChannel {
  typedef void (*RenderFrameCallBack)(uint8_t *, int, int, int);
 public:
  VideoChannel(int streamId, AVCodecContext *pContext);
  ~VideoChannel();
  //进行一些准备工作
  void start();
  void stop();

  // 解码线程
  void runDecodeTask();
  //渲染线程
  void runRenderTask();

  void setRenderFrame(RenderFrameCallBack *callBack);
 private:
  pthread_t decodeThreadId;
  pthread_t renderThreadId;
  SafeQueue<AVFrame *> frameQueue;
  SwsContext *swsContext;
  RenderFrameCallBack *renderFrameCallBack;
};


#endif //PLAYER_VIDEOCHANNEL_H
