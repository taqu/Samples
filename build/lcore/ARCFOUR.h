#ifndef INC_LCORE_ARCFOUR_H__
#define INC_LCORE_ARCFOUR_H__
/**
@file ARCFOUR.h
@author t-sakai
@date 2013/08/09 create
*/
#include "lcore.h"

namespace lcore
{
    class ARCFOURContext;

    class ARCFOUR
    {
    public:
        static const u32 StateSize = 256;
        static const u32 StateMask = 0xFFU;

        static s32 encrypt(const ARCFOURContext& context, u8* data, s32 dataLength);
        static s32 decrypt(const ARCFOURContext& context, u8* cipher, s32 cipherLength);
    };


    class ARCFOURContext
    {
    public:
        ARCFOURContext();
        ARCFOURContext(u8* key, u32 length);
        ~ARCFOURContext();

        void initialize(u8* key, u32 length);

        u8 s_[ARCFOUR::StateSize];
    };
}
#endif //INC_LCORE_ARCFOUR_H__
