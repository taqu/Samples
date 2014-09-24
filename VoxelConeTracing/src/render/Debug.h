#ifndef INC_RENDER_DEBUG_H__
#define INC_RENDER_DEBUG_H__
/**
@file Debug.h
@author t-sakai
@date 2014/09/08 create
*/
#include <lgraphics/api/InputLayoutRef.h>
#include <lgraphics/api/VertexBufferRef.h>
#include <lgraphics/api/IndexBufferRef.h>

#include "render/render.h"
#include <lmath/lmath.h>

namespace render
{
    class DebugVS;
    class DebugPS;

    class Debug
    {
    public:
        struct Vertex
        {
            lmath::Vector3 position_;
            u32 color_;
        };

        Debug();
        ~Debug();

        void create(s32 maxVertices, s32 maxIndices);
        void destroy();

        s32 getMaxVertices() const;
        s32 getMaxIndices() const;

        Vertex* getVertices();
        u16* getIndices();

        void update(s32 numVertices, s32 numIndices);
        void draw(const lmath::Matrix44& mwvp);

        void drawFrustum(
            const lmath::Matrix44& mwvp,
            const lmath::Matrix44& projection,
            f32 znear,
            f32 zfar,
            u32 color);
    private:
        
        s32 maxVertices_;
        s32 maxIndices_;

        Vertex* vertices_;
        u16* indices_;
        lgraphics::InputLayoutRef inputLayout_;
        lgraphics::VertexBufferRef vb_;
        lgraphics::IndexBufferRef ib_;

        DebugVS* vs_;
        DebugPS* ps_;
    };
}
#endif
