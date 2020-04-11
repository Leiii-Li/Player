//
// Created by Administrator on 2020\4\2 0002.
//
#include "android/log.h"
#include "AudioChannel.h"



#define LOG_TAG "[nelson]"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))

AudioChannel::AudioChannel(int streamId, AVCodecContext *pContext) : BaseChannel(streamId,
                                                                                 pContext) {

    openSlElHelper = new OpenSlElHelper(this);
}

PcmData *AudioChannel::getPcmData() {
    return NULL;
}

void *decode_audio_task(void *args) {
    AudioChannel *audioChannel = static_cast<AudioChannel *>(args);
    audioChannel->runDecodeTask();
    return NULL;
}

void AudioChannel::start() {
    channelIsWorking = true;
    packets.setWork(true);
    audioQueue.setWork(true);

    pthread_create(&decodeThreadId, 0, decode_audio_task, this);
}

void AudioChannel::stop() {
    channelIsWorking = false;
    packets.setWork(false);
    packets.clear();
    audioQueue.setWork(false);
    audioQueue.clear();
}

void AudioChannel::runDecodeTask() {
    LOGD("Run Audio Decode Task");
    AVPacket *packet = 0;
    while (channelIsWorking) {
        int ret = packets.pop(packet);
        if (!channelIsWorking) {
            break;
        }
        if (!ret) {
            continue;
        }
        // 将avPacket送至解码器进行解码
        ret = avcodec_send_packet(avCodecContext, packet);
        ReleaseUtils::releaseAvPacket(&packet);
        // 解码失败
        if (ret != 0) {
            LOGD("Send Audio Packet Error");
            break;
        }
        // 代表了某一帧
        AVFrame *frame = av_frame_alloc();
        // 从解码器中读取解码后的数据包
        ret = avcodec_receive_frame(avCodecContext, frame);
        if (ret == AVERROR(EAGAIN)) {
            continue;
        } else if (ret != 0) {
            break;
        }
        audioQueue.push(frame);
    }
    LOGD("Audio Decode Task Finish");
}
void AudioChannel::runRenderTask() {

}