#ifndef INC_FONTSHADER_H__
#define INC_FONTSHADER_H__
/**
@file FontShader.h
@author t-sakai
@date 2011/09/24 create
*/
#include <lmath/Vector4.h>
#include <lgraphics/api/ShaderRef.h>
#include <lgraphics/api/TextureRef.h>
#include <lgraphics/api/ConstantBufferRef.h>

namespace lgraphics
{
    class BlobRef;
    class TextureRef;
}

namespace font
{
    struct ConstantBuffer
    {
        lmath::Vector4 xparam_;
        lmath::Vector4 yparam_;
    };

    /**
    @brief 頂点シェーダ
    */
    class ShaderVS
    {
    public:
        ShaderVS()
        {}

        ~ShaderVS()
        {}

        void initialize();

        void attach()
        {
            shader_.attach();
        }

        void setParameters(const ConstantBuffer& constant)
        {
             constants_.update(0, NULL, &constant, 0, 0);
             constants_.setVS(0);
        }

        lgraphics::VertexShaderRef shader_;
        lgraphics::ConstantBufferRef constants_;
    };


    /**
    @brief ピクセルシェーダ
    */
    class ShaderPS
    {
    public:
        ShaderPS()
        {}

        ~ShaderPS()
        {}

        void attach()
        {
            shader_.attach();
        }
        lgraphics::PixelShaderRef shader_;
    };


    /**
    @brief 頂点シェーダ作成
    */
    void createShader(ShaderVS& vs, lgraphics::BlobRef& blob);

    /**
    @brief ピクセルシェーダ作成
    */
    void createShader(ShaderPS& ps);

    /**
    @brief ピクセルシェーダ作成
    */
    void createDistanceFieldShader(ShaderPS& ps);

}


#endif //INC_FONTSHADER_H__
