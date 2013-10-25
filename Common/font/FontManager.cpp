/**
@file FontManager.cpp
@author t-sakai
@date 2011/09/24
*/
#include <stdarg.h>
#include "FontManager.h"

#include "FontType.h"
#include "FontPack.h"
#include "FontShader.h"

#include <lmath/Vector2.h>
#include <lmath/Matrix44.h>
#include <lgraphics/lgraphics.h>
#include <lgraphics/api/BlobRef.h>
#include <lgraphics/api/InputLayoutRef.h>
#include <lgraphics/api/BlendStateRef.h>
#include <lgraphics/api/VertexBufferRef.h>

namespace font
{
    namespace
    {
        /// １フォントあたりの最大描画文字数
        static const u32 NumVertices = MaxCharsInFont*6;

        /// 文字描画用頂点
        struct Vertex
        {
            lmath::Vector2 position_;
            s16 uv_[2];
        };

        /// 描画データ
        class FontRenderer
        {
        public:

            FontRenderer()
                :numChars_(0)
            {
                buffer_ = lgraphics::VertexBuffer::create(
                    sizeof(Vertex)*NumVertices,
                    lgraphics::Usage_Dynamic,
                    lgraphics::CPUAccessFlag_Write,
                    lgraphics::ResourceMisc_None);
            }

            void swap(FontRenderer& rhs)
            {
                fontPack_.swap(rhs.fontPack_);
            }

            void update()
            {
                lgraphics::MappedSubresource mapped;
                if(buffer_.map(0, lgraphics::MapType_WriteDiscard, mapped)){
                    lcore::memcpy(mapped.data_, vertices_, sizeof(Vertex)*numChars_*6);
                    buffer_.unmap(0);
                }
            }

            FontPack fontPack_;
            u32 numChars_;
            Vertex vertices_[NumVertices];
            lgraphics::VertexBufferRef buffer_;
        };
    }


    //---------------------------------------
    //---
    //--- FontManager::Impl
    //---
    //---------------------------------------
    /// 処理本体
    class FontManager::Impl
    {
    public:
        static const u32 DefaultIncrement = 2;

        Impl();
        ~Impl();

        void initialize();
        void terminate();

        void setResolution(u32 width, u32 height);
        u32 getNumFonts() const{ return numFonts_;}
        bool load(lcore::istream& is);
        void expand();

        void print(u32 fontID, u32 x, u32 y, const char* str);
        void print(u32 fontID, f32 x, f32 y, f32 scale, const char* str);

        void draw();
        void draw(u32 fontID);

        ConstantBuffer constants_;

        lgraphics::InputLayoutRef layout_;
        lgraphics::BlendStateRef blend_;

        ShaderVS vs_;
        ShaderPS ps_;

        ShaderPS dfPS_;

        u16 capacity_;
        u16 numFonts_;
        FontRenderer* fonts_;
    };

    FontManager::Impl::Impl()
        :capacity_(0)
        ,numFonts_(0)
        ,fonts_(NULL)
    {
        constants_.xparam_.zero();
        constants_.yparam_.zero();
    }

    FontManager::Impl::~Impl()
    {
        LIME_DELETE_ARRAY(fonts_);
    }

    void FontManager::Impl::initialize()
    {
        lgraphics::BlobRef blob;
        createShader(vs_, blob);
        createShader(ps_);
        createDistanceFieldShader(dfPS_);

        D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R16G16_SNORM, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        layout_ = lgraphics::InputLayout::create(layout, 2, blob.getPointer(), blob.getSize());

        blend_ = lgraphics::BlendState::create(
            FALSE,
            TRUE,
            lgraphics::Blend_SrcAlpha,
            lgraphics::Blend_InvSrcAlpha,
            lgraphics::BlendOp_Add,
            lgraphics::Blend_SrcAlpha,
            lgraphics::Blend_DestAlpha,
            lgraphics::BlendOp_Add,
            lgraphics::ColorWrite_All);
    }

    void FontManager::Impl::terminate()
    {
    }

    void FontManager::Impl::setResolution(u32 width, u32 height)
    {
        constants_.xparam_.set(2.0f/width, 0.0f, 0.0f, -1.0f);
        constants_.yparam_.set(0.0f, -2.0f/height, 0.0f, 1.0f);
    }


    bool FontManager::Impl::load(lcore::istream& is)
    {

        if(capacity_<=numFonts_){
            expand();
        }
        if(fonts_[numFonts_].fontPack_.load(is)){
            ++numFonts_;
            return true;
        }
        return false;
    }

    void FontManager::Impl::expand()
    {
        capacity_ += DefaultIncrement;

        FontRenderer* tmp = LIME_NEW FontRenderer[capacity_];
        for(u16 i=0; i<numFonts_; ++i){
            tmp[i].swap(fonts_[i]);
        }
        LIME_DELETE_ARRAY(fonts_);
        fonts_ = tmp;
    }

    void FontManager::Impl::print(u32 fontID, u32 x, u32 y, const char* str)
    {
        LASSERT(0<=fontID && fontID<numFonts_);
        LASSERT(str != NULL);

        FontRenderer& font = fonts_[fontID];
        const PackHeader& header = font.fontPack_.getHeader();

        u32 sx = x;
        while(*str != '\0'){
            if(font.numChars_>= MaxCharsInFont){
                break;
            }

            if(*str == '\n'){
                x = sx;
                y += header.textHeight_;
                ++str;
                continue;
            }

            u16 code;
            s32 bytes = SJISToU16(code, str);

            const GlyphInfo* glyph = font.fontPack_.get(code);

            if(NULL == glyph){
                x += header.spaceWidth_;

            }else{
                u32 index = font.numChars_ * 6;
                Vertex& v0 = font.vertices_[ index+0 ];
                Vertex& v1 = font.vertices_[ index+1 ];
                Vertex& v2 = font.vertices_[ index+2 ];
                Vertex& v3 = font.vertices_[ index+3 ];
                Vertex& v4 = font.vertices_[ index+4 ];
                Vertex& v5 = font.vertices_[ index+5 ];

                v0.position_.x_ = static_cast<f32>(x + glyph->offsetX_);
                v0.position_.y_ = static_cast<f32>(y + glyph->offsetY_);
                v0.uv_[0] = static_cast<s16>(glyph->x_ * header.codeRatioX_);
                v0.uv_[1] = static_cast<s16>(glyph->y_ * header.codeRatioY_);

                v1.position_.x_ = v0.position_.x_ + glyph->fitWidth_;
                v1.position_.y_ = v0.position_.y_;
                v1.uv_[0] = v0.uv_[0] + glyph->fitWCoded_;
                v1.uv_[1] = v0.uv_[1];

                v2.position_.x_ = v0.position_.x_;
                v2.position_.y_ = v0.position_.y_ + glyph->fitHeight_;
                v2.uv_[0] = v0.uv_[0];
                v2.uv_[1] = v0.uv_[1] + glyph->fitHCoded_;

                v3.position_ = v2.position_;
                v3.uv_[0] = v2.uv_[0];
                v3.uv_[1] = v2.uv_[1];

                v4.position_ = v1.position_;
                v4.uv_[0] = v1.uv_[0];
                v4.uv_[1] = v1.uv_[1];

                v5.position_.x_ = v1.position_.x_;
                v5.position_.y_ = v2.position_.y_;
                v5.uv_[0] = v1.uv_[0];
                v5.uv_[1] = v2.uv_[1];


                x += glyph->width_;

                font.numChars_ += 1;
            }
            str += bytes;
        }//while
    }

    void FontManager::Impl::print(u32 fontID, f32 tx, f32 ty, f32 scale, const char* str)
    {
        LASSERT(0<=fontID && fontID<numFonts_);
        LASSERT(str != NULL);

        FontRenderer& font = fonts_[fontID];
        const PackHeader& header = font.fontPack_.getHeader();

        f32 scaleFitW;
        f32 scaleFitH;
        f32 scaleOffsetX;
        f32 scaleOffsetY;
        f32 scaleTextH = scale*header.textHeight_;
        f32 halfTextH = scale*header.spaceWidth_;//0.5f * scaleTextH;

        f32 x = tx;
        f32 y = ty;
        while(*str != '\0'){
            if(font.numChars_>= MaxCharsInFont){
                break;
            }

            if(*str == '\n'){
                x = tx;
                y += scaleTextH;
                ++str;
                continue;
            }

            u16 code;
            s32 bytes = SJISToU16(code, str);

            const GlyphInfo* glyph = font.fontPack_.get(code);

            if(NULL == glyph){
                x += header.spaceWidth_ * scale;

            }else{
                u32 index = font.numChars_ * 6;
                Vertex& v0 = font.vertices_[ index+0 ];
                Vertex& v1 = font.vertices_[ index+1 ];
                Vertex& v2 = font.vertices_[ index+2 ];
                Vertex& v3 = font.vertices_[ index+3 ];
                Vertex& v4 = font.vertices_[ index+4 ];
                Vertex& v5 = font.vertices_[ index+5 ];

                scaleFitW = scale * glyph->fitWidth_;
                scaleFitH = scale * glyph->fitHeight_;

                scaleOffsetX = scale * glyph->offsetX_;
                scaleOffsetY = scale * glyph->offsetY_;

                v0.position_.x_ = x + scaleOffsetX;
                v0.position_.y_ = y + scaleOffsetY;
                v0.uv_[0] = static_cast<s16>(glyph->x_ * header.codeRatioX_);
                v0.uv_[1] = static_cast<s16>(glyph->y_ * header.codeRatioY_);

                v1.position_.x_ = v0.position_.x_ + scaleFitW;
                v1.position_.y_ = v0.position_.y_;
                v1.uv_[0] = v0.uv_[0] + glyph->fitWCoded_;
                v1.uv_[1] = v0.uv_[1];

                v2.position_.x_ = v0.position_.x_;
                v2.position_.y_ = v0.position_.y_ + scaleFitH;
                v2.uv_[0] = v0.uv_[0];
                v2.uv_[1] = v0.uv_[1] + glyph->fitHCoded_;

                v3.position_ = v2.position_;
                v3.uv_[0] = v2.uv_[0];
                v3.uv_[1] = v2.uv_[1];

                v4.position_ = v1.position_;
                v4.uv_[0] = v1.uv_[0];
                v4.uv_[1] = v1.uv_[1];

                v5.position_.x_ = v1.position_.x_;
                v5.position_.y_ = v2.position_.y_;
                v5.uv_[0] = v1.uv_[0];
                v5.uv_[1] = v2.uv_[1];


                x += scale * glyph->width_;

                font.numChars_ += 1;
            }
            str += bytes;
        }//while
    }


    void FontManager::Impl::draw()
    {
        lgraphics::GraphicsDeviceRef& device = lgraphics::Graphics::getDevice();

        device.setDepthStencilState(lgraphics::GraphicsDeviceRef::DepthStencil_DDisableWDisable);
        layout_.attach();
        device.setBlendState(blend_);

        vs_.attach();
        vs_.setParameters(constants_);


        for(u32 i=0; i<numFonts_; ++i){
            FontRenderer& font = fonts_[i];


            if(font.numChars_>0){
                font.update();

                if(font.fontPack_.distanceField()){
                    dfPS_.attach();
                }else{
                    ps_.attach();
                }
                device.setPSResources(0, 1, font.fontPack_.getTexture().getView());
                device.setPSSamplers(0, 1, font.fontPack_.getTexture().getSampler());
                font.buffer_.attach(0, sizeof(Vertex), 0);
                device.setPrimitiveTopology(lgraphics::Primitive_TriangleList);

                device.draw(font.numChars_*6, 0);
                font.numChars_ = 0;
            }
        }
    }

    void FontManager::Impl::draw(u32 fontID)
    {
        lgraphics::GraphicsDeviceRef& device = lgraphics::Graphics::getDevice();

        device.setDepthStencilState(lgraphics::GraphicsDeviceRef::DepthStencil_DDisableWDisable);
        layout_.attach();
        device.setBlendState(blend_);

        vs_.attach();
        vs_.setParameters(constants_);

        FontRenderer& font = fonts_[fontID];

        if(font.numChars_>0){
            font.update();

            if(font.fontPack_.distanceField()){
                dfPS_.attach();
            }else{
                ps_.attach();
            }
            device.setPSResources(0, 1, font.fontPack_.getTexture().getView());
            device.setPSSamplers(0, 1, font.fontPack_.getTexture().getSampler());
            font.buffer_.attach(0, sizeof(Vertex), 0);
            device.setPrimitiveTopology(lgraphics::Primitive_TriangleList);

            device.draw(font.numChars_*6, 0);
            font.numChars_ = 0;
        }
    }

    //---------------------------------------
    //---
    //--- FontManager
    //---
    //---------------------------------------
    FontManager::FontManager()
        :impl_(NULL)
    {
    }

    FontManager::~FontManager()
    {
    }

    void FontManager::initialize()
    {
        impl_ = LIME_NEW Impl;
        impl_->initialize();
    }

    void FontManager::terminate()
    {
        impl_->terminate();
        LIME_DELETE(impl_);
    }

    void FontManager::setResolution(u32 width, u32 height)
    {
        impl_->setResolution(width, height);
    }

    bool FontManager::load(lcore::istream& is)
    {
        return impl_->load(is);
    }

    u32 FontManager::getNumFonts() const
    {
        return impl_->getNumFonts();
    }

    FontPack& FontManager::getFontPack(u32 index)
    {
        LASSERT(0<=index && index<impl_->numFonts_);
        return impl_->fonts_[index].fontPack_;
    }

    void FontManager::print(u32 fontID, u32 x, u32 y, const char* str)
    {
        impl_->print(fontID, x, y, str);
    }

    void FontManager::print(u32 fontID, f32 x, f32 y, f32 scale, const char* str)
    {
        impl_->print(fontID, x, y, scale, str);
    }

    void FontManager::format(u32 fontID, u32 x, u32 y, const char* str, ...)
    {
        LASSERT(str != NULL);

        va_list ap;
        va_start(ap, str);

        static const u32 MaxBuffer = 63;
        Char buffer[MaxBuffer+1];
        int count=vsnprintf_s(buffer, MaxBuffer, MaxBuffer, str, ap);

        va_end(ap);
        if(count<0){
            return;
        }
        buffer[MaxBuffer] = '\0';

        impl_->print(fontID, x, y, buffer);
    }

    void FontManager::format(u32 fontID, f32 x, f32 y, f32 scale, const char* str, ...)
    {
        LASSERT(str != NULL);

        va_list ap;
        va_start(ap, str);

        static const u32 MaxBuffer = 63;
        Char buffer[MaxBuffer+1];
        int count=vsnprintf_s(buffer, MaxBuffer, MaxBuffer, str, ap);
        va_end(ap);
        if(count<0){
            return;
        }
        buffer[MaxBuffer] = '\0';

        impl_->print(fontID, x, y, scale, buffer);
    }

    void FontManager::draw()
    {
        impl_->draw();
    }

    void FontManager::draw(u32 fontID)
    {
        impl_->draw(fontID);
    }
}
