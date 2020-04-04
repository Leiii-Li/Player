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
  BaseChannel(int streamId, AVCodecContext *codecContext) {
      this->streamId = streamId;
      this->avCodecContext = codecContext;
  }
  // 如果该方法不声明为虚函数，那么子类会调用父类的析构函数
  virtual ~BaseChannel() {
      packets.setReleaseCallBack(BaseChannel::releaseAvPacket);
      packets.clear();
  }
  /**
   * 释放AVPacket
   * @param avPacket
   */
  static void releaseAvPacket(AVPacket **avPacket) {
      if (avPacket) {
          av_packet_free(avPacket);
          avPacket = 0;
      }
  };

  int streamId;
  bool channelIsWorking = false;

  SafeQueue<AVPacket *> packets;
  AVCodecContext *avCodecContext;

  //进行一些准备工作
  virtual void start() = 0;
  virtual void stop() = 0;

  // 解码线程
  virtual void runDecodeTask() = 0;
  //渲染线程
  virtual void runRenderTask() = 0;

};
#endif //PLAYER_BASECHANNEL_H
