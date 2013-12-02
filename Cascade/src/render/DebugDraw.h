#ifndef INC_RENDER_DEBUGDRAW_H__
#define INC_RENDER_DEBUGDRAW_H__
/**
@file DebugDraw.h
@author t-sakai
@date 2012/05/17 create
*/
#include <lgraphics/api/InputLayoutRef.h>
#include <lgraphics/api/VertexBufferRef.h>
#include <lgraphics/api/IndexBufferRef.h>

#include "render/render.h"

namespace lgraphics
{
    class GraphicsDeviceRef;
}

namespace lmath
{
    class Vector3;
    class Vector4;
}

namespace render
{
    class DebugVCVS;
    class DebugVCPS;

    class DebugDraw
    {
    public:
        struct Vertex
        {
            f32 x_, y_, z_;
            u32 color_;
        };

        DebugDraw();
        ~DebugDraw();

        void initialize(u32 maxLines);
        void terminate();

        void add(const lmath::Vector3& v0, const lmath::Vector3& v1, u32 color);
        void add(const lmath::Vector4& v0, const lmath::Vector4& v1, u32 color);

        void draw(lgraphics::GraphicsDeviceRef& device);
    private:
        DebugVCVS* vs_;
        DebugVCPS* ps_;

        lgraphics::InputLayoutRef inputLayout_;
        lgraphics::VertexBufferRef vbLines_;

        u32 maxLines_;
        u32 numLines_;
        Vertex* lines_;
    };
}
#endif //INC_RENDER_DEBUGDRAW_H__
