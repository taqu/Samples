/**
@file FontPack.cpp
@author t-sakai
@date 2011/09/24
*/
#include "../stdafx.h"
#include "FontPack.h"

#include <lcore/liostream.h>
#include <lgraphics/io11/IODDS.h>

namespace font
{
    FontPack::FontPack()
        :numGlyphs_(0)
        ,glyphs_(NULL)
    {
    }

    FontPack::~FontPack()
    {
        LIME_DELETE_ARRAY(glyphs_);
    }

    //-------------------------------------------------------
    // ロード
    bool FontPack::load(lcore::istream& is)
    {
        PackHeaderTmp tmpHeader;
        lcore::u32 count = lcore::io::read(is, tmpHeader);
        if(count < 1){
            return false;
        }
        header_.textHeight_ = tmpHeader.textHeight_;
        header_.spaceWidth_ = tmpHeader.spaceWidth_;
        header_.startCode_ = tmpHeader.startCode_;
        header_.endCode_ = tmpHeader.endCode_;
        header_.distanceField_ = (tmpHeader.distanceField_ != 0);

        //頂点のUVは符号付16ビットへパックするため、パック比率をあらけじめ計算
        header_.codeRatioX_ = 32767.0f/tmpHeader.resolutionX_;
        header_.codeRatioY_ = 32767.0f/tmpHeader.resolutionY_;

        numGlyphs_ = header_.endCode_ - header_.startCode_ + 1;
        lcore::u32 total = sizeof(GlyphInfo) * numGlyphs_;

        LIME_DELETE_ARRAY(glyphs_);
        glyphs_ = LIME_NEW GlyphInfo[numGlyphs_];
        count = is.read(reinterpret_cast<lcore::Char*>(glyphs_), total);
        if(count<1){
            return false;
        }
        
        lcore::s32 pos = is.tellg();
        is.seekg(0, lcore::ios::end);
        lcore::u32 size = is.tellg() - pos;
        is.seekg(pos, lcore::ios::beg);

        lcore::s8* buff = (lcore::s8*)LIME_MALLOC(size);
        is.read(buff, size);

        bool ret = lgraphics::io::IODDS::read(texture_, buff, size, lgraphics::Usage_Immutable, lgraphics::TexFilter_MinMagMipLinear, lgraphics::TexAddress_Clamp);
        LIME_FREE(buff);
        return ret;
    }

    //-------------------------------------------------------
    // スワップ
    void FontPack::swap(FontPack& rhs)
    {
        lcore::swap(header_, rhs.header_);
        lcore::swap(glyphs_, rhs.glyphs_);
        texture_.swap(rhs.texture_);
    }
}
