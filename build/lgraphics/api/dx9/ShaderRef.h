#ifndef INC_LGRAPHICS_DX9_SHADERREF_H__
#define INC_LGRAPHICS_DX9_SHADERREF_H__
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
            :_constantTable(NULL)
        {}

        explicit ShaderRefBase(ID3DXConstantTable* table)
            :_constantTable(table)
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

        bool valid() const{ return (_constantTable != NULL);}

        void destroy();

#if 0
        void setMatrixByName(const char* name, const lmath::Matrix44& matrix);
        void setFloatByName(const char* name, f32 value);
        void setVector2ByName(const char* name, const lmath::Vector2& vector);
        void setVector3ByName(const char* name, const lmath::Vector3& vector);
        void setVector4ByName(const char* name, const lmath::Vector4& vector);
#endif

        void setMatrix(HANDLE handle, const lmath::Matrix44& matrix);
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
            lcore::swap(_constantTable, rhs._constantTable);
            lcore::swap(key_, rhs.key_);
        }

        const ShaderKey& getKey() const{ return key_;}
        void setKey(const ShaderKey& key){ key_ = key;}
    private:
        ID3DXConstantTable *_constantTable;
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
            :_shader(NULL)
        {
        }

        PixelShaderRef(const PixelShaderRef& rhs);

        ~PixelShaderRef()
        {
            destroy();
        }

        PixelShaderRef& operator=(const PixelShaderRef& rhs)
        {
            PixelShaderRef tmp(rhs);
            tmp.swap(*this);
            return *this;
        }

        bool valid() const{ return ShaderRefBase::valid() && (_shader!=NULL);}
        
        static ShaderType getType(){ return ShaderType_Pixel;}

        void attach() const;

        void swap(PixelShaderRef& rhs)
        {
            ShaderRefBase::swap(rhs);
            lcore::swap(_shader, rhs._shader);
        }

    private:
        friend class Shader;

        void destroy();

        PixelShaderRef(IDirect3DPixelShader9 *shader, ID3DXConstantTable *table)
            :ShaderRefBase(table)
            ,_shader(shader)
        {
        }

        IDirect3DPixelShader9* _shader;
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
            :_shader(NULL)
        {
        }

        VertexShaderRef(const VertexShaderRef& rhs);

        ~VertexShaderRef()
        {
            destroy();
        }

        VertexShaderRef& operator=(const VertexShaderRef& rhs)
        {
            VertexShaderRef tmp(rhs);
            tmp.swap(*this);
            return *this;
        }

        bool valid() const{ return ShaderRefBase::valid() && (_shader!=NULL);}
        
        static ShaderType getType(){ return ShaderType_Vertex;}

        void attach() const;

        void swap(VertexShaderRef& rhs)
        {
            ShaderRefBase::swap(rhs);
            lcore::swap(_shader, rhs._shader);
        }

    private:
        friend class Shader;

        void destroy();

        VertexShaderRef(IDirect3DVertexShader9 *shader, ID3DXConstantTable *table)
            :ShaderRefBase(table)
            ,_shader(shader)
        {
        }

        IDirect3DVertexShader9* _shader;
    };


    //------------------------------------------------------------
    //---
    //--- Shader
    //---
    //------------------------------------------------------------
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
        static PixelShaderRef createPixelShaderFromFile(const char* filename, s32 numDefines, const char** defines);

        /**
        @brief メモリからピクセルシェーダ作成
        @param memory ... 
        @param size ... 
        @param numDefines ... defineマクロの数
        @param defines ... defineマクロ名
        */
        static PixelShaderRef createPixelShaderFromMemory(const char* memory, u32 size, s32 numDefines, const char** defines);

        /**
        @brief ファイルから頂点シェーダ作成
        @param filename ... 
        @param numDefines ... defineマクロの数
        @param defines ... defineマクロ名
        */
        static VertexShaderRef createVertexShaderFromFile(const char* filename, s32 numDefines, const char** defines);

        /**
        @brief メモリから頂点シェーダ作成
        @param memory ... 
        @param size ... 
        @param numDefines ... defineマクロの数
        @param defines ... defineマクロ名
        */
        static VertexShaderRef createVertexShaderFromMemory(const char* memory, u32 size, s32 numDefines, const char** defines);

    private:
        /**
        @brief シェーダコンパイル
        */
        static bool compileShaderFromFile(
            const char* filename,
            const char* entryName,
            const char* profile,
            s32 numDefines,
            const char** defines,
            ID3DXBuffer** shaderBuffer,
            ID3DXConstantTable** constants);

        /**
        @brief シェーダコンパイル
        */
        static bool compileShaderFromMemory(
            const char* memory,
            u32 size,
            const char* entryName,
            const char* profile,
            s32 numDefines,
            const char** defines,
            ID3DXBuffer** shaderBuffer,
            ID3DXConstantTable** constants);
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

#endif //INC_LGRAPHICS_DX9_SHADERREF_H__
