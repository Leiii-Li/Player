//
// Created by Nelson on 2020/4/5.
//

#ifndef PLAYER_RELEASEUTILS_H
#define PLAYER_RELEASEUTILS_H
extern "C" {
#include "../include/libavcodec/avcodec.h"
#include "../include/libavutil/frame.h"
}
class ReleaseUtils {
 public:
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

  static void releaseAvFrame(AVFrame **avFrame) {
      if (avFrame) {
          av_frame_free(avFrame);
          avFrame = 0;
      }
  }
};


#endif //PLAYER_RELEASEUTILS_H
