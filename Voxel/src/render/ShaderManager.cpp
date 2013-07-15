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
        entries_[Shader_Default].vs_ = LIME_NEW DefaultVS(vs);
        entries_[Shader_Default].ps_ = LIME_NEW DefaultPS(ps);

        compiler.compileShaderPS(ps, Shader_DefaultTexture);
        LASSERT(ps.valid());
        entries_[Shader_DefaultTexture].ps_ = LIME_NEW DefaultPS(ps);


        //
        compiler.compileShader(vs, ps, Shader_Depth);
        LASSERT(vs.valid() && ps.valid());
        entries_[Shader_Depth].vs_ = LIME_NEW DepthVS(vs);
        entries_[Shader_Depth].ps_ = LIME_NEW DepthPS(ps);

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
        compiler.compileShaderPS(ps, Shader_TonemappingStatic);
        LASSERT(ps.valid());
        entries_[Shader_TonemappingStatic].ps_ = LIME_NEW TonemappingStaticPS(ps);

        //
        compiler.compileShader(vs, ps, Shader_Voxelize);
        LASSERT(vs.valid() && ps.valid());
        entries_[Shader_Voxelize].vs_ = LIME_NEW VoxelizeVS(vs);
        entries_[Shader_Voxelize].ps_ = LIME_NEW VoxelizePS(ps);

        compiler.compileShaderPS(ps, Shader_VoxelizeTexture);
        LASSERT(ps.valid());
        entries_[Shader_VoxelizeTexture].ps_ = LIME_NEW VoxelizePS(ps);

        compiler.compileShaderGS(gs, Shader_VoxelizeGS);
        geometryShaders_[Shader_VoxelizeGS] = LIME_NEW VoxelizeGS(gs);

        //
        compiler.compileShader(vs, ps, Shader_VoxelRender);
        LASSERT(vs.valid() && ps.valid());
        entries_[Shader_VoxelRender].vs_ = LIME_NEW VoxelRenderVS(vs);
        entries_[Shader_VoxelRender].ps_ = LIME_NEW VoxelRenderPS(ps);

        //
        compiler.compileShader(vs, ps, Shader_OctreeTag);
        LASSERT(vs.valid() && ps.valid());
        entries_[Shader_OctreeTag].vs_ = LIME_NEW OctreeTagVS(vs);
        entries_[Shader_OctreeTag].ps_ = LIME_NEW OctreeTagPS(ps);

        //
        compiler.compileShaderPS(ps, Shader_OctreeCreateNode);
        LASSERT(ps.valid());
        entries_[Shader_OctreeCreateNode].ps_ = LIME_NEW OctreeCreateNodePS(ps);

        //
        compiler.compileShaderPS(ps, Shader_OctreeInitNode);
        LASSERT(ps.valid());
        entries_[Shader_OctreeInitNode].ps_ = LIME_NEW OctreeInitNodePS(ps);

        //
        compiler.compileShaderPS(ps, Shader_OctreeWriteValue);
        LASSERT(ps.valid());
        entries_[Shader_OctreeWriteValue].ps_ = LIME_NEW OctreeWriteValuePS(ps);

        //
        compiler.compileShaderPS(ps, Shader_OctreePropagate);
        LASSERT(ps.valid());
        entries_[Shader_OctreePropagate].ps_ = LIME_NEW OctreePropagatePS(ps);

        //
        compiler.compileShaderPS(ps, Shader_OctreeMipmap);
        LASSERT(ps.valid());
        entries_[Shader_OctreeMipmap].ps_ = LIME_NEW OctreeMipmapPS(ps);


        //
        compiler.compileShaderPS(ps, Shader_DefaultAO);
        LASSERT(ps.valid());
        entries_[Shader_DefaultAO].ps_ = LIME_NEW DefaultAOPS(ps);

        //
        compiler.compileShaderPS(ps, Shader_DefaultAOTexture);
        LASSERT(ps.valid());
        entries_[Shader_DefaultAOTexture].ps_ = LIME_NEW DefaultAOTexturePS(ps);

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
