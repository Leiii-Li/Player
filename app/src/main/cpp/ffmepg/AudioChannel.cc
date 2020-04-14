//
// Created by Administrator on 2020\4\2 0002.
//

#include "android/log.h"
#include "AudioChannel.h"
#include "../constant/Session.h"

#define LOG_TAG "[nelson]"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))


AudioChannel::AudioChannel(int streamId,
                           AVRational time_base,
                           Session *session,
                           AVCodecContext *pContext)
        : BaseChannel(streamId,
                      time_base,
                      pContext) {

    this->session = session;

    openSlElHelper = new OpenSlElHelper(this);

    //声道布局：声道数
    out_channels = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
    // 采样位数格式
    out_sample_size = av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);
    // 采样率
    out_sample_rate = 44100;

    //0+输出声道+输出采样位+输出采样率+  输入的3个参数
    swrContext = swr_alloc_set_opts(0, AV_CH_LAYOUT_STEREO, AV_SAMPLE_FMT_S16, out_sample_rate,
                                    avCodecContext->channel_layout, avCodecContext->sample_fmt,
                                    avCodecContext->sample_rate, 0, 0);
    //初始化
    swr_init(swrContext);


    //44100个16位 44100 * 2
    // 44100*(双声道)*(16位)
    data = static_cast<uint8_t *>(malloc(out_sample_rate * out_channels * out_sample_size));
    memset(data, 0, out_sample_rate * out_channels * out_sample_size);
}

AudioChannel::~AudioChannel() {
    if (data) {
        free(data);
        data = 0;
    }
}

PcmData *AudioChannel::getPcmData() {
    int data_size = 0;
    AVFrame *frame;
    int ret = audioQueue.pop(frame);
    if (!channelIsWorking) {
        if (ret) {
            ReleaseUtils::releaseAvFrame(&frame);
        }
        return NULL;
    }
    //48000HZ 8位 =》 44100 16位
    //重采样
    // 假设我们输入了10个数据 ，swrContext转码器 这一次处理了8个数据
    // 那么如果不加delays(上次没处理完的数据) 会造成积压
    int64_t delays = swr_get_delay(swrContext, frame->sample_rate);
    // 将 nb_samples 个数据 由 sample_rate采样率转成 44100 后 返回多少个数据
    // 10  个 48000 = nb 个 44100
    // AV_ROUND_UP : 向上取整 1.1 = 2
    int64_t max_samples = av_rescale_rnd(delays + frame->nb_samples,
                                         out_sample_rate,
                                         frame->sample_rate,
                                         AV_ROUND_UP);
    //上下文+输出缓冲区+输出缓冲区能接受的最大数据量+输入数据+输入数据个数
    //返回 每一个声道的输出数据
    int samples = swr_convert(swrContext,
                              &data,
                              max_samples,
                              (const uint8_t **) frame->data,
                              frame->nb_samples);
    //获得   samples 个   * 2 声道 * 2字节（16位）
    data_size = samples * out_sample_size * out_channels;

    session->audio_clock = frame->pts * av_q2d(time_base);

    session->currentDuration = static_cast<int>(session->audio_clock);

    ReleaseUtils::releaseAvFrame(&frame);
    return new PcmData(data, data_size);
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
        if (!openSlElHelper->isActive) {
            openSlElHelper->isActive = true;
            openSlElHelper->active();
        }
    }
    ReleaseUtils::releaseAvPacket(&packet);
    LOGD("Audio Decode Task Finish");
}
void AudioChannel::runRenderTask() {

}