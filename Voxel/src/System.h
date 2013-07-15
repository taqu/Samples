#ifndef INC_SYSTEM_H__
#define INC_SYSTEM_H__
/**
@file System.h
@author t-sakai
@date 2011/09/24 create
*/
#include <lcore/random/Random.h>

#include "Timer.h"
#include "render/Renderer.h"
#include "render/InputLayoutFactory.h"

#include "stdafx.h"

struct HWND__;

namespace fractal
{
    class System
    {
    public:
        static const u32 InitBatchSize = 128;
        static const u32 DebugMaxLines = 256;

        static bool initialize(HWND__* hWnd, u32 width, u32 height);
        static void terminate();
        static System& getInstance(){ return *instance_;}

        void update();
        bool reset();

        static lcore::RandomXorshift& getRandom0(){ return instance_->xorshift_;}

        static Timer& getTimer(){ return instance_->timer_;}

        static lscene::Scene& getScene(){ return instance_->renderer_.getScene();}
        static render::Renderer& getRenderer(){ return instance_->renderer_;}
        static render::InputLayoutFactory& getInputLayoutFactory(){ return instance_->inputLayoutFactory_;}

    private:
        System(const System&);
        System& operator=(const System&);

        static System* instance_;

        System()
        {}

        ~System()
        {}

        bool initImpl(HWND__* hWnd, u32 width, u32 height);
        void termImpl();

        lcore::RandomXorshift xorshift_;

        Timer timer_;
        render::Renderer renderer_;
        render::InputLayoutFactory inputLayoutFactory_;
    };
}
#endif //INC_SYSTEM_H__
