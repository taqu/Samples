#ifndef INC_FONTPACK_H__
#define INC_FONTPACK_H__
/**
@file FontPack.h
@author t-sakai
@date 2011/09/24 create
*/
#include <lcore/HashMap.h>

#include <lgraphics/api/TextureRef.h>
#include "FontType.h"

namespace lcore
{
    class istream;
}

namespace font
{
    /// フォントファイルヘッダ
    struct PackHeaderTmp
    {
        s32 textHeight_;
        s32 spaceWidth_;
        u32 numCodes_;
        s32 resolutionX_;
        s32 resolutionY_;
        s32 distanceField_;
    };

    /// フォントファイルヘッダ
    struct PackHeader
    {
        s32 textHeight_;
        s32 spaceWidth_;
        u32 numCodes_;
        f32 codeRatioX_;
        f32 codeRatioY_;
        bool distanceField_;
    };

    /// 文字ひとつの情報
    struct GlyphInfo
    {
        u32 code_;
        s16 x_;
        s16 y_;
        s16 width_;
        s16 fitWidth_;
        s16 fitHeight_;
        s16 fitWCoded_;
        s16 fitHCoded_;
        s8 offsetX_;
        s8 offsetY_;
    };

    s32 SJISToU16(u16& code, const Char* s);

    /// ビットマップフォント
    class FontPack
    {
    public:
        FontPack();
        ~FontPack();

        /**
        */
        bool load(lcore::istream& is);

        /**
        */
        inline const PackHeader& getHeader() const;

        /**
        */
        inline lcore::u32 getNumGlyphs() const;

        /**
        */
        const GlyphInfo* get(lcore::u16 code) const;

        /**
        */
        inline lgraphics::Texture2DRef& getTexture();

        s32 getTextHeight() const{ return header_.textHeight_;}

        /**
        */
        void swap(FontPack& rhs);

        inline bool distanceField() const;
    private:
        FontPack(const FontPack&);
        FontPack& operator=(const FontPack&);

        typedef lcore::HashMap<u16, GlyphInfo*> CodeToGlyphInfo;

        PackHeader header_; /// ファイルヘッダ
        lcore::u32 numGlyphs_; /// 文字数
        GlyphInfo* glyphs_; /// 文字情報
        CodeToGlyphInfo* codeToGlyphInfo_;

        lgraphics::Texture2DRef texture_; /// テクスチャ
    };

    inline const PackHeader& FontPack::getHeader() const
    {
        return header_;
    }

    inline lcore::u32 FontPack::getNumGlyphs() const
    {
        return numGlyphs_;
    }

    inline lgraphics::Texture2DRef& FontPack::getTexture()
    {
        return texture_;
    }

    inline bool FontPack::distanceField() const
    {
        return header_.distanceField_;
    }
}
#endif //INC_FONTPACK_H__
