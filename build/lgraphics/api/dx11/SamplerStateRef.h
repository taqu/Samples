#ifndef INC_LGRAPHICS_DX11_SAMPLERSTATEREF_H__
#define INC_LGRAPHICS_DX11_SAMPLERSTATEREF_H__
/**
@file SamplerStateRef.h
@author t-sakai
@date 2012/07/23 create
*/
#include "../../lgraphicscore.h"
#include "Enumerations.h"

struct ID3D11SamplerState;

namespace lgraphics
{
    //------------------------------------------------------------
    //---
    //--- SamplerStateRef
    //---
    //------------------------------------------------------------
    class SamplerStateRef
    {
    public:
        SamplerStateRef()
            :state_(NULL)
        {}

        SamplerStateRef(const SamplerStateRef& rhs);

        ~SamplerStateRef()
        {
            destroy();
        }

        void destroy();

        ID3D11SamplerState* const* get(){ return &state_;}
        bool valid() const{ return (NULL != state_);}

        void swap(SamplerStateRef& rhs)
        {
            lcore::swap(state_, rhs.state_);
        }

        SamplerStateRef& operator=(const SamplerStateRef& rhs)
        {
            SamplerStateRef tmp(rhs);
            tmp.swap(*this);
            return *this;
        }

    private:
        friend class SamplerState;

        explicit SamplerStateRef(ID3D11SamplerState* state)
            :state_(state)
        {}

        ID3D11SamplerState* state_;
    };

    //------------------------------------------------------------
    //---
    //--- SamplerState
    //---
    //------------------------------------------------------------
    class SamplerState
    {
    public:
        static SamplerStateRef create(
            TextureFilterType filter,
            TextureAddress addressU,
            TextureAddress addressV,
            TextureAddress addressW,
            CmpFunc compFunc,
            f32 borderColor = 0.0f);
    };
}
#endif //INC_LGRAPHICS_DX11_SAMPLERSTATEREF_H__
