#ifndef INC_LGRAPHICS_ES2_TEXTUREREF_H__
#define INC_LGRAPHICS_ES2_TEXTUREREF_H__
/**
@file TextureRef.h
@author t-sakai
@date 2010/06/08 create
*/
#include "../../lgraphicscore.h"
#include "SurfaceRef.h"

namespace lgraphics
{
    struct Descriptor;
    class TextureSystemMemRef;

    //--------------------------------------------
    //---
    //--- TextureRef
    //---
    //--------------------------------------------
    class TextureRef
    {
    public:
        struct TextureDesc;

        TextureRef()
            :texDesc_(NULL)
            ,texID_(NULL)
        {
        }


        TextureRef(const TextureRef& rhs);

        ~TextureRef()
        {
            destroy();
        }

        TextureRef& operator=(const TextureRef& rhs)
        {
            TextureRef tmp(rhs);
            tmp.swap(*this);
            return *this;
        }

        bool valid() const{ return (texID_ != NULL); }

        void destroy();

        bool lock(u32 level, LockedRect& lockedRect, Lock lock = Lock_None);
        void unlock(u32 level);

        /**
        @brief データ転送。GL的インターフェイス
        */
        void blit(u8* data);

        /**
        bindしないので、事前にattachを呼ぶ必要あり。
        */
        void blit(u32 level, u32 width, u32 height, u8* data);

        u16 getLevels();
        bool getLevelDesc(u32 level, SurfaceDesc& desc);

        /**
        @brief シェーダにセット
        @param index ... テクスチャユニットインデックス
        @param location ... シェーダ内ロケーション
        */
        void attach(u32 index, u32 location) const;

        void attach() const;
        void detach()const;

        void swap(TextureRef& rhs)
        {
            lcore::swap(texDesc_, rhs.texDesc_);
            lcore::swap(texID_, rhs.texID_);
        }

        bool operator==(const TextureRef& rhs) const
        {
            LASSERT(valid() && rhs.valid());
            return texID_ == rhs.texID_;
        }

        bool operator!=(const TextureRef& rhs) const
        {
            LASSERT(valid() && rhs.valid());
            return texID_ != rhs.texID_;
        }

        const NameString& getName() const;
        NameString& getName();

        void setName(const NameString& name);
        void setName(const char* name, u32 len);

        /**
        @brief 指定レベルまでのバイトオフセットを計算
        */
        u32 getLevelOffset(u32 level) const;

        /**
        @brief 指定レベルでのサイズ計算
        */
        u32 getLevelSize(u32 level, u32 bpp) const;

        u16 getBytePerPixel() const;

        u32 getTextureID() const;

        s32 getFormat() const;
    private:
        friend class Texture;

        TextureRef(Descriptor* texID, TextureDesc* texDesc);

        TextureDesc *texDesc_;
        Descriptor *texID_;
    };

    //--------------------------------------------
    //---
    //--- Texture
    //---
    //--------------------------------------------
    class Texture
    {
    public:
        static TextureRef create(u32 width, u32 height, u32 levels, Usage usage, BufferFormat format, Pool poolType);
        //static void update(TextureSystemMemRef& src, TextureRef& dst);
    };



    //--------------------------------------------
    //---
    //--- TextureSystemMemRef
    //---
    //--------------------------------------------
    class TextureSystemMemRef : private TextureRef
    {
    public:
        TextureSystemMemRef()
        {
        }

        TextureSystemMemRef(const TextureSystemMemRef& rhs)
            :TextureRef(rhs)
        {
        }

        ~TextureSystemMemRef()
        {
        }

        TextureSystemMemRef& operator=(const TextureSystemMemRef& rhs)
        {
            TextureSystemMemRef tmp(rhs);
            swap(tmp);
            return *this;
        }

        bool valid() const{ return TextureRef::valid(); }

        void destroy()
        {
            TextureRef::destroy();
        }

        bool lock(u32 level, LockedRect& lockedRect, Lock lock = Lock_None)
        {
            return TextureRef::lock(level, lockedRect, lock);
        }

        void unlock(u32 level)
        {
            return TextureRef::unlock(level);
        }

        u32 getLevels(){ return TextureRef::getLevels(); }
        bool getLevelDesc(u32 level, SurfaceDesc& desc){ return TextureRef::getLevelDesc(level, desc);}

        void attach(u32 index, u32 location) const
        {
            TextureRef::attach(index, location);
        }

        void swap(TextureSystemMemRef& rhs)
        {
            TextureRef::swap(rhs);
        }

    private:
        friend class Texture;
        friend class TextureSystemMem;

        explicit TextureSystemMemRef(const TextureRef& texture)
            :TextureRef(texture)
        {
        }
    };


    //--------------------------------------------
    //---
    //--- TextureSystemMem
    //---
    //--------------------------------------------
    class TextureSystemMem
    {
    public:
        static TextureSystemMemRef create(u32 width, u32 height, u32 levels, Usage usage, BufferFormat format)
        {
            return TextureSystemMemRef( Texture::create(width, height, levels, usage, format, lgraphics::Pool_SystemMem) );
        }
    };
}

//---------------------------------------------------
//---
//--- lcore::swap特殊化
//---
//---------------------------------------------------
namespace lcore
{
    template<>
    inline void swap<lgraphics::TextureRef>(lgraphics::TextureRef& l, lgraphics::TextureRef& r)
    {
        l.swap(r);
    }

    template<>
    inline void swap<lgraphics::TextureSystemMemRef>(lgraphics::TextureSystemMemRef& l, lgraphics::TextureSystemMemRef& r)
    {
        l.swap(r);
    }
}
#endif //INC_LGRAPHICS_ES2_TEXTUREREF_H__
