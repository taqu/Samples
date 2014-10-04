#ifndef INC_APPLICATION_H__
#define INC_APPLICATION_H__
/**
@file Application.h
@author t-sakai
@date 2010/02/14 create
*/
#include <lgraphics/api/TextureRef.h>
#include <lframework/Application.h>
#include <lframework/scene/Scene.h>
#include "scene/DebugCamera.h"

#include "render/InputLayoutFactory.h"

namespace render
{
    class Object;
}

namespace fractal
{
    class Application : public lframework::Application
    {
    public:
        Application();
        virtual ~Application();

        void initialize();
        void update();
        void terminate();

    private:
        f32 count_;
        DebugCamera camera_;

        render::Object* object_;
        render::Object* plane_;
        render::Object* box_;
        render::Object* sphere_;

        lmath::Vector4 lightDirection_;
    };

    LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
}
#endif //INC_APPLICATION_H__
