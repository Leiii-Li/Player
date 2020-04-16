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
#include "../constant/Session.h"
#include "pthread.h"

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

  int getTotalDuration();
  int getCurrentDuration();
  void pause();
  void resume();
  int startRecord(const char *video_path);
  void stopRecord();
  void record();
 private:
  char *dataSource;
  AVFormatContext *avFormatContext = 0;
  PlayerCallBack *callBack = 0;
  AudioChannel *audioChannel = 0;
  VideoChannel *videoChannel = 0;
  bool isPlaying = false;
  RenderFrameCallBack renderFrameCallBack;
  Session *session;

  // 录制相关参数
  pthread_mutex_t recordMutex;
  char *videoPath;
  bool isRecording = false;
  AVFormatContext *recordAVFormatContext = 0;
  SafeQueue<AVPacket *> recordQueue;
  pthread_t recordThread;
  void writePacket2File(AVPacket *packet);
  void initAvFormatContext();
};


#endif //PLAYER_PLAYER_H
