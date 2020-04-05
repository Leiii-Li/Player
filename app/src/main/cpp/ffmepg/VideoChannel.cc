//
// Created by Administrator on 2020\4\2 0002.
//
#include "android/log.h"
#include "exception"
extern "C" {
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}
#include "../utils/ReleaseUtils.h"
#define LOG_TAG "[nelson]"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))

void *decode_task(void *args);
void *render_task(void *args);

#include "VideoChannel.h"
VideoChannel::VideoChannel(int streamId, AVCodecContext *pContext) : BaseChannel(streamId,
                                                                                 pContext) {
    frameQueue.setReleaseCallBack(ReleaseUtils::releaseAvFrame);

    // 获取图片转换器转换器
    swsContext = sws_getContext(pContext->width,
                                pContext->height,
                                pContext->pix_fmt,
                                pContext->width,
                                pContext->height,
                                AV_PIX_FMT_RGBA,
                                SWS_BILINEAR,
                                0,
                                0, 0);
}

VideoChannel::~VideoChannel() {
    channelIsWorking = false;
    packets.setWork(false);
    packets.clear();
    frameQueue.setWork(false);
    frameQueue.clear();
}

void VideoChannel::start() {
    channelIsWorking = true;
    packets.setWork(true);
    frameQueue.setWork(true);
    pthread_create(&decodeThreadId, 0, decode_task, this);
//    pthread_create(&renderThreadId, 0, render_task, this);
}

void VideoChannel::stop() {
    channelIsWorking = false;
    packets.setWork(false);
    packets.clear();
    frameQueue.clear();
}

void VideoChannel::runDecodeTask() {
    LOGD("Run VideoDecodeTask");
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
        frameQueue.push(frame);
    }
    ReleaseUtils::releaseAvPacket(&packet);
}

void VideoChannel::runRenderTask() {
    AVFrame *frame = 0;

    //指针数组
    uint8_t *dstData[4];
    int dstStride[4];
    av_image_alloc(dstData, dstStride, avCodecContext->width,
                   avCodecContext->height,
                   avCodecContext->pix_fmt, AV_PIX_FMT_RGBA);
    while (channelIsWorking) {
        int ret = frameQueue.pop(frame);
        if (!channelIsWorking) {
            break;
        }
        if (ret) {
            sws_scale(swsContext,
                      frame->data,
                      frame->linesize,
                      0,
                      avCodecContext->height,
                      dstData,
                      dstStride);
        }
        if (renderFrameCallBack) {

        }
    }
    av_free(dstData[0]);
    ReleaseUtils::releaseAvFrame(&frame);
}

void VideoChannel::setRenderFrame(RenderFrameCallBack *callBack) {
    this->renderFrameCallBack = callBack;
}

void *decode_task(void *args) {
    VideoChannel *videoChannel = static_cast<VideoChannel *>(args);
    videoChannel->runDecodeTask();
    return NULL;
}
void *render_task(void *args) {
    VideoChannel *videoChannel = static_cast<VideoChannel *>(args);
    videoChannel->runRenderTask();
    return NULL;
}