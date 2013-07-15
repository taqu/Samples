#ifndef INC_SOUND_TYPE_H__
#define INC_SOUND_TYPE_H__
/**
@file Type.h
@author t-sakai
@date 2012/08/01 create

*/
#include <lcore/lcore.h>


namespace lsound
{
#define SOUND_SAFERELEASE(ptr) if((ptr)){(ptr)->Release();(ptr)=NULL;}


    using lcore::s8;
    using lcore::s16;
    using lcore::s32;
    using lcore::u8;
    using lcore::u16;
    using lcore::u32;
    using lcore::f32;
    using lcore::f64;

    using lcore::Char;

    static const u32 DefaultBufferSize = 1152*2;
    static const u32 DefaultBufferNum = 2;

    static const u32 DefaultStreamBufferSize = 4096;
    static const u32 DefaultStreamBufferNum = 2;

    struct WaveFormat
    {
        u16 format_;
        u16 channels_;
        u32 samplesPerSec_;
        u32 avgBytesPerSec_;
        u16 blockAlign_;
        u16 bitsPerSample_;
        u16 extraSize_;
    };
}

#endif //INC_SOUND_TYPE_H__
