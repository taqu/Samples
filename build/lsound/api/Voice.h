#ifndef INC_LSOUND_VOICE_H__
#define INC_LSOUND_VOICE_H__
/**
@file Voice.h
@author t-sakai
@date 2012/08/02 create

*/
#include <xaudio2.h>
#include "../Type.h"

namespace lsound
{
    class Voice
    {
    public:
        inline Voice();
        inline explicit Voice(IXAudio2SourceVoice* voice);
        inline ~Voice();

        inline bool start();
        inline bool stop();
        inline bool flush();

        inline f32 getVolume();
        inline void setVolume(f32 volume);

        inline void getState(XAUDIO2_VOICE_STATE& state);

        inline void submit(const XAUDIO2_BUFFER& buffer);

        inline void swap(Voice& rhs);
    private:
        Voice(const Voice& rhs);
        Voice& operator=(const Voice&);

        inline void release();

        IXAudio2SourceVoice* voice_;
    };

    inline Voice::Voice()
        :voice_(NULL)
    {}

    inline Voice::Voice(IXAudio2SourceVoice* voice)
        :voice_(voice)
    {
    }

    inline Voice::~Voice()
    {
        release();
    }

    inline bool Voice::start()
    {
        LASSERT(NULL != voice_);
        HRESULT hr = voice_->Start(0, XAUDIO2_COMMIT_NOW);
        return SUCCEEDED(hr);
    }

    inline bool Voice::stop()
    {
        LASSERT(NULL != voice_);
        HRESULT hr = voice_->Stop(0, XAUDIO2_COMMIT_NOW);
        return SUCCEEDED(hr);
    }

    inline bool Voice::flush()
    {
        LASSERT(NULL != voice_);
        HRESULT hr = voice_->FlushSourceBuffers();
        return SUCCEEDED(hr);
    }

    inline f32 Voice::getVolume()
    {
        LASSERT(NULL != voice_);
        f32 volume = 0.0f;
        voice_->GetVolume(&volume);
        return volume;
    }

    inline void Voice::setVolume(f32 volume)
    {
        LASSERT(NULL != voice_);
        voice_->SetVolume(volume, XAUDIO2_COMMIT_NOW);
    }

    inline void Voice::getState(XAUDIO2_VOICE_STATE& state)
    {
        LASSERT(NULL != voice_);
        voice_->GetState(&state);
    }

    inline void Voice::submit(const XAUDIO2_BUFFER& buffer)
    {
        LASSERT(NULL != voice_);
        voice_->SubmitSourceBuffer(&buffer);
    }

    inline void Voice::swap(Voice& rhs)
    {
        lcore::swap(voice_, rhs.voice_);
    }

    inline void Voice::release()
    {
        if(NULL != voice_){
            voice_->Stop();
            voice_->DestroyVoice();
            voice_ = NULL;
        }
    }
}
#endif //INC_LSOUND_VOICE_H__
