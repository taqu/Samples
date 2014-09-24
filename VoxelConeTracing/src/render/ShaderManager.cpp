/**
@file ShaderManager.cpp
@author t-sakai
@date 2011/10/17 create
*/
#include "ShaderManager.h"
#include "Shader.h"
#include <lgraphics/api/ShaderRef.h>

namespace render
{
    ShaderManager* ShaderManager::instance_ = NULL;

    ShaderManager::ShaderManager()
    {
        for(u32 i=0; i<Shader_Num; ++i){
            entries_[i].vs_ = NULL;
            entries_[i].ps_ = NULL;
        }

        for(u32 i=0; i<ShaderGS_Num; ++i){
            geometryShaders_[i] = NULL;
        }
    }

    ShaderManager::~ShaderManager()
    {
    }

    void ShaderManager::initialize()
    {
        if(NULL == instance_){
            instance_ = LIME_NEW ShaderManager;
            instance_->initializeImpl();
        }
    }

    void ShaderManager::terminate()
    {
        if(NULL != instance_){
            instance_->terminateImpl();
            LIME_DELETE(instance_);
        }
    }


    void ShaderManager::initializeImpl()
    {
        lgraphics::VertexShaderRef vs;
        lgraphics::PixelShaderRef ps;
        lgraphics::GeometryShaderRef gs;

        Compiler compiler;

        //
        compiler.compileShader(vs, ps, Shader_Default);
        LASSERT(vs.valid() && ps.valid());
        entries_[Shader_Default].vs_ = LIME_NEW DefaultPNVS(vs);
        entries_[Shader_Default].ps_ = LIME_NEW DefaultPS(ps);

        compiler.compileShader(vs, ps, Shader_DefaultTexture);
        LASSERT(vs.valid() && ps.valid());
        entries_[Shader_DefaultTexture].vs_ = LIME_NEW DefaultPNUVS(vs);
        entries_[Shader_DefaultTexture].ps_ = LIME_NEW DefaultTexturePS(ps);

        //
        compiler.compileShader(vs, ps, Shader_Depth);
        LASSERT(vs.valid() && ps.valid());
        entries_[Shader_Depth].vs_ = LIME_NEW DepthVS(vs);
        entries_[Shader_Depth].ps_ = LIME_NEW DepthPS(ps);

        compiler.compileShaderGS(gs, ShaderGS_Depth);
        geometryShaders_[ShaderGS_Depth] = LIME_NEW DepthGS(gs);

        //
        compiler.compileShader(vs, ps, Shader_Rectangle);
        LASSERT(vs.valid() && ps.valid());
        entries_[Shader_Rectangle].vs_ = LIME_NEW RectangleVS(vs);
        entries_[Shader_Rectangle].ps_ = LIME_NEW RectanglePS(ps);

        //
        compiler.compileShader(vs, ps, Shader_DebugVC);
        LASSERT(vs.valid() && ps.valid());
        entries_[Shader_DebugVC].vs_ = LIME_NEW DebugVCVS(vs);
        entries_[Shader_DebugVC].ps_ = LIME_NEW DebugVCPS(ps);

        //
        compiler.compileShaderVS(vs, Shader_FullQuad);
        LASSERT(vs.valid());
        entries_[Shader_FullQuad].vs_ = LIME_NEW FullQuadVS(vs);

        //
        compiler.compileShader(vs, ps, Shader_FXAA);
        LASSERT(vs.valid() && ps.valid());
        entries_[Shader_FXAA].vs_ = LIME_NEW FXAAVS(vs);
        entries_[Shader_FXAA].ps_ = LIME_NEW FXAAPS(ps);

        //
        compiler.compileShaderPS(ps, Shader_TonemappingStatic);
        LASSERT(ps.valid());
        entries_[Shader_TonemappingStatic].ps_ = LIME_NEW TonemappingStaticPS(ps);

        //
        compiler.compileShaderPS(ps, Shader_Glare);
        LASSERT(ps.valid());
        entries_[Shader_Glare].ps_ = LIME_NEW GlarePS(ps);

        //
        compiler.compileShaderPS(ps, Shader_GaussianFilter);
        LASSERT(ps.valid());
        entries_[Shader_GaussianFilter].ps_ = LIME_NEW GaussPS(ps);

        //
        compiler.compileShader(vs, ps, Shader_SkyBox);
        LASSERT(vs.valid() && ps.valid());
        entries_[Shader_SkyBox].vs_ = LIME_NEW SkyBoxVS(vs);
        entries_[Shader_SkyBox].ps_ = LIME_NEW SkyBoxPS(ps);

        //
        compiler.compileShader(vs, ps, Shader_Debug);
        LASSERT(vs.valid() && ps.valid());
        entries_[Shader_Debug].vs_ = LIME_NEW DebugVS(vs);
        entries_[Shader_Debug].ps_ = LIME_NEW DebugPS(ps);


        //
        compiler.compileShader(vs, ps, Shader_Voxelize);
        LASSERT(vs.valid() && ps.valid());
        entries_[Shader_Voxelize].vs_ = LIME_NEW VoxelizeVS(vs);
        entries_[Shader_Voxelize].ps_ = LIME_NEW VoxelizePS(ps);

        compiler.compileShaderGS(gs, ShaderGS_Voxelize);
        geometryShaders_[ShaderGS_Voxelize] = LIME_NEW VoxelizeGS(gs);

        //
        compiler.compileShader(vs, ps, Shader_VoxelRender);
        LASSERT(vs.valid() && ps.valid());
        entries_[Shader_VoxelRender].vs_ = LIME_NEW VoxelRenderVS(vs);
        entries_[Shader_VoxelRender].ps_ = LIME_NEW VoxelRenderPS(ps);

        compiler.compileShaderGS(gs, ShaderGS_VoxelRender);
        geometryShaders_[ShaderGS_VoxelRender] = LIME_NEW VoxelRenderGS(gs);

        //
        compiler.compileShader(vs, ps, Shader_ReflectiveShadowMap);
        LASSERT(vs.valid() && ps.valid());
        entries_[Shader_ReflectiveShadowMap].vs_ = LIME_NEW ReflectiveShadowMapVS(vs);
        entries_[Shader_ReflectiveShadowMap].ps_ = LIME_NEW ReflectiveShadowMapPS(ps);

        //
        compiler.compileShader(vs, ps, Shader_ReflectiveShadowMapTexture);
        LASSERT(vs.valid() && ps.valid());
        entries_[Shader_ReflectiveShadowMapTexture].vs_ = LIME_NEW ReflectiveShadowMapTextureVS(vs);
        entries_[Shader_ReflectiveShadowMapTexture].ps_ = LIME_NEW ReflectiveShadowMapTexturePS(ps);

        //
        compiler.compileShader(vs, ps, Shader_VoxelInjection);
        LASSERT(vs.valid() && ps.valid());
        entries_[Shader_VoxelInjection].vs_ = LIME_NEW VoxelInjectionVS(vs);
        entries_[Shader_VoxelInjection].ps_ = LIME_NEW VoxelInjectionPS(ps);

        //
        compiler.compileShader(vs, ps, Shader_VoxelMipmap);
        LASSERT(vs.valid() && ps.valid());
        entries_[Shader_VoxelMipmap].vs_ = LIME_NEW VoxelMipmapVS(vs);
        entries_[Shader_VoxelMipmap].ps_ = LIME_NEW VoxelMipmapPS(ps);


        for(u32 i=0; i<Shader_Num; ++i){
            if(NULL != entries_[i].vs_){
                entries_[i].vs_->initialize();
            }

            if(NULL != entries_[i].ps_){
                entries_[i].ps_->initialize();
            }
        }

        for(u32 i=0; i<ShaderGS_Num; ++i){
            if(NULL != geometryShaders_[i]){
                geometryShaders_[i]->initialize();
            }
        }

    }

    void ShaderManager::terminateImpl()
    {
        for(u32 i=0; i<Shader_Num; ++i){
            LIME_DELETE(entries_[i].ps_);
            LIME_DELETE(entries_[i].vs_);
        }

        for(u32 i=0; i<ShaderGS_Num; ++i){
            LIME_DELETE(geometryShaders_[i]);
        }
    }

    void ShaderManager::get(Shader id, ShaderVSBase*& vs, ShaderPSBase*& ps)
    {
        LASSERT(0<=id && id<Shader_Num);
        vs = entries_[id].vs_;
        ps = entries_[id].ps_;
    }

    void ShaderManager::get(ShaderGS id, ShaderGSBase*& gs)
    {
        LASSERT(0<=id && id<ShaderGS_Num);
        gs = geometryShaders_[id];
    }
}
