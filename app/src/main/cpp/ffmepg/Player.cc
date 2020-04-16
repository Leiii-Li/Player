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

    pthread_mutex_init(&recordMutex, 0);

    this->session = new Session();
    this->callBack = callBack;
    this->avFormatContext = avformat_alloc_context();
}

void Player::setRenderFrameCallBack(RenderFrameCallBack renderFrameCallBack) {
    this->renderFrameCallBack = renderFrameCallBack;
}

Player::~Player() {
    pthread_mutex_destroy(&recordMutex);
    if (avFormatContext) {
        avformat_free_context(avFormatContext);
    }
    if (recordAVFormatContext) {
        avformat_free_context(recordAVFormatContext);
    }
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
    callBack->onResume(THREAD_CHILD);
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
        // 在堆内存中申请一个内存空间
        AVPacket *avPacket = av_packet_alloc();
        int ret = av_read_frame(avFormatContext, avPacket);
        if (ret == 0) {
            if (isRecording) {
                AVPacket *tmp_pkt;
                tmp_pkt = av_packet_alloc();
                av_packet_ref(tmp_pkt, avPacket);
                recordQueue.push(tmp_pkt);
            }
            //读取成功
            if (videoChannel && avPacket->stream_index == videoChannel->streamId) {
                // 视频包
                while (videoChannel->packets.size() > 200) {
                    m_threadSleep(1000);
                }
                videoChannel->packets.push(avPacket);
            } else if (audioChannel && avPacket->stream_index == audioChannel->streamId) {
                // 音频包
                while (audioChannel->packets.size() > 200) {
                    m_threadSleep(1000);
                }
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

    session->totalDuration = avFormatContext->duration / 1000000;

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
        AVRational time_base = stream->time_base;
        if (parameters->codec_type == AVMEDIA_TYPE_VIDEO) {
            // 视频流
            AVRational rational = stream->avg_frame_rate;
            double fps = av_q2d(rational);
            videoChannel =
                    new VideoChannel(i, time_base, session, fps, codecContext, renderFrameCallBack);
        } else if (parameters->codec_type == AVMEDIA_TYPE_AUDIO) {
            // 音频流
            audioChannel = new AudioChannel(i, time_base, session, codecContext);
        }
        if (!videoChannel && !audioChannel) {
            callBack->onError(THREAD_CHILD, FFMPEG_NOMEDIA);
            return;
        }
    }
    callBack->onPrepare(THREAD_CHILD);
}

int Player::getTotalDuration() {
    return session->totalDuration;
}

int Player::getCurrentDuration() {
    return session->currentDuration;
}

void Player::pause() {
    videoChannel->pause();
    audioChannel->pause();
}
void Player::resume() {
    videoChannel->resume();
    audioChannel->resume();
}

void *record_task(void *args) {
    Player *player = static_cast<Player *>(args);
    player->record();
}

int Player::startRecord(const char *video_path) {
    if (!avFormatContext) {
        return 0;
    }
    pthread_mutex_lock(&recordMutex);

    this->videoPath = new char[strlen(video_path) + 1];
    strcpy(this->videoPath, video_path);

    initAvFormatContext();

    isRecording = true;
    pthread_create(&recordThread, NULL, record_task, this);
    recordQueue.setWork(true);
    pthread_mutex_unlock(&recordMutex);
    return 1;
}

void Player::stopRecord() {
    pthread_mutex_lock(&recordMutex);

    if (recordAVFormatContext) {
        av_write_trailer(recordAVFormatContext);
    }
    isRecording = false;
    recordQueue.setWork(false);
    recordQueue.clear();
    delete videoPath;

    videoPath = 0;

    avformat_free_context(recordAVFormatContext);

    pthread_mutex_unlock(&recordMutex);
}

void Player::initAvFormatContext() {
    if (recordAVFormatContext) {
        avformat_free_context(recordAVFormatContext);
    }

    const char *formatName = avFormatContext->iformat->name;

    int ret = avformat_alloc_output_context2(&recordAVFormatContext,
                                             NULL,
                                             NULL,
                                             videoPath);

    for (int i = 0; i < avFormatContext->nb_streams; i++) {

        AVStream *in_stream = avFormatContext->streams[i];
        //创建输出流
        AVStream *out_stream = avformat_new_stream(recordAVFormatContext, in_stream->codec->codec);

        // 拷贝输出流参数
        avcodec_copy_context(out_stream->codec, in_stream->codec);

        out_stream->codec->codec_tag = 0;
    }

    // 创建输出文件
    av_dump_format(recordAVFormatContext, 0, videoPath, 1);

    // open output file
    if (!(recordAVFormatContext->flags & AVFMT_NOFILE)) {
        avio_open(&recordAVFormatContext->pb, videoPath, AVIO_FLAG_WRITE);
    }

    // 写入文件头
    avformat_write_header(recordAVFormatContext, NULL);
}
void Player::record() {
    AVPacket *packet = 0;
    while (isRecording) {
        int ret = recordQueue.pop(packet);
        if (!isRecording) {
            break;
        }
        if (!ret) {
            continue;
        }
        // 写入文件
        av_interleaved_write_frame(recordAVFormatContext, packet);

        // 释放
        ReleaseUtils::releaseAvPacket(&packet);
    }
}
