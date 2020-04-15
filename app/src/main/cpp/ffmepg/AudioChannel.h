//
// Created by Administrator on 2020\4\2 0002.
//

#ifndef PLAYER_AUDIOCHANNEL_H
#define PLAYER_AUDIOCHANNEL_H
#include "BaseChannel.h"
#include "../ffmepg/GetPcmCallBack.h"
#include "../helper/OpenSlElHelper.h"
#include "../constant/Session.h"
extern "C" {
#include <libswresample/swresample.h>
}
class AudioChannel: public BaseChannel, GetPcmCallBack {
 public:
  AudioChannel(int streamId, AVRational time_base, Session *session, AVCodecContext *pContext);
  ~AudioChannel();
  //进行一些准备工作
  void start();
  void stop();

  PcmData *getPcmData();

  OpenSlElHelper *openSlElHelper;

  // 解码线程
  void runDecodeTask();
  //渲染线程
  void runRenderTask();
  void pause();
  void resume();
 private:
  pthread_t decodeThreadId;
  SafeQueue<AVFrame *> audioQueue;
  int out_channels;
  int out_sample_size;
  int out_sample_rate;
  SwrContext *swrContext;
  uint8_t *data;
  Session *session;
};


#endif //PLAYER_AUDIOCHANNEL_H
