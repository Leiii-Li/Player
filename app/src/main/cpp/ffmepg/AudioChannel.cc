//
// Created by Administrator on 2020\4\2 0002.
//

#include "AudioChannel.h"
AudioChannel::AudioChannel(int streamId, AVCodecContext *pContext) : BaseChannel(streamId,
                                                                                 pContext) {

}
void AudioChannel::start() {
    channelIsWorking = true;
    packets.setWork(true);
}

void AudioChannel::stop() {
    channelIsWorking = false;
    packets.setWork(false);
    packets.clear();
}

void AudioChannel::runDecodeTask() {

}
void AudioChannel::runRenderTask() {

}