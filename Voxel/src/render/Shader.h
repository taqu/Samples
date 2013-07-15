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
    //--- Voxelize
    //---
    //----------------------------------------------
    class VoxelizeVS : public ShaderVSBase
    {
    public:
        //struct Constant
        //{
        //    lmath::Matrix44 mw_;
        //};

        VoxelizeVS(const lgraphics::VertexShaderRef& ref)
            :ShaderVSBase(ref)
        {}

        virtual void initialize();

        void set(const lmath::Matrix44& world)
        {
            constant0_.update(0, NULL, &world, 0, 0);
            constant0_.setVS(0);
        }
    private:
        lgraphics::ConstantBufferRef constant0_;
    };

    class VoxelizeGS : public ShaderGSBase
    {
    public:
        struct Constant
        {
            lmath::Vector4 worldCenter_;
            lmath::Vector4 worldSize_; //world size, half world size, voxel size, half voxel size
        };

        VoxelizeGS(const lgraphics::GeometryShaderRef& ref)
            :ShaderGSBase(ref)
        {}

        virtual void initialize();

        void set(const Constant& constant)
        {
            constant0_.update(0, NULL, &constant, 0, 0);
            constant0_.setGS(0);
        }
    private:
        lgraphics::ConstantBufferRef constant0_;
    };

    class VoxelizePS : public ShaderPSBase
    {
    public:
        struct Constant0
        {
            lmath::Vector4 diffuse_;
        };

        struct Constant1
        {
            u32 voxelSize_[4]; //(1, voxel resolution, (voxel resolution)^2, (voxel resolution)^3)
        };

        VoxelizePS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}

        virtual void initialize();

        void set(const Constant0& constant)
        {
            constant0_.update(0, NULL, &constant, 0, 0);
            constant0_.setPS(0);
        }

        void update(const Constant1& constant)
        {
            constant1_.update(0, NULL, &constant, 0, 0);
        }

        void setConstant1()
        {
            constant1_.setPS(1);
        }
    private:
        lgraphics::ConstantBufferRef constant0_;
        lgraphics::ConstantBufferRef constant1_;
    };

    //----------------------------------------------
    //---
    //--- VoxelRender
    //---
    //----------------------------------------------
    class VoxelRenderVS : public ShaderVSBase
    {
    public:
        struct Constant0
        {
            lmath::Vector4 info_; //{near, aspect}
            u32 screen_[4]; //{width, height, 0, 0}
            lmath::Matrix44 invView_;
        };

        VoxelRenderVS(const lgraphics::VertexShaderRef& ref)
            :ShaderVSBase(ref)
        {}

        virtual void initialize();

        void set(const Constant0& constant)
        {
            constant0_.update(0, NULL, &constant, 0, 0);
            constant0_.setVS(0);
        }

    private:
        lgraphics::ConstantBufferRef constant0_;
    };

    class VoxelRenderPS : public ShaderPSBase
    {
    public:
        struct Constant0
        {
            lmath::Vector4 eye_;
            lmath::Vector4 worldMin_;
            lmath::Vector4 worldMax_;
            lmath::Vector4 worldSize_;
            s32 info_[4];
        };

        VoxelRenderPS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}

        virtual void initialize();

        void set(const Constant0& constant)
        {
            constant0_.update(0, NULL, &constant, 0, 0);
            constant0_.setPS(0);
        }

    private:
        lgraphics::ConstantBufferRef constant0_;
    };

    //----------------------------------------------
    //---
    //--- OctreeTag
    //---
    //----------------------------------------------
    class OctreeTagVS : public ShaderVSBase
    {
    public:
        OctreeTagVS(const lgraphics::VertexShaderRef& ref)
            :ShaderVSBase(ref)
        {}
    };

    class OctreeTagPS : public ShaderPSBase
    {
    public:
        struct Constant0
        {
            u32 levelInfo_[4]; //{(voxel resolution), level, (max level), (node top)}
        };

        OctreeTagPS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}

        virtual void initialize();

        void update(const OctreeTagPS::Constant0& constant)
        {
            constant0_.update(0, NULL, &constant, 0, 0);
        }

        void set()
        {
            constant0_.setPS(0);
        }

    private:
        lgraphics::ConstantBufferRef constant0_;
    };

    //----------------------------------------------
    //---
    //--- OctreeCreateNode
    //---
    //----------------------------------------------
    class OctreeCreateNodePS : public ShaderPSBase
    {
    public:
        OctreeCreateNodePS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}
    };

    //----------------------------------------------
    //---
    //--- OctreeInitNode
    //---
    //----------------------------------------------
    class OctreeInitNodePS : public ShaderPSBase
    {
    public:
        OctreeInitNodePS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}
    };

    //----------------------------------------------
    //---
    //--- OctreeWriteValuePS
    //---
    //----------------------------------------------
    class OctreeWriteValuePS : public ShaderPSBase
    {
    public:
        OctreeWriteValuePS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}
    };

    //----------------------------------------------
    //---
    //--- OctreePropagatePS
    //---
    //----------------------------------------------
    class OctreePropagatePS : public ShaderPSBase
    {
    public:
        struct Constant0
        {
            u32 info_[4]; //{(node top), (node end), 0, 0}
        };

        OctreePropagatePS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}

        virtual void initialize();

        void set(const OctreePropagatePS::Constant0& constant)
        {
            constant0_.update(0, NULL, &constant, 0, 0);
            constant0_.setPS(0);
        }

    private:
        lgraphics::ConstantBufferRef constant0_;
    };

    //----------------------------------------------
    //---
    //--- OctreeMipmapPS
    //---
    //----------------------------------------------
    class OctreeMipmapPS : public ShaderPSBase
    {
    public:
        OctreeMipmapPS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}
    };

    //----------------------------------------------
    //---
    //--- DefaultAO
    //---
    //----------------------------------------------
    class DefaultAOPS : public ShaderPSBase
    {
    public:
        struct Constant0
        {
            lmath::Vector4 dlDir_;
            lmath::Vector4 dlColor_;
            lmath::Vector4 cameraPos_;

            lmath::Vector4 eye_;
            lmath::Vector4 worldMin_;
            lmath::Vector4 worldMax_;
            lmath::Vector4 worldSize_;
            s32 info_[4];
        };

        struct Constant1
        {
            lmath::Vector4 diffuse_;
            lmath::Vector4 specular_;
            lmath::Vector4 transparent_;
        };

        DefaultAOPS(const lgraphics::PixelShaderRef& ref)
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

    //----------------------------------------------
    //---
    //--- DefaultAOTexture
    //---
    //----------------------------------------------
    class DefaultAOTexturePS : public ShaderPSBase
    {
    public:

        DefaultAOTexturePS(const lgraphics::PixelShaderRef& ref)
            :ShaderPSBase(ref)
        {}
    };
}
#endif //INC_RENDER_SHADER_H__
