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

namespace lmath
{
    class DualQuaternion;
}

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
            lmath::Vector4 cameraPos_;
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
            //f32 cascadeDepth_[4];
        };

        struct Constant1
        {
            lmath::Vector4 diffuse_;
            lmath::Vector4 specular_;
            lmath::Vector4 shadow_;
        };

        struct Constant2
        {
            f32 voxelDiameter_;
            f32 invVoxelDiameter_;
            f32 reserved0_;
            f32 reserved1_;
            lmath::Matrix44 worldToVoxelSpace_;
            lmath::Vector4 voxelScale_;
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

        void update2(const Constant2& constant2)
        {
            constant2_.update(0, NULL, &constant2, 0, 0);
            constant2_.setPS(2);
        }
    private:
        lgraphics::ConstantBufferRef constant0_;
        lgraphics::ConstantBufferRef constant1_;
        lgraphics::ConstantBufferRef constant2_;
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

        void set()
        {
            constants_.setVS(0);
        }

        void update(const lmath::Matrix44 mat[4]);
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
    //--- TonemappingStaticAO
    //---
    //----------------------------------------------
    class TonemappingStaticAOPS : public ShaderPSBase
    {
    public:
        TonemappingStaticAOPS(const lgraphics::PixelShaderRef& ref)
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
    //--- Debug
    //---
    //----------------------------------------------
    class DebugVS : public ShaderVSBase
    {
    public:
        DebugVS(const lgraphics::VertexShaderRef& ref)
            :ShaderVSBase(ref)
        {}

        virtual void initialize()
        {
            constant_ = lgraphics::ConstantBuffer::create(
            sizeof(lmath::Matrix44),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);
        }

        void update(const lmath::Matrix44& mwvp)
        {
            constant_.update(0, NULL, &mwvp, 0, 0);
            constant_.setVS(0);
        }
    private:
        lgraphics::ConstantBufferRef constant_;
    };


    class DebugPS : public ShaderPSBase
    {
    public:
        DebugPS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}
    };

    //----------------------------------------------
    //---
    //--- Voxelize
    //---
    //----------------------------------------------
    class VoxelizeVS : public ShaderVSBase
    {
    public:
        VoxelizeVS(const lgraphics::VertexShaderRef& ref)
            :ShaderVSBase(ref)
        {}

        virtual void initialize();

        void set(const lmath::Matrix44& world)
        {
            constants_.update(0, NULL, &world, 0, 0);
            constants_.setVS(0);
        }

        void setGS(s32 index, const lmath::Matrix44& mwvp)
        {
            constants_.update(0, NULL, &mwvp, 0, 0);
            constants_.setGS(index);
        }
    private:
        lgraphics::ConstantBufferRef constants_;
    };

    class VoxelizeGS : public ShaderGSBase
    {
    public:
        VoxelizeGS(const lgraphics::GeometryShaderRef& ref)
            :ShaderGSBase(ref)
        {}

        virtual void initialize()
        {}
    };

    class VoxelizePS : public ShaderPSBase
    {
    public:
        VoxelizePS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}

        virtual void initialize()
        {}
    };

    //----------------------------------------------
    //---
    //--- VoxelRender
    //---
    //----------------------------------------------
    class VoxelRenderVS : public ShaderVSBase
    {
    public:
        VoxelRenderVS(const lgraphics::VertexShaderRef& ref)
            :ShaderVSBase(ref)
        {}

        virtual void initialize()
        {}
    };

    class VoxelRenderGS : public ShaderGSBase
    {
    public:
        VoxelRenderGS(const lgraphics::GeometryShaderRef& ref)
            :ShaderGSBase(ref)
        {}

        virtual void initialize()
        {}
    };

    class VoxelRenderPS : public ShaderPSBase
    {
    public:
        VoxelRenderPS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}

        virtual void initialize()
        {}
    };

    //----------------------------------------------
    //---
    //--- Reflective ShadowMap
    //---
    //----------------------------------------------
    class ReflectiveShadowMapVS : public ShaderVSBase
    {
    public:
        struct Constant0
        {
            lmath::Matrix44 mwvp_;
            lmath::Matrix44 mw_;
        };

        ReflectiveShadowMapVS(const lgraphics::VertexShaderRef& ref)
            :ShaderVSBase(ref)
        {}

        virtual void initialize();

        void update0(const Constant0& constant0)
        {
            constant0_.update(0, NULL, &constant0, 0, 0);
            constant0_.setVS(0);
        }
    private:
        lgraphics::ConstantBufferRef constant0_;
    };

    class ReflectiveShadowMapPS : public ShaderPSBase
    {
    public:
        struct Constant0
        {
            lmath::Vector4 dlDir_;
            lmath::Vector4 dlColor_;
        };

        struct Constant1
        {
            lmath::Vector4 diffuse_;
        };

        ReflectiveShadowMapPS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}

        virtual void initialize();

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

    class ReflectiveShadowMapTextureVS : public ShaderVSBase
    {
    public:
        ReflectiveShadowMapTextureVS(const lgraphics::VertexShaderRef& ref)
            :ShaderVSBase(ref)
        {}

        virtual void initialize()
        {}
    };

    class ReflectiveShadowMapTexturePS : public ShaderPSBase
    {
    public:
        ReflectiveShadowMapTexturePS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}

        virtual void initialize()
        {}
    };

    //----------------------------------------------
    //---
    //--- VoxelInjection
    //---
    //----------------------------------------------
    class VoxelInjectionVS : public ShaderVSBase
    {
    public:
        VoxelInjectionVS(const lgraphics::VertexShaderRef& ref)
            :ShaderVSBase(ref)
        {}

        virtual void initialize()
        {}
    };

    class VoxelInjectionPS : public ShaderPSBase
    {
    public:
        VoxelInjectionPS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}

        virtual void initialize()
        {}
    };

    //----------------------------------------------
    //---
    //--- VoxelMipmap
    //---
    //----------------------------------------------
    class VoxelMipmapVS : public ShaderVSBase
    {
    public:
        VoxelMipmapVS(const lgraphics::VertexShaderRef& ref)
            :ShaderVSBase(ref)
        {}

        virtual void initialize()
        {}
    };

    class VoxelMipmapPS : public ShaderPSBase
    {
    public:
        VoxelMipmapPS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}

        virtual void initialize()
        {}
    };

    //----------------------------------------------
    //---
    //--- ReconstructZ
    //---
    //----------------------------------------------
    class ReconstructZPS : public ShaderPSBase
    {
    public:
        ReconstructZPS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}
    };

    //----------------------------------------------
    //---
    //--- Downsample2x2
    //---
    //----------------------------------------------
    class Downsample2x2PS : public ShaderPSBase
    {
    public:
        Downsample2x2PS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}
    };

    //----------------------------------------------
    //---
    //--- Copy
    //---
    //----------------------------------------------
    class CopyPS : public ShaderPSBase
    {
    public:
        CopyPS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}
    };

    //----------------------------------------------
    //---
    //--- SAO
    //---
    //----------------------------------------------
    class SAOPS : public ShaderPSBase
    {
    public:
        SAOPS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}
    };

    //----------------------------------------------
    //---
    //--- SAOBlur
    //---
    //----------------------------------------------
    class SAOBlurPS : public ShaderPSBase
    {
    public:
        SAOBlurPS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}
    };
}
#endif //INC_RENDER_SHADER_H__
