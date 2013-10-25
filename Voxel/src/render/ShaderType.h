#ifndef INC_RENDER_SHADERTYPE_H__
#define INC_RENDER_SHADERTYPE_H__
/**
@file ShaderType.h
@author t-sakai
@date 2011/10/16 create
*/
#include "render/render.h"

namespace lgraphics
{
    class VertexShaderRef;
    class GeometryShaderRef;
    class PixelShaderRef;
}

namespace render
{
    class ShaderVSBase;
    class ShaderPSBase;

    /// シェーダID
    enum Shader
    {
        Shader_Default =0,
        Shader_DefaultTexture,
        Shader_Depth,
        Shader_Rectangle,
        Shader_DebugVC,
        Shader_TonemappingStatic,
        Shader_Voxelize,
        Shader_VoxelizeTexture,
        Shader_VoxelRender,
        Shader_OctreeTag,
        Shader_OctreeCreateNode,
        Shader_OctreeInitNode,
        Shader_OctreeWriteValue,
        Shader_OctreePropagate,
        Shader_OctreeMipmap,
        Shader_DefaultAO,
        Shader_DefaultAOTexture,
        Shader_Num,
    };

    enum ShaderGS
    {
        Shader_VoxelizePNGS,
        Shader_VoxelizePNUGS,
        ShaderGS_Num,
    };


    class Compiler
    {
    public:
        static const u32 DefaultSize = 2048;

        Compiler();
        ~Compiler();

        /// シェーダコンパイル
        void compileShaderVS(lgraphics::VertexShaderRef& vs, Shader id);

        /// シェーダコンパイル
        //void compileShaderGS(lgraphics::GeometryShaderRef& gs, ShaderGS id);

        /// シェーダコンパイル
        void compileShaderPS(lgraphics::PixelShaderRef& ps, Shader id);

        /// シェーダコンパイル
        inline void compileShader(lgraphics::VertexShaderRef& vs, lgraphics::PixelShaderRef& ps, Shader id)
        {
            compileShaderVS(vs, id);
            compileShaderPS(ps, id);
        }

        /// シェーダコンパイル
        void compileShaderGS(lgraphics::GeometryShaderRef& gs, ShaderGS id);

        void resize(u32 size);
        u32 decompress(u32 size, const u8* data);
        const u8* getData(){ return buffer_;}

    private:
        u32 size_;
        u8* buffer_;
    };
}
#endif //INC_RENDER_SHADERTYPE_H__
