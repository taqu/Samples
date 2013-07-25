#ifndef INC_RENDER_RENDERER_H__
#define INC_RENDER_RENDERER_H__
/**
@file Renderer.h
@author t-sakai
@date 2011/12/31 create
*/
#include "render.h"
namespace lmath
{
    class Vector3;
}

namespace lscene
{
    class Scene;
}
namespace terrain
{
    class Area;
}

namespace render
{
    class Object;
    class NodeBase;

    class Renderer
    {
    public:
        enum Flag
        {
            Flag_HDR = (0x01U<<0),
        };

        Renderer();
        ~Renderer();

        bool initialize(u32 numEntries, u32 maxDebugLines, u32 virtualWidth, u32 virtualHeight);
        void terminate();
        bool reset();

        void setFlag(Renderer::Flag flag, bool enable);

        lscene::Scene& getScene();

        void add(Object* obj);
        void add(NodeBase* node);

        void draw();
        void calcZFar(f32 minz, f32 maxz);

        void debugDraw(const lmath::Vector3& v0, const lmath::Vector3& v1, u32 color);
        void debugDraw(const lmath::Vector4& v0, const lmath::Vector4& v1, u32 color);
        void debugDrawSphere(const lmath::Vector3& position, f32 radius, u32 color);
        void debugDrawSphere(const lmath::Vector4& position, f32 radius, u32 color);
    private:
        Renderer(const Renderer&);
        Renderer& operator=(const Renderer&);

        class Impl;
        Impl* impl_;
    };

}
#endif //INC_RENDER_RENDERER_H__
