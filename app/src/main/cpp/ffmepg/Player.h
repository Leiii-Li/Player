//
// Created by Administrator on 2020\4\2 0002.
//

#ifndef PLAYER_PLAYER_H
#define PLAYER_PLAYER_H

extern "C" {
#include <libavformat/avformat.h>
}

#include "PlayerCallBack.h"
#include "AudioChannel.h"
#include "VideoChannel.h"

class Player {
 public:
  Player(PlayerCallBack *callBack);
  ~Player();
  void setDataSource(const char *dataSource);
  void prepare();
  void _prepare();
  void start();
  void _start();
  void setRenderFrameCallBack(RenderFrameCallBack renderFrameCallBack);
 private:
  char *dataSource;
  AVFormatContext *avFormatContext;
  PlayerCallBack *callBack;
  AudioChannel *audioChannel = 0;
  VideoChannel *videoChannel = 0;
  bool isPlaying = false;
  RenderFrameCallBack renderFrameCallBack;
};


#endif //PLAYER_PLAYER_H
