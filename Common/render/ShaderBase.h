#ifndef INC_RENDER_SHADERBASE_H__
#define INC_RENDER_SHADERBASE_H__
/**
@file ShaderBase.h
@author t-sakai
@date 2011/10/16 create
*/
#include <lmath/lmath.h>
#include <lgraphics/api/ShaderRef.h>

namespace lscene
{
    class Scene;
}

namespace render
{
    class Material;
    class Mesh;
    class BounceMap;

    //----------------------------------------
    //---
    //--- ShaderVSBase
    //---
    //----------------------------------------
    class ShaderVSBase
    {
    public:
        virtual ~ShaderVSBase()
        {}

        virtual void initialize(){}

        lgraphics::VertexShaderRef& getShaderRef(){ return shader_;}
        void attach(){ shader_.attach();}
    protected:
        ShaderVSBase(const ShaderVSBase&);
        ShaderVSBase& operator=(const ShaderVSBase&);

        ShaderVSBase()
        {}

        ShaderVSBase(const lgraphics::VertexShaderRef& ref)
            :shader_(ref)
        {}


        lgraphics::VertexShaderRef shader_;
    };

    //----------------------------------------
    //---
    //--- ShaderGSBase
    //---
    //----------------------------------------
    class ShaderGSBase
    {
    public:
        virtual ~ShaderGSBase()
        {}

        virtual void initialize(){}

        lgraphics::GeometryShaderRef& getShaderRef(){ return shader_;}
        void attach(){ shader_.attach();}
    protected:
        ShaderGSBase(const ShaderGSBase&);
        ShaderGSBase& operator=(const ShaderGSBase&);

        ShaderGSBase()
        {}

        ShaderGSBase(const lgraphics::GeometryShaderRef& ref)
            :shader_(ref)
        {}


        lgraphics::GeometryShaderRef shader_;
    };


    //----------------------------------------
    //---
    //--- ShaderPSBase
    //---
    //----------------------------------------
    class ShaderPSBase
    {
    public:
        virtual ~ShaderPSBase()
        {}

        virtual void initialize(){}

        lgraphics::PixelShaderRef& getShaderRef(){ return shader_;}
        void attach(){ shader_.attach();}

    protected:
        ShaderPSBase(const ShaderPSBase&);
        ShaderPSBase& operator=(const ShaderPSBase&);

        ShaderPSBase()
        {}

        ShaderPSBase(const lgraphics::PixelShaderRef& ref)
            :shader_(ref)
        {
        }

        lgraphics::PixelShaderRef shader_;
    };
}

#endif //INC_RENDER_SHADERBASE_H__
