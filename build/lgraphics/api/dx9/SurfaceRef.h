#ifndef INC_LGRAPHICS_DX9_SURFACEREF_H__
#define INC_LGRAPHICS_DX9_SURFACEREF_H__
/**
@file SurfaceRef.h
@author t-sakai
@date 2009/05/01 create
*/
#include "../../lgraphicscore.h"
#include "Enumerations.h"

struct IDirect3DSurface9;

namespace lgraphics
{
    class SurfaceOffscreenRef;

    struct LockedRect
    {
        s32 pitch_;
        void* bits_;
    };

    struct SurfaceDesc
    {
        BufferFormat format_;
        ResourceType type_;
        u32 usage_;
        Pool pool_;
        MutiSampleType multiSampleType_;
        u32 multiSampleQuality_;
        u32 width_;
        u32 height_;
    };

    typedef IDirect3DSurface9 SurfaceRaw;

    //-----------------------------------------------
    //---
    //--- SurfaceRef
    //---
    //-----------------------------------------------
    class SurfaceRef
    {
    public:
        SurfaceRef()
            :surface_(NULL)
        {
        }

        SurfaceRef(const SurfaceRef& rhs);

        explicit SurfaceRef(IDirect3DSurface9 *surface)
            :surface_(surface)
        {
        }

        ~SurfaceRef()
        {
            destroy();
        }

        SurfaceRef& operator=(const SurfaceRef& rhs)
        {
            SurfaceRef tmp(rhs);
            tmp.swap(*this);
            return *this;
        }

        SurfaceRaw* get()
        {
            return surface_;
        }

        bool valid() const{ return (surface_ != NULL); }

        void destroy();

        //void attach(u32 index) const;

        bool getData(SurfaceOffscreenRef& offscreen);

        bool getDesc(SurfaceDesc& desc);

        void swap(SurfaceRef& rhs)
        {
            lcore::swap(surface_, rhs.surface_);
        }

    protected:
        bool lock(LockedRect& lockedRect, Lock lock = Lock_None);
        void unlock();

    private:
        friend class Surface;
        friend class SurfaceOffscreen;

        IDirect3DSurface9 *surface_;
    };

    //-----------------------------------------------
    //---
    //--- Surface
    //---
    //-----------------------------------------------
    class Surface
    {
    public:
        static SurfaceRef create(
            u32 width,
            u32 height,
            BufferFormat format,
            MutiSampleType sampleType = MutiSample_None,
            u32 multiSampleQuality = 0);

        static SurfaceRef createDepthStencil(
            u32 width,
            u32 height,
            BufferFormat format,
            MutiSampleType sampleType = MutiSample_None,
            u32 multiSampleQuality = 0);
    };



    //-----------------------------------------------
    //---
    //--- SurfaceOffscreenRef
    //---
    //-----------------------------------------------
    class SurfaceOffscreenRef : private SurfaceRef
    {
    public:
        SurfaceOffscreenRef()
        {
        }

        ~SurfaceOffscreenRef()
        {
        }

        bool valid() const{ return SurfaceRef::valid(); }

        void destroy(){ SurfaceRef::destroy();}

        bool lock(LockedRect& lockedRect, Lock lock = Lock_None)
        {
            return SurfaceRef::lock(lockedRect, lock);
        }

        void unlock()
        {
            SurfaceRef::unlock();
        }

        void swap(SurfaceOffscreenRef& rhs)
        {
            SurfaceRef::swap(rhs);
        }
    private:
        friend class SurfaceRef;
        friend class SurfaceOffscreen;

        explicit SurfaceOffscreenRef(const SurfaceRef& rhs)
            :SurfaceRef(rhs)
        {
        }
    };


    //-----------------------------------------------
    //---
    //--- SurfaceOffscreen
    //---
    //-----------------------------------------------
    class SurfaceOffscreen
    {
    public:
        static SurfaceOffscreenRef create(
            u32 width,
            u32 height,
            BufferFormat format);
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
    inline void swap<lgraphics::SurfaceRef>(lgraphics::SurfaceRef& l, lgraphics::SurfaceRef& r)
    {
        l.swap(r);
    }

    template<>
    inline void swap<lgraphics::SurfaceOffscreenRef>(lgraphics::SurfaceOffscreenRef& l, lgraphics::SurfaceOffscreenRef& r)
    {
        l.swap(r);
    }
}
#endif //INC_LGRAPHICS_DX9_SURFACEREF_H__
