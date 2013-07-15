#ifndef INC_SHADERBASE_H__
#define INC_SHADERBASE_H__
/**
@file ShaderBase.h
@author t-sakai
@date 2010/04/27 create

*/
#include "lscene.h"
#include <lgraphics/api/ShaderRef.h>
#include <lgraphics/api/ShaderKey.h>

namespace lmath
{
    class Matrix43;
}

namespace lrender
{
    class Drawable;
    class Batch;
}

namespace lscene
{
    class Geometry;
    class Material;
    class Scene;

    class ShaderBase
    {
    public:
        virtual ~ShaderBase(){}

    protected:
        ShaderBase(){}
    };

#if defined(LIME_DX9)
    class ShaderVSBase : public ShaderBase
    {
    public:
        virtual ~ShaderVSBase()
        {}

        virtual void initialize() =0;
        virtual void setParameters(const lrender::Batch& batch, const Scene& scene) =0;

        void attach(){shader_.attach();}

        lgraphics::VertexShaderRef& getShaderRef(){ return shader_;}

        void setKey(lgraphics::ShaderKey& key){ shader_.setKey(key);}
        const lgraphics::ShaderKey& getKey() const{ return shader_.getKey();}
    protected:
        ShaderVSBase()
        {}

        ShaderVSBase(const lgraphics::VertexShaderRef& ref)
            :shader_(ref)
        {}

        inline void setHandle(lgraphics::HANDLE& handle, const char* paramName);
        inline void setSampler(u32& index, const char* name);

        lgraphics::VertexShaderRef shader_;
    };

    inline void ShaderVSBase::setHandle(lgraphics::HANDLE& handle, const char* paramName)
    {
        LASSERT(paramName != NULL);
        handle = shader_.getHandle(paramName);
    }

    inline void ShaderVSBase::setSampler(u32& index, const char* name)
    {
        LASSERT(name != NULL);
        index = shader_.getSamplerIndex(name);
    }



    class ShaderPSBase : public ShaderBase
    {
    public:
        virtual ~ShaderPSBase()
        {}

        virtual void initialize() =0;
        virtual void setParameters(const lrender::Batch& batch, const Scene& scene) =0;

        void attach(){shader_.attach();}

        lgraphics::PixelShaderRef& getShaderRef(){ return shader_;}

        void setKey(lgraphics::ShaderKey& key){ shader_.setKey(key);}
        const lgraphics::ShaderKey& getKey() const{ return shader_.getKey();}
    protected:
        ShaderPSBase()
        {}

        ShaderPSBase(const lgraphics::PixelShaderRef& ref)
            :shader_(ref)
        {
        }

        inline void setHandle(lgraphics::HANDLE& handle, const char* paramName);
        inline void setSampler(u32& index, const char* name);

        lgraphics::PixelShaderRef shader_;
    };

    inline void ShaderPSBase::setHandle(lgraphics::HANDLE& handle, const char* paramName)
    {
        LASSERT(paramName != NULL);
        handle = shader_.getHandle(paramName);
    }

    inline void ShaderPSBase::setSampler(u32& index, const char* name)
    {
        LASSERT(name != NULL);
        index = shader_.getSamplerIndex(name);
    }


#elif defined(LIME_GLES1)
    class ShaderVSBase : public ShaderBase
    {
    public:
        virtual ~ShaderVSBase()
        {
        }

        virtual void initialize() =0;
        virtual void setParameters(const lrender::Drawable& drawable, const lgraphics::Material& material, const lgraphics::Scene& scene) =0;
 
        const ShaderKey& getKey() const{ return key_;}

        void attach(){}

    protected:
        ShaderVSBase()
        {
        }

        ShaderKey key_;
    };


    class ShaderPSBase : public ShaderBase
    {
    public:
        virtual ~ShaderPSBase()
        {
        }

        virtual void initialize() =0;
        virtual void setParameters(const lrender::Drawable& drawable, const lgraphics::Material& material, const lgraphics::Scene& scene) =0;

        const ShaderKey& getKey() const{ return key_;}

        void attach(){}

    protected:
        ShaderPSBase()
        {
        }

        ShaderKey key_;
    };


#elif defined(LIME_GLES2)

    class ShaderVSBase : public ShaderBase
    {
    public:
        virtual ~ShaderVSBase()
        {}

        virtual void initialize() =0;
        virtual void setParameters(const lrender::Batch& batch, const Scene& scene) =0;

        void attach(){shader_.attach();}

        lgraphics::VertexShaderRef& getShaderRef(){ return shader_;}

        void setKey(lgraphics::ShaderKey& key){ shader_.setKey(key);}
        const lgraphics::ShaderKey& getKey() const{ return shader_.getKey();}
    protected:
        ShaderVSBase()
        {}

        ShaderVSBase(const lgraphics::VertexShaderRef& ref)
            :shader_(ref)
        {}

        inline void setHandle(lgraphics::HANDLE& handle, const char* paramName);
        inline void setSampler(u32& index, const char* name);

        lgraphics::VertexShaderRef shader_;
    };

    inline void ShaderVSBase::setHandle(lgraphics::HANDLE& handle, const char* paramName)
    {
        LASSERT(paramName != NULL);
        handle = shader_.getHandle(paramName);
    }

    inline void ShaderVSBase::setSampler(u32& index, const char* name)
    {
        LASSERT(name != NULL);
        index = shader_.getSamplerIndex(name);
    }



    class ShaderPSBase : public ShaderBase
    {
    public:
        virtual ~ShaderPSBase()
        {}

        virtual void initialize() =0;
        virtual void setParameters(const lrender::Batch& batch, const Scene& scene) =0;

        void attach(){shader_.attach();}

        lgraphics::PixelShaderRef& getShaderRef(){ return shader_;}

        void setKey(lgraphics::ShaderKey& key){ shader_.setKey(key);}
        const lgraphics::ShaderKey& getKey() const{ return shader_.getKey();}
    protected:
        ShaderPSBase()
        {}

        ShaderPSBase(const lgraphics::PixelShaderRef& ref)
            :shader_(ref)
        {
        }

        inline void setHandle(lgraphics::HANDLE& handle, const char* paramName);
        inline void setSampler(u32& index, const char* name);

        lgraphics::PixelShaderRef shader_;
    };

    inline void ShaderPSBase::setHandle(lgraphics::HANDLE& handle, const char* paramName)
    {
        LASSERT(paramName != NULL);
        handle = shader_.getHandle(paramName);
    }

    inline void ShaderPSBase::setSampler(u32& index, const char* name)
    {
        LASSERT(name != NULL);
        index = shader_.getSamplerIndex(name);
    }
#endif
}

#endif //INC_SHADERBASE_H__
