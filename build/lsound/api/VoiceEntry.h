#ifndef INC_LSOUND_VOICEENTRY_H__
#define INC_LSOUND_VOICEENTRY_H__
/**
@file VoiceEntry.h
@author t-sakai
@date 2013/02/12 create
*/
#include "FLACDecoder.h"

namespace lsound
{
    class VoiceEntry
    {
    public:
        VoiceEntry* next_;
        FLACDecoder decoder_;
    };
}
#endif //INC_LSOUND_VOICEENTRY_H__
