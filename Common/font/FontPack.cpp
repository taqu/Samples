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
    s32 SJISToU16(u16& code, const Char* s)
    {
        u8 c = *((u8*)&(s[0]));
        code = c;

        if((0x81U<=c && c<=0x9FU)
            || (0xE0U<=c && c<=0xFCU))
        {
            c = *((u8*)&(s[1]));
            if((0x40U<=c && c<=0xFCU)){
                code |= (c<<8);
                return 2;
            }
        }
        return 1;
    }

    FontPack::FontPack()
        :numGlyphs_(0)
        ,glyphs_(NULL)
        ,codeToGlyphInfo_(NULL)
    {
    }

    FontPack::~FontPack()
    {
        LIME_DELETE(codeToGlyphInfo_);
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
        header_.numCodes_ = tmpHeader.numCodes_;
        header_.distanceField_ = (tmpHeader.distanceField_ != 0);

        //頂点のUVは符号付16ビットへパックするため、パック比率をあらけじめ計算
        header_.codeRatioX_ = 32767.0f/tmpHeader.resolutionX_;
        header_.codeRatioY_ = 32767.0f/tmpHeader.resolutionY_;

        numGlyphs_ = tmpHeader.numCodes_;
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

        lcore::u8* buff = (lcore::u8*)LIME_MALLOC(size);
        lcore::io::read(is, buff, size);
        bool ret = lgraphics::io::IODDS::read(texture_, buff, size, lgraphics::Usage_Immutable, lgraphics::TexFilter_MinMagMipLinear, lgraphics::TexAddress_Clamp);
        LIME_FREE(buff);

        //マップ作成
        LIME_DELETE(codeToGlyphInfo_);
        u16 emptyKey = 0;
        codeToGlyphInfo_ = LIME_NEW CodeToGlyphInfo(numGlyphs_*2, emptyKey);
        for(u32 i=0; i<numGlyphs_; ++i){
            codeToGlyphInfo_->insert(static_cast<u16>(glyphs_[i].code_), &glyphs_[i]);
        }
        return ret;
    }

    const GlyphInfo* FontPack::get(lcore::u16 code) const
    {
        LASSERT(NULL != codeToGlyphInfo_);
        CodeToGlyphInfo::size_type pos = codeToGlyphInfo_->find(code);
        return (pos != codeToGlyphInfo_->end())? codeToGlyphInfo_->getValue(pos) : NULL;
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
