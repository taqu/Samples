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
#include <lgraphics/io11/IODDS.h>
#include "CascadedShadowMap.h"

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

#include "render/Glare.h"
#include "Voxel.h"

#include "Noise.h"

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
        void drawVoxel();

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

        CascadedShadowMap shadowMap_;

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

        lgraphics::Texture2DRef envMap_;
        lgraphics::Texture2DRef reflectMap_;
        scene::SkyBox skyBox_;

        lgraphics::DepthStencilStateRef stencilWrite_;
        lgraphics::DepthStencilStateRef stencilMask_;
        lgraphics::BlendStateRef blendAO_;

        render::HDAOPS* hdaoPS_;

        lgraphics::InputLayoutRef voxelInputLayout_;
        lgraphics::VertexBufferRef voxelVertexBuffer_;
        lgraphics::Texture3DRef voxelTexture_;
        render::VoxelVS* voxelVS_;
        render::VoxelGS* voxelGS_;
        render::VoxelPS* voxelPS_;

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
        shadowMap_.reset(shadowMapSize, 2);

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
            lcore::ifstream in("../data/texture/sky.dds", lcore::ios::binary);
            if(in.is_open()){
                u32 size = in.getSize(0);
                const s8* buffer = LIME_NEW s8[size];
                in.read((Char*)buffer, size);
                lgraphics::io::IODDS::read(envMap_, buffer, size, lgraphics::Usage_Immutable, lgraphics::TexFilter_MinMagMipLinear, lgraphics::TexAddress_Clamp);
                LIME_DELETE_ARRAY(buffer);

                in.close();
            }

            if(in.open("../data/texture/reflect.dds", lcore::ios::binary)){
                u32 size = in.getSize(0);
                const s8* buffer = LIME_NEW s8[size];
                in.read((Char*)buffer, size);
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

        {
            voxelInputLayout_ = fractal::System::getInputLayoutFactory().get(render::Layout_Voxel);

            voxelVertexBuffer_ = lgraphics::VertexBuffer::create(
                sizeof(render::Voxel)*128,
                lgraphics::Usage_Dynamic,
                lgraphics::CPUAccessFlag_Write,
                lgraphics::ResourceMisc_None);

            render::ShaderManager& shaderManager = render::ShaderManager::getInstance();
            render::ShaderVSBase* vs;
            render::ShaderPSBase* ps;
            render::ShaderGSBase* gs;

            shaderManager.get(
                render::Shader_Voxel,
                vs,
                ps);

            voxelVS_ = reinterpret_cast<VoxelVS*>(vs);
            voxelPS_ = reinterpret_cast<VoxelPS*>(ps);

            shaderManager.get(
                render::ShaderGS_Voxel,
                gs);
            voxelGS_ = reinterpret_cast<VoxelGS*>(gs);


            fractal::Perlin noise;
            //noise.initialize(lcore::getDefaultSeed());
            noise.initialize(0x8FC309F7U);
            fractal::FractalParam fractalParam;
            fractalParam.octaves_ = 4;
            fractalParam.frequency_ = 1.7f;

            fractal::FractalFuc3 fractalFunc3(&fractalParam, &noise, fractal::turbulence3);

            s32 size = 16;

            f32* data = LIME_NEW f32[size*size*size];
            for(s32 i=0; i<size; ++i){
                for(s32 j=0; j<size; ++j){
                    for(s32 k=0; k<size; ++k){
                        f32 z = static_cast<f32>(i)/size;
                        f32 y = static_cast<f32>(j)/size;
                        f32 x = static_cast<f32>(k)/size;

                        f32 dz = 2.0f*z - 1.0f;
                        f32 dy = 2.0f*y - 1.0f;
                        f32 dx = 2.0f*x - 1.0f;

                        f32 length = lmath::sqrt(dx*dx + dy*dy + dz*dz);

                        f32 l = lcore::clamp01( length );
                        l = lmath::smoothstep(l, 0.8f, 0.95f);
                        l = 1.0f - l*l*l;

                        if(lmath::isZero(length)){
                            data[i*size*size + j*size + k] = 0.0f;//fractalFunc3(0.0f, 0.0f, 0.0f);
                            continue;
                        }

                        lmath::Vector3 dir(dx, dy, dz);
                        dir /= length;

                        f32 cs;
                        f32 sn;
                        f32 d = lmath::sqrt(dir.x_*dir.x_ + dir.z_*dir.z_);

                        sn = dir.y_;
                        if(lmath::isZero(d)){
                            cs = 0.0f;
                        }else{
                            cs = dir.x_/d;
                        }

                        cs = 0.5f * cs + 0.5f;
                        sn = 0.5f * sn + 0.5f;

                        f32 v = fractalFunc3(cs, sn, length);
                        v = lmath::smoothstep(v, 0.5f, 0.8f);
                        data[i*size*size + j*size + k] = v * l;

                        if(i==0 || (size-1)==i
                            || j==0 || (size-1)==j
                            || k==0 || (size-1)==k)
                        {
                            data[i*size*size + j*size + k] = 0.0f;
                        }
                    }
                }
            }
            lgraphics::SubResourceData initData;
            initData.mem_ = data;
            initData.pitch_ = size * sizeof(f32);
            initData.slicePitch_ = size * size * sizeof(f32);

            lgraphics::ResourceViewDesc resDesc;
            resDesc.dimension_ = lgraphics::ViewSRVDimension_Texture3D;
            resDesc.format_ = lgraphics::Data_R32_Float;
            resDesc.tex3D_.mipLevels_ = 1;
            resDesc.tex3D_.mostDetailedMip_ = 0;

            voxelTexture_ = lgraphics::Texture::create3D(
                size,
                size,
                size,
                1,
                lgraphics::Data_R32_Float,
                lgraphics::Usage_Default,
                lgraphics::BindFlag_ShaderResource,
                lgraphics::CPUAccessFlag_None,
                lgraphics::ResourceMisc_None,
                lgraphics::TexFilter_MinMagLinearMipPoint,
                lgraphics::TexAddress_Clamp,
                lgraphics::Cmp_Never,
                0.0f,
                &initData,
                &resDesc);

            LIME_DELETE_ARRAY(data);
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
        shadowMap_.release();

        reflectMap_.destroy();
        envMap_.destroy();

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

        renderShadowMap();

        lgraphics::GraphicsDeviceRef& device = lgraphics::Graphics::getDevice();

        //HDR用RenderTargetセット
        ID3D11RenderTargetView* views[2];
        if(getFlag(Renderer::Flag_HDR)){
            views[0] = renderTargetF16View_.getView();
            //device.clearRenderTargetView(renderTargetF16View_.getView(), device.getClearColor());
        }else{
            views[0] = renderTargetLDRView_.getView();
            //device.clearRenderTargetView(renderTargetLDRView_.getView(), device.getClearColor());
        }

        f32 zero[4] = {0.0f, 0.0f, 0.0f, 1.0f};
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
        lmath::Matrix44 shadowMapProjection[4];
        for(s32 i=0; i<scene_.getNumCascades(); ++i){
            scene_.getShadowMapProjection(shadowMapProjection[i], i);
        }

        DefaultPNVS::Constant vsConstant0;
        DefaultPS::Constant0 psConstant0;
        DefaultPS::Constant1 psConstant1;

        vsConstant0.mv_ = scene_.getCamera().getViewMatrix();
        psConstant0.dlColor_ = dlight.getColor();
        psConstant0.dlDir_ = dlight.getDirection();

        scene_.getCamera().getEyePosition(psConstant0.cameraPos_);

        scene_.getCascadeSplitDepth(psConstant0.cascadeDepth_);

        defaultPS_->update0(psConstant0);

        shadowMap_.getTex().attachPS(0, 0);

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
            for(s32 k=0; k<scene_.getNumCascades(); ++k){
                vsConstant0.mlwvp_[k].mul(shadowMapProjection[k], node->world_);
            }
            defaultVS_->update(vsConstant0);

            for(u32 j=0; j<node->numMeshes_; ++j){
                Mesh& mesh = node->meshes_[j];

                Material& material = *(mesh.getMaterial());

                device.setPrimitiveTopology(mesh.getType());

                psConstant1.diffuse_ = material.diffuse_;
                psConstant1.specular_ = material.specular_;
                psConstant1.shadow_ = material.shadow_;
                psConstant1.shadow_.w_ = lcore::calcFresnelTerm(material.shadow_.w_);

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

#if 1
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
#endif

        //ボクセル用にRenderTarget再セット
        device.setRenderTargets(1, views, depthStencilView_);

        drawVoxel();

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
        scene_.calcLightViewProjection();

        // ShadowMap
        //---------------------------
        shadowMap_.begin(device);

        DepthVS* vs = shadowMap_.getVS();

        lmath::Matrix44 mat[4];

        for(NodeVector::iterator itr = nodeVector_.begin();
            itr != nodeVector_.end();
            ++itr)
        {
            NodeBase* node = (*itr);

            for(u32 j=0; j<node->numMeshes_; ++j){
                Mesh& mesh = node->meshes_[j];

                device.setPrimitiveTopology(mesh.getType());

                //行列セット
                for(s32 k=0; k<scene_.getNumCascades(); ++k){
                    mat[k].mul(scene_.getLightViewProjection(k), node->world_);
                }
                vs->set(mat);

                Geometry& geometry = *(mesh.getGeometry());

                geometry.attach();
                device.drawIndexedInstanced(mesh.getNumIndices(), scene_.getNumCascades(), mesh.getIndexOffset(), 0, 0);
            }
        }

        shadowMap_.end(device, scene_.getNumCascades());
        device.setViewport(0, 0, backWidth_, backHeight_);
    }

    void Renderer::Impl::drawVoxel()
    {
        render::Voxel voxel;

        lmath::Quaternion orientation;
        orientation.identity();

        static f32 count = 0.0f;
        count += 1.0f;
        if(360.0f<count){
            count -= 360.0f;
        }

        orientation.setRotateZ(PI*count/180.0f);

        voxel.setHalfSize(lmath::Vector3(12.0f, 12.0f, 12.0f));
        voxel.setOrientation(orientation);
        voxel.setPosition(lmath::Vector3(0.0f, 5.0f, 0.0f));
        voxel.setColor(lmath::Vector4(0.7f, 0.7f, 0.0f, 1.0f));

        lgraphics::MappedSubresource mapped;
        if(voxelVertexBuffer_.map(0, lgraphics::MapType_WriteDiscard, mapped)){
            lcore::memcpy(mapped.data_, &voxel, sizeof(Voxel));
            voxelVertexBuffer_.unmap(0);
        }

        lgraphics::GraphicsDeviceRef& device = lgraphics::Graphics::getDevice();

        device.setRasterizerState(lgraphics::GraphicsDeviceRef::Rasterizer_FillSolid);
        device.setDepthStencilState(lgraphics::GraphicsDeviceRef::DepthStencil_DEnableWDisable);
        device.setBlendState(lgraphics::GraphicsDeviceRef::BlendState_Alpha);

        device.setPrimitiveTopology(lgraphics::Primitive_PointList);

        voxelInputLayout_.attach();
        voxelVS_->attach();
        voxelGS_->attach();
        voxelPS_->attach();

        voxelTexture_.attachPS(0);
        renderTargetNormalDepth_.attachPS(1, 1);

        VoxelGS::Constant0 constantGS0;
        constantGS0.viewProj_ = scene_.getCamera().getViewProjMatrix();
        scene_.getCamera().getEyePosition(constantGS0.cameraPos_);
        voxelGS_->set(constantGS0);

        VoxelPS::Constant0 constantPS0;
        constantPS0.dlDir_ = scene_.getLightEnv().getDirectionalLight().getDirection();
        constantPS0.dlColor_ = scene_.getLightEnv().getDirectionalLight().getColor();
        constantPS0.invDepthExtent_.x_ = 1.0f/(scene_.getCamera().getZFar()-scene_.getCamera().getZNear());
        voxelPS_->set(constantPS0);

        voxelVertexBuffer_.attach(0, sizeof(render::Voxel), 0);
        device.draw(1, 0);

        device.setVertexShader(NULL);
        device.setGeometryShader(NULL);
        device.setPixelShader(NULL);

        device.clearPSResources(2);
        device.setBlendState(lgraphics::GraphicsDeviceRef::BlendState_NoAlpha);
    }

    void Renderer::Impl::calcZFar(f32 minz, f32 maxz)
    {
        f32 zfar = lscene::Frustum::calcFitZFar(
            lmath::Vector4(-64.0f, -64.0f, -12.0f, 0.0f),
            lmath::Vector4(64.0f, 64.0f, 200.0f, 0.0f),
            scene_.getCamera(),
            minz,
            maxz);

        scene_.setShadowMapZNear(scene_.getCamera().getZNear());
        scene_.setShadowMapZFar(zfar);
        scene_.calcCascadeSplitDepth();


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
