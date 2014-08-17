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
        @return �p�f�B���O��̃T�C�Y
        @param length
        @param blockSize ... �u���b�N�T�C�Y
        */
        static s32 getPaddedLength(s32 length, u32 blockSize);

        /**
        @brief �p�f�B���O
        @param data ... getPaddedLength(length)�ȏ�̃T�C�Y
        @param length ... �p�f�B���O�Ȃ��̃f�[�^�T�C�Y
        @param blockSize ... �u���b�N�T�C�Y
        */
        static void padding(u8* data, s32 length, u32 blockSize);

        /**
        @brief �p�f�B���O�O�̃T�C�Y��Ԃ�
        @param data ... �p�f�B���O��̃f�[�^
        @param length ... �p�f�B���O��̃T�C�Y
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
        @param cipher ... dataLength�ȏ�̃T�C�Y
        @param data ... 
        @param dataLength ... data�̃o�C�g�T�C�Y�B16�̔{��
        */
        static s32 encrypt(const AESContext& context, u8* cipher, const u8* data, s32 dataLength);

        /**
        @param context
        @param data ... cipherLength�ȏ�̃T�C�Y
        @param cipher ... 
        @param cipherLength ... cipher�̃o�C�g�T�C�Y�B16�̔{��
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
        @param cipher ... dataLength�ȏ�̃T�C�Y
        @param data ... 
        @param dataLength ... data�̃o�C�g�T�C�Y�B8�̔{��
        */
        static void encrypt(const BlowFishContext& context, u8* cipher, const u8* data, s32 dataLength);

        /**
        @param context
        @param data ... cipherLength�ȏ�̃T�C�Y
        @param cipher ... 
        @param cipherLength ... cipher�̃o�C�g�T�C�Y�B8�̔{��
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
