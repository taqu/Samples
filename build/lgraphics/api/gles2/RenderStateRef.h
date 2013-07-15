#ifndef INC_LGRAPHICS_ES2_RENDERSTATEREF_H__
#define INC_LGRAPHICS_ES2_RENDERSTATEREF_H__
/**
@file RenderStateRef.h
@author t-sakai
@date 2010/06/16 create
@date 2010/12/12 modify for gles2
*/
#include "../../lgraphicscore.h"
#include "Enumerations.h"

namespace lgraphics
{
    //----------------------------------------------------------
    //---
    //--- RenderState
    //---
    //----------------------------------------------------------
    class RenderState
    {
    public:
        static void setAlphaTest(bool enable);
        static bool getAlphaTest();

        static void setAlphaTestFunc(CmpFunc func);
        static CmpFunc getAlphaTestFunc();

        static void setAlphaTestRef(s32 refValue);
        static s32 getAlphaTestRef();

        static void setCullMode(CullMode mode);
        static CullMode getCullMode();

        static void setMultiSampleAlias(bool enable);
        static bool getMultiSampleAlias();

        static void setZEnable(bool enable);
        static bool getZEnable();

        static void setZWriteEnable(bool enable);
        static bool getZWriteEnable();

        static void setAlphaBlendEnable(bool enable);
        static bool getAlphaBlendEnable();

        static void setAlphaBlend(BlendType src, BlendType dst);
        static BlendType getAlphaBlendSrc();
        static BlendType getAlphaBlendDst();

        // Vertex, Pixel以外のステート
        //--------------------------------------------------------
        static void setViewPort(s32 x, s32 y, s32 width, s32 height, f32 minz, f32 maxz);
        static void getViewPort(s32& x, s32& y, s32& width, s32& height, f32& minz, f32& maxz);

    };


    //----------------------------------------------------------
    //---
    //--- RenderStateRef
    //---
    //----------------------------------------------------------
    class RenderStateRef
    {
    public:
        inline void setAlphaTest(bool enable);
        inline bool getAlphaTest() const;

        inline void setAlphaTestFunc(CmpFunc func);
        inline CmpFunc getAlphaTestFunc() const;

        inline void setAlphaTestRef(s32 refValue);
        inline s32 getAlphaTestRef() const;

        inline void setCullMode(CullMode mode);
        inline CullMode getCullMode() const;

        inline void setMultiSampleAlias(bool enable);
        inline bool getMultiSampleAlias() const;

        inline void setZEnable(bool enable);
        inline bool getZEnable() const;

        inline void setZWriteEnable(bool enable);
        inline bool getZWriteEnable() const;

        inline void setAlphaBlendEnable(bool enable);
        inline bool getAlphaBlendEnable() const;

        inline void setAlphaBlend(BlendType src, BlendType dst);
        inline BlendType getAlphaBlendSrc() const;
        inline BlendType getAlphaBlendDst() const;


        RenderStateRef();
        RenderStateRef(const RenderStateRef& rhs);

        ~RenderStateRef();

        RenderStateRef& operator=(const RenderStateRef& rhs);

        void apply();

        void swap(RenderStateRef& rhs);

        enum Bit
        {
            Bit_AlphaTest = (0x00000001U << 0),
            Bit_MultiSampleAlias = (0x00000001U << 1),
            Bit_ZEnable = (0x00000001U << 2),
            Bit_ZWriteEnable = (0x00000001U << 3),
            Bit_CullingEnable = (0x00000001U << 4),
            Bit_AlphaBlendEnable = (0x00000001U << 5),
        };

        inline bool check(Bit bit) const;

    private:
        inline void set(Bit bit);
        inline void set(Bit bit, bool enable);
        inline void reset(Bit bit);

        u32 flag_; //ON・OFFフラッグ
        CmpFunc alphaTestFunc_;
        s32 alphaTestRef_;
        CullMode cullMode_;
        BlendType alphaBlendSrc_;
        BlendType alphaBlendDst_;
    };

    inline void RenderStateRef::setAlphaTest(bool enable)
    {
        set(Bit_AlphaTest, enable);
    }

    inline bool RenderStateRef::getAlphaTest() const
    {
        return check(Bit_AlphaTest);
    }

    inline void RenderStateRef::setAlphaTestFunc(CmpFunc func)
    {
        alphaTestFunc_ = func;
    }

    inline CmpFunc RenderStateRef::getAlphaTestFunc() const
    {
        return alphaTestFunc_;
    }

    inline void RenderStateRef::setAlphaTestRef(s32 refValue)
    {
        alphaTestRef_ = refValue;
    }

    inline s32 RenderStateRef::getAlphaTestRef() const
    {
        return alphaTestRef_;
    }

    inline void RenderStateRef::setCullMode(CullMode mode)
    {
        cullMode_ = mode;
        //if(CullMode_None == cullMode_){
        //    set(Bit_CullingEnable, false);
        //}else{
        //    set(Bit_CullingEnable, true);
        //}
    }

    inline CullMode RenderStateRef::getCullMode() const
    {
        return cullMode_;
    }

    inline void RenderStateRef::setMultiSampleAlias(bool )
    {
    }

    inline bool RenderStateRef::getMultiSampleAlias() const
    {
        return false;
    }

    inline void RenderStateRef::setZEnable(bool enable)
    {
        set(Bit_ZEnable, enable);
    }

    inline bool RenderStateRef::getZEnable() const
    {
        return check(Bit_ZEnable);
    }

    inline void RenderStateRef::setZWriteEnable(bool enable)
    {
        set(Bit_ZWriteEnable, enable);
    }

    inline bool RenderStateRef::getZWriteEnable() const
    {
        return check(Bit_ZWriteEnable);
    }

    inline void RenderStateRef::setAlphaBlendEnable(bool enable)
    {
        set(Bit_AlphaBlendEnable, enable);
    }

    inline bool RenderStateRef::getAlphaBlendEnable() const
    {
        return check(Bit_AlphaBlendEnable);
    }

    inline void RenderStateRef::setAlphaBlend(BlendType src, BlendType dst)
    {
        alphaBlendSrc_ = src;
        alphaBlendDst_ = dst;
    }

    inline BlendType RenderStateRef::getAlphaBlendSrc() const
    {
        return alphaBlendSrc_;
    }

    inline BlendType RenderStateRef::getAlphaBlendDst() const
    {
        return alphaBlendDst_;
    }

    inline void RenderStateRef::set(Bit bit)
    {
        flag_ |= bit;
    }

    inline void RenderStateRef::set(Bit bit, bool enable)
    {
        (enable)? flag_ |= bit : flag_ &= (~bit);
    }

    inline void RenderStateRef::reset(Bit bit)
    {
        flag_ &= ~bit;
    }

    inline bool RenderStateRef::check(Bit bit) const
    {
        return (flag_ & bit) != 0;
    }
}

//---------------------------------------------------
//---
//--- lcore::swap特殊化
//---
//---------------------------------------------------
namespace lcore
{
    template<>
    inline void swap<lgraphics::RenderStateRef>(lgraphics::RenderStateRef& l, lgraphics::RenderStateRef& r)
    {
        l.swap(r);
    }
}
#endif //INC_LGRAPHICS_ES2_RENDERSTATEREF_H__
