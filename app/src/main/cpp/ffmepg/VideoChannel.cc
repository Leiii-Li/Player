//
// Created by Administrator on 2020\4\2 0002.
//
#include "android/log.h"
#include "exception"

#define LOG_TAG "[nelson]"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))

void *decode_task(void *args);

#include "VideoChannel.h"
VideoChannel::VideoChannel(int streamId, AVCodecContext *pContext) : BaseChannel(streamId,
                                                                                 pContext) {

}

void VideoChannel::start() {
    channelIsWorking = true;
    packets.setWork(true);
    pthread_create(&decodeThreadId, 0, decode_task, this);
}

void VideoChannel::stop() {
    channelIsWorking = true;
    packets.setWork(false);
}

void VideoChannel::runDecodeTask() {
    LOGD("Run VideoDecodeTask");
    AVPacket *packet = 0;
    while (channelIsWorking) {
        try
        {
            int ret = packets.pop(packet);
            if (!channelIsWorking) {
                break;
            }
            if (!ret) {
                continue;
            }
            // 将avPacket送至解码器进行解码
            ret = avcodec_send_packet(avCodecContext, packet);
            // 解码失败
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
        }catch(std::exception_ptr  e1 ){
        }
    }
    releaseAvPacket(&packet);
}

void VideoChannel::runRenderTask() {

}

void *decode_task(void *args) {
    VideoChannel *videoChannel = static_cast<VideoChannel *>(args);
    videoChannel->runDecodeTask();
    return NULL;
}