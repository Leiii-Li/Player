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
#include "../utils/RenderCallBack.h"
#include "../constant/Session.h"


class VideoChannel: public BaseChannel {

 public:
  VideoChannel(int streamId,
               AVRational time_base, Session *session,
               double fps,
               AVCodecContext *pContext,
               RenderFrameCallBack callBack);
  ~VideoChannel();
  //进行一些准备工作
  void start();
  void stop();

  // 解码线程
  void runDecodeTask();
  //渲染线程
  void runRenderTask();
 private:
  double fps;
  double frame_delays;
  pthread_t decodeThreadId;
  pthread_t renderThreadId;
  SafeQueue<AVFrame *> frameQueue;
  SwsContext *swsContext;
  RenderFrameCallBack renderFrameCallBack;
  Session *session;
};


#endif //PLAYER_VIDEOCHANNEL_H
