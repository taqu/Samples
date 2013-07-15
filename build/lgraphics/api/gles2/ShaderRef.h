#ifndef INC_LGRAPHICS_GLES2_SHADERREF_H__
#define INC_LGRAPHICS_GLES2_SHADERREF_H__
/**
@file ShaderRef.h
@author t-sakai
@date 2009/01/26 create
*/
#include <lcore/CLibrary.h>

#include "../../lgraphicsAPIInclude.h"
#include "../ShaderKey.h"

struct IDirect3DDevice9;
struct ID3DXConstantTable;
struct IDirect3DPixelShader9;
struct IDirect3DVertexShader9;
struct ID3DXBuffer;

namespace lmath
{
    class Vector2;
    class Vector3;
    class Vector4;

    class Matrix34;
    class Matrix44;
}

namespace lgraphics
{
    class ShaderKey;
    class Scene;
    class Material;
    class Geometry;
    struct Descriptor;

    class VertexDeclarationRef;

    extern const char* ShaderEntryFunctionName; //="main";

    //--------------------------------------------------------
    /// タイプ
    enum ShaderType
    {
        ShaderType_Vertex =0,
        ShaderType_Pixel,
        ShaderType_Num,
    };

    //------------------------------------------------------------
    //---
    //--- ShaderRefBase
    //---
    //------------------------------------------------------------
    class ShaderRefBase
    {
    protected:
        ShaderRefBase()
            :programID_(NULL)
        {}

        explicit ShaderRefBase(Descriptor* programID)
            :programID_(programID)
        {
        }

        ShaderRefBase(const ShaderRefBase& rhs);

        ~ShaderRefBase()
        {
        }

    public:
        ShaderRefBase& operator=(const ShaderRefBase& rhs)
        {
            ShaderRefBase tmp(rhs);
            tmp.swap(*this);
            return *this;
        }

        bool valid() const{ return (programID_ != NULL);}

        void destroy();

        /**
        @param transpose ... 転置するか
        */
        void setMatrix(HANDLE handle, const lmath::Matrix44& matrix, u8 transpose = 0);
        void setMatrix(HANDLE handle, const lmath::Matrix34& matrix);
        void setFloat(HANDLE handle, f32 value);
        void setVector2(HANDLE handle, const lmath::Vector2& vector);
        void setVector3(HANDLE handle, const lmath::Vector3& vector);
        void setVector4(HANDLE handle, const lmath::Vector4& vector);

        void setVector4Array(HANDLE handle, const lmath::Vector4* vector, u32 count);

        lgraphics::HANDLE getHandle(const char* name);

        u32 getSamplerIndex(const char* name);
        
        void swap(ShaderRefBase& rhs)
        {
            lcore::swap(programID_, rhs.programID_);
            lcore::swap(key_, rhs.key_);
        }

        const ShaderKey& getKey() const{ return key_;}
        void setKey(const ShaderKey& key){ key_ = key;}

        void attach() const;

        const Descriptor* getProgramID() const{ return programID_;}
    private:
        friend class Shader;
        friend class VertexDeclarationRef;

        Descriptor *programID_;
        ShaderKey key_;
    };


    //------------------------------------------------------------
    //---
    //--- PixelShaderRef
    //---
    //------------------------------------------------------------
    class PixelShaderRef : public ShaderRefBase
    {
    public:
        PixelShaderRef()
            :shaderID_(NULL)
        {
        }

        PixelShaderRef(const PixelShaderRef& rhs);

        ~PixelShaderRef()
        {
            destroy();
        }

        void destroy();

        PixelShaderRef& operator=(const PixelShaderRef& rhs)
        {
            PixelShaderRef tmp(rhs);
            tmp.swap(*this);
            return *this;
        }

        bool valid() const{ return (shaderID_!=NULL);}
        
        static ShaderType getType(){ return ShaderType_Pixel;}

        void swap(PixelShaderRef& rhs)
        {
            ShaderRefBase::swap(rhs);
            lcore::swap(shaderID_, rhs.shaderID_);
        }

        const Descriptor* getDescriptor() const{ return shaderID_;}

    private:
        friend class Shader;

        PixelShaderRef(Descriptor* program, Descriptor* shader)
            :ShaderRefBase(program)
            ,shaderID_(shader)
        {
        }

        Descriptor* shaderID_;
    };


    //------------------------------------------------------------
    //---
    //--- VertexShaderRef
    //---
    //------------------------------------------------------------
    class VertexShaderRef : public ShaderRefBase
    {
    public:
         VertexShaderRef()
            :shaderID_(NULL)
        {
        }

        VertexShaderRef(const VertexShaderRef& rhs);

        ~VertexShaderRef()
        {
            destroy();
        }

        void destroy();

        VertexShaderRef& operator=(const VertexShaderRef& rhs)
        {
            VertexShaderRef tmp(rhs);
            tmp.swap(*this);
            return *this;
        }

        bool valid() const{ return (shaderID_!=NULL);}
        
        static ShaderType getType(){ return ShaderType_Vertex;}

        void swap(VertexShaderRef& rhs)
        {
            ShaderRefBase::swap(rhs);
            lcore::swap(shaderID_, rhs.shaderID_);
        }

        const Descriptor* getDescriptor() const{ return shaderID_;}

    private:
        friend class Shader;

        VertexShaderRef(Descriptor* program, Descriptor* shader)
            :ShaderRefBase(program)
            ,shaderID_(shader)
        {
        }

        Descriptor* shaderID_;
    };


    class Shader
    {
    public:
        static const s32 MaxDefines = 63; /// defineの最大数

        /**
        @brief ファイルからピクセルシェーダ作成
        @param filename ... 
        @param numDefines ... defineマクロの数
        @param defines ... defineマクロ名
        */
        static Descriptor* createPixelShaderFromFile(const char* filename, s32 numDefines, const char** defines);

        /**
        @brief メモリからピクセルシェーダ作成
        @param memory ... 
        @param size ... 
        @param numDefines ... defineマクロの数
        @param defines ... defineマクロ名
        */
        static Descriptor* createPixelShaderFromMemory(const char* memory, u32 size, s32 numDefines, const char** defines);

        /**
        @brief ファイルから頂点シェーダ作成
        @param filename ... 
        @param numDefines ... defineマクロの数
        @param defines ... defineマクロ名
        */
        static Descriptor* createVertexShaderFromFile(const char* filename, s32 numDefines, const char** defines);

        /**
        @brief メモリから頂点シェーダ作成
        @param memory ... 
        @param size ... 
        @param numDefines ... defineマクロの数
        @param defines ... defineマクロ名
        */
        static Descriptor* createVertexShaderFromMemory(const char* memory, u32 size, s32 numDefines, const char** defines);

        static bool linkShader(VertexShaderRef& vs, PixelShaderRef& ps, u32 numStreams, VertexDeclarationRef& decl, Descriptor* vsDesc, Descriptor* psDesc);
    private:

        /**
        @brief シェーダコンパイル
        @param shader ... 
        @param str ... ヌル文字終了
        @param numDefines ... 
        @param defines ... 改行で終わる定義文字列
        */
        static bool compileShaderFromMemory(
            u32 shader,
            const char* str,
            u32 size,
            s32 numDefines,
            const char** defines);
    };
}

//---------------------------------------------------
//---
//--- lcore::swap特殊化
//---
//---------------------------------------------------
namespace lcore
{
    template<>
    inline void swap<lgraphics::PixelShaderRef>(lgraphics::PixelShaderRef& l, lgraphics::PixelShaderRef& r)
    {
        l.swap(r);
    }

    template<>
    inline void swap<lgraphics::VertexShaderRef>(lgraphics::VertexShaderRef& l, lgraphics::VertexShaderRef& r)
    {
        l.swap(r);
    }
}

#endif //INC_LGRAPHICS_GLES2_SHADERREF_H__
