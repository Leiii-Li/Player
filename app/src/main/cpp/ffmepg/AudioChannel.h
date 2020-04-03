//
// Created by Administrator on 2020\4\2 0002.
//

#ifndef PLAYER_AUDIOCHANNEL_H
#define PLAYER_AUDIOCHANNEL_H

#include "BaseChannel.h"

class AudioChannel: public BaseChannel {
 public:
  AudioChannel(int streamId, AVCodecContext *pContext);
  void play();
};


#endif //PLAYER_AUDIOCHANNEL_H
