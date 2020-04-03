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
  void play();
  void decode();
 private:
  pthread_t decodeThreadId;
};


#endif //PLAYER_VIDEOCHANNEL_H
