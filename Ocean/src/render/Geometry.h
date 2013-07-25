#ifndef INC_RENDER_GEOMETRY_H__
#define INC_RENDER_GEOMETRY_H__
/**
@file Geometry.h
@author t-sakai
@date 2012/04/05 create

*/
#include <lgraphics/api/GraphicsDeviceRef.h>
#include <lgraphics/api/InputLayoutRef.h>
#include <lgraphics/api/VertexBufferRef.h>
#include <lgraphics/api/IndexBufferRef.h>

#include "render.h"

namespace render
{
    class Geometry
    {
    public:
        Geometry();
        ~Geometry();

        void create(
            u32 vflag,
            u32 vsize,
            u32 numVertices,
            u8* vertices,
            u32 numIndices,
            u16* indices,
            lgraphics::InputLayoutRef& inputLayout,
            lgraphics::VertexBufferRef& vb,
            lgraphics::IndexBufferRef& ib);

        void clone(Geometry& dst);
        inline u32 getVFlag() const;
        inline u32 getVSize() const;

        inline u32 getNumVertices() const;
        inline const u8* getVertices() const;
        inline u32 getNumIndices() const;
        inline const u16* getIndices() const;

        inline void attach();

        void swap(Geometry& rhs);

        inline bool valid() const;
    private:
        Geometry(const Geometry&);
        Geometry& operator=(const Geometry&);

        u32 vflag_;
        u32 vsize_;
        u32 numVertices_;
        u8* vertices_;
        u32 numIndices_;
        u16* indices_;

        lgraphics::InputLayoutRef inputLayout_;
        lgraphics::VertexBufferRef vb_;
        lgraphics::IndexBufferRef ib_;
    };

    inline u32 Geometry::getVFlag() const
    {
        return vflag_;
    }

    inline u32 Geometry::getVSize() const
    {
        return vsize_;
    }

    inline u32 Geometry::getNumVertices() const
    {
        return numVertices_;
    }

    inline const u8* Geometry::getVertices() const
    {
        return vertices_;
    }

    inline u32 Geometry::getNumIndices() const
    {
        return numIndices_;
    }

    inline const u16* Geometry::getIndices() const
    {
        return indices_;
    }

    inline void Geometry::attach()
    {
        inputLayout_.attach();
        vb_.attach(0, vsize_, 0);
        ib_.attach(lgraphics::Data_R16_UInt, 0);
    }

    inline bool Geometry::valid() const
    {
        return (inputLayout_.valid() && vb_.valid());
    }
}
#endif //INC_RENDER_GEOMETRY_H__
