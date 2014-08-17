#ifndef INC_LCORE_CRYPTOGRAPHY_H__
#define INC_LCORE_CRYPTOGRAPHY_H__
/**
@file Cryptography.h
@author t-sakai
@date 2014/01/06 create
*/
#include "lcore.h"

namespace lcore
{
    //----------------------------------------------------
    //---
    //--- PKCS5
    //---
    //----------------------------------------------------
    class PKCS5
    {
    public:
        /**
        @return パディング後のサイズ
        @param length
        @param blockSize ... ブロックサイズ
        */
        static s32 getPaddedLength(s32 length, u32 blockSize);

        /**
        @brief パディング
        @param data ... getPaddedLength(length)以上のサイズ
        @param length ... パディングなしのデータサイズ
        @param blockSize ... ブロックサイズ
        */
        static void padding(u8* data, s32 length, u32 blockSize);

        /**
        @brief パディング前のサイズを返す
        @param data ... パディング後のデータ
        @param length ... パディング後のサイズ
        */
        static s32 getLengthFromPadded(u8* data, s32 length);

        static bool checkLength(s32 length, u32 blockSize);
    };

    //----------------------------------------------------
    //---
    //--- ARCFOUR
    //---
    //----------------------------------------------------
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


    //----------------------------------------------------
    //---
    //--- AES
    //---
    //----------------------------------------------------
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
        static s32 encrypt(const AESContext& context, u8* cipher, const u8* data, s32 dataLength);

        /**
        @param context
        @param data ... cipherLength以上のサイズ
        @param cipher ... 
        @param cipherLength ... cipherのバイトサイズ。16の倍数
        */
        static s32 decrypt(const AESContext& context, u8* data, const u8* cipher, s32 cipherLength);
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

    //----------------------------------------------------
    //---
    //--- BlowFish
    //---
    //----------------------------------------------------
    class BlowFishContext;

    class BlowFish
    {
    public:
        static const u32 MaxKeyBytes = 56;
        static const s32 NPass = 16;
        static const u32 BlockSizeInBytes = 8;

        struct Bytes
        {
            u8 byte3_;
            u8 byte2_;
            u8 byte1_;
            u8 byte0_;
        };

        union aword
        {
            u32 dword_;
            u8 bytes_[4];
            Bytes w_;
        };

        /**
        @param context
        @param cipher ... dataLength以上のサイズ
        @param data ... 
        @param dataLength ... dataのバイトサイズ。8の倍数
        */
        static void encrypt(const BlowFishContext& context, u8* cipher, const u8* data, s32 dataLength);

        /**
        @param context
        @param data ... cipherLength以上のサイズ
        @param cipher ... 
        @param cipherLength ... cipherのバイトサイズ。8の倍数
        */
        static void decrypt(const BlowFishContext& context, u8* data, const u8* cipher, s32 cipherLength);

    private:
        friend class BlowFishContext;

        static void encrypt(const BlowFishContext& context, u32* xl, u32* xr);
        static void decrypt(const BlowFishContext& context, u32* xl, u32* xr);
    };

    class BlowFishContext
    {
    public:
        void initialize(u8* key, s32 length);

    private:
        friend class BlowFish;

        u32 parray_[18];
        u32 sboxes_[4][256];
    };
}
#endif //INC_LCORE_CRYPTOGRAPHY_H__
