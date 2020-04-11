//
// Created by Administrator on 2020\4\8 0008.
//
#include <cstdlib>
#include "SLES/OpenSLES.h"
#include "OpenSlElHelper.h"
#include "SLES/OpenSLES_Android.h"
#include "jni.h"

void playerCallBack(SLAndroidSimpleBufferQueueItf queue, void *context);

OpenSlElHelper::OpenSlElHelper(GetPcmCallBack *callBack) {
    this->callBack = callBack;
    /**
      * 1、创建引擎并获取引擎接口
      */
    SLresult result;
    // 1.1 创建引擎 SLObjectItf engineObject
    result = slCreateEngine(&audioEngine, 0, NULL, 0, NULL, NULL);
    if (SL_RESULT_SUCCESS != result) {
        return;
    }
    // 1.2 初始化引擎  init
    result = (*audioEngine)->Realize(audioEngine, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != result) {
        return;
    }
    // 1.3 获取引擎接口SLEngineItf engineInterface
    result = (*audioEngine)->GetInterface(audioEngine, SL_IID_ENGINE,
                                          &audioEngineInterface);
    if (SL_RESULT_SUCCESS != result) {
        return;
    }

    /**
     * 2、设置混音器
     */
    // 2.1 创建混音器SLObjectItf outputMixObject
    result = (*audioEngineInterface)->CreateOutputMix(audioEngineInterface, &outputMixObject, 0,
                                                      0, 0);
    if (SL_RESULT_SUCCESS != result) {
        return;
    }
    // 2.2 初始化混音器outputMixObject
    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != result) {
        return;
    }

    /**
     * 3、创建播放器
     */
    //3.1 配置输入声音信息
    //创建buffer缓冲类型的队列 2个队列
    SLDataLocator_AndroidSimpleBufferQueue android_queue = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,
                                                            2};
    //pcm数据格式
    //pcm+2(双声道)+44100(采样率)+ 16(采样位)+16(数据的大小)+LEFT|RIGHT(双声道)+小端数据
    SLDataFormat_PCM pcm = {SL_DATAFORMAT_PCM, 2, SL_SAMPLINGRATE_44_1, SL_PCMSAMPLEFORMAT_FIXED_16,
                            SL_PCMSAMPLEFORMAT_FIXED_16,
                            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
                            SL_BYTEORDER_LITTLEENDIAN};

    //数据源 将上述配置信息放到这个数据源中
    SLDataSource slDataSource = {&android_queue, &pcm};

    //3.2  配置音轨(输出)
    //设置混音器
    SLDataLocator_OutputMix outputMix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSink audioSnk = {&outputMix, NULL};
    //需要的接口  操作队列的接口
    const SLInterfaceID ids[1] = {SL_IID_BUFFERQUEUE};
    const SLboolean req[1] = {SL_BOOLEAN_TRUE};
    //3.3 创建播放器
    (*audioEngineInterface)->CreateAudioPlayer(audioEngineInterface, &playerObject, &slDataSource,
                                               &audioSnk, 1,
                                               ids, req);
    //初始化播放器
    (*playerObject)->Realize(playerObject, SL_BOOLEAN_FALSE);

    //得到接口后调用  获取Player接口
    (*playerObject)->GetInterface(playerObject, SL_IID_PLAY, &playerInterface);
    /**
    * 4、设置播放回调函数
    */
    //获取播放器队列接口
    (*playerObject)->GetInterface(playerObject, SL_IID_BUFFERQUEUE,
                                  &bufferQueue);
    //设置回调
    (*bufferQueue)->RegisterCallback(bufferQueue, playerCallBack, this);

    /**
     * 5、设置播放状态
    */
    (*playerInterface)->SetPlayState(playerInterface, SL_PLAYSTATE_PLAYING);

    /**
     * 6、手动激活一下这个回调
     */
    playerCallBack(bufferQueue, this);
}
OpenSlElHelper::~OpenSlElHelper() {
}
void playerCallBack(SLAndroidSimpleBufferQueueItf queue, void *context) {
    OpenSlElHelper *openSlElHelper = static_cast<OpenSlElHelper *>(context);
    PcmData *pcmData = openSlElHelper->callBack->getPcmData();
    if (pcmData && pcmData->dataSize > 0) {
        (*queue)->Enqueue(queue, pcmData->data, pcmData->dataSize);
    }
}