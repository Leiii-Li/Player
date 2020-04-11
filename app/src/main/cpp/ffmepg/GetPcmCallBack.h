//
// Created by Administrator on 2020\4\10 0010.
//

#ifndef PLAYER_GETPCMCALLBACK_H
#define PLAYER_GETPCMCALLBACK_H

struct PcmData {
  uint8_t *data;
  int dataSize;
  PcmData(uint8_t *data, int dataSize) {
      this->data = data;
      this->dataSize = dataSize;
  }
};

class GetPcmCallBack {
 public:
  virtual PcmData *getPcmData() = 0;
};


#endif //PLAYER_GETPCMCALLBACK_H
