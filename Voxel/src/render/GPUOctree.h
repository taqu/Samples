#ifndef INC_RENDER_GPUOCTREE_H__
#define INC_RENDER_GPUOCTREE_H__
/**
@file GPUOctree.h
@author t-sakai
@date 2013/06/23 create

*/
#include <lmath/lmath.h>
#include <lgraphics/api/TextureRef.h>
#include "render/render.h"

#include "Shader.h"

namespace lgraphics
{
    class GraphicsDeviceRef;
}

namespace lscene
{
    class Scene;
}

namespace render
{
    //--------------------------------------------------
    //---
    //--- AtomicCounter
    //---
    //--------------------------------------------------
    class AtomicCounter
    {
    public:
        AtomicCounter();
        ~AtomicCounter();

        bool create();
        void release();

        u32 read(lgraphics::GraphicsDeviceRef& device);

        lgraphics::BufferRef counter_;
        lgraphics::UnorderedAccessViewRef counterView_;

        lgraphics::BufferRef readback_;
    };

    //--------------------------------------------------
    //---
    //--- GPUOctreeVoxel
    //---
    //--------------------------------------------------
    struct GPUOctreeVoxel
    {
        u32 index_;
        u32 diffuse_;
    };

    //--------------------------------------------------
    //---
    //--- GPUOctreeVoxelPool
    //---
    //--------------------------------------------------
    class GPUOctreeVoxelPool
    {
    public:
        GPUOctreeVoxelPool();
        ~GPUOctreeVoxelPool();

        bool create(u32 voxelResolution);
        void release();

        void readback(lgraphics::GraphicsDeviceRef& device, u32 num);

        lgraphics::BufferRef voxelBuffer_;
        lgraphics::UnorderedAccessViewRef voxelView_;

        lgraphics::BufferRef readBackBuffer_;

        static const u32 NumReadBack = 256;
        GPUOctreeVoxel voxelsReadBack_[NumReadBack];

        struct Pos
        {
            u32 x_, y_, z_;
        };

        Pos voxelPosReadBack_[NumReadBack];
    };


    //--------------------------------------------------
    //---
    //--- GPUOctreeNode
    //---
    //--------------------------------------------------
    struct GPUOctreeNode0
    {
        u32 nexts_[8]; //devide flag, data type flag, 30bit next Node index
        u32 pos_;
        u32 data_;
    };

    //--------------------------------------------------
    //---
    //--- GPUOctreeNodePool
    //---
    //--------------------------------------------------
    class GPUOctreeNodePool
    {
    public:
        static const u32 ConstInitLevel = 1;//3;
        static const u32 NumConstInit = 1;//73;

        GPUOctreeNodePool();
        ~GPUOctreeNodePool();

        bool create(u32 voxelResolution);
        void release();

        void initialize(lgraphics::GraphicsDeviceRef& device);
        void readback(lgraphics::GraphicsDeviceRef& device);

        //êÛÇ¢ÉåÉxÉãÇÕå≈íËÇ≈èâä˙âª
        GPUOctreeNode0 nodeInitData_[NumConstInit]; //an = an-1 + 8^(n-1)

        lgraphics::BufferRef node0Buffer_;
        lgraphics::UnorderedAccessViewRef node0View_;

        //lgraphics::BufferRef node1Buffer_;
        //lgraphics::UnorderedAccessViewRef node1View_;

        lgraphics::BufferRef readBack0Buffer_;
        //lgraphics::BufferRef readBack1Buffer_;

        static const u32 NumReadBack = 4*4*4;//73;//585;
        GPUOctreeNode0 node0ReadBack_[NumReadBack];
        //u32 node1ReadBack_[NumReadBack];
    };

    //--------------------------------------------------
    //---
    //--- GPUOctreeBrickPool
    //---
    //--------------------------------------------------
    class GPUOctreeBrickPool
    {
    public:
        GPUOctreeBrickPool();
        ~GPUOctreeBrickPool();

        bool create(u32 voxelResolution);
        void release();

        void readback(lgraphics::GraphicsDeviceRef& device);

        lgraphics::Texture3DRef bricks_;
        lgraphics::UnorderedAccessViewRef bricksView_;

        lgraphics::Texture3DRef readBackBuffer_;

        static const u32 NumReadBack = 2*2*2;

        f32 readBack_[NumReadBack*3*3*3];
    };

    //--------------------------------------------------
    //---
    //--- GPUOctree
    //---
    //--------------------------------------------------
    class GPUOctree
    {
    public:
        static const u32 NumNodes[];
        static const u32 MaxLevel = 9;

        GPUOctree();
        ~GPUOctree();

        bool reset(f32 worldSize, u32 voxelResolution);
        void release();

        void begin(lgraphics::GraphicsDeviceRef& device, const lscene::Scene& scene);
        void end(lgraphics::GraphicsDeviceRef& device);

        VoxelizeVS* getVoxelizeVS() { return voxelizeVS_;}
        VoxelizeGS* getVoxelizeGS() { return voxelizeGS_;}
        VoxelizePS* getVoxelizePS() { return voxelizePS_;}
        VoxelizePS* getVoxelizeTexturePS() { return voxelizeTexturePS_;}

        //VoxelRenderVS* getVoxelRenderVS(){ return voxelRenderVS_;}
        //VoxelRenderPS* getVoxelRenderPS(){ return voxelRenderPS_;}

        void attachNodePool(u32 index);
        void attachTexture(u32 viewIndex, u32 sampleIndex);

        void getConstants(DefaultAOPS::Constant0& constants, const lscene::Scene& scene);

        void drawVoxel(lgraphics::GraphicsDeviceRef& device, const lscene::Scene& scene);

    private:
        struct LevelInfo
        {
            u32 top_;
            u32 num_;
        };

        VoxelizeVS* voxelizeVS_;
        VoxelizeGS* voxelizeGS_;
        VoxelizePS* voxelizePS_;
        VoxelizePS* voxelizeTexturePS_;

        VoxelRenderVS* voxelRenderVS_;
        VoxelRenderPS* voxelRenderPS_;

        OctreeTagVS* octreeTagVS_;
        OctreeTagPS* octreeTagPS_;

        OctreeCreateNodePS* octreeCreateNodePS_;

        OctreeInitNodePS* octreeInitNodePS_;

        OctreeWriteValuePS* octreeWriteValuePS_;
        OctreePropagatePS* octreePropagatePS_;
        OctreeMipmapPS* octreeMipmapPS_;

        u32 voxelResolution_;
        u32 numMaxVoxels_;
        u32 numLevels_;
        LevelInfo levelInfo_[MaxLevel+1];

        AtomicCounter atomicCounter_;

        render::VoxelizeGS::Constant worldConstant_;
        GPUOctreeVoxelPool voxelPool_;
        GPUOctreeNodePool nodePool_;

        GPUOctreeBrickPool brickPool_;
    };
}
#endif //INC_RENDER_GPUOCTREE_H__
