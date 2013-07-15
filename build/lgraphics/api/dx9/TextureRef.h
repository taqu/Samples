#ifndef INC_LGRAPHICS_DX9_TEXTUREREF_H__
#define INC_LGRAPHICS_DX9_TEXTUREREF_H__
/**
@file TextureRef.h
@author t-sakai
@date 2009/05/12 create
*/
#include "../../lgraphicscore.h"
#include "SurfaceRef.h"

struct IDirect3DTexture9;

namespace lgraphics
{
    class TextureSystemMemRef;
    class SurfaceRef;

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
            ,texture_(NULL)
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

        bool valid() const{ return (texture_ != NULL); }

        void destroy();

        bool lock(u32 level, LockedRect& lockedRect, Lock lock = Lock_None);
        void unlock(u32 level);

        /**
        @brief データ転送。GL的インターフェイス
        */
        void blit(u8* data);

        /**
        @brief データ転送。GL的インターフェイス
        */
        void blit(u32 level, u32 width, u32 height, u8* data);

        u32 getLevels();
        bool getLevelDesc(u32 level, SurfaceDesc& desc);

        /**
        TODO:GLとインターフェイスを合わせるためだが、意味がないので混乱の元
        */
        void attach() const{}

        void attach(u32 index) const;
        void detach(){}

        void debugout(const char* filename);

        void swap(TextureRef& rhs)
        {
            lcore::swap(texDesc_, rhs.texDesc_);
            lcore::swap(texture_, rhs.texture_);
        }

        bool operator==(const TextureRef& rhs) const
        {
            return texture_ == rhs.texture_;
        }

        bool operator!=(const TextureRef& rhs) const
        {
            return texture_ != rhs.texture_;
        }

        const NameString& getName() const;
        NameString& getName();

        void setName(const NameString& name);
        void setName(const char* name, u32 len);

        s32 getFormat() const;

        bool getSurface(u32 level, SurfaceRef& surface);

    private:
        friend class Texture;

        TextureRef(IDirect3DTexture9* texture, TextureDesc* texDesc);

        TextureDesc *texDesc_;
        IDirect3DTexture9 *texture_;
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
        static void update(TextureSystemMemRef& src, TextureRef& dst);
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

        void attach(u32 index) const
        {
            TextureRef::attach(index);
        }

        void debugout(const char* filename)
        {
            TextureRef::debugout(filename);
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
#endif //INC_LGRAPHICS_DX9_TEXTUREREF_H__
