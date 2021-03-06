/**
@file Renderer.cpp
@author t-sakai
@date 2011/12/31 create
*/
#include "stdafx.h"
#include "Renderer.h"

#include <lcore/Vector.h>
#include <lcore/liostream.h>
#include <lgraphics/api/BlendStateRef.h>
#include <lgraphics/api/TextureRef.h>
#include <lgraphics/api/SamplerStateRef.h>

#include <lframework/scene/Frustum.h>
#include <lframework/scene/Scene.h>
#include <lframework/scene/ShadowMap.h>
#include <lgraphics/io11/IODDS.h>
#include "ShadowMap.h"

#include "../System.h"
#include "Input.h"
#include "render/Rectangle.h"
#include "ShaderManager.h"
#include "Shader.h"

#include "render/Material.h"
#include "render/Mesh.h"
#include "render/Object.h"
#include "render/Node.h"

#include "render/DebugDraw.h"
#include "GPUOctree.h"

namespace render
{
    class Renderer::Impl
    {
    public:
        typedef lcore::vector_arena<Object*> ObjectVector;
        typedef lcore::vector_arena<NodeBase*> NodeVector;

        Impl();
        ~Impl();

        bool initialize(u32 numEntries, u32 maxDebugLines, u32 virtualWidth, u32 virtualHeight);
        void terminate();
        bool reset(){ return true;}

        void updateLightColor();

        void setFlag(Renderer::Flag flag, bool enable)
        {
            if(enable){
                flags_ |= flag;
            }else{
                flags_ &= ~flag;
            }

            updateLightColor();
        }

        bool getFlag(Renderer::Flag flag) const
        {
            return (flags_ & flag) != 0;
        }

        void add(Object* obj);
        void add(NodeBase* node);

        void addArea(terrain::Area* area);

        void addSprite(f32 x, f32 y, s32 texture, u16 uv, u16 layer, f32 width, f32 height, f32 rotation, u32 color);

        void draw();

        void calcZFar(f32 minz, f32 maxz);

        void renderOctree();
        void renderShadowMap();

        u32 halfVirtualWidth_;
        u32 halfVirtualHeight_;

        u32 flags_;
        lgraphics::Texture2DRef renderTargetF16_;
        lgraphics::RenderTargetViewRef renderTargetF16View_;

        lscene::Scene scene_;
        lmath::Vector4 lightColor_;

        NodeVector nodeVector_;

        Rectangle rectangle_;
        u32 backWidth_;
        u32 backHeight_;
        ID3D11RenderTargetView* renderTargetView_;
        ID3D11DepthStencilView* depthStencilView_;

        ShadowMap shadowMapResource_;
        lscene::ShadowMap shadowMap_;

        render::DefaultPNVS* defaultPNVS_;
        render::DefaultPS* defaultPS_;

        render::DefaultPNUVS* defaultPNUVS_;
        render::DefaultTexturePS* defaultTexturePS_;

        render::DefaultAOPS* defaultAOPS_;
        render::DefaultAOTexturePS* defaultAOTexturePS_;

        render::TonemappingStaticPS* tonemappingPS_;

        render::GPUOctree gpuOctree_;

#if _DEBUG
        DebugDraw debugDraw_;
#endif
    };

    Renderer::Impl::Impl()
        :halfVirtualWidth_(1)
        ,halfVirtualHeight_(1)
        ,flags_(0)
        ,backWidth_(0)
        ,backHeight_(0)
        ,renderTargetView_(NULL)
        ,depthStencilView_(NULL)
        ,defaultPNVS_(NULL)
        ,defaultPS_(NULL)
        ,defaultPNUVS_(NULL)
        ,defaultTexturePS_(NULL)
    {
    }

    Renderer::Impl::~Impl()
    {
    }

    bool Renderer::Impl::initialize(u32 numEntries, u32 maxDebugLines, u32 virtualWidth, u32 virtualHeight)
    {
        halfVirtualWidth_ = virtualWidth >> 1;
        halfVirtualHeight_ = virtualHeight >> 1;

        lgraphics::GraphicsDeviceRef& device = lgraphics::Graphics::getDevice();

        const f32 color[] = {0.3f, 0.3f, 0.3f, 1.0f};
        //const f32 color[] = {0.0f, 0.0f, 0.0f, 1.0f};
        device.setClearColor(color);
        device.getRenderTargetDesc(backWidth_, backHeight_);
        device.getRenderTargets(1, &renderTargetView_, &depthStencilView_);

        {//HDR用RenderTarget作成
            lgraphics::ResourceViewDesc desc;
            desc.dimension_ = lgraphics::ViewSRVDimension_Texture2D;
            desc.format_ = lgraphics::Data_R16G16B16A16_Float;
            desc.tex2D_.mipLevels_ = 1;
            desc.tex2D_.mostDetailedMip_ = 0;

            renderTargetF16_ = lgraphics::Texture::create2D(
                backWidth_,
                backHeight_,
                1,
                1,
                lgraphics::Data_R16G16B16A16_Float,
                lgraphics::Usage_Default,
                lgraphics::BindFlag_ShaderResource|lgraphics::BindFlag_RenderTarget,
                lgraphics::CPUAccessFlag_None,
                lgraphics::ResourceMisc_None,
                lgraphics::TexFilter_MinMagMipPoint,
                lgraphics::TexAddress_Clamp,
                lgraphics::Cmp_Never,
                0.0f,
                NULL,
                &desc);

            lgraphics::RTVDesc rtvDesc;
            rtvDesc.dimension_ = lgraphics::ViewRTVDimension_Texture2D;
            rtvDesc.format_ = lgraphics::Data_R16G16B16A16_Float;
            rtvDesc.tex2D_.mipSlice_ = 0;

            renderTargetF16View_ = renderTargetF16_.createRTView(rtvDesc);
        }

        NodeVector tmpNode(numEntries);
        nodeVector_.swap(tmpNode);

        lightColor_.one();
        updateLightColor();

        u32 shadowMapSize = 2048;

        shadowMap_.initialize(1, shadowMapSize, 0.01f, 100.0f);

        shadowMapResource_.reset(shadowMapSize);

        if(false == rectangle_.create()){
            return false;
        }

        {//シェーダ取得
            render::ShaderManager& shaderManager = render::ShaderManager::getInstance();
            render::ShaderVSBase* vs;
            render::ShaderPSBase* ps;

            shaderManager.get(
                render::Shader_Default,
                vs,
                ps);

            defaultPNVS_ = reinterpret_cast<DefaultPNVS*>(vs);
            defaultPS_ = reinterpret_cast<DefaultPS*>(ps);

            shaderManager.get(
                render::Shader_DefaultTexture,
                vs,
                ps);

            defaultPNUVS_ = reinterpret_cast<DefaultPNUVS*>(vs);
            defaultTexturePS_ = reinterpret_cast<DefaultTexturePS*>(ps);

            shaderManager.get(
                render::Shader_DefaultAO,
                vs,
                ps);
            defaultAOPS_ = reinterpret_cast<DefaultAOPS*>(ps);

            shaderManager.get(
                render::Shader_DefaultAOTexture,
                vs,
                ps);
            defaultAOTexturePS_ = reinterpret_cast<DefaultAOTexturePS*>(ps);

            shaderManager.get(
                render::Shader_TonemappingStatic,
                vs,
                ps);
            tonemappingPS_ = reinterpret_cast<TonemappingStaticPS*>(ps);
        }

        gpuOctree_.reset(32.0f, 64);

#if _DEBUG
        debugDraw_.initialize(maxDebugLines);
#endif
        return true;
    }

    void Renderer::Impl::terminate()
    {
#if _DEBUG
        debugDraw_.terminate();
#endif
        gpuOctree_.release();

        SAFE_RELEASE(renderTargetView_);
        SAFE_RELEASE(depthStencilView_);
    }

    void Renderer::Impl::updateLightColor()
    {
        //HDRの場合はintensityをかける
        if(getFlag(Renderer::Flag_HDR)){
            lmath::Vector4 color(lightColor_.x_*lightColor_.w_, lightColor_.y_*lightColor_.w_, lightColor_.z_*lightColor_.w_, lightColor_.w_);
            scene_.getLightEnv().getDirectionalLight().setColor(color);
        }else{
            scene_.getLightEnv().getDirectionalLight().setColor(lightColor_);
        }
    }

    void Renderer::Impl::add(Object* obj)
    {
        LASSERT(obj != NULL);
        lmath::Matrix44 mat;
        obj->rotation().getMatrix(mat);
        mat.setTranslate( obj->position().x_, obj->position().y_, obj->position().z_ );

        lmath::Matrix44 local;
        u32 i;
        for(i=0; i<obj->getNumNodes(); ++i){
            Node& node = obj->getNode(i);
            if(InvalidNodeIndex != node.parentIndex_){
                break;
            }

            node.calcLocalMatrix(local);
            node.world_.mul(mat, local);
            nodeVector_.push_back(&node);
        }

        for(;i<obj->getNumNodes(); ++i){
            Node& node = obj->getNode(i);
            Node& parent = obj->getNode( node.parentIndex_ );

            node.calcLocalMatrix(local);
            node.world_.mul(parent.world_, local);
            //node.world_ = node.matrix_;
            nodeVector_.push_back(&node);
        }
    }

    void Renderer::Impl::add(NodeBase* node)
    {
        LASSERT(node != NULL);
        nodeVector_.push_back(node);
    }


    void Renderer::Impl::draw()
    {
        f32 deltaTime = fractal::System::getTimer().getDeltaTime();

        renderOctree();
        //renderShadowMap();

        lgraphics::GraphicsDeviceRef& device = lgraphics::Graphics::getDevice();

        //HDR用RenderTargetセット
        ID3D11RenderTargetView* views[1];
        if(getFlag(Renderer::Flag_HDR)){
            views[0] = renderTargetF16View_.getView();
            device.setRenderTargets(1, views, depthStencilView_);
            device.clearRenderTargetView(renderTargetF16View_.getView(), device.getClearColor());

        }else{
            device.clearRenderTargetView();
        }

        device.clearDepthStencilView();

        device.setDepthStencilState(lgraphics::GraphicsDeviceRef::DepthStencil_DEnableWEnable);
        device.setBlendState(lgraphics::GraphicsDeviceRef::BlendState_NoAlpha);
        device.setRasterizerState(lgraphics::GraphicsDeviceRef::Rasterizer_FillSolid);

        const lscene::DirectionalLight& dlight = scene_.getLightEnv().getDirectionalLight();
        lmath::Matrix44 shadowMapProjection = shadowMap_.getLightProjection(0);

        DefaultPNVS::Constant vsConstant0;
        DefaultAOPS::Constant0 psConstant0;
        DefaultAOPS::Constant1 psConstant1;

        psConstant0.dlColor_ = dlight.getColor();
        psConstant0.dlDir_ = dlight.getDirection();

        lmath::Matrix44 invView = scene_.getCamera().getViewMatrix();
        invView.invert();
        invView.getTranslate(psConstant0.cameraPos_);

        gpuOctree_.getConstants(psConstant0, scene_);
        defaultAOPS_->update0(psConstant0);

        shadowMapResource_.getTex().attachPS(0, 0);

        gpuOctree_.attachNodePool(1);
        gpuOctree_.attachTexture(2, 1);
#if 1
        ShaderVSBase* vs = NULL;
        ShaderPSBase* ps = NULL;
        u32 numBatches = 0;
        for(NodeVector::iterator itr = nodeVector_.begin();
            itr != nodeVector_.end();
            ++itr)
        {
            NodeBase* node = (*itr);
            vsConstant0.mwvp_.mul(scene_.getViewProjMatrix(), node->world_);
            vsConstant0.mw_ = node->world_;
            vsConstant0.mlwvp_.mul(shadowMapProjection, node->world_);
            defaultPNVS_->update(vsConstant0);

            for(u32 j=0; j<node->numMeshes_; ++j){
                Mesh& mesh = node->meshes_[j];
                device.setPrimitiveTopology(mesh.getType());

                Material& material = *(mesh.getMaterial());

                psConstant1.diffuse_ = material.diffuse_;
                psConstant1.specular_ = material.specular_;
                psConstant1.shadow_ = material.shadow_;

                defaultAOPS_->update1(psConstant1);

                ShaderVSBase* selectedVS;
                ShaderPSBase* selectedPS;
                if(NULL == material.textures_[0]){
                    selectedVS = defaultPNVS_;
                    selectedPS = defaultAOPS_;
                }else{
                    selectedVS = defaultPNUVS_;
                    selectedPS = defaultAOTexturePS_;
                    material.textures_[0]->attachPS(3, 2);
                }
                if(selectedPS != ps){
                    vs = selectedVS;
                    vs->attach();

                    ps = selectedPS;
                    ps->attach();
                }

                Geometry& geometry = *(mesh.getGeometry());

                geometry.attach();

                device.drawIndexed(mesh.getNumIndices(), mesh.getIndexOffset(), 0);
                ++numBatches;
            }
        }
#else
        gpuOctree_.drawVoxel(device, scene_);
#endif
        //device.setRasterizerState(lgraphics::GraphicsDeviceRef::Rasterizer_FillSolid);
        nodeVector_.clear();

        if(getFlag(Renderer::Flag_HDR)){
            //元に戻す
            views[0] = renderTargetView_;
            device.setRenderTargets(1, views, depthStencilView_);
            device.setViewport(0, 0, backWidth_, backHeight_);

            device.setDepthStencilState(lgraphics::GraphicsDeviceRef::DepthStencil_DDisableWDisable);
            tonemappingPS_->attach();
        }

#if _DEBUG
        //debugDraw_.draw(device);
#endif

        device.clearPSResources(4);
    }

    void Renderer::Impl::renderOctree()
    {
        lgraphics::GraphicsDeviceRef& device = lgraphics::Graphics::getDevice();

        gpuOctree_.begin(device, scene_);

        const lscene::DirectionalLight& dlight = scene_.getLightEnv().getDirectionalLight();

        //lmath::Matrix44 invView = scene_.getCamera().getViewMatrix();
        //invView.invert();
        //invView.getTranslate(psConstant0.cameraPos_);

        VoxelizePNVS* voxelizePNVS = gpuOctree_.getVoxelizePNVS();
        VoxelizePNUVS* voxelizePNUVS = gpuOctree_.getVoxelizePNUVS();

        VoxelizePNGS* voxelizePNGS = gpuOctree_.getVoxelizePNGS();
        VoxelizePNUGS* voxelizePNUGS = gpuOctree_.getVoxelizePNUGS();

        VoxelizePS* voxelizePS = gpuOctree_.getVoxelizePS();
        VoxelizePS* voxelizeTexturePS = gpuOctree_.getVoxelizeTexturePS();
        VoxelizePS::Constant0 voxelizePSConstant;

        ShaderVSBase* vs = NULL;
        ShaderGSBase* gs = NULL;
        ShaderPSBase* ps = NULL;
        for(NodeVector::iterator itr = nodeVector_.begin();
            itr != nodeVector_.end();
            ++itr)
        {
            NodeBase* node = (*itr);
            voxelizePNVS->set(node->world_);
            for(u32 j=0; j<node->numMeshes_; ++j){
                Mesh& mesh = node->meshes_[j];
                device.setPrimitiveTopology(mesh.getType());

                Material& material = *(mesh.getMaterial());

                voxelizePSConstant.diffuse_ = material.diffuse_;

                voxelizePS->set(voxelizePSConstant);

                ShaderVSBase* selectedVS;
                ShaderGSBase* selectedGS;
                ShaderPSBase* selectedPS;
                if(NULL == material.textures_[0]){
                    selectedVS = voxelizePNVS;
                    selectedGS = voxelizePNGS;
                    selectedPS = voxelizePS;
                }else{
                    selectedVS = voxelizePNUVS;
                    selectedGS = voxelizePNUGS;
                    selectedPS = voxelizeTexturePS;
                    material.textures_[0]->attachPS(0, 0);
                }
                if(selectedPS != ps){
                    vs = selectedVS;
                    vs->attach();
                    gs = selectedGS;
                    gs->attach();
                    ps = selectedPS;
                    ps->attach();
                }

                Geometry& geometry = *(mesh.getGeometry());

                geometry.attach();

                device.drawIndexed(mesh.getNumIndices(), mesh.getIndexOffset(), 0);
            }
        }

        gpuOctree_.end(device);
    }

    void Renderer::Impl::renderShadowMap()
    {
        lgraphics::GraphicsDeviceRef& device = lgraphics::Graphics::getDevice();

        device.setRasterizerState(lgraphics::GraphicsDeviceRef::Rasterizer_DepthMap);
        shadowMap_.update(scene_);
        // ShadowMap
        //---------------------------
        shadowMapResource_.begin(device);

        const lmath::Matrix44& lightViewProj = shadowMap_.getLightProjection(0);

        DepthVS* vs = shadowMapResource_.getVS();

        lmath::Matrix44 mat;
        for(NodeVector::iterator itr = nodeVector_.begin();
            itr != nodeVector_.end();
            ++itr)
        {
            NodeBase* node = (*itr);
            mat.mul(lightViewProj, node->world_);
            vs->set(mat);

            for(u32 j=0; j<node->numMeshes_; ++j){
                Mesh& mesh = node->meshes_[j];
                device.setPrimitiveTopology(mesh.getType());

                Geometry& geometry = *(mesh.getGeometry());

                geometry.attach();

                device.drawIndexed(mesh.getNumIndices(), mesh.getIndexOffset(), 0);
            }
        }

        shadowMapResource_.end(device);
        device.setViewport(0, 0, backWidth_, backHeight_);
    }

    void Renderer::Impl::calcZFar(f32 minz, f32 maxz)
    {

        lmath::Vector4 aabbMin(-64.0f, -64.0f, -64.0f, 0.0f);
        lmath::Vector4 aabbMax(64.0f, 64.0f, 64.0f, 0.0f);

        shadowMap_.setSceneAABB(aabbMin, aabbMax);
    }


    //---------------------------------------------------------------
    //---
    //--- Renderer
    //---
    //---------------------------------------------------------------
    Renderer::Renderer()
    {
        impl_ = LIME_NEW Impl();
    }

    Renderer::~Renderer()
    {
        LIME_DELETE(impl_);
    }

    bool Renderer::initialize(u32 numEntries, u32 maxDebugLines, u32 virtualWidth, u32 virtualHeight)
    {
        return impl_->initialize(numEntries, maxDebugLines, virtualWidth, virtualHeight);
    }

    void Renderer::terminate()
    {
        return impl_->terminate();
    }

    bool Renderer::reset()
    {
        return impl_->reset();
    }

    void Renderer::setFlag(Renderer::Flag flag, bool enable)
    {
        impl_->setFlag(flag, enable);
    }

    lscene::Scene& Renderer::getScene()
    {
        return impl_->scene_;
    }

    void Renderer::setLightColor(f32 r, f32 g, f32 b, f32 intensity)
    {
        impl_->lightColor_.set(r, g, b, intensity);
        impl_->updateLightColor();
    }

    void Renderer::setLightDirection(const lmath::Vector4& direction)
    {
        impl_->scene_.getLightEnv().getDirectionalLight().setDirection(direction);
    }

    void Renderer::add(Object* obj)
    {
        impl_->add(obj);
    }

    void Renderer::add(NodeBase* node)
    {
        impl_->add(node);
    }

    void Renderer::draw()
    {
        impl_->draw();
    }

    void Renderer::calcZFar(f32 minz, f32 maxz)
    {
        impl_->calcZFar(minz, maxz);
    }

#if _DEBUG
    void Renderer::debugDraw(const lmath::Vector3& v0, const lmath::Vector3& v1, u32 color)
    {
        impl_->debugDraw_.add(v0, v1, color);
    }

    void Renderer::debugDraw(const lmath::Vector4& v0, const lmath::Vector4& v1, u32 color)
    {
        impl_->debugDraw_.add(v0, v1, color);
    }

    void Renderer::debugDrawSphere(const lmath::Vector3& position, f32 radius, u32 color)
    {
        lmath::Matrix44 view = impl_->scene_.getCamera().getViewMatrix();
        view.transpose33();

        lmath::Vector3 pos[8]=
        {
            lmath::Vector3(0.0f, 1.0f, 0.0f),
            lmath::Vector3(0.7071f, 0.7071f, 0.0f),
            lmath::Vector3(1.0f, 0.0f, 0.0f),
            lmath::Vector3(0.7071f, -0.7071f, 0.0f),
            lmath::Vector3(0.0f, -1.0f, 0.0f),
            lmath::Vector3(-0.7071f, -0.7071f, 0.0f),
            lmath::Vector3(-1.0f, 0.0f, 0.0f),
            lmath::Vector3(-0.7071f, 0.7071f, 0.0f),
        };

        for(s32 i=0; i<8; ++i){
            pos[i] *= radius;
            pos[i].mul33(view, pos[i]);
            pos[i] += position;
        }

        for(s32 i=0; i<7; ++i){
            debugDraw(pos[i], pos[i+1], color);
        }
        debugDraw(pos[7], pos[0], color);
    }

    void Renderer::debugDrawSphere(const lmath::Vector4& position, f32 radius, u32 color)
    {
        lmath::Matrix44 view = impl_->scene_.getCamera().getViewMatrix();
        view.transpose33();

        lmath::Vector4 pos[8]=
        {
            lmath::Vector4(0.0f, 1.0f, 0.0f, 0.0f),
            lmath::Vector4(0.7071f, 0.7071f, 0.0f, 0.0f),
            lmath::Vector4(1.0f, 0.0f, 0.0f, 0.0f),
            lmath::Vector4(0.7071f, -0.7071f, 0.0f, 0.0f),
            lmath::Vector4(0.0f, -1.0f, 0.0f, 0.0f),
            lmath::Vector4(-0.7071f, -0.7071f, 0.0f, 0.0f),
            lmath::Vector4(-1.0f, 0.0f, 0.0f, 0.0f),
            lmath::Vector4(-0.7071f, 0.7071f, 0.0f, 0.0f),
        };

        const lmath::Matrix44& vp = impl_->scene_.getCamera().getViewProjMatrix();

        for(s32 i=0; i<8; ++i){
            pos[i] *= radius;
            pos[i].mul(view, pos[i]);
            pos[i].x_ +=  position.x_;
            pos[i].y_ +=  position.y_;
            pos[i].z_ +=  position.z_;

            pos[i].mulPoint(vp, pos[i]);
        }

        for(s32 i=0; i<7; ++i){
            debugDraw(pos[i], pos[i+1], color);
        }
        debugDraw(pos[7], pos[0], color);
    }
#else
    void Renderer::debugDraw(const lmath::Vector3& v0, const lmath::Vector3& v1, u32 color)
    {
    }

    void Renderer::debugDraw(const lmath::Vector4& v0, const lmath::Vector4& v1, u32 color)
    {
    }

    void Renderer::debugDrawSphere(const lmath::Vector3& position, f32 radius, u32 color)
    {
    }

    void Renderer::debugDrawSphere(const lmath::Vector4& position, f32 radius, u32 color)
    {
    }
#endif
}
