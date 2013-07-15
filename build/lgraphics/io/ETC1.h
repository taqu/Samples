#ifndef INC_LGRAPHICS_ETC1_H__
#define INC_LGRAPHICS_ETC1_H__
/**
@file ETC1.h
@author t-sakai
@date 2011/05/14 create
*/
#include <lgraphics/lgraphicscore.h>

namespace lgraphics
{
    class TextureRef;

namespace etc
{
    static const u32 WeightRError = static_cast<u32>(0xFFU * 0.299f);
    static const u32 WeightGError = static_cast<u32>(0xFFU * 0.587f);
    static const u32 WeightBError = static_cast<u32>(0xFFU * 0.114f);

    static const u32 MaxError = 0xFFFFFFFFU;

    static const s32 NumBitShift = 16;

    struct Texel
    {
        u8 r_;
        u8 g_;
        u8 b_;

        u32 getGrayFixed() const
        {
            return (WeightRError * r_ + WeightGError * g_ + WeightBError * b_);
        }

        void set(u8 r, u8 g, u8 b)
        {
            r_ = r; g_ = g; b_ = b;
        }
    };

    struct TexelFixedPoint
    {
        static const s32 NumBitShift = 16;
        static const s32 NumHalfShift = 8;

        u32 r_;
        u32 g_;
        u32 b_;

        void set(u32 r, u32 g, u32 b)
        {
            r_ = r; g_ = g; b_ = b;
        }

        void setShift(u32 r, u32 g, u32 b)
        {
            r_ = (r << NumBitShift);
            g_ = (g << NumBitShift);
            b_ = (b << NumBitShift);
        }

        void setShift(u32 r, u32 g, u32 b, s32 shift)
        {
            r_ = (r << shift);
            g_ = (g << shift);
            b_ = (b << shift);
        }

        void lshift(s32 v)
        {
            r_ <<= v; g_ <<= v; b_ <<= v;
        }

        void rshift(s32 v)
        {
            r_ >>= v; g_ >>= v; b_ >>= v;
        }

        void add(u32 rhs)
        {
            r_ += rhs;
            g_ += rhs;
            b_ += rhs;
        }

        void mul(u32 rhs)
        {
            r_ *= rhs;
            g_ *= rhs;
            b_ *= rhs;
            r_ >>= NumBitShift;
            g_ >>= NumBitShift;
            b_ >>= NumBitShift;
        }

        void get(u8& r, u8& g, u8& b)
        {
            r = static_cast<u8>(r_ >> NumBitShift);
            g = static_cast<u8>(g_ >> NumBitShift);
            b = static_cast<u8>(b_ >> NumBitShift);
        }
    };


#if 1
    class ETC1Codec
    {
    public:
        ETC1Codec(){}
        ~ETC1Codec(){}

        static u32 calcEncodedSize(u32 width, u32 height);

        //-----------------------------------------------------
        /**
        @brief ミップマップ作成
        @param src ...
        @param width ... 
        @param height ... 
        @param format ... 

        フルレベルでミップマップ作成。バッファを破壊するので注意。
        */
        void encodeMipMap(TextureRef& dst, u8* src, u32 width, u32 height, u32 levels);

        //-----------------------------------------------------
        /**
        @brief エンコード
        @param src ...
        @param width ... 
        @param height ... 
        */
        void encode(TextureRef& dst, const u8* src, u32 width, u32 height);

        //-----------------------------------------------------------------------------
        bool encode(u8* dst, const u8* src, u32 width, u32 height);

    private:
        inline void getTexel(const u8* src, u32 sx, u32 sy);
        inline void getTexelPartial(const u8* src, u32 sx, u32 ex, u32 sy, u32 ey);

        inline f32 sqr(f32 v) const
        {
            return v*v;
        }

        inline s32 sqr(s32 v) const
        {
            return v*v;
        }


        //---------------------------------------------------------------------
        void encodeBlock(u8*& dst);

        //---------------------------------------------------------------------
        u32 encodeBlock2x4(u8* dst);

        //---------------------------------------------------------------------
        u32 encodeBlock2x4avg(u8* dst);

        //---------------------------------------------------------------------
        u32 encodeBlock4x2(u8* dst);

        //---------------------------------------------------------------------
        u32 encodeBlock4x2avg(u8* dst);

        //---------------------------------------------------------------------
        void calcAvg2x4(etc::TexelFixedPoint& texel, u32 sx, u32 ex);

        //---------------------------------------------------------------------
        void calcAvg4x2(etc::TexelFixedPoint& texel, u32 sx);

        //---------------------------------------------------------------------
        void quantizeTo444(u8* encoded, const TexelFixedPoint& avg);

        //---------------------------------------------------------------------
        void quantizeTo555(u8* encoded, const TexelFixedPoint& avg);

        void quantize(u8* encoded, const TexelFixedPoint& avg, const s32* low, const s32* high, const u32* lbit, const u32* hbit);

        u32 encode2x4(u8* dst, u8& min_table, const u8* rgb, u32 sx, u32 ex);
        u32 encode2x4(u8* msb, u8* lsb, u8 table, const u8* rgb, u32 sx, u32 ex);

        u32 encode4x2(u8* dst, u8& min_table, const u8* rgb, u32 sx);
        u32 encode4x2(u8* msb, u8* lsb, u8 table, const u8* rgb, u32 sx);

        u32 width_, height_;
        Texel texels_[16];
    };

#else
    /***********************************************************************/
    /***                                                                   */
    /*** Old                                                               */
    /***                                                                   */
    /***********************************************************************/

    struct TexelF32
    {
        f32 r_;
        f32 g_;
        f32 b_;

        f32 getGray() const
        {
            return WeightR * r_ + WeightG * g_ + WeightB * b_;
        }

        void set(u8 r, u8 g, u8 b)
        {
            r_ = r; g_ = g; b_ = b;
        }
        void mul(f32 v)
        {
            r_ *= v;
            g_ *= v;
            b_ *= v;
        }
    };

    class ETC1Codec
    {
    public:
        static const f32 OneEight;

        ETC1Codec(){}
        ~ETC1Codec(){}

        static u32 calcEncodedSize(u32 width, u32 height);

        //-----------------------------------------------------
        /**
        @brief ミップマップ作成
        @param src ...
        @param width ... 
        @param height ... 
        @param format ... 

        フルレベルでミップマップ作成。convertToPow2Imageで２のべき乗に変換する。
        バッファを破壊するので注意。
        */
        void encodeMipMap(TextureRef& dst, u8* src, u32 width, u32 height, u32 levels);

        //-----------------------------------------------------------------------------
        bool encode(u8* dst, const u8* src, u32 width, u32 height);

    private:
        inline void getTexel(const u8* src, u32 sx, u32 sy);
        inline void getTexelPartial(const u8* src, u32 sx, u32 ex, u32 sy, u32 ey);

        inline f32 sqr(f32 v) const
        {
            return v*v;
        }

        inline s32 sqr(s32 v) const
        {
            return v*v;
        }


        //---------------------------------------------------------------------
        void encodeBlock(u8*& dst);

        //---------------------------------------------------------------------
        f32 encodeBlock2x4(u8* dst);

        //---------------------------------------------------------------------
        f32 encodeBlock2x4avg(u8* dst);

        //---------------------------------------------------------------------
        f32 encodeBlock4x2(u8* dst);

        //---------------------------------------------------------------------
        f32 encodeBlock4x2avg(u8* dst);

        //---------------------------------------------------------------------
        void calcAvg2x4(TexelF32& texel, u32 sx, u32 ex);

        //---------------------------------------------------------------------
        void calcAvg4x2(TexelF32& texel, u32 sx, u32 ex);

        //---------------------------------------------------------------------
        void quantizeTo444(u8* encoded, const TexelF32& avg);

        //---------------------------------------------------------------------
        void quantizeTo555(u8* encoded, const TexelF32& avg);

        void quantize(u8* encoded, const TexelF32& avg, const s32* low, const s32* high, const s32* lbit, const s32* hbit);

        f32 encode2x4(u8* dst, u8& min_table, const u8* rgb, u32 sx, u32 ex);
        f32 encode2x4(u8* msb, u8* lsb, u8 table, const u8* rgb, u32 sx, u32 ex);

        f32 encode4x2(u8* dst, u8& min_table, const u8* rgb, u32 sx, u32 ex);
        f32 encode4x2(u8* msb, u8* lsb, u8 table, const u8* rgb, u32 sx, u32 ex);

        u32 width_, height_;
        Texel texels_[16];
        GrayStaticPoint grays_[16];
    };
#endif
}
}
#endif //INC_LGRAPHICS_ETC1_H__
