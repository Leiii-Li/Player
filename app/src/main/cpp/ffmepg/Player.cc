//
// Created by Administrator on 2020\4\2 0002.
//

#include <pthread.h>
#include "Player.h"
#include "cstring"
#include "android/log.h"
#include "../constant/Global.h"
#include "VideoChannel.h"
#include "AudioChannel.h"
#include "../utils/RenderCallBack.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#define LOG_TAG "[nelson]"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))

void *prepare_thread(void *args);
void *reader_thread(void *args);

/**
 * 线程睡眠(单位为毫秒)
 * @param nsec
 */
void m_threadSleep(int nsec) {
    struct timespec sleepTime;
    struct timespec returnTime;
    sleepTime.tv_sec = 0;
    sleepTime.tv_nsec = nsec * 1000000;
    nanosleep(&sleepTime, &returnTime);
}

Player::Player(PlayerCallBack *callBack) {
    avcodec_register_all();
    av_register_all();
    avformat_network_init();
    this->callBack = callBack;
    this->avFormatContext = avformat_alloc_context();
}
void Player::setRenderFrameCallBack(RenderFrameCallBack renderFrameCallBack) {
    this->renderFrameCallBack = renderFrameCallBack;
}
Player::~Player() {

}
// 设置数据源
void Player::setDataSource(const char *dataSource) {
    //防止dataSource在其他地方被释放
    this->dataSource = new char[strlen(dataSource) + 1];
    strcpy(this->dataSource, dataSource);
}

//进行编解码准备工作
void Player::prepare() {
    LOGD("Prepare");
    pthread_t prepareThread;
    //创建线程，进行探测流工作
    pthread_create(&prepareThread, 0, prepare_thread, this);
}

void Player::start() {
    LOGD("Start");
    if (isPlaying) {
        return;
    }
    isPlaying = true;
    pthread_t playThreadId;
    pthread_create(&playThreadId, 0, reader_thread, this);
}

void *reader_thread(void *args) {
    Player *player = static_cast<Player *>(args);
    player->_start();
    return NULL;
}

/**
 * 读取数据包
 */
void Player::_start() {
    LOGD("Player Start");
    // 读取数据包
    videoChannel->start();
    audioChannel->start();
    while (isPlaying) {
        m_threadSleep(10);
        // 在堆内存中申请一个内存空间
        AVPacket *avPacket = av_packet_alloc();
        int ret = av_read_frame(avFormatContext, avPacket);
        if (ret == 0) {
            //读取成功
            if (videoChannel && avPacket->stream_index == videoChannel->streamId) {
                // 视频包
                videoChannel->packets.push(avPacket);
            } else if (audioChannel && avPacket->stream_index == audioChannel->streamId) {
                // 音频包
                audioChannel->packets.push(avPacket);
            }
        } else if (ret == AVERROR_EOF) {
            // 读取完成
            break;
        } else {
            continue;
        }
    }
    LOGD("Red File Finish");
}


/**
 * 该流程主要参考官方demo
 * @param args
 * @return
 */
void *prepare_thread(void *args) {
    Player *player = static_cast<Player *>(args);
    player->_prepare();
    return NULL;
}

void Player::_prepare() {
    int ret = avformat_open_input(&avFormatContext, dataSource, NULL, NULL);
    if (ret != 0) {
        callBack->onError(THREAD_CHILD, FFMPEG_CAN_NOT_OPEN_URL);
        return;
    }

    // 打开链接成功，AVFormatContext中通常会有两个两个流 ：音频流与视频流
    for (int i = 0; i < avFormatContext->nb_streams; i++) {
        // 获取流对象
        AVStream *stream = avFormatContext->streams[i];
        // 获取其编码信息
        AVCodecParameters *parameters = stream->codecpar;

        //查找解码器
        AVCodec *avCodec = avcodec_find_decoder(parameters->codec_id);
        if (avCodec == NULL) {
            callBack->onError(THREAD_CHILD, FFMPEG_FIND_DECODER_FAIL);
            return;
        }

        // 获取解码器
        AVCodecContext *codecContext = avcodec_alloc_context3(avCodec);
        if (codecContext == NULL) {
            callBack->onError(THREAD_CHILD, FFMPEG_ALLOC_CODEC_CONTEXT_FAIL);
            return;
        }

        //设置解码器上下文的参数
        int ret = avcodec_parameters_to_context(codecContext, parameters);
        if (ret < 0) {
            callBack->onError(THREAD_CHILD, FFMPEG_CODEC_CONTEXT_PARAMETERS_FAIL);
            return;
        }

        // 打开解码器
        ret = avcodec_open2(codecContext, avCodec, 0);
        if (ret != 0) {
            callBack->onError(THREAD_CHILD, FFMPEG_OPEN_DECODER_FAIL);
            return;
        }

        if (parameters->codec_type == AVMEDIA_TYPE_VIDEO) {
            // 视频流
            AVRational rational = stream->avg_frame_rate;
            double fps = av_q2d(rational);
            videoChannel = new VideoChannel(i,fps, codecContext, renderFrameCallBack);
        } else if (parameters->codec_type == AVMEDIA_TYPE_AUDIO) {
            // 音频流
            audioChannel = new AudioChannel(i, codecContext);
        }
        if (!videoChannel && !audioChannel) {
            callBack->onError(THREAD_CHILD, FFMPEG_NOMEDIA);
            return;
        }
    }
    callBack->onPrepare(THREAD_CHILD);
}