
#include <jni.h>
#include <string>
#include <math.h>

// Minimal audio streaming using OpenSL.
//
// Loosely based on the Android NDK sample code.
// Hardcoded to 44.1kHz stereo 16-bit audio, because as far as I'm concerned,
// that's the only format that makes any sense.

#include <assert.h>

// for native audio
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#include "native-lib.h"

// This is kinda ugly, but for simplicity I've left these as globals just like in the sample,
// as there's not really any use case for this where we have multiple audio devices yet.

// engine interfaces
static SLObjectItf engineObject;
static SLEngineItf engineEngine;
static SLObjectItf outputMixObject;

// buffer queue player interfaces
static SLObjectItf bqPlayerObject = NULL;
static SLPlayItf bqPlayerPlay;
static SLAndroidSimpleBufferQueueItf bqPlayerBufferQueue;
static SLVolumeItf bqPlayerVolume;

int _BUFFER_SIZE ;
int _BUFFER_SIZE_IN_SAMPLES;

// Double buffering.
static short* buffer[2];
static int curBuffer = 0;

typedef int (*AndroidAudioCallback)(short *buffer, int num_samples);
static AndroidAudioCallback audioCallback;

// This callback handler is called every time a buffer finishes playing.
// The documentation available is very unclear about how to best manage buffers.
// I've chosen to this approach: Instantly enqueue a buffer that was rendered to the last time,
// and then render the next. Hopefully it's okay to spend time in this callback after having enqueued.
static void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq, void *context) {
    assert(bq == bqPlayerBufferQueue);
    assert(NULL == context);

    short *nextBuffer = buffer[curBuffer];
    int nextSize = _BUFFER_SIZE * sizeof(short);

    SLresult result;
    result = (*bqPlayerBufferQueue)->Enqueue(bqPlayerBufferQueue, nextBuffer, nextSize);

    // Comment from sample code:
    // the most likely other result is SL_RESULT_BUFFER_INSUFFICIENT,
    // which for this code example would indicate a programming error
    assert(SL_RESULT_SUCCESS == result);

    curBuffer ^= 1;  // Switch buffer
    // Render to the fresh buffer
    audioCallback(buffer[curBuffer], _BUFFER_SIZE_IN_SAMPLES);
}

// create the engine and output mix objects

// create the engine and output mix objects
bool OpenSLWrap_Init(int framesPerBufferInt, AndroidAudioCallback cb) {
    audioCallback = cb;

    SLresult result;
    // create engine
    result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
    assert(SL_RESULT_SUCCESS == result);
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    assert(SL_RESULT_SUCCESS == result);
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);
    assert(SL_RESULT_SUCCESS == result);
    result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 0, 0, 0);
    assert(SL_RESULT_SUCCESS == result);
    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    assert(SL_RESULT_SUCCESS == result);

    SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 1};
    SLDataFormat_PCM format_pcm = {
            SL_DATAFORMAT_PCM,
            2,
            SL_SAMPLINGRATE_44_1,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
            SL_BYTEORDER_LITTLEENDIAN,

    };

    SLDataSource audioSrc = {&loc_bufq, &format_pcm};

    // configure audio sink
    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSink audioSnk = {&loc_outmix, NULL};

    // create audio player
    const SLInterfaceID ids[2] = {SL_IID_ANDROIDSIMPLEBUFFERQUEUE, SL_IID_VOLUME};
    const SLboolean req[2] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
    result = (*engineEngine)->CreateAudioPlayer(engineEngine, &bqPlayerObject, &audioSrc, &audioSnk, 2, ids, req);
    assert(SL_RESULT_SUCCESS == result);

    result = (*bqPlayerObject)->Realize(bqPlayerObject, SL_BOOLEAN_FALSE);
    assert(SL_RESULT_SUCCESS == result);
    result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_PLAY, &bqPlayerPlay);
    assert(SL_RESULT_SUCCESS == result);
    result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_BUFFERQUEUE,
                                             &bqPlayerBufferQueue);
    assert(SL_RESULT_SUCCESS == result);
    result = (*bqPlayerBufferQueue)->RegisterCallback(bqPlayerBufferQueue, bqPlayerCallback, NULL);
    assert(SL_RESULT_SUCCESS == result);
    result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_VOLUME, &bqPlayerVolume);
    assert(SL_RESULT_SUCCESS == result);

    //result = (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, SL_PLAYSTATE_PLAYING);
    //assert(SL_RESULT_SUCCESS == result);

    // Render and enqueue a first buffer. (or should we just play the buffer empty?)
    curBuffer = 0;

    _BUFFER_SIZE = framesPerBufferInt;
    _BUFFER_SIZE_IN_SAMPLES = _BUFFER_SIZE / 2;

    for (int i = 0; i < 2; i++) {
        buffer[i] = new short[_BUFFER_SIZE];
        for (int j = 0; j < _BUFFER_SIZE; j++) {
            buffer[i][j] = 0; // or only curBuffer
        }
    }
    //audioCallback(buffer[curBuffer], BUFFER_SIZE_IN_SAMPLES);

    result = (*bqPlayerBufferQueue)->Enqueue(bqPlayerBufferQueue, buffer[curBuffer],  _BUFFER_SIZE * sizeof(short) );
    if (SL_RESULT_SUCCESS != result) {
        return false;
    }
    curBuffer ^= 1;
    return true;
}

// shut down the native audio system
void OpenSLWrap_Shutdown() {
    if (bqPlayerObject != NULL) {
        (*bqPlayerObject)->Destroy(bqPlayerObject);
        bqPlayerObject = NULL;
        bqPlayerPlay = NULL;
        bqPlayerBufferQueue = NULL;
        bqPlayerVolume = NULL;
    }
    if (outputMixObject != NULL) {
        (*outputMixObject)->Destroy(outputMixObject);
        outputMixObject = NULL;
    }
    if (engineObject != NULL) {
        (*engineObject)->Destroy(engineObject);
        engineObject = NULL;
        engineEngine = NULL;
    }
    for (int i = 0; i < 2; i++) {
            if (buffer[i] != NULL) {
            delete[] buffer[i];
        }
    }
}



#include "M1AudioPlayer.h"

M1AudioPlayer audioPlayer;
bool isInitialized;

static int MyAndroidAudioCallback(short *buffer, int num_samples)
{
    audioPlayer.Get(buffer, num_samples);


    return 0;
}

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>

#define  MY_LOG_TAG "MYTAG"

extern "C"
JNIEXPORT void JNICALL
Java_com_mach1_example_decode_MainActivity_playAudio(JNIEnv *env, jobject /* this */,
                                                           jobject assetManager, jstring assetName) {

    const char *szAssetName = env->GetStringUTFChars(assetName, NULL);
    AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);
    AAsset* asset = AAssetManager_open(mgr, szAssetName, AASSET_MODE_BUFFER);
    if (NULL == asset) {
        __android_log_print(ANDROID_LOG_ERROR, MY_LOG_TAG, "_ASSET_NOT_FOUND_");
        return;
    }

    env->ReleaseStringUTFChars(assetName, szAssetName);
    int64_t offset, length;
    int fd = AAsset_openFileDescriptor64(asset, &offset, &length);

    uint8_t* asset_buffer = (uint8_t* )AAsset_getBuffer(asset);
    length = 	AAsset_getLength (asset);
    if (asset_buffer == nullptr) {
        printf("Error readingfrom asset manager.");
    }

    audioPlayer.Play(asset_buffer, length);

//    AAsset_close(asset);

    assert(SL_RESULT_SUCCESS == (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, SL_PLAYSTATE_PLAYING));
}

extern "C"
JNIEXPORT void JNICALL
Java_com_mach1_example_decode_MainActivity_stopAudio(
        JNIEnv *env,
        jobject /* this */) {

    audioPlayer.Stop();

    assert(SL_RESULT_SUCCESS == (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, SL_PLAYSTATE_STOPPED));
}

extern "C"
JNIEXPORT void JNICALL
Java_com_mach1_example_decode_MainActivity_setAudioAngles(
        JNIEnv *env,
        jobject /* this */,
        jfloat Yaw, jfloat Pitch, jfloat Roll) {
    audioPlayer.SetAngles(Yaw, Pitch, Roll);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_mach1_example_decode_MainActivity_initAudio(
        JNIEnv *env,
        jobject /* this */,
        jint  framesPerBufferInt ) {
    if(!isInitialized) {
        OpenSLWrap_Init(framesPerBufferInt, MyAndroidAudioCallback);
        isInitialized = true;
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_mach1_example_decode_MainActivity_shutdownAudio(
        JNIEnv *env,
        jobject /* this */) {
    OpenSLWrap_Shutdown();
}
