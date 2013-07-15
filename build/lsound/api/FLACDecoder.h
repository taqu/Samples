#ifndef INC_LSOUND_FLACDECODER_H__
#define INC_LSOUND_FLACDECODER_H__
/**
@file FLACDecoder.h
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
    class FLACDecoder : public FLAC::Decoder::Stream, public IXAudio2VoiceCallback
    {
    public:
        FLACDecoder();
        ~FLACDecoder();

        bool initialize();
        void cleanup();

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

        void setData(lcore::u32 size, const lcore::u8* data);

        void setEnd(bool isEnd);
        bool isEnd();

        bool ignoreErrors_;
        bool errorOccurred_;
        bool isEnd_;

        lcore::CriticalSection endLock_;

        Voice voice_;

        lcore::u32 size_;
        lcore::u32 currentPos_;
        const lcore::u8* data_;
        u32 currentBuffer_;
        XAUDIO2_BUFFER xaudio2Buffers_[DefaultBufferNum];
        s16 buffers_[DefaultBufferNum][DefaultBufferSize];
    };

}

#endif //INC_LSOUND_FLACDECODER_H__
