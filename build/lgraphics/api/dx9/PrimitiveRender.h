#ifndef INC_PRIMITIVERENDER_H__
#define INC_PRIMITIVERENDER_H__
/**
@file PrimitiveRender.h
@author t-sakai
@date 2009/05/13 create
*/
#include <lcore/NonCopyable.h>
#include <lmath/lmath.h>
#include "../lgraphicscore.h"

namespace lgraphics
{
    class Texture;
    class VertexBuffer;
    class RenderState;

    extern const u32 PRIMITIVE_FVF;

    struct PrimitiveVertex
    {
        PrimitiveVertex(f32 x, f32 y, f32 z, u32 color = 0xFFFFFFFFU, f32 u=0.0f, f32 v=0.0f)
            :position_(x,y,z)
            ,color_(color)
            ,uv_(u, v)
        {
        }

        PrimitiveVertex(const lmath::Vector3& position, u32 color, const lmath::Vector2& uv)
            :position_(position)
            ,color_(color)
            ,uv_(uv)
        {
        }


        lmath::Vector3 position_;
        u32 color_;
        lmath::Vector2 uv_;
    };

    class PrimitiveRender : private lcore::NonCopyable<PrimitiveRender>
    {
        PrimitiveRender(VertexBuffer* vb, RenderState* state, u32 maxTriangle, Texture* texture);
    public:
        static PrimitiveRender* create(u32 maxTriangle, Texture* texture);
        
        ~PrimitiveRender();

        void draw(const lmath::Matrix44& view, const lmath::Matrix44& proj);

        void drawNoStateChange();

        bool add(const PrimitiveVertex& v1, const PrimitiveVertex& v2, const PrimitiveVertex& v3);

        void lock();
        void unlock();
    private:
        void clear();
        static void setState(VertexBuffer* vb, Texture* texture);

        VertexBuffer *vb_;
        Texture *texture_;
        RenderState *state_;
        u32 maxTriangle_;
        u32 numTriangle_;

        PrimitiveVertex *lockedBuffer_;
    };
}

#endif //INC_PRIMITIVERENDER_H__
