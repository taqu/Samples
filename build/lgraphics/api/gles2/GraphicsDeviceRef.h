#ifndef INC_LGRAPHICS_ES2_GRAPHICSDEVICEREF_H__
#define INC_LGRAPHICS_ES2_GRAPHICSDEVICEREF_H__
/**
@file GraphicsDeviceRef.h
@author t-sakai
@date 2010/05/29 create
@date 2010/12/12 modify for gles2
*/
#include "../../lgraphicsAPIInclude.h"
#include "../../lgraphicscore.h"
#include "Enumerations.h"
#include <lcore/Stack.h>
#include <lcore/BitSet.h>

namespace lmath
{
    class Vector3;
    class Vector4;
    class Matrix44;
}

namespace lgraphics
{
    class RenderStateRef;
    class SamplerState;

    struct InitParam;

    struct Descriptor
    {
        void addRef()
        {
            ++counter_;
        }

        void release();
        u32 counter_;
        u32 id_;
    };


    class GraphicsDeviceRef
    {
    public:

        static const u32 InitialIDAllocatorSize = 128;

        GraphicsDeviceRef();
        ~GraphicsDeviceRef();

        bool initialize(const InitParam& initParam);
        void terminate();

        inline void beginScene();
        inline void endScene();

        inline void clear(GLbitfield target);
        void setClearColor(u32 color);
        void setClearDepth(f32 depth);
        void setClearStencil(u32 stencil);

        inline void present();
        inline void flush();

        Descriptor* allocate()
        {
            Descriptor *ret= descAllocator_.pop();
            ret->counter_ = 0;
            ret->id_ = 0;
            return ret;
        }
        void freeDesc(Descriptor* ptr){ descAllocator_.push(ptr);}

        // ビューポート設定
        inline void setViewport(s32 left, s32 top, s32 width, s32 height);

        // Draw系
        //-----------------------------------------------------------------------------
        inline void draw(PrimitiveType type, u32 numVertices, u32 offsetVertex) const;
        inline void drawIndexed(PrimitiveType type, u32 numIndices, u32 offsetIndex) const;

        //inline void drawUP(PrimitiveType type, u32 primCount, const void* vertices, u32 stride);
        //inline void drawIndexedUP(
        //    PrimitiveType type,
        //    u32 minIndex,
        //    u32 vertexNum,
        //    u32 primCount,
        //    const void* indices,
        //    BufferFormat indexFormat,
        //    const void* vertices,
        //    u32 stride);



        void checkError();

        //-------------------------------------
        //--- RenderState
        //-------------------------------------
        void setAlphaTest(bool enable);
        inline bool getAlphaTest() const;

        void setAlphaTestFunc(CmpFunc func);
        inline CmpFunc getAlphaTestFunc() const;
        
        void setAlphaTestRef(s32 ref);
        inline s32 getAlphaTestRef() const;

        //void setCullingEnable(bool enable);
        //inline bool getCullingEnable() const;

        void setCullMode(CullMode mode);
        inline CullMode getCullMode() const;

        void setMultiSampleAlias(bool enable);
        inline bool getMultiSampleAlias() const;

        void setZEnable(bool enable);
        inline bool getZEnable() const;

        void setZWriteEnable(bool enable);
        inline bool getZWriteEnable() const;

        void setAlphaBlendEnable(bool enable);
        inline bool getAlphaBlendEnable() const;

        void setAlphaBlend(BlendType src, BlendType dst);
        inline BlendType getAlphaBlendSrc() const;
        inline BlendType getAlphaBlendDst() const;

        void setRenderState(const RenderStateRef& rhs);

        // テクスチャサンプラステート関係
        //-------------------------------------
        void clearActiveTextures();

        /**
        @brief テクスチャセット
        @param index ... テクスチャユニットインデックス
        @param id ... テクスチャオブジェクトハンドル
        @param location ... シェーダのuniform変数ハンドル
        */
        void setTexture(u32 index, u32 id, u32 location);

        inline bool checkExtension(Extension extension);
    private:
        friend struct Descriptor;

        typedef lcore::Stack<Descriptor> DescriptorAllocator;

        void initializeRenderState();
        void initializeExtension();

        static const s32 DefaultAlphaTestRef = 128;
        static const CmpFunc DefaultAlphaTestFunc = Cmp_Less;

        /// サンプラステート
        struct Sampler
        {
            u32 id_;
        };

        /// レンダリングステート
        struct State
        {
            enum Flag
            {
                Flag_AlphaTest = 0,
                Flag_MultiSampleAlias,
                Flag_ZEnable,
                Flag_ZWriteEnable,
                //Flag_CullingEnable,
                Flag_AlphaBlendEnable,
                Flag_Num,
            };

            Sampler samplerStates_[MAX_TEXTURES];

            s32 alphaTestRef_;      //アルファテスト参照値
            CmpFunc alphaTestFunc_; //アルファテスト比較関数

            CullMode cullMode_; //カリングモード

            BlendType alphaBlendSrc_; //srcブレンド設定
            BlendType alphaBlendDst_; //dstブレンド設定

            bool flush_;
            bool flags_[Flag_Num];
        };

        State state_; // レンダリングステート

#if defined(ANDROID)
#else
        EGLDisplay display_;
        EGLSurface surface_;
        EGLContext context_;
        NativeWindowType window_;
#endif

        DescriptorAllocator descAllocator_;

        lcore::BitSet32 extension_;

        //u32 maxMultiTextures_; /// マルチテクスチャ最大枚数
    };



    inline void GraphicsDeviceRef::beginScene()
    {
        if(state_.flush_){
            state_.flush_ = false;
            glFlush();
        }
    }

    inline void GraphicsDeviceRef::endScene()
    {
    }


    inline void GraphicsDeviceRef::clear(GLbitfield target)
    {
        glClear(target);
    }

    inline void GraphicsDeviceRef::present()
    {
#if defined(ANDROID)

#else
        eglSwapBuffers(display_, surface_);
#endif
    }

    inline void GraphicsDeviceRef::flush()
    {
        state_.flush_ = true;
    }

    // ビューポート設定
    inline void GraphicsDeviceRef::setViewport(s32 left, s32 top, s32 width, s32 height)
    {
        glViewport(left, top, width, height);
    }

    // Draw系
    //------------------------------------------------------------------------------------------
    inline void GraphicsDeviceRef::draw(PrimitiveType type, u32 numVertices, u32 offsetVertex) const
    {
        glDrawArrays(type, offsetVertex, numVertices);
    }

    inline void GraphicsDeviceRef::drawIndexed(PrimitiveType type, u32 numIndices, u32 offsetIndex) const
    {
        LASSERT(glDrawElements != NULL);
        offsetIndex <<= 1; //バイト単位のオフセットに修正
        glDrawElements(type, numIndices, GL_UNSIGNED_SHORT, LBUFFER_OFFSET(offsetIndex));
    }

    // Rendering State系
    //------------------------------------------------------------------------------------------
    inline bool GraphicsDeviceRef::getAlphaTest() const
    {
        return state_.flags_[State::Flag_AlphaTest];
    }

    inline s32 GraphicsDeviceRef::getAlphaTestRef() const
    {
        return state_.alphaTestRef_;
    }

    inline CmpFunc GraphicsDeviceRef::getAlphaTestFunc() const
    {
        return state_.alphaTestFunc_;
    }

    //inline bool GraphicsDeviceRef::getCullingEnable() const
    //{
    //    return state_.flags_[State::Flag_CullingEnable];
    //}

    inline CullMode GraphicsDeviceRef::getCullMode() const
    {
        return state_.cullMode_;
    }

    inline bool GraphicsDeviceRef::getMultiSampleAlias() const
    {
        return state_.flags_[State::Flag_MultiSampleAlias];
    }

    inline bool GraphicsDeviceRef::getZEnable() const
    {
        return state_.flags_[State::Flag_ZEnable];
    }

    inline bool GraphicsDeviceRef::getZWriteEnable() const
    {
        return state_.flags_[State::Flag_ZWriteEnable];
    }

    inline bool GraphicsDeviceRef::getAlphaBlendEnable() const
    {
        return state_.flags_[State::Flag_AlphaBlendEnable];
    }

    inline BlendType GraphicsDeviceRef::getAlphaBlendSrc() const
    {
        return state_.alphaBlendSrc_;
    }

    inline BlendType GraphicsDeviceRef::getAlphaBlendDst() const
    {
        return state_.alphaBlendDst_;
    }

    inline bool GraphicsDeviceRef::checkExtension(Extension extension)
    {
        return extension_.check(extension);
    }
}
#endif //INC_LGRAPHICS_ES2_GRAPHICSDEVICEREF_H__
