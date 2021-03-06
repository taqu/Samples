/**
@file Renderer.cpp
@author t-sakai
@date 2011/12/31 create
*/
#include "stdafx.h"
#include "Renderer.h"

#include <lcore/Vector.h>
#include <lcore/liostream.h>
#include <lmath/DualQuaternion.h>

#include <lgraphics/api/BlendStateRef.h>
#include <lgraphics/api/TextureRef.h>
#include <lgraphics/api/SamplerStateRef.h>

#include <lframework/scene/Frustum.h>
#include <lframework/scene/Scene.h>
#include <lframework/scene/ShadowMap.h>

#include <lgraphics/io11/IODDS.h>
#include "render/CascadedShadowMap.h"

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
#include "scene/SkyBox.h"

#include "SAO.h"

#include "render/Debug.h"
#include "vct/Voxel.h"

namespace render
{
    class Renderer::Impl
    {
    public:
        static const u32 ShadowMapSize = 1024;
        static const s32 ShadowMapCascades = 3;

        static const s32 VoxelResolution = 128;
        static const s32 VoxelMipmaps = 6;
        static const s32 ReflectiveShadowMapSize = 384;

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

        void renderDebug();

        void voxelize();

        u32 halfVirtualWidth_;
        u32 halfVirtualHeight_;

        u32 flags_;
        lgraphics::Texture2DRef renderTargetLDR_;
        lgraphics::RenderTargetViewRef renderTargetLDRView_;

        lgraphics::Texture2DRef renderTargetF16_;
        lgraphics::RenderTargetViewRef renderTargetF16View_;

        lgraphics::Texture2DRef renderTargetNormal_;
        lgraphics::RenderTargetViewRef renderTargetNormalView_;

        lscene::Scene scene_;

        NodeVector nodeVector_;

        Rectangle rectangle_;
        u32 backWidth_;
        u32 backHeight_;
        ID3D11RenderTargetView* renderTargetView_;
        ID3D11DepthStencilView* depthStencilView_;

        f32 shadowBias_;
        render::CascadedShadowMap shadowMapResource_;
        lscene::ShadowMap shadowMap_;
        lmath::Matrix44 lightViewProj_[lscene::ShadowMap::MaxCascades];

        render::DefaultPNVS* defaultVS_;
        render::DefaultPS* defaultPS_;
        render::DefaultPNUVS* defaultTextureVS_;
        render::DefaultTexturePS* defaultTexturePS_;

        lgraphics::ConstantBufferRef fxaaConstant0_;
        render::FullQuadVS* fullQuadVS_;

        render::FXAAVS* fxaaVS_;
        render::FXAAPS* fxaaPS_;
        render::TonemappingStaticPS* tonemappingPS_;

        render::Glare glare_;
        render::SAO sao_;

        lgraphics::Texture2DRef envMap_;
        lgraphics::Texture2DRef reflectMap_;
        scene::SkyBox skyBox_;

        vct::Voxel voxel_;

#if _DEBUG
        DebugDraw debugDraw_;
#endif

        lgraphics::Texture2DRef debugTexture0_;
        lgraphics::RenderTargetViewRef debugRenderTarget0_;
        Debug debug_;
    };

    Renderer::Impl::Impl()
        :halfVirtualWidth_(1)
        ,halfVirtualHeight_(1)
        ,flags_(0)
        ,backWidth_(0)
        ,backHeight_(0)
        ,shadowBias_(-0.01f)
        ,renderTargetView_(NULL)
        ,depthStencilView_(NULL)
        ,defaultVS_(NULL)
        ,defaultPS_(NULL)
        ,defaultTextureVS_(NULL)
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
        const lscene::Camera& camera = scene_.getCamera();

        const f32 color[] = {0.3f, 0.3f, 0.3f, 1.0f};
        //const f32 color[] = {0.0f, 0.0f, 0.0f, 1.0f};
        device.setClearColor(color);
        device.getRenderTargetDesc(backWidth_, backHeight_);
        device.getRenderTargets(1, &renderTargetView_, &depthStencilView_);


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
            desc.format_ = lgraphics::Data_R8G8B8A8_SNorm;
            desc.tex2D_.mipLevels_ = 1;
            desc.tex2D_.mostDetailedMip_ = 0;

            renderTargetNormal_ = lgraphics::Texture::create2D(
                backWidth_,
                backHeight_,
                1,
                1,
                lgraphics::Data_R8G8B8A8_SNorm,
                lgraphics::Usage_Default,
                lgraphics::BindFlag_ShaderResource|lgraphics::BindFlag_RenderTarget,
                lgraphics::CPUAccessFlag_None,
                lgraphics::ResourceMisc_None,
                lgraphics::TexFilter_MinMagLinearMipPoint,
                lgraphics::TexAddress_Clamp,
                lgraphics::Cmp_Never,
                0.0f,
                NULL,
                &desc);

            lgraphics::RTVDesc rtvDesc;
            rtvDesc.dimension_ = lgraphics::ViewRTVDimension_Texture2D;
            rtvDesc.format_ = lgraphics::Data_R8G8B8A8_SNorm;
            rtvDesc.tex2D_.mipSlice_ = 0;

            renderTargetNormalView_ = renderTargetNormal_.createRTView(rtvDesc);
        }

        NodeVector tmpNode(numEntries);
        nodeVector_.swap(tmpNode);

        shadowMapResource_.reset(ShadowMapSize, ShadowMapCascades);
        shadowMap_.initialize(ShadowMapCascades, ShadowMapSize, 0.1f, 500.0f, 0.6f);

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

        }

        glare_.reset(backWidth_>>3, backHeight_>>3);
        sao_.reset(backWidth_, backHeight_, scene_.getCamera().getProjMatrix(), camera.getZNear(), camera.getZFar());

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

#if _DEBUG
        debugDraw_.initialize(maxDebugLines);
#endif

        {
            lgraphics::SRVDesc desc;
            desc.dimension_ = lgraphics::ViewSRVDimension_Texture2D;
            desc.format_ = lgraphics::Data_B8G8R8A8_UNorm;
            desc.tex2D_.mipLevels_ = 1;
            desc.tex2D_.mostDetailedMip_ = 0;

            debugTexture0_ = lgraphics::Texture::create2D(
                256,
                256,
                1,
                1,
                lgraphics::Data_B8G8R8A8_UNorm,
                lgraphics::Usage_Default,
                lgraphics::BindFlag_ShaderResource | lgraphics::BindFlag_RenderTarget,
                lgraphics::CPUAccessFlag_None,
                lgraphics::ResourceMisc_None,
                lgraphics::TexFilter_MinMagLinearMipPoint,
                lgraphics::TexAddress_Clamp,
                lgraphics::Cmp_Less,
                0.0f, NULL, &desc);

            lgraphics::RTVDesc rtDesc;
            rtDesc.dimension_ = lgraphics::ViewRTVDimension_Texture2D;
            rtDesc.format_ = lgraphics::Data_B8G8R8A8_UNorm;
            rtDesc.tex2D_.mipSlice_ = 0;

            debugRenderTarget0_ = debugTexture0_.createRTView(rtDesc);

            debug_.create(128, 128);

        }

        {
            voxel_.create(VoxelResolution, VoxelMipmaps);
            voxel_.setView(
                lmath::Vector4(0.0f, 140.0f, 0.0f, 0.0f),
                lmath::Vector4(0.0f, 0.0f, 1.0f, 0.0f),
                lmath::Vector4(300.0f, 300.0f, 300.0f, 0.0f),
                ReflectiveShadowMapSize);
        }
        return true;
    }

    void Renderer::Impl::terminate()
    {
        voxel_.destroy();

        debug_.destroy();
        debugRenderTarget0_.destroy();
        debugTexture0_.destroy();

        sao_.destroy();
        glare_.destroy();

#if _DEBUG
        debugDraw_.terminate();
#endif
        shadowMapResource_.release();

        reflectMap_.destroy();
        envMap_.destroy();

        fxaaConstant0_.destroy();

        SAFE_RELEASE(renderTargetView_);
        SAFE_RELEASE(depthStencilView_);

        renderTargetNormalView_.destroy();
        renderTargetNormal_.destroy();

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

        renderShadowMap();
        voxelize();

        lgraphics::GraphicsDeviceRef& device = lgraphics::Graphics::getDevice();

        //HDR用RenderTargetセット
        ID3D11RenderTargetView* views[1];
        if(getFlag(Renderer::Flag_HDR)){
            views[0] = renderTargetF16View_.getView();
            device.clearRenderTargetView(renderTargetF16View_.getView(), device.getClearColor());
        }else{
            views[0] = renderTargetLDRView_.getView();
            device.clearRenderTargetView(renderTargetLDRView_.getView(), device.getClearColor());
        }
        device.setRenderTargets(1, views, depthStencilView_);

        device.clearDepthStencilView();

        device.setDepthStencilState(lgraphics::GraphicsDeviceRef::DepthStencil_DEnableWEnable);

        device.setBlendState(lgraphics::GraphicsDeviceRef::BlendState_NoAlpha);
        device.setRasterizerState(lgraphics::GraphicsDeviceRef::Rasterizer_FillSolid);

        const lscene::DirectionalLight& dlight = scene_.getLightEnv().getDirectionalLight();

        defaultVS_->attach();

        DefaultPNVS::Constant vsConstant0;
        DefaultPS::Constant0 psConstant0;
        DefaultPS::Constant1 psConstant1;

        psConstant0.dlColor_ = dlight.getColor();
        psConstant0.dlDir_ = dlight.getDirection();
        psConstant0.dlDir_.w_ = shadowBias_;

        scene_.getCamera().getEyePosition(psConstant0.cameraPos_);
        defaultPS_->update0(psConstant0);

        shadowMapResource_.getTex().attachPS(0, 0);

        //
        //---------------------------------------
        {
            DefaultPS::Constant2 voxelParam;
            voxel_.setVoxelTextureToPS(device, 2, 2);
            voxel_.getVoxelParam(
                voxelParam.voxelDiameter_,
                voxelParam.invVoxelDiameter_,
                voxelParam.worldToVoxelSpace_,
                voxelParam.voxelScale_);
            defaultPS_->update2(voxelParam);
        }

        //
        //---------------------------------------
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
            defaultVS_->update(vsConstant0);

            for(u32 j=0; j<node->numMeshes_; ++j){
                Mesh& mesh = node->meshes_[j];

                device.setPrimitiveTopology(mesh.getType());

                Material& material = *(mesh.getMaterial());

                psConstant1.diffuse_ = material.diffuse_;
                psConstant1.specular_ = material.specular_;
                psConstant1.shadow_ = material.shadow_;

                defaultPS_->update1(psConstant1);

                ShaderVSBase* selectedVS;
                ShaderPSBase* selectedPS;
                if(NULL == material.textures_[0]){
                    selectedVS = defaultVS_;
                    selectedPS = defaultPS_;
                }else{
                    selectedVS = defaultTextureVS_;
                    selectedPS = defaultTexturePS_;
                    material.textures_[0]->attachPS(1, 1);
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

        nodeVector_.clear();
        device.clearPSResources(8);
        device.setRenderTargets(1, views, depthStencilView_);

        //------------------------------------------------------------------------------
        if(getFlag(Renderer::Flag_DrawVoxel)){
            voxel_.draw(device, scene_);
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

        //SAO
        if(getFlag(Renderer::Flag_SAO)){
            sao_.run(device, scene_.getCamera().getZNear(), scene_.getCamera().getZFar());
        }else{
            sao_.clearTarget(device);
        }

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
            sao_.get().attachPS(2);

            device.draw(3, 0);

        }

        //RenderTarget元に戻す
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
        //renderDebug();
        //rectangle_.draw(0.75f, 1.0f, 0.75f, 1.0f, debugTexture0_);
        rectangle_.draw(0.75f, 1.0f, 0.75f, 1.0f, voxel_.getReflectiveShadowMap());
        rectangle_.draw(0.5f, 0.75f, 0.75f, 1.0f, sao_.get());

        device.clearVSResources(1);
        device.clearPSResources(4);

        {//debug text
            font::FontManager& fontManager = fractal::System::getFontManager();
            const Char* ON = "*";
            const Char* OFF = " ";
            const Char* hdrStr = getFlag(Renderer::Flag_HDR)? ON : OFF;
            const Char* saoStr = getFlag(Renderer::Flag_SAO)? ON : OFF;
            const Char* voxelStr = getFlag(Renderer::Flag_DrawVoxel)? ON : OFF;
            const Char* giStr = getFlag(Renderer::Flag_GI)? ON : OFF;

            fontManager.format(0, 0, 0, 0.6f, "%dx%d sec %f", backWidth_, backHeight_, deltaTime);
            fontManager.format(0, 0, 16, 0.6f, "ShadowMap:%dx%d", ShadowMapSize, ShadowMapCascades);
            fontManager.format(0, 0, 32, 0.6f, "Voxel:%dx%d RSM:%d", VoxelResolution, VoxelMipmaps, ReflectiveShadowMapSize);
            fontManager.format(0, 0, 48, 0.6f, "H:HDR[%s] O:SAO[%s] V:Voxel[%s] G:GI[%s]", hdrStr, saoStr, voxelStr, giStr);
        }
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

        vs->set();

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
            vs->update(mat);


            for(u32 j=0; j<node->numMeshes_; ++j){
                Mesh& mesh = node->meshes_[j];

                device.setPrimitiveTopology(mesh.getType());

                Geometry& geometry = *(mesh.getGeometry());

                geometry.attach();

                vs->attach();
                device.drawIndexedInstanced(mesh.getNumIndices(), shadowMap_.getCascadeLevels(), mesh.getIndexOffset(), 0, 0);
            }
        }

        shadowMapResource_.end(device, shadowMap_.getCascadeLevels());
        device.setViewport(0, 0, backWidth_, backHeight_);
    }

    void Renderer::Impl::calcZFar(f32 minz, f32 maxz)
    {
        lmath::Vector4 aabbMin(-100.0f, -10.0f, -100.0f, 0.0f);
        lmath::Vector4 aabbMax(100.0f, 190.0f, 100.0f, 0.0f);

        shadowMap_.setSceneAABB(aabbMin, aabbMax);
    }

    void Renderer::Impl::renderDebug()
    {
        lgraphics::GraphicsDeviceRef& device = lgraphics::Graphics::getDevice();

        device.setRenderTargets(1, debugRenderTarget0_.get(), NULL);
        device.clearRenderTargetView(debugRenderTarget0_.getView(), device.getClearColor());
        device.setViewport(0, 0, 256, 256);

        device.setRasterizerState(lgraphics::GraphicsDeviceRef::Rasterizer_FillSolid);
        device.setDepthStencilState(lgraphics::GraphicsDeviceRef::DepthStencil_DDisableWDisable);

        lmath::Vector4 eye;
        lmath::Vector4 at;
        scene_.getCamera().getEyePosition(eye);
        at = eye;
        eye.y_ += 100.0f;

        lmath::Matrix44 view;
        view.lookAt(eye, at, lmath::Vector4(0.0f, 0.0f, 1.0f, 0.0));
        lmath::Matrix44 proj;
        proj.ortho(100.0f, 100.0f, 0.1f, 1000.0f);

        lmath::Matrix44 cameraView = scene_.getViewMatrix();
        cameraView.invert();

        lmath::Matrix44 mwvp;
        mwvp.mul(proj, view);
        mwvp.mul(mwvp, cameraView);
        debug_.drawFrustum(mwvp, scene_.getProjMatrix(), 0.01f, 100.0f, 0xFFFFFFFFU);
        device.restoreDefaultRenderTargets();
    }

    void Renderer::Impl::voxelize()
    {
        lgraphics::GraphicsDeviceRef& device = lgraphics::Graphics::getDevice();

        if(!getFlag(Renderer::Flag_GI)){
            //OFFの場合はボクセルのクリアだけ
            voxel_.clearVoxels(device);
            return;
        }

        // Reflective ShadowMap
        //----------------------------
        {
            voxel_.beginReflective(device, scene_);

            render::ReflectiveShadowMapVS* reflectiveVS = voxel_.getReflectiveShadowMapVS();
            render::ReflectiveShadowMapPS* reflectivePS = voxel_.getReflectiveShadowMapPS();
            render::ReflectiveShadowMapTextureVS* reflectiveTextureVS = voxel_.getReflectiveSHadowMapTextureVS();
            render::ReflectiveShadowMapTexturePS* reflectiveTexturePS = voxel_.getReflectiveSHadowMapTexturePS();

            render::ReflectiveShadowMapVS::Constant0 vsConstant0;
            render::ReflectiveShadowMapPS::Constant0 psConstant0;
            render::ReflectiveShadowMapPS::Constant1 psConstant1;

            const lscene::DirectionalLight& dlight = scene_.getLightEnv().getDirectionalLight();
            psConstant0.dlColor_ = dlight.getColor();
            psConstant0.dlDir_ = dlight.getDirection();
            reflectivePS->update0(psConstant0);

            lmath::Matrix44 lightViewProj;
            lightViewProj.mul(voxel_.getLightProjection(), dlight.getLightView());

            ShaderVSBase* vs = NULL;
            ShaderPSBase* ps = NULL;
            for(NodeVector::iterator itr = nodeVector_.begin();
                itr != nodeVector_.end();
                ++itr)
            {
                NodeBase* node = (*itr);
                vsConstant0.mwvp_.mul(lightViewProj, node->world_);
                vsConstant0.mw_ = node->world_;

                reflectiveVS->update0(vsConstant0);

                for(u32 j=0; j<node->numMeshes_; ++j){
                    Mesh& mesh = node->meshes_[j];

                    device.setPrimitiveTopology(mesh.getType());

                    Material& material = *(mesh.getMaterial());

                    psConstant1.diffuse_ = material.diffuse_;

                    reflectivePS->update1(psConstant1);

                    ShaderVSBase* selectedVS;
                    ShaderPSBase* selectedPS;
                    if(NULL == material.textures_[0]){
                        selectedVS = reflectiveVS;
                        selectedPS = reflectivePS;
                    }else{
                        selectedVS = reflectiveTextureVS;
                        selectedPS = reflectiveTexturePS;
                        material.textures_[0]->attachPS(0,0);
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
                }
            }

            voxel_.endReflective(device);
        }
        
        //
        //----------------------------
        {
            voxel_.begin(device);

            render::VoxelizeVS* vs = voxel_.getVoxelizeVS();

            lmath::Matrix44 viewProj;
            voxel_.getViewProjection(viewProj);

            lmath::Matrix44 mwvp;
            for(NodeVector::iterator itr = nodeVector_.begin();
                itr != nodeVector_.end();
                ++itr)
            {
                NodeBase* node = (*itr);
                mwvp.mul(viewProj, node->world_);
                vs->set(mwvp);

                for(u32 j=0; j<node->numMeshes_; ++j){
                    Mesh& mesh = node->meshes_[j];

                    device.setPrimitiveTopology(mesh.getType());

                    Geometry& geometry = *(mesh.getGeometry());

                    geometry.attach();

                    vs->attach();
                    device.drawIndexed(mesh.getNumIndices(), mesh.getIndexOffset(), 0);
                }
            }
            voxel_.end(device);
        }
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

    bool Renderer::getFlag(Renderer::Flag flag) const
    {
        return impl_->getFlag(flag);
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
