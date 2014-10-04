#ifndef INC_LGRAPHICS_DX11_VIEWREF_H__
#define INC_LGRAPHICS_DX11_VIEWREF_H__
/**
@file ViewRef.h
@author t-sakai
@date 2014/10/01 create
*/
#include "../../lgraphics.h"
#include "Enumerations.h"

struct ID3D11ShaderResourceView;
struct ID3D11DepthStencilView;

namespace lgraphics
{
    struct Texture1DRTV
    {
        u32 mipSlice_;
    };

    struct Texture1DArrayRTV
    {
        u32 mipSlice_;
        u32 firstArraySlice_;
        u32 arraySize_;
    };

    struct Texture2DRTV
    {
        u32 mipSlice_;
    };

    struct Texture2DArrayRTV
    {
        u32 mipSlice_;
        u32 firstArraySlice_;
        u32 arraySize_;
    };

    struct Texture3DRTV
    {
        u32 mipSlice_;
        u32 firstWSlice_;
        u32 wSize_;
    };

    struct RTVDesc
    {
        DataFormat format_;
        ViewRTVDimension dimension_;
        union
        {
            Texture1DRTV tex1D_;
            Texture1DArrayRTV tex1DArray_;

            Texture2DRTV tex2D_;
            Texture2DArrayRTV tex2DArray_;

            Texture3DRTV tex3D_;
        };
    };


    struct Texture1DDSV
    {
        u32 mipSlice_;
    };

    struct Texture1DArrayDSV
    {
        u32 mipSlice_;
        u32 firstArraySlice_;
        u32 arraySize_;
    };

    struct Texture2DDSV
    {
        u32 mipSlice_;
    };

    struct Texture2DArrayDSV
    {
        u32 mipSlice_;
        u32 firstArraySlice_;
        u32 arraySize_;
    };


    struct DSVDesc
    {
        DataFormat format_;
        ViewDSVDimension dimension_;
        union
        {
            Texture1DDSV tex1D_;
            Texture1DArrayDSV tex1DArray_;

            Texture2DDSV tex2D_;
            Texture2DArrayDSV tex2DArray_;
        };
    };

    struct BufferSRV
    {
        u32 firstElement_;
        u32 numElements_;
    };

    struct Texture1DSRV
    {
        u32 mostDetailedMip_;
        u32 mipLevels_;
    };

    struct Texture1DArraySRV
    {
        u32 mostDetailedMip_;
        u32 mipLevels_;
        u32 firstArraySlice_;
        u32 arraySize_;
    };

    struct Texture2DSRV
    {
        u32 mostDetailedMip_;
        u32 mipLevels_;
    };

    struct Texture2DArraySRV
    {
        u32 mostDetailedMip_;
        u32 mipLevels_;
        u32 firstArraySlice_;
        u32 arraySize_;
    };

    struct Texture3DSRV
    {
        u32 mostDetailedMip_;
        u32 mipLevels_;
    };

    struct TextureCubeSRV
    {
        u32 mostDetailedMip_;
        u32 mipLevels_;
    };

    struct TextureCubeArraySRV
    {
        u32 mostDetailedMip_;
        u32 mipLevels_;
        u32 first2DArraySlice_;
        u32 numCubes_;
    };

    struct SRVDesc
    {
        DataFormat format_;
        ViewSRVDimension dimension_;
        union
        {
            BufferSRV buffer_;
            Texture1DSRV tex1D_;
            Texture1DArraySRV tex1DArray_;

            Texture2DSRV tex2D_;
            Texture2DArraySRV tex2DArray_;

            Texture3DSRV tex3D_;

            TextureCubeSRV texCube_;
            TextureCubeArraySRV texCubeArray_;
        };

        static bool copy(D3D11_SHADER_RESOURCE_VIEW_DESC& viewDesc, const SRVDesc& desc);
    };

    typedef SRVDesc ResourceViewDesc;

    struct BufferUAV
    {
        u32 firstElement_;
        u32 numElements_;
        u32 flags_;
    };

    struct Texture1DUAV
    {
        u32 mipSlice_;
    };

    struct Texture1DArrayUAV
    {
        u32 mipSlice_;
        u32 firstArraySlice_;
        u32 arraySize_;
    };

    struct Texture2DUAV
    {
        u32 mipSlice_;
    };

    struct Texture2DArrayUAV
    {
        u32 mipSlice_;
        u32 firstArraySlice_;
        u32 arraySize_;
    };

    struct Texture3DUAV
    {
        u32 mipSlice_;
        u32 firstWSlice_;
        u32 wsize_;
    };

    struct UAVDesc
    {
        DataFormat format_;
        UAVDimension dimension_;
        union
        {
            BufferUAV buffer_;
            Texture1DUAV tex1D_;
            Texture1DArrayUAV tex1DArray_;

            Texture2DUAV tex2D_;
            Texture2DArrayUAV tex2DArray_;

            Texture3DUAV tex3D_;
        };
    };

    struct SubResourceData
    {
        const void* mem_;
        u32 pitch_;
        u32 slicePitch_;
    };

    //--------------------------------------------------------
    //---
    //--- ShaderResourceViewRef
    //---
    //--------------------------------------------------------
    class ShaderResourceViewRef
    {
    public:
        ShaderResourceViewRef()
            :view_(NULL)
        {}

        ShaderResourceViewRef(const ShaderResourceViewRef& rhs);

        explicit ShaderResourceViewRef(ID3D11ShaderResourceView* view)
            :view_(view)
        {}

        ~ShaderResourceViewRef()
        {
            destroy();
        }

        ShaderResourceViewRef& operator=(const ShaderResourceViewRef& rhs)
        {
            ShaderResourceViewRef tmp(rhs);
            tmp.swap(*this);
            return *this;
        }

        void destroy();

        bool valid() const{ return (NULL != view_);}

        void swap(ShaderResourceViewRef& rhs)
        {
            lcore::swap(view_, rhs.view_);
        }

        ID3D11ShaderResourceView* getView(){ return view_;}
        ID3D11ShaderResourceView* const* get(){ return &view_;}
    private:
        ID3D11ShaderResourceView* view_;
    };

    //--------------------------------------------------------
    //---
    //--- RenderTargetViewRef
    //---
    //--------------------------------------------------------
    class RenderTargetViewRef
    {
    public:
        RenderTargetViewRef()
            :view_(NULL)
        {}

        RenderTargetViewRef(const RenderTargetViewRef& rhs);

        explicit RenderTargetViewRef(ID3D11RenderTargetView* view)
            :view_(view)
        {}

        ~RenderTargetViewRef()
        {
            destroy();
        }

        RenderTargetViewRef& operator=(const RenderTargetViewRef& rhs)
        {
            RenderTargetViewRef tmp(rhs);
            tmp.swap(*this);
            return *this;
        }

        void destroy();

        bool valid() const{ return (NULL != view_);}

        void swap(RenderTargetViewRef& rhs)
        {
            lcore::swap(view_, rhs.view_);
        }

        ID3D11RenderTargetView* getView(){ return view_;}
        ID3D11RenderTargetView* const* get(){ return &view_;}

        ShaderResourceViewRef createSRView(const SRVDesc& desc);
    private:
        ID3D11RenderTargetView* view_;
    };

    //--------------------------------------------------------
    //---
    //--- DepthStencilViewRef
    //---
    //--------------------------------------------------------
    class DepthStencilViewRef
    {
    public:
        DepthStencilViewRef()
            :view_(NULL)
        {}

        DepthStencilViewRef(const DepthStencilViewRef& rhs);

        explicit DepthStencilViewRef(ID3D11DepthStencilView* view)
            :view_(view)
        {}

        ~DepthStencilViewRef()
        {
            destroy();
        }

        DepthStencilViewRef& operator=(const DepthStencilViewRef& rhs)
        {
            DepthStencilViewRef tmp(rhs);
            tmp.swap(*this);
            return *this;
        }

        void destroy();

        bool valid() const{ return (NULL != view_);}

        void swap(DepthStencilViewRef& rhs)
        {
            lcore::swap(view_, rhs.view_);
        }

        ID3D11DepthStencilView* getView(){ return view_;}
        ID3D11DepthStencilView* const* get(){ return &view_;}
    private:
        ID3D11DepthStencilView* view_;
    };

    //--------------------------------------------------------
    //---
    //--- UnorderedAccessViewRef
    //---
    //--------------------------------------------------------
    class UnorderedAccessViewRef
    {
    public:
        UnorderedAccessViewRef()
            :view_(NULL)
        {}

        UnorderedAccessViewRef(const UnorderedAccessViewRef& rhs);

        explicit UnorderedAccessViewRef(ID3D11UnorderedAccessView* view)
            :view_(view)
        {}

        ~UnorderedAccessViewRef()
        {
            destroy();
        }

        UnorderedAccessViewRef& operator=(const UnorderedAccessViewRef& rhs)
        {
            UnorderedAccessViewRef tmp(rhs);
            tmp.swap(*this);
            return *this;
        }

        void destroy();

        bool valid() const{ return (NULL != view_);}

        void swap(UnorderedAccessViewRef& rhs)
        {
            lcore::swap(view_, rhs.view_);
        }

        ID3D11UnorderedAccessView* getView(){ return view_;}
    private:
        ID3D11UnorderedAccessView* view_;
    };

    class View
    {
    public:
        static UnorderedAccessViewRef createUAView(const UAVDesc& desc, ID3D11Resource* resource);
    };
}
#endif //INC_LGRAPHICS_DX11_VIEWREF_H__
