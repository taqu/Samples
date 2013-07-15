#ifndef INC_LSOUND_FLACFILEDECODER_H__
#define INC_LSOUND_FLACFILEDECODER_H__
/**
@file FLACFileDecoder.h
@author t-sakai
@date 2013/02/12 create
*/
#include "../Type.h"

#include <xaudio2.h>
#include <FLAC++/decoder.h>

#include <lcore/async/SyncObject.h>

#include "Voice.h"

namespace lsound
{
    class FLACFileDecoder : public FLAC::Decoder::Stream, public IXAudio2VoiceCallback
    {
    public:
        enum State
        {
            State_Play,
            State_Pause,
            State_Stop,
            State_End,
            State_Loop,
        };

        FLACFileDecoder();
        ~FLACFileDecoder();

        bool open(const Char* filepath);
        void close();

        bool initialize(s32 offset, u32 size);

        bool initData();
        bool update();

        bool isLoop();
        void setLoop(bool loop);

        COM_DECLSPEC_NOTHROW void STDMETHODCALLTYPE OnStreamEnd(void);

        COM_DECLSPEC_NOTHROW void STDMETHODCALLTYPE OnVoiceProcessingPassEnd();
        COM_DECLSPEC_NOTHROW void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 samplesRequired);
        COM_DECLSPEC_NOTHROW void STDMETHODCALLTYPE OnBufferStart(void * bufferContext);
        COM_DECLSPEC_NOTHROW void STDMETHODCALLTYPE OnBufferEnd(void * bufferContext);
        COM_DECLSPEC_NOTHROW void STDMETHODCALLTYPE OnLoopEnd(void * bufferContext);
        COM_DECLSPEC_NOTHROW void STDMETHODCALLTYPE OnVoiceError(void * bufferContext, HRESULT error);


        ::FLAC__StreamDecoderReadStatus read_callback(FLAC__byte buffer[], size_t *bytes);
        ::FLAC__StreamDecoderSeekStatus seek_callback(FLAC__uint64 absolute_byte_offset);
        ::FLAC__StreamDecoderTellStatus tell_callback(FLAC__uint64 *absolute_byte_offset);
        ::FLAC__StreamDecoderLengthStatus length_callback(FLAC__uint64 *stream_length);
        bool eof_callback();

        ::FLAC__StreamDecoderWriteStatus write_callback(const ::FLAC__Frame *frame, const FLAC__int32 * const buffer[]);
        void metadata_callback(const ::FLAC__StreamMetadata *metadata);
        void error_callback(::FLAC__StreamDecoderErrorStatus status);

        void setState(State state);
        State getState();

        bool ignoreErrors_;
        bool errorOccurred_;
        bool loop_;
        State state_;

        lcore::CriticalSection loopLock_;
        lcore::CriticalSection stateLock_;

        Voice voice_;

        u32 currentFilePos_;
        s32 fileOffset_;
        u32 fileSize_;
        HANDLE file_;

        u32 numBuffers_;
        u32 currentBuffer_;
        XAUDIO2_BUFFER xaudio2Buffers_[DefaultStreamBufferNum];
        s16 buffers_[DefaultStreamBufferNum][DefaultStreamBufferSize*2];
    };

}

#endif //INC_LSOUND_FLACFILEDECODER_H__
