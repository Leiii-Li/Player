//
// Created by Administrator on 2020\4\2 0002.
//

#include "VideoChannel.h"
VideoChannel::VideoChannel(int streamId, AVCodecContext *pContext) : BaseChannel(streamId,
                                                                                 pContext) {

}

void VideoChannel::readTask() {

}

void VideoChannel::decodeTask() {

}
void VideoChannel::renderTask() {

}

void *decode_task(void *args) {
    VideoChannel *videoChannel = static_cast<VideoChannel *>(args);
    videoChannel->decode();
    return NULL;
}

void VideoChannel::decode() {
    AVPacket *packet = 0;
    while (isPlaying) {
        int ret = packets.deQueue(packet);
        if (!isPlaying) {
            break;
        }
        if (!ret) {
            continue;
        }
        // 将avPacket送至解码器进行解码
        ret = avcodec_send_packet(avCodecContext, packet);
        // 重试
        if (ret != 0) {
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
    }
    releaseAvPacket(&packet);
}

void VideoChannel::play() {
    // 解码
    isPlaying = 1;
    pthread_create(&decodeThreadId, 0, decode_task, this);
}