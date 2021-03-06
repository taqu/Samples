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

#include "scene/SkyBox.h"

#include "scene/Ocean.h"
#include "scene/QuadTreeMesh.h"
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
        ShadowMap shadowMapResource_;
        lmath::Matrix44 lightViewProj_;

        render::DefaultVS* defaultVS_;
        render::DefaultPS* defaultPS_;
        render::DefaultTexturePS* defaultTexturePS_;

        lgraphics::ConstantBufferRef fxaaConstant0_;
        render::FullQuadVS* fullQuadVS_;

        render::FXAAVS* fxaaVS_;
        render::FXAAPS* fxaaPS_;
        render::TonemappingStaticPS* tonemappingPS_;

        render::Glare glare_;

        lgraphics::Texture2DRef oceanDz_;
        lgraphics::Texture2DRef noise2D_;
        scene::Ocean* ocean_;

        render::OceanVS* oceanVS_;
        render::OceanPS* oceanPS_;
        render::OceanWirePS* oceanWirePS_;

        f32 oceanTime_;
        scene::QuadTreeMesh* quadTreeMesh_;

        render::OceanPS::Constant1 oceanPSConstant1_;
        lgraphics::ConstantBufferRef oceanPSConstant1Buffer_;

        lgraphics::Texture2DRef envMap_;
        lgraphics::Texture2DRef reflectMap_;
        scene::SkyBox skyBox_;

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
        ,oceanVS_(NULL)
        ,oceanPS_(NULL)
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
            desc.format_ = lgraphics::Data_R8G8B8A8_UNorm;
            desc.dimension_ = lgraphics::ViewSRVDimension_Texture2D;
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
            desc.format_ = lgraphics::Data_R16G16B16A16_Float;
            desc.dimension_ = lgraphics::ViewSRVDimension_Texture2D;
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
            desc.format_ = lgraphics::Data_R32G32B32A32_Float;
            desc.dimension_ = lgraphics::ViewSRVDimension_Texture2D;
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

        u32 shadowMapSize = 512;

        shadowMap_.initialize(1, shadowMapSize, 0.01f, 1000.0f);
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

            defaultVS_ = reinterpret_cast<DefaultVS*>(vs);
            defaultPS_ = reinterpret_cast<DefaultPS*>(ps);

            shaderManager.get(
                render::Shader_DefaultTexture,
                vs,
                ps);

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
                render::Shader_Ocean,
                vs,
                ps);

            oceanVS_ = reinterpret_cast<OceanVS*>(vs);
            oceanPS_ = reinterpret_cast<OceanPS*>(ps);

            shaderManager.get(
                render::Shader_OceanWire,
                vs,
                ps);
            oceanWirePS_ = reinterpret_cast<OceanWirePS*>(ps);


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
            lgraphics::ResourceViewDesc desc;
            desc.format_ = lgraphics::Data_R16G16B16A16_Float;
            desc.dimension_ = lgraphics::ViewSRVDimension_Texture2D;
            desc.tex2D_.mipLevels_ = 1;
            desc.tex2D_.mostDetailedMip_ = 0;

            oceanDz_ = lgraphics::Texture::create2D(
                scene::Ocean::Resolution,
                scene::Ocean::Resolution,
                1,
                1,
                lgraphics::Data_R16G16B16A16_Float,
                lgraphics::Usage_Dynamic,
                lgraphics::BindFlag_ShaderResource,
                lgraphics::CPUAccessFlag_Write,
                lgraphics::ResourceMisc_None,
                lgraphics::TexFilter_MinMagLinearMipPoint,
                lgraphics::TexAddress_Wrap,
                lgraphics::Cmp_Never,
                0.0f,
                NULL,
                &desc);

            scene::Ocean::Parameter param;
            ocean_ = LIME_NEW scene::Ocean();
            ocean_->initialize(param);
            oceanTime_ = 0.0f;

            quadTreeMesh_ = LIME_NEW scene::QuadTreeMesh();
            quadTreeMesh_->create(scene::Ocean::Resolution, param.size_);
            quadTreeMesh_->updateViewport(backWidth_*backHeight_, 256.0f, scene_.getCamera());

            oceanPSConstant1Buffer_ = lgraphics::ConstantBuffer::create(
                sizeof(render::OceanPS::Constant1),
                lgraphics::Usage_Default,
                lgraphics::CPUAccessFlag_None,
                lgraphics::ResourceMisc_None);

            //oceanPSConstant1_.noiseAmplitude_.set(33.0f, 41.0f, 61.0f, ocean_->getParameter().noiseSize_);
            static const f32 scaleAmplitude = 1.0f;
            static const f32 scaleOctave = 4.0f;
            oceanPSConstant1_.noiseAmplitude_.set(scaleAmplitude*0.017f/3.0f, scaleAmplitude*0.031f/3.0f, scaleAmplitude*0.057f/3.0f, ocean_->getParameter().noiseSize_);
            oceanPSConstant1_.noiseOctave_.set(scaleOctave*4.11f, scaleOctave*2.59f, scaleOctave*1.23f, 0.0f);
            oceanPSConstant1_.bendParam_.set(0.1f, -0.4f, 0.2f, 0.0f);

            oceanPSConstant1_.diffuse_ = ocean_->getDiffuse();
            oceanPSConstant1_.specular_ = ocean_->getSpecular();
            oceanPSConstant1_.transparent_ = ocean_->getTransparent();
            oceanPSConstant1_.transparent_.w_ = lcore::calcFresnelTerm(ocean_->getTransparent().w_);

            //ノイズテクスチャ作成
            desc.format_ = lgraphics::Data_R16G16_Float;
            desc.tex2D_.mipLevels_ = 1;
            desc.tex2D_.mostDetailedMip_ = 0;

            scene::Ocean::Bump* noise = LIME_NEW scene::Ocean::Bump[scene::Ocean::Resolution*scene::Ocean::Resolution];
            scene::Ocean::generateBumpNoise2D(noise, scene::Ocean::Resolution);
            lgraphics::SubResourceData initData;
            initData.mem_ = noise;
            initData.pitch_ = scene::Ocean::Resolution * sizeof(f32);
            initData.slicePitch_ = 0;


            noise2D_ = lgraphics::Texture::create2D(
                scene::Ocean::Resolution,
                scene::Ocean::Resolution,
                1,
                1,
                lgraphics::Data_R16G16_Float,
                lgraphics::Usage_Immutable,
                lgraphics::BindFlag_ShaderResource,
                lgraphics::CPUAccessFlag_None,
                lgraphics::ResourceMisc_None,
                lgraphics::TexFilter_MinMagLinearMipPoint,
                lgraphics::TexAddress_Wrap,
                lgraphics::Cmp_Never,
                0.0f,
                &initData,
                &desc);

            LIME_DELETE_ARRAY(noise);
        }


        {
            lcore::ifstream in("../data/texture/sky.dds", lcore::ios::binary);
            if(in.is_open()){
                u32 size = in.getSize(0);
                u8* buffer = LIME_NEW u8[size];
                lcore::io::read(in, buffer, size);
                lgraphics::io::IODDS::read(envMap_, buffer, size, lgraphics::Usage_Immutable, lgraphics::TexFilter_MinMagMipLinear, lgraphics::TexAddress_Clamp);
                LIME_DELETE_ARRAY(buffer);

                in.close();
            }

            if(in.open("../data/texture/reflect.dds", lcore::ios::binary)){
                u32 size = in.getSize(0);
                u8* buffer = LIME_NEW u8[size];
                lcore::io::read(in, buffer, size);
                lgraphics::io::IODDS::read(reflectMap_, buffer, size, lgraphics::Usage_Immutable, lgraphics::TexFilter_MinMagMipLinear, lgraphics::TexAddress_Clamp);
                LIME_DELETE_ARRAY(buffer);
            }

            skyBox_.initialize();
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

#if _DEBUG
        debugDraw_.initialize(maxDebugLines);
#endif
        return true;
    }

    void Renderer::Impl::terminate()
    {
        glare_.destroy();

#if _DEBUG
        debugDraw_.terminate();
#endif
        shadowMapResource_.release();

        reflectMap_.destroy();
        envMap_.destroy();

        oceanPSConstant1Buffer_.destroy();

        oceanDz_.destroy();
        noise2D_.destroy();

        LIME_DELETE(quadTreeMesh_);
        LIME_DELETE(ocean_);

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

        {
            oceanTime_ += deltaTime;
            if(60.0f*10.0f<oceanTime_){
                oceanTime_ -= 60.0f*10.0f;
            }
            ocean_->update(oceanTime_);

            void* buffer = NULL;
            u32 rowPitch, depthPitch;
            if(oceanDz_.map(buffer, rowPitch, depthPitch, 0, lgraphics::MapType_WriteDiscard)){
                const scene::Ocean::Displacement* v = ocean_->getDisplacement();
                u8* c = reinterpret_cast<u8*>(buffer);
                lcore::memcpy(c, v, sizeof(scene::Ocean::Displacement)*scene::Ocean::Resolution*scene::Ocean::Resolution);
                oceanDz_.unmap(0);
            }
            quadTreeMesh_->calcVisibleNode(scene_.getCamera());
        }

        renderShadowMap();

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

        defaultVS_->attach();
        //defaultPS_->attach();

        const lscene::DirectionalLight& dlight = scene_.getLightEnv().getDirectionalLight();

        DefaultVS::Constant vsConstant0;
        DefaultPS::Constant0 psConstant0;
        DefaultPS::Constant1 psConstant1;

        vsConstant0.mv_ = scene_.getCamera().getViewMatrix();
        psConstant0.dlColor_ = dlight.getColor();
        psConstant0.dlDir_ = dlight.getDirection();

        scene_.getCamera().getEyePosition(psConstant0.cameraPos_);
        defaultPS_->update0(psConstant0);

        ShaderPSBase* ps = NULL;
        device.setPrimitiveTopology(lgraphics::Primitive_TriangleList);
        shadowMapResource_.getTex().attachPS(0, 0);

        u32 numBatches = 0;
        for(NodeVector::iterator itr = nodeVector_.begin();
            itr != nodeVector_.end();
            ++itr)
        {
            NodeBase* node = (*itr);

            vsConstant0.mwvp_.mul(scene_.getViewProjMatrix(), node->world_);
            vsConstant0.mw_ = node->world_;
            vsConstant0.mlwvp_.mul(lightViewProj_, node->world_);
            defaultVS_->update(vsConstant0);

            for(u32 j=0; j<node->numMeshes_; ++j){
                Mesh& mesh = node->meshes_[j];

                Material& material = *(mesh.getMaterial());

                psConstant1.diffuse_ = material.diffuse_;
                psConstant1.specular_ = material.specular_;
                psConstant1.transparent_ = material.shadow_;

                defaultPS_->update1(psConstant1);


                ShaderPSBase* selectedPS;
                if(NULL == material.textures_[0]){
                    selectedPS = defaultPS_;
                }else{
                    //selectedPS = defaultAOPS_;
                    selectedPS = defaultTexturePS_;
                    material.textures_[0]->attachPS(1, 1);
                }
                if(selectedPS != ps){
                    ps = selectedPS;
                    ps->attach();
                }

                Geometry& geometry = *(mesh.getGeometry());

                geometry.attach();

                device.drawIndexed(mesh.getNumIndices(), mesh.getIndexOffset(), 0);
                ++numBatches;
            }
        }

        device.setDepthStencilState(lgraphics::GraphicsDeviceRef::DepthStencil_DEnableWEnable);
        nodeVector_.clear();

        {//Ocean
            //device.setRasterizerState(lgraphics::GraphicsDeviceRef::Rasterizer_FillWireFrameNoCull);
            device.setRasterizerState(lgraphics::GraphicsDeviceRef::Rasterizer_FillSolidNoCull);
            quadTreeMesh_->attach();

            OceanVS::Constant0 vsConstant0;

            //vsConstant0.mwvp_ = scene_.getViewProjMatrix();
            //vsConstant0.mw_.identity();
            vsConstant0.mlwvp_ = lightViewProj_;

            oceanPSConstant1Buffer_.setPS(1);

            f32 invPatchSize = 1.0f/ocean_->getParameter().size_;

            OceanVS::Constant1 oceanConstant1;
            oceanConstant1.info_.x_ = ocean_->getParameter().choppyScale_;
            oceanConstant1.info_.y_ = ocean_->getParameter().gridLength_;
            oceanConstant1.info_.z_ = 2.0f*ocean_->getParameter().amplitude_;
            oceanConstant1.uvInfo_.x_ = invPatchSize;
            oceanConstant1.uvInfo_.y_ = 0.5f/scene::Ocean::Resolution;

            oceanVS_->update1(oceanConstant1);

            oceanVS_->set();
            oceanVS_->attach();

            vsConstant0.mlocal_.identity();
            vsConstant0.mw_.identity();

            oceanDz_.attachVS(0, 0);
            noise2D_.attachPS(0, 0);
            reflectMap_.attachPS(1, 1);

            //ノイズ
            f32 noiseScale = invPatchSize * ocean_->getParameter().noiseSize_;
            oceanPSConstant1_.noiseUV_.z_ = ocean_->getParameter().noiseSpeed_ * oceanTime_ * ocean_->getParameter().windDirectionX_;
            oceanPSConstant1_.noiseUV_.w_ = ocean_->getParameter().noiseSpeed_ * oceanTime_ * ocean_->getParameter().windDirectionY_;

            const scene::QuadTreeMesh::NodeVector& nodes = quadTreeMesh_->getVisibleNodes();
            for(u32 i=0; i<nodes.size(); ++i){
                const scene::QuadTreeMesh::QuadNode& node = nodes[i];
                if(!node.isLeaf()){
                    continue;
                }

                const scene::QuadTreeMesh::Quad& quad = quadTreeMesh_->selectMeshPattern(node);


                s32 reso = quadTreeMesh_->getResolution();
                reso >>= node.lod_;

                f32 s = node.length_/reso;

                vsConstant0.mlocal_.setScale(s, 0.0f, s);
                vsConstant0.mw_.setTranslate(node.bottomLeft_.x_, 0.0f, node.bottomLeft_.y_);
                vsConstant0.mwvp_.mul(scene_.getViewProjMatrix(), vsConstant0.mw_);

                oceanVS_->update0(vsConstant0);

                oceanPSConstant1_.noiseUV_.x_ = node.bottomLeft_.x_ * noiseScale;
                oceanPSConstant1_.noiseUV_.y_ = node.bottomLeft_.y_ * noiseScale;

                oceanPSConstant1Buffer_.update(0, NULL, &oceanPSConstant1_, 0, 0);

                device.setRasterizerState(lgraphics::GraphicsDeviceRef::Rasterizer_FillSolidNoCull);
                oceanPS_->attach();
                if(0<quad.numInnerFaces_){
                    device.setPrimitiveTopology(lgraphics::Primitive_TriangleStrip);
                    device.drawIndexed(quad.numInnerFaces_+2, quad.innerStartIndex_, 0);
                }

                if(0<quad.numBoundaryFaces_){
                    device.setPrimitiveTopology(lgraphics::Primitive_TriangleList);
                    device.drawIndexed(quad.numBoundaryFaces_*3, quad.boundaryStartIndex_, 0);
                }
#if 0
                device.setRasterizerState(lgraphics::GraphicsDeviceRef::Rasterizer_FillWireFrameNoCull);
                oceanWirePS_->attach();
                if(0<quad.numInnerFaces_){
                    device.setPrimitiveTopology(lgraphics::Primitive_TriangleStrip);
                    device.drawIndexed(quad.numInnerFaces_+2, quad.innerStartIndex_, 0);
                }

                if(0<quad.numBoundaryFaces_){
                    device.setPrimitiveTopology(lgraphics::Primitive_TriangleList);
                    device.drawIndexed(quad.numBoundaryFaces_*3, quad.boundaryStartIndex_, 0);
                }
#endif
            }
        }

        {//SkyBox
            device.setRasterizerState(lgraphics::GraphicsDeviceRef::Rasterizer_FillSolidNoCull);
            device.setDepthStencilState(lgraphics::GraphicsDeviceRef::DepthStencil_DEnableWDisable);

            device.setPrimitiveTopology(lgraphics::Primitive_TriangleStrip);
            device.setInputLayout(NULL);
            device.setVertexBuffers(0, 0, NULL, NULL, NULL);

            skyBox_.update(scene_.getCamera(), backWidth_, backHeight_);
            envMap_.attachPS(0, 0);
            device.draw(4, 0);
        }

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
        fractal::System::getFontManager().format(0, 0, 1, 0.6f, "HDR[%s]", hdrStr);
    }

    void Renderer::Impl::renderShadowMap()
    {
        lgraphics::GraphicsDeviceRef& device = lgraphics::Graphics::getDevice();

        device.setRasterizerState(lgraphics::GraphicsDeviceRef::Rasterizer_DepthMap);
        shadowMap_.update(scene_);

        // ShadowMap
        //---------------------------
        shadowMapResource_.begin(device);

        device.setPrimitiveTopology(lgraphics::Primitive_TriangleList);

        const lmath::Matrix44& lightView = scene_.getLightEnv().getDirectionalLight().getLightView();
        lightViewProj_.mul(shadowMap_.getLightProjection(0), lightView);

        DepthVS* vs = shadowMapResource_.getVS();

        lmath::Matrix44 mat;
        for(NodeVector::iterator itr = nodeVector_.begin();
            itr != nodeVector_.end();
            ++itr)
        {
            NodeBase* node = (*itr);
            mat.mul(lightViewProj_, node->world_);
            vs->set(mat);

            for(u32 j=0; j<node->numMeshes_; ++j){
                Mesh& mesh = node->meshes_[j];

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
