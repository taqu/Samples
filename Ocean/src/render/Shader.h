#ifndef INC_RENDER_SHADER_H__
#define INC_RENDER_SHADER_H__
/**
@file Shader.h
@author t-sakai
@date 2011/10/16 create
*/
#include <lgraphics/api/ConstantBufferRef.h>

#include "render.h"
#include "ShaderBase.h"
#include "ShaderType.h"

namespace lgraphics
{
    class TextureRef;
}

namespace render
{

    //----------------------------------------------
    //---
    //--- Default
    //---
    //----------------------------------------------
    class DefaultVS : public ShaderVSBase
    {
    public:
        struct Constant
        {
            lmath::Matrix44 mwvp_;
            lmath::Matrix44 mw_;
            lmath::Matrix44 mv_;
            lmath::Matrix44 mlwvp_;
        };

        DefaultVS(const lgraphics::VertexShaderRef& ref)
            :ShaderVSBase(ref)
        {}

        virtual void initialize();

        void update(const Constant& constant)
        {
            constant_.update(0, NULL, &constant, 0, 0);
            constant_.setVS(0);
        }
    private:
        lgraphics::ConstantBufferRef constant_;
    };


    class DefaultPS : public ShaderPSBase
    {
    public:
        struct Constant0
        {
            lmath::Vector4 dlDir_;
            lmath::Vector4 dlColor_;
            lmath::Vector4 cameraPos_;
        };

        struct Constant1
        {
            lmath::Vector4 diffuse_;
            lmath::Vector4 specular_;
            lmath::Vector4 transparent_;
        };

        DefaultPS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}

        virtual void initialize();
        void set(const lmath::Matrix44& mat, const Material& material, const lscene::Scene& scene);

        void update0(const Constant0& constant0)
        {
            constant0_.update(0, NULL, &constant0, 0, 0);
            constant0_.setPS(0);
        }

        void update1(const Constant1& constant1)
        {
            constant1_.update(0, NULL, &constant1, 0, 0);
            constant1_.setPS(1);
        }
    private:
        lgraphics::ConstantBufferRef constant0_;
        lgraphics::ConstantBufferRef constant1_;
    };

    class DefaultTexturePS : public ShaderPSBase
    {
    public:
        DefaultTexturePS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}
    };

    //----------------------------------------------
    //---
    //--- Depth
    //---
    //----------------------------------------------
    class DepthVS : public ShaderVSBase
    {
    public:
        DepthVS(const lgraphics::VertexShaderRef& ref)
            :ShaderVSBase(ref)
        {}

        virtual void initialize();
        void set(const lmath::Matrix44& mat);
    private:
        lgraphics::ConstantBufferRef constants_;
    };


    class DepthPS : public ShaderPSBase
    {
    public:
        DepthPS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}
    private:
    };

    //----------------------------------------------
    //---
    //--- Rectangle
    //---
    //----------------------------------------------
    class RectangleVS : public ShaderVSBase
    {
    public:
        struct Constant
        {
            lmath::Vector4 pos_[4];
        };

        RectangleVS(const lgraphics::VertexShaderRef& ref)
            :ShaderVSBase(ref)
        {}

        virtual void initialize();
        void set(const lmath::Vector4* constant)
        {
            constants_.update(0, NULL, constant, 0, 0);
            constants_.setVS(0);
        }

    private:
        lgraphics::ConstantBufferRef constants_;
    };


    class RectanglePS : public ShaderPSBase
    {
    public:
        RectanglePS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}
    };


    //----------------------------------------------
    //---
    //--- DebugVC
    //---
    //----------------------------------------------
    class DebugVCVS : public ShaderVSBase
    {
    public:
        DebugVCVS(const lgraphics::VertexShaderRef& ref)
            :ShaderVSBase(ref)
        {}
    };


    class DebugVCPS : public ShaderPSBase
    {
    public:
        DebugVCPS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}
    };

    //----------------------------------------------
    //---
    //--- FullQuad
    //---
    //----------------------------------------------
    class FullQuadVS : public ShaderVSBase
    {
    public:
        FullQuadVS(const lgraphics::VertexShaderRef& ref)
            :ShaderVSBase(ref)
        {}
    };

    //----------------------------------------------
    //---
    //--- FXAA
    //---
    //----------------------------------------------
    class FXAAVS : public ShaderVSBase
    {
    public:
        FXAAVS(const lgraphics::VertexShaderRef& ref)
            :ShaderVSBase(ref)
        {}
    };

    class FXAAPS : public ShaderPSBase
    {
    public:
        struct Constant0
        {
            lmath::Vector4 rcpFrame_; //(1/width, 1/height, 0, 0)
            lmath::Vector4 rcpFrameOpt_; //(2/width, 2/height, 0.5/width, 0.5/height)
        };

        FXAAPS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}
    };

    //----------------------------------------------
    //---
    //--- TonemappingStatic
    //---
    //----------------------------------------------
    class TonemappingStaticPS : public ShaderPSBase
    {
    public:
        TonemappingStaticPS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}
    };

    //----------------------------------------------
    //---
    //--- Glare
    //---
    //----------------------------------------------
    class GlarePS : public ShaderPSBase
    {
    public:
        GlarePS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}
    };

    //----------------------------------------------
    //---
    //--- Gauss
    //---
    //----------------------------------------------
    class GaussPS : public ShaderPSBase
    {
    public:
        GaussPS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}
    };

    //----------------------------------------------
    //---
    //--- Ocean
    //---
    //----------------------------------------------
    class OceanVS : public ShaderVSBase
    {
    public:
        struct Constant0
        {
            lmath::Matrix44 mwvp_;
            lmath::Matrix44 mlocal_;
            lmath::Matrix44 mw_;
            lmath::Matrix44 mlwvp_;
        };

        struct Constant1
        {
            lmath::Vector4 info_; //{choppy scale, grid length, grid length * 2, 0}
            lmath::Vector4 uvInfo_; //{uv scale, uv offset, 0, 0}
        };

        OceanVS(const lgraphics::VertexShaderRef& ref)
            :ShaderVSBase(ref)
        {}

        virtual void initialize();

        void set()
        {
            constant0_.setVS(0);
            constant1_.setVS(1);
        }

        void update0(const Constant0& constant0)
        {
            constant0_.update(0, NULL, &constant0, 0, 0);
        }

        void update1(const Constant1& constant1)
        {
            constant1_.update(0, NULL, &constant1, 0, 0);
        }
    private:
        lgraphics::ConstantBufferRef constant0_;
        lgraphics::ConstantBufferRef constant1_;
    };


    class OceanPS : public ShaderPSBase
    {
    public:
        struct Constant1
        {
            lmath::Vector4 diffuse_;
            lmath::Vector4 specular_;
            lmath::Vector4 transparent_;
            lmath::Vector4 noiseUV_;
            lmath::Vector4 noiseAmplitude_;
            lmath::Vector4 noiseOctave_;
            lmath::Vector4 bendParam_;
        };

        OceanPS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}
    };

    class OceanWirePS : public ShaderPSBase
    {
    public:
        OceanWirePS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}
    };

    //----------------------------------------------
    //---
    //--- SkyBox
    //---
    //----------------------------------------------
    class SkyBoxVS : public ShaderVSBase
    {
    public:
        struct Constant0
        {
            f32 nearAspect_[4]; //{near, aspect, 0, 0}
            lmath::Matrix44 invViewProj_;
        };

        SkyBoxVS(const lgraphics::VertexShaderRef& ref)
            :ShaderVSBase(ref)
        {}

        virtual void initialize();

        void set()
        {
            constant0_.setVS(0);
        }

        void update0(const Constant0& constant0)
        {
            constant0_.update(0, NULL, &constant0, 0, 0);
        }
    private:
        lgraphics::ConstantBufferRef constant0_;
    };


    class SkyBoxPS : public ShaderPSBase
    {
    public:
        SkyBoxPS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}
    };


    //----------------------------------------------
    //---
    //--- HDAO
    //---
    //----------------------------------------------
    class HDAOPS : public ShaderPSBase
    {
    public:
        struct Constant0
        {
            Constant0()
                :screenWidth_(0.0f)
                ,screenHeight_(0.0f)
                ,normalScale_(0.0001f)
                ,rejectRadius_(0.0013f)
                ,acceptRadius_(0.000015f)
                ,acceptAngle_(0.975f)
                ,intensity_(2.14f)
                ,dummy_(0.0f)
            {}

            f32 screenWidth_;
            f32 screenHeight_;
            f32 normalScale_;
            f32 rejectRadius_;
            f32 acceptRadius_;
            f32 acceptAngle_;
            f32 intensity_;
            f32 dummy_;
        };

        HDAOPS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}

        virtual void initialize();

        void set()
        {
            constant0_.setPS(0);
        }

        void update0(const Constant0& constant0)
        {
            constant0_.update(0, NULL, &constant0, 0, 0);
        }
    private:
        lgraphics::ConstantBufferRef constant0_;
    };
}
#endif //INC_RENDER_SHADER_H__
