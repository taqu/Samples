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
        compiler.compileShaderPS(ps, Shader_HDAO);
        LASSERT(ps.valid());
        entries_[Shader_HDAO].ps_ = LIME_NEW HDAOPS(ps);

        //
        compiler.compileShader(vs, ps, Shader_ReflectiveShadowMap);
        LASSERT(vs.valid() && ps.valid());
        entries_[Shader_ReflectiveShadowMap].vs_ = LIME_NEW ReflectiveShadowMapVS(vs);
        entries_[Shader_ReflectiveShadowMap].ps_ = LIME_NEW ReflectiveShadowMapPS(ps);

        //
        compiler.compileShader(vs, ps, Shader_LPVInjection);
        LASSERT(vs.valid() && ps.valid());
        entries_[Shader_LPVInjection].vs_ = LIME_NEW LPVInjectionVS(vs);
        entries_[Shader_LPVInjection].ps_ = LIME_NEW LPVInjectionPS(ps);

        //
        compiler.compileShader(vs, ps, Shader_LPVOcclusionInjection);
        LASSERT(vs.valid() && ps.valid());
        entries_[Shader_LPVOcclusionInjection].vs_ = LIME_NEW LPVOcclusionInjectionVS(vs);
        entries_[Shader_LPVOcclusionInjection].ps_ = LIME_NEW LPVOcclusionInjectionPS(ps);

        //
        compiler.compileShader(vs, ps, Shader_LPVPropagate);
        LASSERT(vs.valid() && ps.valid());
        entries_[Shader_LPVPropagate].vs_ = LIME_NEW LPVPropagateVS(vs);
        entries_[Shader_LPVPropagate].ps_ = LIME_NEW LPVPropagatePS(ps);

        //
        compiler.compileShaderPS(ps, Shader_LPVOcclusionPropagate);
        LASSERT(ps.valid());
        entries_[Shader_LPVOcclusionPropagate].ps_ = LIME_NEW LPVOcclusionPropagatePS(ps);

        //
        compiler.compileShaderPS(ps, Shader_LPVAccumulate);
        LASSERT(ps.valid());
        entries_[Shader_LPVAccumulate].ps_ = LIME_NEW LPVAccumulatePS(ps);


        //
        compiler.compileShader(vs, ps, Shader_LPVRender);
        LASSERT(vs.valid() && ps.valid());
        entries_[Shader_LPVRender].vs_ = LIME_NEW LPVRenderVS(vs);
        entries_[Shader_LPVRender].ps_ = LIME_NEW LPVRenderPS(ps);

        //
        compiler.compileShaderGS(gs, ShaderGS_LPVInjection);
        geometryShaders_[ShaderGS_LPVInjection] = LIME_NEW LPVInjectionGS(gs);

        compiler.compileShaderGS(gs, ShaderGS_LPVPropagate);
        geometryShaders_[ShaderGS_LPVPropagate] = LIME_NEW LPVPropagateGS(gs);


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
