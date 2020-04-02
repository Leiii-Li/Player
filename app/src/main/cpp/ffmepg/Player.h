//
// Created by Administrator on 2020\4\2 0002.
//

#ifndef PLAYER_PLAYER_H
#define PLAYER_PLAYER_H

extern "C" {
#include <libavformat/avformat.h>
}

#include "PlayerCallBack.h"
class Player {
 public:
  Player(PlayerCallBack *callBack);
  ~Player();
  void setDataSource(const char *dataSource);
  void prepare();
  char *dataSource;
  AVFormatContext *avFormatContext;
 private:
  PlayerCallBack *callBack;
};


#endif //PLAYER_PLAYER_H
