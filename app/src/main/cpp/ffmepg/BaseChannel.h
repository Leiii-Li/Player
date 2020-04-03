//
// Created by Administrator on 2020\4\2 0002.
//

#ifndef PLAYER_BASECHANNEL_H
#define PLAYER_BASECHANNEL_H
extern "C" {
#include <libavcodec/avcodec.h>
}
#include "../SafeQueue.h"

class BaseChannel {
 public:
  BaseChannel(int streamId) {
      this->streamId = streamId;
  }
  // 如果该方法不声明为虚函数，那么子类会调用父类的析构函数
  virtual ~BaseChannel() {

  }
  int streamId;
  SafeQueue<AVPacket *> packets;
};
#endif //PLAYER_BASECHANNEL_H
