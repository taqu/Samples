#ifndef INC_RENDER_SHADER_H__
#define INC_RENDER_SHADER_H__
/**
@file Shader.h
@author t-sakai
@date 2011/10/16 create
*/
#include <lgraphics/api/ConstantBufferRef.h>

#include "render/render.h"
#include "render/ShaderBase.h"
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
    class DefaultPNVS : public ShaderVSBase
    {
    public:
        struct Constant
        {
            lmath::Matrix44 mwvp_;
            lmath::Matrix44 mw_;
            lmath::Matrix44 mv_;
            lmath::Matrix44 mlwvp_[4];
        };

        DefaultPNVS(const lgraphics::VertexShaderRef& ref)
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
            f32 cascadeDepth_[4];
        };

        struct Constant1
        {
            lmath::Vector4 diffuse_;
            lmath::Vector4 specular_;
            lmath::Vector4 shadow_;
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

    class DefaultPNUVS : public ShaderVSBase
    {
    public:
        DefaultPNUVS(const lgraphics::VertexShaderRef& ref)
            :ShaderVSBase(ref)
        {}

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
        void set(const lmath::Matrix44 mat[4]);
    private:
        lgraphics::ConstantBufferRef constants_;
    };

    class DepthGS : public ShaderGSBase
    {
    public:
        DepthGS(const lgraphics::GeometryShaderRef& ref)
            :ShaderGSBase(ref)
        {}
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


    //----------------------------------------------
    //---
    //--- ReflectiveShadowMap
    //---
    //----------------------------------------------
    class ReflectiveShadowMapVS : public ShaderVSBase
    {
    public:
        struct Constant0
        {
            lmath::Vector4 dlDir_;
            lmath::Vector4 dlColor_;
        };

        struct Constant1
        {
            lmath::Matrix44 mwvp_;
            lmath::Matrix44 mw_;
            lmath::Vector4 diffuse_;
        };

        ReflectiveShadowMapVS(const lgraphics::VertexShaderRef& ref)
            :ShaderVSBase(ref)
        {}

        virtual void initialize();

        void update0(const Constant0& constants0)
        {
            constants0_.update(0, NULL, &constants0, 0, 0);
            constants0_.setVS(0);
        }

        void update1(const Constant1& constants1)
        {
            constants1_.update(0, NULL, &constants1, 0, 0);
            constants1_.setVS(1);
        }
    private:
        lgraphics::ConstantBufferRef constants0_;
        lgraphics::ConstantBufferRef constants1_;
    };


    class ReflectiveShadowMapPS : public ShaderPSBase
    {
    public:
        //struct Constant
        //{
        //    lmath::Vector4 dlDir_;
        //    lmath::Vector4 dlColor_;
        //    lmath::Vector4 diffuse_;
        //};

        ReflectiveShadowMapPS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}

        //virtual void initialize();

        //void set(const Constant& constants);
    private:
        //lgraphics::ConstantBufferRef constants_;
    };


    //----------------------------------------------
    //---
    //--- LPVInjection
    //---
    //----------------------------------------------
    class LPVInjectionVS : public ShaderVSBase
    {
    public:
        struct Constant0
        {
            s32 size_;
            s32 mask_;
            s32 shift_;
            s32 padding00_;

            f32 invWidth_;
            f32 invHeight_;
            f32 occlusionAttenuation_;
            f32 padding02_;
        };

        struct Constant1
        {
            lmath::Matrix44 mivp_;
            lmath::Vector4 LPVMin_; //ÇSî‘ñ⁄Ç™ÉZÉãÉTÉCÉYÇÃãtêî
            lmath::Vector4 viewZ_;
        };

        LPVInjectionVS(const lgraphics::VertexShaderRef& ref)
            :ShaderVSBase(ref)
        {}

        void setConstant(const Constant0& constants)
        {
            constants0_.update(0, NULL, &constants, 0, 0);
        }

        void updateConstant(const Constant1& constants)
        {
            constants1_.update(0, NULL, &constants, 0, 0);
            constants0_.setVS(0);
            constants1_.setVS(1);
        }

        virtual void initialize();
    private:
        lgraphics::ConstantBufferRef constants0_;
        lgraphics::ConstantBufferRef constants1_;
    };


    class LPVInjectionGS : public ShaderGSBase
    {
    public:
        struct Constant
        {
            f32 invLPVResolution_;
            f32 padding0_;
            f32 padding1_;
            f32 padding2_;
        };

        LPVInjectionGS(const lgraphics::GeometryShaderRef& ref)
            :ShaderGSBase(ref)
        {}

        void set(f32 invLPVResolution)
        {
            Constant constants;
            constants.invLPVResolution_ = invLPVResolution;
            constants_.update(0, NULL, &constants, 0, 0);
        }

        void update()
        {
            constants_.setGS(0);
        }

        virtual void initialize();
    private:
        lgraphics::ConstantBufferRef constants_;
    };

    class LPVInjectionPS : public ShaderPSBase
    {
    public:
        LPVInjectionPS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}

    private:
    };


    //----------------------------------------------
    //---
    //--- LPVOcclusionInjection
    //---
    //----------------------------------------------
    class LPVOcclusionInjectionVS : public ShaderVSBase
    {
    public:
        //struct Constant0
        //{
        //    s32 size_;
        //    s32 mask_;
        //    s32 shift_;
        //    s32 padding00_;

        //    f32 invWidth_;
        //    f32 invHeight_;
        //    f32 occlusionAttenuation_;
        //    f32 padding02_;
        //};

        //struct Constant1
        //{
        //    lmath::Matrix44 mivp_;
        //    lmath::Vector4 LPVMin_; //ÇSî‘ñ⁄Ç™ÉZÉãÉTÉCÉYÇÃãtêî
        //    lmath::Vector4 viewZ_;
        //};

        LPVOcclusionInjectionVS(const lgraphics::VertexShaderRef& ref)
            :ShaderVSBase(ref)
        {}

        //void setConstant(const Constant0& constants)
        //{
        //    constants0_.update(0, NULL, &constants, 0, 0);
        //}

        //void updateConstant(const Constant1& constants)
        //{
        //    constants1_.update(0, NULL, &constants, 0, 0);
        //    constants0_.setVS(0);
        //    constants1_.setVS(1);
        //}

        //virtual void initialize();
    private:
        //lgraphics::ConstantBufferRef constants0_;
        //lgraphics::ConstantBufferRef constants1_;
    };

    class LPVOcclusionInjectionPS : public ShaderPSBase
    {
    public:
        LPVOcclusionInjectionPS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}

    private:
    };

    //----------------------------------------------
    //---
    //--- LPVPropagate
    //---
    //----------------------------------------------
    class LPVPropagateVS : public ShaderVSBase
    {
    public:

        LPVPropagateVS(const lgraphics::VertexShaderRef& ref)
            :ShaderVSBase(ref)
        {}

    private:
    };

    class LPVPropagateGS : public ShaderGSBase
    {
    public:
        LPVPropagateGS(const lgraphics::GeometryShaderRef& ref)
            :ShaderGSBase(ref)
        {}

    private:
    };

    class LPVPropagatePS : public ShaderPSBase
    {
    public:
        LPVPropagatePS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}

    private:
    };

    class LPVOcclusionPropagatePS : public ShaderPSBase
    {
    public:
        struct Constant0
        {
            f32 invLPVSize_;
            f32 padding0_;
            f32 padding1_;
            f32 padding2_;
        };

        LPVOcclusionPropagatePS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}

        virtual void initialize();

        void set(f32 invLPVSize)
        {
            Constant0 constants0;
            constants0.invLPVSize_ = invLPVSize;
            constants0_.update(0, NULL, &constants0, 0, 0);
        }

        void update()
        {
            constants0_.setPS(0);
        }
    private:
        lgraphics::ConstantBufferRef constants0_;
    };

    //----------------------------------------------
    //---
    //--- LPVAccumulate
    //---
    //----------------------------------------------
    class LPVAccumulatePS : public ShaderPSBase
    {
    public:
        LPVAccumulatePS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}

    private:
    };

    //----------------------------------------------
    //---
    //--- LPVRender
    //---
    //----------------------------------------------
    class LPVRenderVS : public ShaderVSBase
    {
    public:
        struct Constant0
        {
            lmath::Matrix44 mwvp_;
            lmath::Matrix44 mw_;
            lmath::Matrix44 mlwvp_[4];
        };

        struct Constant1
        {
            lmath::Vector4 LPVMin_; //ÇSî‘ñ⁄Ç™ÉZÉãÉTÉCÉYÇÃãtêî
            f32 invLPVSize_;
            f32 invTexSize_;
            f32 LPVAttenuation_;
            f32 padding2_;
        };

        LPVRenderVS(const lgraphics::VertexShaderRef& ref)
            :ShaderVSBase(ref)
        {}

        virtual void initialize();

        void update0(const Constant0& constants0)
        {
            constants0_.update(0, NULL, &constants0, 0, 0);
            constants0_.setVS(0);
        }

        void update1(const Constant1& constants1)
        {
            constants1_.update(0, NULL, &constants1, 0, 0);
            constants1_.setVS(1);
        }
    private:
        lgraphics::ConstantBufferRef constants0_;
        lgraphics::ConstantBufferRef constants1_;
    };


    class LPVRenderPS : public ShaderPSBase
    {
    public:
        struct Constant0
        {
            lmath::Vector4 dlDir_;
            lmath::Vector4 dlColor_;
            lmath::Vector4 cameraPos_;
            f32 cascadeDepth_[4];
        };

        struct Constant1
        {
            lmath::Vector4 diffuse_;
            lmath::Vector4 specular_;
            lmath::Vector4 shadow_;
            //lmath::Vector4 LPVMin_; //ÇSî‘ñ⁄Ç™ÉZÉãÉTÉCÉYÇÃãtêî
            //f32 invLPVSize_;
            //f32 padding0_;
            //f32 padding1_;
            //f32 padding2_;
        };

        LPVRenderPS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}

        virtual void initialize();

        void update0(const Constant0& constants0)
        {
            constants0_.update(0, NULL, &constants0, 0, 0);
            constants0_.setPS(0);
        }

        void update1(const Constant1& constants1)
        {
            constants1_.update(0, NULL, &constants1, 0, 0);
            constants1_.setPS(1);
        }
    private:
        lgraphics::ConstantBufferRef constants0_;
        lgraphics::ConstantBufferRef constants1_;
    };
}
#endif //INC_RENDER_SHADER_H__
