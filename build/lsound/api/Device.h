#ifndef INC_LSOUND_DEVICE_H__
#define INC_LSOUND_DEVICE_H__
/**
@file Device.h
@author t-sakai
@date 2012/08/01 create

*/
#include <lcore/async/SyncObject.h>

#include "../Type.h"
#include "Voice.h"

struct IXAudio2;
struct IXAudio2MasteringVoice;
struct IXAudio2VoiceCallback;

namespace lsound
{
    class Device
    {
    public:
        Device();
        ~Device();

        bool create(u32 numChannels, u32 samplesPerSec);
        bool createVoice(Voice& voice, const WaveFormat& format, IXAudio2VoiceCallback* callback);

        f32 getVolume();
        void setVolume(f32 volume);
    private:

        Device(const Device&);
        Device& operator=(const Device&);

        void release();

        IXAudio2* device_;
        IXAudio2MasteringVoice* masteringVoice_;
    };
}
#endif //INC_LSOUND_DEVICE_H__
