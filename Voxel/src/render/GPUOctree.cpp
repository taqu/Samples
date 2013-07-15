/**
@file GPUOctree.cpp
@author t-sakai
@date 2013/06/23 create

*/
#include "GPUOctree.h"
#include "ShaderManager.h"
#include "Shader.h"

#include "System.h"

#include <lframework/scene/Scene.h>

namespace render
{
    //--------------------------------------------------
    //---
    //--- AtomicCounter
    //---
    //--------------------------------------------------
    AtomicCounter::AtomicCounter()
    {
    }

    AtomicCounter::~AtomicCounter()
    {
    }

    bool AtomicCounter::create()
    {
        release();

        counter_ = lgraphics::Texture::createBuffer(
            sizeof(u32),
            lgraphics::Usage_Default,
            lgraphics::BindFlag_UnorderedAccess,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_AllowRawView,
            sizeof(u32),
            NULL,
            NULL);
        if(!counter_.valid()){
            return false;
        }

        lgraphics::UAVDesc uavDesc;
        uavDesc.dimension_ = lgraphics::UAVDimension_Buffer;
        uavDesc.format_ = lgraphics::Data_R32_TypeLess;
        uavDesc.buffer_.firstElement_ = 0;
        uavDesc.buffer_.numElements_ = 1;
        uavDesc.buffer_.flags_ = lgraphics::UAVFlag_RAW;

        counterView_ = counter_.createUAView(uavDesc);
        if(!counterView_.valid()){
            return false;
        }

        readback_ = lgraphics::Texture::createBuffer(
            sizeof(u32),
            lgraphics::Usage_Staging,
            0,
            lgraphics::CPUAccessFlag_Read,
            lgraphics::ResourceMisc_None,
            sizeof(u32),
            NULL,
            NULL);
        if(!readback_.valid()){
            return false;
        }

        return true;
    }

    void AtomicCounter::release()
    {
        readback_.destroy();
        counterView_.destroy();
        counter_.destroy();
    }

    u32 AtomicCounter::read(lgraphics::GraphicsDeviceRef& device)
    {
        device.copyResource(readback_.get(), counter_.get());

        u32 counter = 0;
        void* data = NULL;
        u32 rowPitch, depthPitch;
        if(readback_.map(data, rowPitch, depthPitch, lgraphics::GraphicsDeviceRef::MapType_Read)){
            counter = *((u32*)data);
            readback_.unmap();
        }
        return counter;
    }

    //--------------------------------------------------
    //---
    //--- GPUOctreeVoxelPool
    //---
    //--------------------------------------------------
    GPUOctreeVoxelPool::GPUOctreeVoxelPool()
    {
    }

    GPUOctreeVoxelPool::~GPUOctreeVoxelPool()
    {
    }

    bool GPUOctreeVoxelPool::create(u32 voxelResolution)
    {
        release();

        u32 num = voxelResolution*voxelResolution*voxelResolution;
        u32 size = num*sizeof(GPUOctreeVoxel);

        {
            lgraphics::ResourceViewDesc desc;
            desc.format_ = lgraphics::Data_Unknown;
            desc.buffer_.firstElement_ = 0;
            desc.buffer_.numElements_ = num;

            voxelBuffer_ = lgraphics::Texture::createBuffer(
                size,
                lgraphics::Usage_Default,
                lgraphics::BindFlag_ShaderResource|lgraphics::BindFlag_UnorderedAccess,
                lgraphics::CPUAccessFlag_None,
                lgraphics::ResourceMisc_Structured,
                sizeof(GPUOctreeVoxel),
                NULL,
                &desc);

            lgraphics::UAVDesc uavDesc;
            uavDesc.dimension_ = lgraphics::UAVDimension_Buffer;
            uavDesc.format_ = lgraphics::Data_Unknown;
            uavDesc.buffer_.firstElement_ = 0;
            uavDesc.buffer_.numElements_ = num;
            uavDesc.buffer_.flags_ = 0;

            voxelView_ = voxelBuffer_.createUAView(uavDesc);
            if(!voxelView_.valid()){
                return false;
            }
        }

        readBackBuffer_ = lgraphics::Texture::createBuffer(
                NumReadBack*sizeof(GPUOctreeVoxel),
                lgraphics::Usage_Staging,
                0,
                lgraphics::CPUAccessFlag_Read,
                lgraphics::ResourceMisc_None,
                sizeof(GPUOctreeNode0),
                NULL,
                NULL);
        return true;
    }

    void GPUOctreeVoxelPool::release()
    {
        voxelView_.destroy();
        voxelBuffer_.destroy();
    }

    void GPUOctreeVoxelPool::readback(lgraphics::GraphicsDeviceRef& device, u32 num)
    {
        num = lcore::minimum(num, NumReadBack);

        lgraphics::Box box;
        box.left_ = 0;
        box.right_ = num * sizeof(GPUOctreeVoxel);
        box.top_ = box.front_ = 0;
        box.bottom_ = box.back_ = 1;

        device.copyResource(readBackBuffer_.get(), 0, 0, 0, 0, voxelBuffer_.get(), 0, &box);

        void* data = NULL;
        u32 rowPitch, depthPitch;
        if(readBackBuffer_.map(data, rowPitch, depthPitch, lgraphics::GraphicsDeviceRef::MapType_Read)){
            lcore::memcpy(voxelsReadBack_, data, sizeof(GPUOctreeVoxel)*num);
            readBackBuffer_.unmap();

            for(u32 i=0; i<num; ++i){
                voxelPosReadBack_[i].x_ = (voxelsReadBack_[i].index_ >> 20) & 0x1FFU;
                voxelPosReadBack_[i].y_ = (voxelsReadBack_[i].index_ >> 10) & 0x1FFU;
                voxelPosReadBack_[i].z_ = (voxelsReadBack_[i].index_ >> 0) & 0x1FFU;
            }
        }
    }


    //--------------------------------------------------
    //---
    //--- GPUOctreeNodePool
    //---
    //--------------------------------------------------
    GPUOctreeNodePool::GPUOctreeNodePool()
    {
        lcore::memset(node0ReadBack_, 0, sizeof(GPUOctreeNode0)*NumReadBack);
        //lcore::memset(node1ReadBack_, 0, sizeof(u32)*NumReadBack);
    }

    GPUOctreeNodePool::~GPUOctreeNodePool()
    {
    }

    bool GPUOctreeNodePool::create(u32 voxelResolution)
    {
        release();

        //s32 level = lmath::floorS32(lmath::log(static_cast<f32>(voxelResolution))*INV_LOG2);

        u32 num = voxelResolution*voxelResolution*voxelResolution;

        {
            lgraphics::ResourceViewDesc desc;
            desc.format_ = lgraphics::Data_Unknown;
            desc.buffer_.firstElement_ = 0;
            desc.buffer_.numElements_ = num;

            node0Buffer_ = lgraphics::Texture::createBuffer(
                num*sizeof(GPUOctreeNode0),
                lgraphics::Usage_Default,
                lgraphics::BindFlag_ShaderResource|lgraphics::BindFlag_UnorderedAccess,
                lgraphics::CPUAccessFlag_None,
                lgraphics::ResourceMisc_Structured,
                sizeof(GPUOctreeNode0),
                NULL,
                &desc);

            lgraphics::UAVDesc uavDesc;
            uavDesc.dimension_ = lgraphics::UAVDimension_Buffer;
            uavDesc.format_ = lgraphics::Data_Unknown;
            uavDesc.buffer_.firstElement_ = 0;
            uavDesc.buffer_.numElements_ = num;
            uavDesc.buffer_.flags_ = 0;

            node0View_ = node0Buffer_.createUAView(uavDesc);
            if(!node0View_.valid()){
                return false;
            }
        }

        //{
        //    lgraphics::ResourceViewDesc desc;
        //    desc.format_ = lgraphics::Data_Unknown;
        //    desc.buffer_.firstElement_ = 0;
        //    desc.buffer_.numElements_ = num;

        //    node1Buffer_ = lgraphics::Texture::createBuffer(
        //        num*sizeof(u32),
        //        lgraphics::Usage_Default,
        //        lgraphics::BindFlag_ShaderResource|lgraphics::BindFlag_UnorderedAccess,
        //        lgraphics::CPUAccessFlag_None,
        //        lgraphics::ResourceMisc_Structured,
        //        sizeof(u32),
        //        NULL,
        //        &desc);

        //    lgraphics::UAVDesc uavDesc;
        //    uavDesc.dimension_ = lgraphics::UAVDimension_Buffer;
        //    uavDesc.format_ = lgraphics::Data_Unknown;
        //    uavDesc.buffer_.firstElement_ = 0;
        //    uavDesc.buffer_.numElements_ = num;
        //    uavDesc.buffer_.flags_ = 0;

        //    node1View_ = node1Buffer_.createUAView(uavDesc);
        //    if(!node1View_.valid()){
        //        return false;
        //    }
        //}

        readBack0Buffer_ = lgraphics::Texture::createBuffer(
                NumReadBack*sizeof(GPUOctreeNode0),
                lgraphics::Usage_Staging,
                0,
                lgraphics::CPUAccessFlag_Read,
                lgraphics::ResourceMisc_None,
                sizeof(GPUOctreeNode0),
                NULL,
                NULL);

        //readBack1Buffer_ = lgraphics::Texture::createBuffer(
        //        NumReadBack*sizeof(u32),
        //        lgraphics::Usage_Staging,
        //        0,
        //        lgraphics::CPUAccessFlag_Read,
        //        lgraphics::ResourceMisc_None,
        //        sizeof(u32),
        //        NULL,
        //        NULL);

        //浅いレベルの初期化データ
        u32 top = 0;
        u32 numInLevel = 1;
        for(u32 i=0; i<ConstInitLevel; ++i){

            for(u32 j=0; j<numInLevel; ++j){
                GPUOctreeNode0& node = nodeInitData_[top+j];
                for(u32 k=0; k<8; ++k){
                    //node.nexts_[k] = top + numInLevel + j*8 + k;
                    //node.nexts_[k] |= (0x01U<<31); //分割フラグ
                    node.nexts_[k] = (0x01U<<30); //終了フラグ
                    node.pos_ = 0;
                    node.data_ = (0x01U<<31);
                }
            }
            top += numInLevel;
            numInLevel *= 8;
        }

        return true;
    }

    void GPUOctreeNodePool::release()
    {
        //node1View_.destroy();
        //node1Buffer_.destroy();

        node0View_.destroy();
        node0Buffer_.destroy();
    }

    void GPUOctreeNodePool::initialize(lgraphics::GraphicsDeviceRef& device)
    {
        //浅いレベルは固定で初期化
        //バッファの範囲指定はバイト単位
        lgraphics::Box box;
        box.left_ = 0;
        box.right_ = NumConstInit * sizeof(GPUOctreeNode0);
        box.top_ = box.front_ = 0;
        box.bottom_ = box.back_ = 1;

        device.updateSubresource(node0Buffer_.get(), 0, &box, nodeInitData_, 0, 0);

        //u32 data[1] = {(0x01U<<31)};
        //box.left_ = 0;
        //box.right_ = sizeof(u32);
        //device.updateSubresource(node1Buffer_.get(), 0, &box, data, 0, 0);

    }

    void GPUOctreeNodePool::readback(lgraphics::GraphicsDeviceRef& device)
    {
        lgraphics::Box box;
        box.left_ = 0;
        box.right_ = NumReadBack * sizeof(GPUOctreeNode0);
        box.top_ = box.front_ = 0;
        box.bottom_ = box.back_ = 1;

        device.copyResource(readBack0Buffer_.get(), 0, 0, 0, 0, node0Buffer_.get(), 0, &box);

        void* data = NULL;
        u32 rowPitch, depthPitch;
        if(readBack0Buffer_.map(data, rowPitch, depthPitch, lgraphics::GraphicsDeviceRef::MapType_Read)){
            lcore::memcpy(node0ReadBack_, data, sizeof(GPUOctreeNode0)*NumReadBack);
            readBack0Buffer_.unmap();
        }


        //box.right_ = NumReadBack * sizeof(u32);

        //device.copyResource(readBack1Buffer_.get(), 0, 0, 0, 0, node1Buffer_.get(), 0, &box);

        //if(readBack1Buffer_.map(data, rowPitch, depthPitch, lgraphics::GraphicsDeviceRef::MapType_Read)){
        //    lcore::memcpy(node1ReadBack_, data, sizeof(u32)*NumReadBack);
        //    readBack1Buffer_.unmap();
        //}
    }


    //--------------------------------------------------
    //---
    //--- GPUOctreeBrickPool
    //---
    //--------------------------------------------------
    GPUOctreeBrickPool::GPUOctreeBrickPool()
    {
    }

    GPUOctreeBrickPool::~GPUOctreeBrickPool()
    {
    }

    bool GPUOctreeBrickPool::create(u32 voxelResolution)
    {
        release();

        //s32 level = lmath::floorS32(lmath::log(static_cast<f32>(voxelResolution))*INV_LOG2);

        u32 num = voxelResolution>>1; //2x2x2のボクセルグループを3x3x3に保存

        {
            lgraphics::ResourceViewDesc desc;
            desc.format_ = lgraphics::Data_R32_Float;
            desc.tex3D_.mostDetailedMip_ = 0;
            desc.tex3D_.mipLevels_ = 1;

            u32 width = num*3;
            u32 height = num*3;
            u32 depth = num*3;
            bricks_ = lgraphics::Texture::create3D(
                width,
                height,
                depth,
                1,
                lgraphics::Data_R32_Float,
                lgraphics::Usage_Default,
                lgraphics::BindFlag_ShaderResource|lgraphics::BindFlag_UnorderedAccess,
                lgraphics::CPUAccessFlag_None,
                lgraphics::ResourceMisc_None,
                lgraphics::TexFilter_MinMagLinearMipPoint,
                lgraphics::TexAddress_Clamp,
                NULL,
                &desc);

            lgraphics::UAVDesc uavDesc;
            uavDesc.dimension_ = lgraphics::UAVDimension_Texture3D;
            uavDesc.format_ = lgraphics::Data_R32_Float;
            uavDesc.tex3D_.mipSlice_ = 0;
            uavDesc.tex3D_.firstWSlice_ = 0;
            uavDesc.tex3D_.wsize_ = depth;

            bricksView_ = bricks_.createUAView(uavDesc);
            if(!bricksView_.valid()){
                return false;
            }
        }

        {
            u32 width = num*3;
            u32 height = num*3;
            u32 depth = num*3;
            readBackBuffer_ = lgraphics::Texture::create3D(
                width,
                height,
                depth,
                1,
                lgraphics::Data_R32_Float,
                lgraphics::Usage_Staging,
                0,
                lgraphics::CPUAccessFlag_Read,
                lgraphics::ResourceMisc_None,
                lgraphics::TexFilter_MinMagLinearMipPoint,
                lgraphics::TexAddress_Clamp,
                NULL,
                NULL);

            if(!readBackBuffer_.valid()){
                return false;
            }
        }
        return true;
    }

    void GPUOctreeBrickPool::release()
    {
        bricksView_.destroy();
        bricks_.destroy();
    }

    void GPUOctreeBrickPool::readback(lgraphics::GraphicsDeviceRef& device)
    {
        lgraphics::Box box;
        box.left_ = 0;
        box.right_ = 6;
        box.top_ = box.front_ = 0;
        box.bottom_ = box.back_ = 6;

        device.copyResource(readBackBuffer_.get(), 0, 0, 0, 0, bricks_.get(), 0, &box);

        void* data = NULL;
        u32 rowPitch, depthPitch;
        if(readBackBuffer_.map(data, rowPitch, depthPitch, lgraphics::GraphicsDeviceRef::MapType_Read)){
            lcore::memcpy(readBack_, data, sizeof(f32)*NumReadBack*3*3*3);
            readBackBuffer_.unmap();
        }
    }

    //--------------------------------------------------
    //---
    //--- GPUOctree
    //---
    //--------------------------------------------------
    const u32 GPUOctree::NumNodes[] =
    {
        1,      //1
        9,      //2
        73,     //4
        585,    //8
        4681,   //16
        37449,  //32
        299563, //64
        2396715,//128
        19173931, //256
    };

    GPUOctree::GPUOctree()
        :voxelizeVS_(NULL)
        ,voxelizeGS_(NULL)
        ,voxelizePS_(NULL)
        ,voxelizeTexturePS_(NULL)

        ,voxelRenderVS_(NULL)
        ,voxelRenderPS_(NULL)

        ,octreeTagVS_(NULL)
        ,octreeTagPS_(NULL)
        ,octreeCreateNodePS_(NULL)
        ,octreeInitNodePS_(NULL)
        ,octreeWriteValuePS_(NULL)
        ,octreePropagatePS_(NULL)
        ,octreeMipmapPS_(NULL)

        ,voxelResolution_(0)
        ,numMaxVoxels_(0)
        ,numLevels_(0)
    {
        worldConstant_.worldCenter_.zero();
        worldConstant_.worldSize_.zero();

        worldConstant_.worldCenter_.set(0.0f, 16.0f, 0.0f, 0.0f);
    }

    GPUOctree::~GPUOctree()
    {
    }

    bool GPUOctree::reset(f32 worldSize, u32 voxelResolution)
    {
        release();

        f32 voxelSize = worldSize/voxelResolution;
        voxelResolution_ = voxelResolution;

        numLevels_ = lmath::floorS32( lmath::log(static_cast<f32>(voxelResolution_))*INV_LOG2 );
        numMaxVoxels_ = voxelResolution_*voxelResolution_*voxelResolution_;

        worldConstant_.worldSize_.set(worldSize, worldSize*0.5f, voxelSize, voxelSize*0.5f);

        render::VoxelizePS::Constant1 voxelSizeConstant;
        voxelSizeConstant.voxelSize_[0] = 1;
        voxelSizeConstant.voxelSize_[1] = voxelResolution_;
        voxelSizeConstant.voxelSize_[2] = voxelResolution_ * voxelResolution_;
        voxelSizeConstant.voxelSize_[3] = voxelResolution_ * voxelResolution_ * voxelResolution_;

        render::ShaderManager& shaderManager = render::ShaderManager::getInstance();
        render::ShaderVSBase* vs;
        render::ShaderPSBase* ps;
        render::ShaderGSBase* gs;


        shaderManager.get(
            render::Shader_Voxelize,
            vs,
            ps);
        shaderManager.get(
            render::Shader_VoxelizeGS,
            gs);

        voxelizeVS_ = reinterpret_cast<VoxelizeVS*>(vs);
        voxelizePS_ = reinterpret_cast<VoxelizePS*>(ps);
        voxelizeGS_ = reinterpret_cast<VoxelizeGS*>(gs);

        shaderManager.get(
            render::Shader_VoxelizeTexture,
            vs,
            ps);
        voxelizeTexturePS_ = reinterpret_cast<VoxelizePS*>(ps);

        //定数バッファ初期化
        voxelizePS_->update(voxelSizeConstant);
        voxelizeTexturePS_->update(voxelSizeConstant);

        shaderManager.get(
            render::Shader_VoxelRender,
            vs,
            ps);

        voxelRenderVS_ = reinterpret_cast<VoxelRenderVS*>(vs);
        voxelRenderPS_ = reinterpret_cast<VoxelRenderPS*>(ps);

        shaderManager.get(
            render::Shader_OctreeTag,
            vs,
            ps);

        octreeTagVS_ = reinterpret_cast<OctreeTagVS*>(vs);
        octreeTagPS_ = reinterpret_cast<OctreeTagPS*>(ps);

        shaderManager.get(
            render::Shader_OctreeCreateNode,
            vs,
            ps);
        octreeCreateNodePS_ = reinterpret_cast<OctreeCreateNodePS*>(ps);

        shaderManager.get(
            render::Shader_OctreeInitNode,
            vs,
            ps);
        octreeInitNodePS_ = reinterpret_cast<OctreeInitNodePS*>(ps);

        shaderManager.get(
            render::Shader_OctreeWriteValue,
            vs,
            ps);
        octreeWriteValuePS_ = reinterpret_cast<OctreeWriteValuePS*>(ps);

        shaderManager.get(
            render::Shader_OctreePropagate,
            vs,
            ps);
        octreePropagatePS_ = reinterpret_cast<OctreePropagatePS*>(ps);

        shaderManager.get(
            render::Shader_OctreeMipmap,
            vs,
            ps);
        octreeMipmapPS_ = reinterpret_cast<OctreeMipmapPS*>(ps);

        if(!atomicCounter_.create()){
            return false;
        }

        if(!voxelPool_.create(voxelResolution_)){
            return false;
        }

        if(!nodePool_.create(voxelResolution_)){
            return false;
        }

        if(!brickPool_.create(voxelResolution_)){
            return false;
        }

        return true;
    }

    void GPUOctree::release()
    {
        //brickPool_.release();
        nodePool_.release();
        voxelPool_.release();
        atomicCounter_.release();
    }

    void GPUOctree::begin(lgraphics::GraphicsDeviceRef& device, const lscene::Scene& scene)
    {
        ID3D11UnorderedAccessView* const views[] =
        {
            voxelPool_.voxelView_.getView(),
            atomicCounter_.counterView_.getView(),
        };

        device.setRasterizerState(lgraphics::GraphicsDeviceRef::Rasterizer_FillSolidNoCull);
        device.setRenderTargetsAndUAV(0, NULL, NULL, 0, 2, views, NULL);
        device.setViewport(0, 0, voxelResolution_, voxelResolution_);

        const u32 zero[] = {0, 0, 0, 0};
        device.clearUnorderedAccessView(voxelPool_.voxelView_.getView(), zero);
        device.clearUnorderedAccessView(atomicCounter_.counterView_.getView(), zero);

        device.setDepthStencilState(lgraphics::GraphicsDeviceRef::DepthStencil_DDisableWDisable);

        voxelizeVS_->attach();
        voxelizeGS_->attach();
        //voxelizePS_->attach();

        voxelizeGS_->set(worldConstant_);
        voxelizePS_->setConstant1(); //定数バッファセット
    }

    void GPUOctree::end(lgraphics::GraphicsDeviceRef& device)
    {
        device.setGeometryShader(NULL);
        device.setInputLayout(NULL);
        device.setPrimitiveTopology(lgraphics::Primitive_PointList);
        device.setVertexBuffers(0, 0, NULL, NULL, NULL);


        //const u32 zero[] = {0, 0, 0, 0};
        //device.clearUnorderedAccessView(nodePool_.node0View_.getView(), zero);
        nodePool_.initialize(device);

        octreeTagVS_->attach();

        u32 numVoxels = atomicCounter_.read(device);
        numVoxels = lcore::minimum(numVoxels, numMaxVoxels_);
        //lcore::Log("voxels: %d", counter);
        //voxelPool_.readback(device, numVoxels);

        ID3D11UnorderedAccessView* const views0[] =
        {
            nodePool_.node0View_.getView(),
            atomicCounter_.counterView_.getView(),
            brickPool_.bricksView_.getView(),
        };

        device.setRenderTargetsAndUAV(0, NULL, NULL, 0, 2, views0, NULL);

        const u32 one[] = {1, 1, 1, 1};
        device.clearUnorderedAccessView(atomicCounter_.counterView_.getView(), one);

        octreeTagPS_->set();
        voxelPool_.voxelBuffer_.attachPS(0, 0);

        OctreeTagPS::Constant0 constant0;
        constant0.levelInfo_[0] = voxelResolution_;
        constant0.levelInfo_[2] = numLevels_;

        //----------------------------------------------
        levelInfo_[1].top_ = 0;
        levelInfo_[1].num_ = 1;
        for(u32 i=1; i<numLevels_; ++i){
            //iレベル目で分割するノードにタグ付け
            octreeTagPS_->attach();
            constant0.levelInfo_[1] = i; //level
            octreeTagPS_->update(constant0);
            device.draw(numVoxels, 0);

            //新規ノード作成
            octreeCreateNodePS_->attach();
            constant0.levelInfo_[3] = levelInfo_[i].top_; //node top index
            octreeTagPS_->update(constant0);
            device.draw(levelInfo_[i].num_, 0);

            u32 count = atomicCounter_.read(device);

            levelInfo_[i+1].top_ = levelInfo_[i].top_ + levelInfo_[i].num_;
            levelInfo_[i+1].num_ = count - levelInfo_[i+1].top_;

            //新規ノード初期化
            octreeInitNodePS_->attach();
            constant0.levelInfo_[3] = levelInfo_[i+1].top_; //node top index
            octreeTagPS_->update(constant0);
            device.draw(levelInfo_[i+1].num_, 0);
        }


        //----------------------------------------------
        ID3D11UnorderedAccessView* const views1[] =
        {
            nodePool_.node0View_.getView(),
            brickPool_.bricksView_.getView(),
        };

        device.setRenderTargetsAndUAV(0, NULL, NULL, 0, 2, views1, NULL);

        octreeWriteValuePS_->attach();
        device.draw(numVoxels, 0);

        OctreePropagatePS::Constant0 propagateConstant;
        propagateConstant.info_[0] = levelInfo_[numLevels_].top_;
        propagateConstant.info_[1] = levelInfo_[numLevels_].top_ + levelInfo_[numLevels_].num_;

        octreePropagatePS_->set(propagateConstant);
        octreePropagatePS_->attach();
        device.draw(levelInfo_[numLevels_].num_, 0);

        octreeMipmapPS_->attach();
        for(u32 i=numLevels_-1; 0<i; --i){
            propagateConstant.info_[0] = levelInfo_[i].top_;
            octreePropagatePS_->set(propagateConstant);

            device.draw(levelInfo_[i].num_, 0);
        }

        //----------------------------------------------
        //nodePool_.readback(device);
        //brickPool_.readback(device);


        //----------------------------------------------
        ID3D11UnorderedAccessView* const nullViews[] =
        {
            NULL,
            NULL,
            NULL,
            NULL,
        };
        device.setRenderTargetsAndUAV(0, NULL, NULL, 0, 4, nullViews, NULL);

        device.setRasterizerState(lgraphics::GraphicsDeviceRef::Rasterizer_FillSolid);
        device.restoreDefaultRenderTargets();
        device.setVertexShader(NULL);
        device.setGeometryShader(NULL);
        device.setPixelShader(NULL);
    }

    const lmath::Vector4 dir[] =
    {
        lmath::Vector4(0.0f, 1.0f, 0.0f, PI/4),
        lmath::Vector4(0.0f, 0.5f, 0.866025f, PI/4),
        lmath::Vector4(0.823639f, 0.5f, 0.267617f, PI*3/20),
        lmath::Vector4(0.509037f, 0.5f, -0.700629f, PI*3/20),
        lmath::Vector4(-0.509037f, 0.5f, -0.700629f, PI*3/20),
        lmath::Vector4(-0.823639f, 0.5f, 0.267617f, PI*3/20),
    };

    void GPUOctree::attachNodePool(u32 index)
    {
        nodePool_.node0Buffer_.attachPS(index);
    }

    void GPUOctree::attachTexture(u32 viewIndex, u32 sampleIndex)
    {
        brickPool_.bricks_.attachPS(viewIndex, sampleIndex);
    }

    void GPUOctree::getConstants(DefaultAOPS::Constant0& constants, const lscene::Scene& scene)
    {
        const lmath::Matrix44& view = scene.getCamera().getViewMatrix();

        f32 halfVoxelWorld = worldConstant_.worldSize_.y_;
        scene.getCamera().getEyePosition(constants.eye_);
        constants.worldMin_.set(-halfVoxelWorld, -halfVoxelWorld, -halfVoxelWorld, 0.0f);
        constants.worldMin_ += worldConstant_.worldCenter_;
        constants.worldMax_.set(halfVoxelWorld, halfVoxelWorld, halfVoxelWorld, 0.0f);
        constants.worldMax_ += worldConstant_.worldCenter_;
        constants.info_[0] = numLevels_;
        constants.eye_.w_ = worldConstant_.worldSize_.z_*1.5f;

        constants.worldSize_.x_ = worldConstant_.worldSize_.x_;
        constants.worldSize_.y_ = worldConstant_.worldSize_.z_;
        constants.worldSize_.z_ = 1.0f/worldConstant_.worldSize_.z_;
        constants.worldSize_.w_ = voxelResolution_;

    }

    void GPUOctree::drawVoxel(lgraphics::GraphicsDeviceRef& device, const lscene::Scene& scene)
    {
        //lcore::RandomXorshift& random = fractal::System::getRandom0();

        //f32 dots[6] = {0,0,0,0,0,0};
        //for(int i=0; i<100; ++i){
        //    f32 r = random.frand()*PI;
        //    f32 cs = lmath::cos(r);
        //    f32 sn = lmath::sin(r);
        //    lmath::Vector4 n(cs, sn*cs, sn, 0.0f);
        //    n.normalize();

        ////    f32 x = v0.y_ * v1.z_ - v0.z_ * v1.y_;
        ////f32 y = v0.z_ * v1.x_ - v0.x_ * v1.z_;
        ////f32 z = v0.x_ * v1.y_ - v0.y_ * v1.x_;

        //    lmath::Vector4 d[6];
        //    for(int j=0; j<6; ++j){
        //        lmath::Vector4 zaxis(0.0f, -dir[j].z_, dir[j].y_, 0.0f);
        //        lmath::Vector4 xaxis(dir[j].y_, -dir[j].x_, 0.0f, 0.0f);

        //        f32 nt = dir[j].dot(zaxis);
        //        f32 nb = dir[j].dot(xaxis);

        //        d[j].x_ = n.dot(xaxis);
        //        d[j].y_ = n.dot(dir[j]);
        //        d[j].z_ = n.dot(zaxis);
        //        d[j].w_ = 0.0f;
        //        dots[j] += n.dot(d[j]);
        //    }
        //}

        //for(int j=0; j<6; ++j){
        //    dots[j] *= 0.01f;
        //}

        const lmath::Matrix44& proj = scene.getCamera().getProjMatrix();
        const lmath::Matrix44& view = scene.getCamera().getViewMatrix();

        f32 halfVoxelWorld = worldConstant_.worldSize_.y_;
        u32 w,h;
        device.getRenderTargetDesc(w, h);
        VoxelRenderVS::Constant0 vsConstant0;
        vsConstant0.info_.x_ = proj.m_[1][1];
        vsConstant0.info_.y_ = static_cast<f32>(w)/h;
        vsConstant0.invView_ = view;
        vsConstant0.invView_.invert();
        vsConstant0.screen_[0] = w;
        vsConstant0.screen_[1] = h;

        VoxelRenderPS::Constant0 psConstant0;
        psConstant0.eye_.x_ = vsConstant0.invView_.m_[0][3];
        psConstant0.eye_.y_ = vsConstant0.invView_.m_[1][3];
        psConstant0.eye_.z_ = vsConstant0.invView_.m_[2][3];
        psConstant0.worldMin_.set(-halfVoxelWorld, -halfVoxelWorld, -halfVoxelWorld, 0.0f);
        psConstant0.worldMin_ += worldConstant_.worldCenter_;
        psConstant0.worldMax_.set(halfVoxelWorld, halfVoxelWorld, halfVoxelWorld, 0.0f);
        psConstant0.worldMax_ += worldConstant_.worldCenter_;
        psConstant0.info_[0] = numLevels_;

        psConstant0.worldSize_.x_ = worldConstant_.worldSize_.x_;
        psConstant0.worldSize_.y_ = worldConstant_.worldSize_.z_;
        psConstant0.worldSize_.z_ = 1.0f/worldConstant_.worldSize_.z_;
        psConstant0.worldSize_.w_ = voxelResolution_;

        voxelRenderVS_->set(vsConstant0);
        voxelRenderPS_->set(psConstant0);

        nodePool_.node0Buffer_.attachPS(0);
        brickPool_.bricks_.attachPS(1, 0);

        voxelRenderVS_->attach();
        voxelRenderPS_->attach();

        device.setInputLayout(NULL);
        device.setPrimitiveTopology(lgraphics::Primitive_PointList);
        device.setVertexBuffers(0, 0, NULL, NULL, NULL);
        device.draw(w*h, 0);

        //device.setVertexShader(NULL);
        //device.setGeometryShader(NULL);
        //device.setPixelShader(NULL);
        //device.clearVSResources(1);
    }
}
