#ifndef INC_SCENE_SKYBOX_H__
#define INC_SCENE_SKYBOX_H__
/**
@file SkyBox.h
@author t-sakai
@date 2013/07/12 create
*/
#include "scene/scene.h"
#include <lmath/lmath.h>
#include <lgraphics/api/GraphicsDeviceRef.h>
#include <lgraphics/api/InputLayoutRef.h>
#include <lgraphics/api/VertexBufferRef.h>

namespace lscene
{
    class Camera;
}

namespace render
{
    class SkyBoxVS;
    class SkyBoxPS;
}

namespace scene
{
    class SkyBox
    {
    public:
        SkyBox();
        ~SkyBox();

        void initialize();

        void update(const lscene::Camera& camera, u32 screenWidth, u32 screenHeight);
    private:
        SkyBox(const SkyBox&);
        SkyBox& operator=(const SkyBox&);

        render::SkyBoxVS* vs_;
        render::SkyBoxPS* ps_;
    };
}
#endif //INC_SCENE_OCEAN_H__
