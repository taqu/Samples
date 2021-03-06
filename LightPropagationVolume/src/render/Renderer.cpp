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
#include "CascadedShadowMap.h"
#include "LightPropagationVolume.h"

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

#include "render/Glare.h"

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

        void setFlag(Renderer::Flag flag, bool enable)
        {
            if(enable){
                flags_ |= flag;
            }else{
                flags_ &= ~flag;
            }
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

        void renderShadowMap();
        void renderLPV();

        u32 halfVirtualWidth_;
        u32 halfVirtualHeight_;

        u32 flags_;
        lgraphics::Texture2DRef renderTargetLDR_;
        lgraphics::RenderTargetViewRef renderTargetLDRView_;

        lgraphics::Texture2DRef renderTargetF16_;
        lgraphics::RenderTargetViewRef renderTargetF16View_;

        lgraphics::Texture2DRef renderTargetNormalDepth_;
        lgraphics::RenderTargetViewRef renderTargetNormalDepthView_;

        lscene::Scene scene_;

        NodeVector nodeVector_;

        Rectangle rectangle_;
        u32 backWidth_;
        u32 backHeight_;
        ID3D11RenderTargetView* renderTargetView_;
        ID3D11DepthStencilView* depthStencilView_;

        lscene::ShadowMap shadowMap_;
        CascadedShadowMap shadowMapResource_;
        lmath::Matrix44 lightViewProj_[lscene::ShadowMap::MaxCascades];

        render::DefaultPNVS* defaultVS_;
        render::DefaultPS* defaultPS_;
        render::DefaultPNUVS* defaultTextureVS_;
        render::DefaultTexturePS* defaultTexturePS_;

        LightPropagationVolume LPV_;

        lgraphics::ConstantBufferRef fxaaConstant0_;
        render::FullQuadVS* fullQuadVS_;

        render::FXAAVS* fxaaVS_;
        render::FXAAPS* fxaaPS_;
        render::TonemappingStaticPS* tonemappingPS_;

        render::Glare glare_;

        lgraphics::DepthStencilStateRef stencilWrite_;
        lgraphics::DepthStencilStateRef stencilMask_;
        lgraphics::BlendStateRef blendAO_;

        render::HDAOPS* hdaoPS_;
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
        ,defaultVS_(NULL)
        ,defaultPS_(NULL)
        ,defaultTexturePS_(NULL)
        ,fullQuadVS_(NULL)
        ,fxaaPS_(NULL)
        ,tonemappingPS_(NULL)
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
        device.setClearDepthStencil(lgraphics::GraphicsDeviceRef::ClearDepth|lgraphics::ClearFlag_Stencil, 1.0f, 0); //ステンシルもクリア


        render::FXAAPS::Constant0 fxaaConstant0;
        fxaaConstant0.rcpFrame_.set(1.0f/backWidth_, 1.0f/backHeight_, 0.0f, 0.0f);
        fxaaConstant0.rcpFrameOpt_.set(2.0f/backWidth_, 2.0f/backHeight_, 0.5f/backWidth_, 0.5f/backHeight_);
        fxaaConstant0_ = lgraphics::ConstantBuffer::create(
            sizeof(render::FXAAPS::Constant0),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);
        fxaaConstant0_.update(0, NULL, &fxaaConstant0, 0, 0);

        {//LDR用RenderTarget作成
            lgraphics::ResourceViewDesc desc;
            desc.dimension_ = lgraphics::ViewSRVDimension_Texture2D;
            desc.format_ = lgraphics::Data_R8G8B8A8_UNorm;
            desc.tex2D_.mipLevels_ = 1;
            desc.tex2D_.mostDetailedMip_ = 0;

            renderTargetLDR_ = lgraphics::Texture::create2D(
                backWidth_,
                backHeight_,
                1,
                1,
                lgraphics::Data_R8G8B8A8_UNorm,
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
            rtvDesc.format_ = lgraphics::Data_R8G8B8A8_UNorm;
            rtvDesc.tex2D_.mipSlice_ = 0;

            renderTargetLDRView_ = renderTargetLDR_.createRTView(rtvDesc);
        }

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

        {//法線用RenderTarget
            lgraphics::ResourceViewDesc desc;
            desc.dimension_ = lgraphics::ViewSRVDimension_Texture2D;
            desc.format_ = lgraphics::Data_R32G32B32A32_Float;
            desc.tex2D_.mipLevels_ = 1;
            desc.tex2D_.mostDetailedMip_ = 0;

            renderTargetNormalDepth_ = lgraphics::Texture::create2D(
                backWidth_,
                backHeight_,
                1,
                1,
                lgraphics::Data_R32G32B32A32_Float,
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
            rtvDesc.format_ = lgraphics::Data_R32G32B32A32_Float;
            rtvDesc.tex2D_.mipSlice_ = 0;

            renderTargetNormalDepthView_ = renderTargetNormalDepth_.createRTView(rtvDesc);
        }

        NodeVector tmpNode(numEntries);
        nodeVector_.swap(tmpNode);

        u32 shadowMapSize = 1024;
        shadowMap_.initialize(3, shadowMapSize, 0.1f, 100.0f);
        shadowMapResource_.reset(shadowMapSize, 3);

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

            defaultVS_ = reinterpret_cast<DefaultPNVS*>(vs);
            defaultPS_ = reinterpret_cast<DefaultPS*>(ps);

            shaderManager.get(
                render::Shader_DefaultTexture,
                vs,
                ps);

            defaultTextureVS_ = reinterpret_cast<DefaultPNUVS*>(vs);
            defaultTexturePS_ = reinterpret_cast<DefaultTexturePS*>(ps);

            shaderManager.get(
                render::Shader_FullQuad,
                vs,
                ps);
            fullQuadVS_ = reinterpret_cast<FullQuadVS*>(vs);

            shaderManager.get(
                render::Shader_FXAA,
                vs,
                ps);
            fxaaVS_ = reinterpret_cast<FXAAVS*>(vs);
            fxaaPS_ = reinterpret_cast<FXAAPS*>(ps);

            shaderManager.get(
                render::Shader_TonemappingStatic,
                vs,
                ps);
            tonemappingPS_ = reinterpret_cast<TonemappingStaticPS*>(ps);


            shaderManager.get(
                render::Shader_HDAO,
                vs,
                ps);
            hdaoPS_ = reinterpret_cast<HDAOPS*>(ps);

            render::HDAOPS::Constant0 hdaoConstant0;
            hdaoConstant0.screenWidth_ = backWidth_ - 1;
            hdaoConstant0.screenHeight_ = backHeight_ - 1;
            hdaoPS_->update0(hdaoConstant0);
        }

        {
            glare_.reset(backWidth_>>3, backHeight_>>3);
        }

        {

            lgraphics::DepthStencilStateRef::StencilOPDesc frontFace;
            lgraphics::DepthStencilStateRef::StencilOPDesc backFace;

            //
            frontFace.cmpFunc_ = lgraphics::Cmp_Always;
            frontFace.failOp_ = lgraphics::StencilOp_Keep;
            frontFace.depthFailOp_ = lgraphics::StencilOp_Keep;
            frontFace.passOp_ = lgraphics::StencilOp_IncrSat;

            backFace.cmpFunc_ = lgraphics::Cmp_Never;
            backFace.failOp_ = lgraphics::StencilOp_Keep;
            backFace.depthFailOp_ = lgraphics::StencilOp_Keep;
            backFace.passOp_ = lgraphics::StencilOp_Keep;

            stencilWrite_ = device.createDepthStencilState(
                true,
                lgraphics::DepthWrite_All,
                lgraphics::Cmp_Less,
                true,
                0xFFU,
                0xFFU,
                frontFace,
                backFace);


            //
            frontFace.cmpFunc_ = lgraphics::Cmp_NotEqual;
            frontFace.failOp_ = lgraphics::StencilOp_Keep;
            frontFace.depthFailOp_ = lgraphics::StencilOp_Keep;
            frontFace.passOp_ = lgraphics::StencilOp_Keep;

            backFace.cmpFunc_ = lgraphics::Cmp_Never;
            backFace.failOp_ = lgraphics::StencilOp_Keep;
            backFace.depthFailOp_ = lgraphics::StencilOp_Keep;
            backFace.passOp_ = lgraphics::StencilOp_Keep;

            stencilMask_ = device.createDepthStencilState(
                false,
                lgraphics::DepthWrite_Zero,
                lgraphics::Cmp_Less,
                true,
                0xFFU,
                0xFFU,
                frontFace,
                backFace);


            blendAO_ = lgraphics::BlendState::create(
                FALSE,
                TRUE,
                lgraphics::Blend_InvSrcAlpha,
                lgraphics::Blend_SrcAlpha,
                lgraphics::BlendOp_Add,
                lgraphics::Blend_DestAlpha,
                lgraphics::Blend_DestAlpha,
                lgraphics::BlendOp_Max,
                lgraphics::ColorWrite_All);
        }

        u32 reflectiveMapSize = 128;
        u32 volumeResolution = 32;

        if(!LPV_.initialize(
            reflectiveMapSize,
            volumeResolution,
            128.0f,
            1.0f))
        {
            return false;
        }

#if _DEBUG
        debugDraw_.initialize(maxDebugLines);
#endif
        return true;
    }

    void Renderer::Impl::terminate()
    {
        LPV_.release();

        glare_.destroy();

#if _DEBUG
        debugDraw_.terminate();
#endif
        shadowMapResource_.release();

        fxaaConstant0_.destroy();

        SAFE_RELEASE(renderTargetView_);
        SAFE_RELEASE(depthStencilView_);

        renderTargetNormalDepthView_.destroy();
        renderTargetNormalDepth_.destroy();

        renderTargetF16View_.destroy();
        renderTargetF16_.destroy();

        renderTargetLDRView_.destroy();
        renderTargetLDR_.destroy();
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

        //if(fractal::Input::getKeyboard().isClick(fractal::Input::Key_Tab)){
        //    LPV_.toggleOcclusion();
        //}

        renderShadowMap();
        renderLPV();

        lgraphics::GraphicsDeviceRef& device = lgraphics::Graphics::getDevice();

        //HDR用RenderTargetセット
        ID3D11RenderTargetView* views[2];
        if(getFlag(Renderer::Flag_HDR)){
            views[0] = renderTargetF16View_.getView();
            device.clearRenderTargetView(renderTargetF16View_.getView(), device.getClearColor());
        }else{
            views[0] = renderTargetLDRView_.getView();
            device.clearRenderTargetView(renderTargetLDRView_.getView(), device.getClearColor());
        }

        f32 zero[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        views[1] = renderTargetNormalDepthView_.getView(); //法線と深度保存
        device.clearRenderTargetView(renderTargetNormalDepthView_.getView(), zero);

        device.setRenderTargets(2, views, depthStencilView_);

        device.clearDepthStencilView();

        //device.setDepthStencilState(lgraphics::GraphicsDeviceRef::DepthStencil_DEnableWEnable);
        device.setDepthStencilState(stencilWrite_, 0); //ステンシル書き込み
        device.setBlendState(lgraphics::GraphicsDeviceRef::BlendState_NoAlpha);
        device.setRasterizerState(lgraphics::GraphicsDeviceRef::Rasterizer_FillSolid);

        LPVRenderVS* renderVS = LPV_.getRenderVS();
        LPVRenderPS* renderPS = LPV_.getRenderPS();

        ID3D11ShaderResourceView* const resViews[] =
        {
            *LPV_.getTexLPVR().getView(),
            *LPV_.getTexLPVG().getView(),
            *LPV_.getTexLPVB().getView(),
            *shadowMapResource_.getTex().getView(),
        };

        ID3D11SamplerState* const samplers[] =
        {
            *LPV_.getTexLPVR().getSampler(),
            *shadowMapResource_.getTex().getSampler(),
        };

        //defaultVS_->attach();
        //defaultPS_->attach();

        const lscene::DirectionalLight& dlight = scene_.getLightEnv().getDirectionalLight();

        renderVS->attach();
        renderPS->attach();

        device.setPSResources(0, 4, resViews);
        device.setPSSamplers(0, 2, samplers);

        LPVRenderVS::Constant0 vsConstant0;
        LPVRenderVS::Constant1 vsConstant1;
        LPVRenderPS::Constant0 psConstant0;
        LPVRenderPS::Constant1 psConstant1;

        vsConstant1.LPVMin_ = LPV_.getLPVMin();
        vsConstant1.invLPVSize_ = LPV_.getInvLPVSize();
        vsConstant1.invTexSize_ = LPV_.getInvVolumeResolution();
        vsConstant1.LPVAttenuation_ = LPV_.getLPVAttenuation();
        renderVS->update1(vsConstant1);

        psConstant0.dlColor_ = dlight.getColor();
        psConstant0.dlDir_ = dlight.getDirection();
        scene_.getCamera().getEyePosition(psConstant0.cameraPos_);

        for(s32 i=0; i<shadowMap_.getCascadeLevels(); ++i){
            psConstant0.cascadeDepth_[i] = shadowMap_.getCascadePartition(i);
        }

        renderPS->update0(psConstant0);

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

            //ライト視点行列セット
            for(s32 k=0; k<shadowMap_.getCascadeLevels(); ++k){
                vsConstant0.mlwvp_[k].mul(lightViewProj_[k], node->world_);
            }
            renderVS->update0(vsConstant0);

            for(u32 j=0; j<node->numMeshes_; ++j){
                Mesh& mesh = node->meshes_[j];

                Material& material = *(mesh.getMaterial());

                device.setPrimitiveTopology(mesh.getType());

                psConstant1.diffuse_ = material.diffuse_;
                psConstant1.specular_ = material.specular_;
                psConstant1.shadow_ = material.shadow_;

                renderPS->update1(psConstant1);

                Geometry& geometry = *(mesh.getGeometry());

                geometry.attach();

                device.drawIndexed(mesh.getNumIndices(), mesh.getIndexOffset(), 0);
                ++numBatches;
            }
        }

        device.setDepthStencilState(lgraphics::GraphicsDeviceRef::DepthStencil_DEnableWEnable);
        nodeVector_.clear();

        //
        //------------------------------------------------------------------------------
        device.setRasterizerState(lgraphics::GraphicsDeviceRef::Rasterizer_FillSolid);
        device.setDepthStencilState(lgraphics::GraphicsDeviceRef::DepthStencil_DDisableWDisable);
        device.setPrimitiveTopology(lgraphics::Primitive_TriangleList);

        //Tone mapping
        if(getFlag(Renderer::Flag_HDR)){

            glare_.run(device, renderTargetF16_);


            device.setViewport(0, 0, backWidth_, backHeight_);
            views[0] = renderTargetLDRView_.getView();
            device.setRenderTargets(1, views, NULL);

            fullQuadVS_->attach();
            tonemappingPS_->attach();
            renderTargetF16_.attachPS(0,0);
            glare_.getResult().attachPS(1,1);

            device.draw(3, 0);

        }

        device.setDepthStencilState(stencilMask_, 0);
        device.setBlendState(blendAO_);
        //device.setBlendState(lgraphics::GraphicsDeviceRef::BlendState_NoAlpha);
        views[0] = renderTargetLDRView_.getView();
        device.setRenderTargets(1, views, depthStencilView_);
        fullQuadVS_->attach();
        hdaoPS_->attach();
        hdaoPS_->set();
        renderTargetNormalDepth_.attachPS(0, 0);
        device.draw(3, 0);

        //RenderTarget元に戻す
        device.setDepthStencilState(lgraphics::GraphicsDeviceRef::DepthStencil_DDisableWDisable);
        device.setBlendState(lgraphics::GraphicsDeviceRef::BlendState_NoAlpha);
        device.restoreDefaultRenderTargets();

        fxaaConstant0_.setVS(0);
        fxaaConstant0_.setPS(0);

        fxaaVS_->attach();
        fxaaPS_->attach();

        renderTargetLDR_.attachPS(0,0);
        device.draw(3, 0);

#if _DEBUG
        //debugDraw_.draw(device);
#endif
        //rectangle_.draw(-1.0f, 0.0f, 0.0f, 1.0f, oceanDz_);
        device.clearVSResources(1);
        device.clearPSResources(4);

        const Char* hdrStr = getFlag(Renderer::Flag_HDR)? "ON" : "OFF";
        fractal::System::getFontManager().format(0, 0, 0, 0.6f, "sec %f", deltaTime);
        fractal::System::getFontManager().format(0, 0, 16, 0.6f, "HDR[%s]", hdrStr);
    }

    void Renderer::Impl::renderShadowMap()
    {
        lgraphics::GraphicsDeviceRef& device = lgraphics::Graphics::getDevice();

        device.setRasterizerState(lgraphics::GraphicsDeviceRef::Rasterizer_DepthMap);
        shadowMap_.update(scene_);

        const lmath::Matrix44& lightView = scene_.getLightEnv().getDirectionalLight().getLightView();
        for(s32 i=0; i<shadowMap_.getCascadeLevels(); ++i){
            lightViewProj_[i].mul(shadowMap_.getLightProjection(i), lightView);
        }

        // ShadowMap
        //---------------------------
        shadowMapResource_.begin(device);

        DepthVS* vs = shadowMapResource_.getVS();

        lmath::Matrix44 mat[4];

        for(NodeVector::iterator itr = nodeVector_.begin();
            itr != nodeVector_.end();
            ++itr)
        {
            NodeBase* node = (*itr);

            //行列セット
            for(s32 k=0; k<shadowMap_.getCascadeLevels(); ++k){
                mat[k].mul(lightViewProj_[k], node->world_);
            }
            vs->set(mat);

            for(u32 j=0; j<node->numMeshes_; ++j){
                Mesh& mesh = node->meshes_[j];

                device.setPrimitiveTopology(mesh.getType());

                Geometry& geometry = *(mesh.getGeometry());

                geometry.attach();
                device.drawIndexedInstanced(mesh.getNumIndices(), shadowMap_.getCascadeLevels(), mesh.getIndexOffset(), 0, 0);
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

    void Renderer::Impl::renderLPV()
    {
        lgraphics::GraphicsDeviceRef& device = lgraphics::Graphics::getDevice();

        const lscene::DirectionalLight& directional = scene_.getLightEnv().getDirectionalLight();

        //scene_.calcLightViewProjection();

        // ReflectiveShadowMap
        //---------------------------
        LPV_.beginReflectiveShadowMap(device, scene_);
        ReflectiveShadowMap& reflectiveShadowMap = LPV_.getReflectiveShadowMap();


        const lmath::Matrix44& lightViewProj = LPV_.getLightViewProjection();

        device.setRasterizerState(lgraphics::GraphicsDeviceRef::Rasterizer_FillSolid);
        device.setBlendState(lgraphics::GraphicsDeviceRef::BlendState_NoAlpha);
        device.setPrimitiveTopology(lgraphics::Primitive_TriangleList);

        ReflectiveShadowMapVS* vs = reflectiveShadowMap.getVS();
        //ReflectiveShadowMapPS* ps = reflectiveShadowMap.getPS();

        ReflectiveShadowMapVS::Constant0 rsmConstants0;
        ReflectiveShadowMapVS::Constant1 rsmConstants1;
        rsmConstants0.dlDir_ = directional.getDirection();
        rsmConstants0.dlColor_ = directional.getColor();

        vs->update0(rsmConstants0);

        for(NodeVector::iterator itr = nodeVector_.begin();
            itr != nodeVector_.end();
            ++itr)
        {
            NodeBase* node = (*itr);

            rsmConstants1.mwvp_.mul(lightViewProj, node->world_);
            rsmConstants1.mw_ = node->world_;

            for(u32 j=0; j<node->numMeshes_; ++j){
                Mesh& mesh = node->meshes_[j];

                device.setPrimitiveTopology(mesh.getType());

                rsmConstants1.diffuse_ = mesh.getMaterial()->diffuse_;
                vs->update1(rsmConstants1);

                Geometry& geometry = *(mesh.getGeometry());

                geometry.attach();

                device.drawIndexed(mesh.getNumIndices(), mesh.getIndexOffset(), 0);
            }
        }

        LPV_.beginLPVInjection(
            device,
            scene_);

        LPV_.beginLPVPropagate(device);

        LPV_.end(device);

        device.setViewport(0, 0, backWidth_, backHeight_);
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
