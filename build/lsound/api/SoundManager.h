#ifndef INC_LSOUND_SOUNDMANAGER_H__
#define INC_LSOUND_SOUNDMANAGER_H__
/**
@file SoundManager.h
@author t-sakai
@date 2013/02/12 create
*/
#include <lcore/async/SyncObject.h>
#include <lframework/pack/lpack.h>

#include "lsound.h"
#include "Device.h"
#include "Stream.h"

namespace lsound
{
    class VoiceEntry;
    class FLACFileDecoder;

    class SoundManager
    {
    public:
        static const u32 DefaultSamplesPerSec = 22050;
        //static const u32 DefaultSamplesPerSec = 44100;
        static const u32 DefaultChannels = 2;
        static const u32 DefaultBitsPerSample = 16;
        static const u32 DefaultVoices = 128;

        static const u32 StreamFilePathSize = 128;
        static const u32 DefaultStreams = 4;

        static bool initialize();
        static void destroy();
        static SoundManager& getInstance(){ return *instance_;}
        static bool exist() { return NULL != instance_;}

        bool setPack(const Char* filepath);
        bool setStreamPack(const Char* filepath);

        bool play(u32 index, f32 volume = 1.0f);

        Stream createStream(u32 index);
    private:
        class Impl;
        friend class Impl;

        SoundManager();
        ~SoundManager();

        SoundManager(SoundManager&);
        SoundManager& operator=(SoundManager&);

        void createVoices();
        void releaseVoices();
        VoiceEntry* popVoice();
        void pushVoices(VoiceEntry* entry);

        void pushToStop(VoiceEntry* voice);
        void pushToPlay(VoiceEntry* voice);


        void createStreams();
        void releaseStreams();
        FLACFileDecoder* popStream();
        void pushStream(FLACFileDecoder* entry);

        void processStart();
        void processPlay();
        void processStop();

        static SoundManager* instance_;

        Device device_;
        lpack::PackMemory voicePack_;
        lpack::PackEntry streamPack_;

        Impl *impl_;

        lcore::CriticalSection voicesLock_;
        lcore::CriticalSection startLock_;

        VoiceEntry* voiceTop_;

        VoiceEntry* startTop_;
        VoiceEntry* playTop_;
        VoiceEntry* stopTop_;

        VoiceEntry* voices_;

        Char streamFilePath_[StreamFilePathSize];
        lcore::CriticalSection streamsLock_;
        FLACFileDecoder* streams_;
        FLACFileDecoder** streamPlays_;
    };
}
#endif //INC_LSOUND_SOUNDMANAGER_H__
