#ifndef INC_LCORE_AES_H__
#define INC_LCORE_AES_H__
/**
@file AES.h
@author t-sakai
@date 2013/08/09 create
*/
#include "lcore.h"

namespace lcore
{
    class AESContext;

    class AES
    {
    public:
        static const u32 Key128Bytes = 16;
        static const u32 Key192Bytes = 24;
        static const u32 Key256Bytes = 32;
        static const u32 BlockSizeInBytes = 16;

        enum Mode
        {
            Mode_None = -1,
            Mode_ECB = 0,
            Mode_CBC = 1,
        };

        /**
        @param context
        @param cipher ... dataLength以上のサイズ
        @param data ... 
        @param dataLength ... dataのバイトサイズ。16の倍数
        */
        static s32 encrypt(AESContext& context, u8* cipher, const u8* data, s32 dataLength);

        /**
        @param context
        @param data ... cipherLength以上のサイズ
        @param cipher ... 
        @param cipherLength ... cipherのバイトサイズ。16の倍数
        */
        static s32 decrypt(AESContext& context, u8* data, const u8* cipher, s32 cipherLength);

    };

    class AESContext
    {
    public:
        static const u32 BufferSize = 60;

        AESContext();
        AESContext(const u8* IV, const u8* key, u32 keyLength, AES::Mode mode);

        ~AESContext();

        bool isInitialized() const;

        bool initialize(const u8* IV, const u8* key, u32 keyLength, AES::Mode mode);

        u32 Ek_[BufferSize];
        u32 Dk_[BufferSize];
        u32 IV_[4];
        u8 Nr_;
        s8 mode_;
        u8 dummy0_;
        u8 dummy1_;
    };
}
#endif //INC_LCORE_AES_H__
