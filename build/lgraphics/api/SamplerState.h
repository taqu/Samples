#ifndef INC_LGRAPHICS_SAMPLERSTATE_H__
#define INC_LGRAPHICS_SAMPLERSTATE_H__
/**
@file SamplerState.h
@author t-sakai
@date 2010/05/06 create
*/
#include "../lgraphicscore.h"
#include "Enumerations.h"

namespace lgraphics
{
    class SamplerState
    {
    public:
        // U方向アドレッシング
        inline void setAddressU(TextureAddress addressType);
        inline TextureAddress getAddressU() const;

        // V方向アドレッシング
        inline void setAddressV(TextureAddress addressType);
        inline TextureAddress getAddressV() const;

        inline void setMagFilter(TextureFilterType filterType);
        inline TextureFilterType getMagFilter() const;

        inline void setMinFilter(TextureFilterType filterType);
        inline TextureFilterType getMinFilter() const;

        inline void setMipFilter(TextureFilterType filterType);
        inline TextureFilterType getMipFilter() const;

        inline void setMipLODBias(u32 bias);
        inline u32 getMipLODBias() const;

        inline void setMaxMipLevel(u32 level);
        inline u32 getMaxMipLevel() const;

        inline void setMaxAnisotropy(u32 aniso);
        inline u32 getMaxAnisotropy() const;


        SamplerState();
        SamplerState(const SamplerState& rhs);

        ~SamplerState();

        SamplerState& operator=(const SamplerState& rhs)
        {
            addressU_ = rhs.addressU_;
            addressV_ = rhs.addressV_;
            magFilter_ = rhs.magFilter_;
            minFilter_ = rhs.minFilter_;
            mipFilter_ = rhs.mipFilter_;
            mipmapLODBias_ = rhs.mipmapLODBias_;
            maxMipLevel_ = rhs.maxMipLevel_;
            maxAnisotropy_ = rhs.maxAnisotropy_;
            return *this;
        }

        void apply(u32 sampler) const;

        void swap(SamplerState& rhs);
    private:
        TextureAddress addressU_;
        TextureAddress addressV_;
        TextureFilterType magFilter_;
        TextureFilterType minFilter_;
        TextureFilterType mipFilter_;
        u32 mipmapLODBias_;
        u32 maxMipLevel_;
        u32 maxAnisotropy_;
    };

    // U方向アドレッシング
    inline void SamplerState::setAddressU(TextureAddress addressType)
    {
        addressU_ = addressType;
    }

    inline TextureAddress SamplerState::getAddressU() const
    {
        return addressU_;
    }

    // V方向アドレッシング
    inline void SamplerState::setAddressV(TextureAddress addressType)
    {
        addressV_ = addressType;
    }

    inline TextureAddress SamplerState::getAddressV() const
    {
        return addressV_;
    }

    inline void SamplerState::setMagFilter(TextureFilterType filterType)
    {
#if defined(LIME_GLES1)
        if(filterType >=TexFilter_Anisotropic){
            filterType = TexFilter_Linear;
        }
#endif
        magFilter_ = filterType;
    }

    inline TextureFilterType SamplerState::getMagFilter() const
    {
        return magFilter_;
    }

    inline void SamplerState::setMinFilter(TextureFilterType filterType)
    {
#if defined(LIME_GLES1)
        if(filterType >=TexFilter_Anisotropic){
            filterType = TexFilter_Linear;
        }
#endif
        minFilter_ = filterType;
    }

    inline TextureFilterType SamplerState::getMinFilter() const
    {
        return minFilter_;
    }

    inline void SamplerState::setMipFilter(TextureFilterType filterType)
    {
#if defined(LIME_GLES1)
        if(filterType >=TexFilter_Anisotropic){
            filterType = TexFilter_Linear;
        }
#endif
        mipFilter_ = filterType;
    }

    inline TextureFilterType SamplerState::getMipFilter() const
    {
        return mipFilter_;
    }

    inline void SamplerState::setMipLODBias(u32 bias)
    {
        mipmapLODBias_ = bias;
    }

    inline u32 SamplerState::getMipLODBias() const
    {
        return mipmapLODBias_;
    }

    inline void SamplerState::setMaxMipLevel(u32 level)
    {
        maxMipLevel_ = level;
    }

    inline u32 SamplerState::getMaxMipLevel() const
    {
        return maxMipLevel_;
    }

    inline void SamplerState::setMaxAnisotropy(u32 aniso)
    {
        maxAnisotropy_ = aniso;
    }

    inline u32 SamplerState::getMaxAnisotropy() const
    {
        return maxAnisotropy_;
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
    inline void swap<lgraphics::SamplerState>(lgraphics::SamplerState& l, lgraphics::SamplerState& r)
    {
        l.swap(r);
    }
}
#endif //INC_LGRAPHICS_SAMPLERSTATE_H__
