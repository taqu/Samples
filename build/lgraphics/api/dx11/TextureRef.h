#ifndef INC_LGRAPHICS_DX11_TEXTUREREF_H__
#define INC_LGRAPHICS_DX11_TEXTUREREF_H__
/**
@file TextureRef.h
@author t-sakai
@date 2012/07/24 create
*/
#include "../../lgraphics.h"
#include "Enumerations.h"
#include "SamplerStateRef.h"

struct ID3D11ShaderResourceView;
struct ID3D11DepthStencilView;
struct ID3D11Texture1D;
struct ID3D11Texture2D;
struct ID3D11Texture3D;

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

    //--------------------------------------------------------
    //---
    //--- TextureRefBase
    //---
    //--------------------------------------------------------
    template<class T>
    class TextureRefBase
    {
    public:
        typedef TextureRefBase<T> this_type;
        typedef T element_type;

        element_type* get() { return texture_;}
        bool hasSampler() const{ return sampler_.valid();}
        bool hasResourceView() const{ return shaderResourceView_.valid();}
        bool valid() const{ return (NULL != texture_);}

        ID3D11SamplerState* const* getSampler(){ return sampler_.get();}
        ID3D11ShaderResourceView* const* getView(){ return shaderResourceView_.get();}

        ShaderResourceViewRef createSRView(const SRVDesc& desc)
        {
            return createSRView(desc, texture_);
        }

        RenderTargetViewRef createRTView(const RTVDesc& desc)
        {
            return createRTView(desc, texture_);
        }

        DepthStencilViewRef createDSView(const DSVDesc& desc)
        {
            return createDSView(desc, texture_);
        }

        UnorderedAccessViewRef createUAView(const UAVDesc& desc)
        {
            return createUAView(desc, texture_);
        }

        void setSamplerState(TextureFilterType filter, TextureAddress adress)
        {
            sampler_ = SamplerState::create(filter, adress, adress, adress);
        }

        inline void attachVS(u32 viewIndex, u32 samplerIndex);
        inline void attachGS(u32 viewIndex, u32 samplerIndex);
        inline void attachPS(u32 viewIndex, u32 samplerIndex);

        inline void attachVS(u32 viewIndex);
        inline void attachGS(u32 viewIndex);
        inline void attachPS(u32 viewIndex);

        inline void copy(this_type& src);
        inline void updateSubresource(u32 index, const Box* box, const void* data, u32 rowPitch, u32 depthPitch);

        inline bool map(void*& data, u32& rowPitch, u32& depthPitch, s32 type);
        inline void unmap();

        inline bool operator==(const TextureRefBase& rhs) const
        {
            return (texture_ == rhs.texture_);
        }

        inline bool operator!=(const TextureRefBase& rhs) const
        {
            return (texture_ != rhs.texture_);
        }
    protected:

        TextureRefBase()
            :texture_(NULL)
        {}

        TextureRefBase(const this_type& rhs)
            :sampler_(rhs.sampler_)
            ,shaderResourceView_(rhs.shaderResourceView_)
            ,texture_(rhs.texture_)
        {
            if(texture_){
                texture_->AddRef();
            }
        }

        explicit TextureRefBase(const SamplerStateRef& sampler, ID3D11ShaderResourceView* view, element_type* texture)
            :sampler_(sampler)
            ,shaderResourceView_(view)
            ,texture_(texture)
        {}

        ~TextureRefBase()
        {
            destroy();
        }

        void destroy()
        {
            sampler_.destroy();
            shaderResourceView_.destroy();
            SAFE_RELEASE(texture_);
        }

        void swap(this_type& rhs)
        {
            sampler_.swap(rhs.sampler_);
            shaderResourceView_.swap(rhs.shaderResourceView_);
            lcore::swap(texture_, rhs.texture_);
        }

        ShaderResourceViewRef createSRView(const SRVDesc& desc, ID3D11Resource* resource);
        RenderTargetViewRef createRTView(const RTVDesc& desc, ID3D11Resource* resource);
        DepthStencilViewRef createDSView(const DSVDesc& desc, ID3D11Resource* resource);
        UnorderedAccessViewRef createUAView(const UAVDesc& desc, ID3D11Resource* resource);

        SamplerStateRef sampler_;
        ShaderResourceViewRef shaderResourceView_;
        element_type* texture_;
    };

    template<class T>
    ShaderResourceViewRef TextureRefBase<T>::createSRView(const SRVDesc& desc, ID3D11Resource* /*resource*/)
    {
        D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;

        if(SRVDesc::copy(viewDesc, desc)){
            ID3D11Device* device = Graphics::getDevice().getD3DDevice();

            ID3D11ShaderResourceView* view = NULL;
            HRESULT hr = device->CreateShaderResourceView(
                texture_,
                &viewDesc,
                &view);

            if(SUCCEEDED(hr)){
                return ShaderResourceViewRef(view);
            }
        }
        return ShaderResourceViewRef();
    }

    template<class T>
    RenderTargetViewRef TextureRefBase<T>::createRTView(const RTVDesc& desc, ID3D11Resource* resource)
    {
        D3D11_RENDER_TARGET_VIEW_DESC viewDesc;
        viewDesc.Format = static_cast<DXGI_FORMAT>(desc.format_);
        viewDesc.ViewDimension = static_cast<D3D11_RTV_DIMENSION>(desc.dimension_);
        switch(desc.dimension_)
        {
        case ViewRTVDimension_Texture1D:
            viewDesc.Texture1D.MipSlice = desc.tex1D_.mipSlice_;
            break;

        case ViewRTVDimension_Texture1DArray:
            viewDesc.Texture1DArray.MipSlice        = desc.tex1DArray_.mipSlice_;
            viewDesc.Texture1DArray.FirstArraySlice = desc.tex1DArray_.firstArraySlice_;
            viewDesc.Texture1DArray.ArraySize       = desc.tex1DArray_.arraySize_;
            break;

        case ViewRTVDimension_Texture2D:
            viewDesc.Texture2D.MipSlice = desc.tex2D_.mipSlice_;
            break;

        case ViewRTVDimension_Texture2DArray:
            viewDesc.Texture2DArray.MipSlice        = desc.tex2DArray_.mipSlice_;
            viewDesc.Texture2DArray.FirstArraySlice = desc.tex2DArray_.firstArraySlice_;
            viewDesc.Texture2DArray.ArraySize       = desc.tex2DArray_.arraySize_;
            break;

        case ViewRTVDimension_Texture3D:
            viewDesc.Texture3D.MipSlice    = desc.tex3D_.mipSlice_;
            viewDesc.Texture3D.FirstWSlice = desc.tex3D_.firstWSlice_;
            viewDesc.Texture3D.WSize       = desc.tex3D_.wSize_;
            break;
        };

        ID3D11Device* device = Graphics::getDevice().getD3DDevice();

        ID3D11RenderTargetView* view = NULL;
        HRESULT hr = device->CreateRenderTargetView(
            resource,
            &viewDesc,
            &view);

        return (SUCCEEDED(hr))? RenderTargetViewRef(view) : RenderTargetViewRef();
    }

    template<class T>
    DepthStencilViewRef TextureRefBase<T>::createDSView(const DSVDesc& desc, ID3D11Resource* resource)
    {
        D3D11_DEPTH_STENCIL_VIEW_DESC viewDesc;
        viewDesc.Format = static_cast<DXGI_FORMAT>(desc.format_);
        viewDesc.ViewDimension = static_cast<D3D11_DSV_DIMENSION>(desc.dimension_);
        viewDesc.Flags = 0;

        switch(desc.dimension_)
        {
        case ViewDSVDimension_Texture1D:
            viewDesc.Texture1D.MipSlice = desc.tex1D_.mipSlice_;
            break;

        case ViewDSVDimension_Texture1DArray:
            viewDesc.Texture1DArray.MipSlice        = desc.tex1DArray_.mipSlice_;
            viewDesc.Texture1DArray.FirstArraySlice = desc.tex1DArray_.firstArraySlice_;
            viewDesc.Texture1DArray.ArraySize       = desc.tex1DArray_.arraySize_;
            break;

        case ViewDSVDimension_Texture2D:
            viewDesc.Texture2D.MipSlice = desc.tex2D_.mipSlice_;
            break;

        case ViewDSVDimension_Texture2DArray:
            viewDesc.Texture2DArray.MipSlice        = desc.tex2DArray_.mipSlice_;
            viewDesc.Texture2DArray.FirstArraySlice = desc.tex2DArray_.firstArraySlice_;
            viewDesc.Texture2DArray.ArraySize       = desc.tex2DArray_.arraySize_;
            break;
        };

        ID3D11Device* device = Graphics::getDevice().getD3DDevice();

        ID3D11DepthStencilView* view = NULL;
        HRESULT hr = device->CreateDepthStencilView(
            resource,
            &viewDesc,
            &view);

        return (SUCCEEDED(hr))? DepthStencilViewRef(view) : DepthStencilViewRef();
    }

    template<class T>
    UnorderedAccessViewRef TextureRefBase<T>::createUAView(const UAVDesc& desc, ID3D11Resource* resource)
    {
        D3D11_UNORDERED_ACCESS_VIEW_DESC viewDesc;
        viewDesc.Format = static_cast<DXGI_FORMAT>(desc.format_);
        viewDesc.ViewDimension = static_cast<D3D11_UAV_DIMENSION>(desc.dimension_);

        switch(desc.dimension_)
        {
        case UAVDimension_Unknown:
            break;

        case UAVDimension_Buffer:
            viewDesc.Buffer.FirstElement = desc.buffer_.firstElement_;
            viewDesc.Buffer.NumElements = desc.buffer_.numElements_;
            viewDesc.Buffer.Flags = desc.buffer_.flags_;
            break;

        case UAVDimension_Texture1D:
            viewDesc.Texture1D.MipSlice = desc.tex1D_.mipSlice_;
            break;

        case UAVDimension_Texture1DArray:
            viewDesc.Texture1DArray.MipSlice        = desc.tex1DArray_.mipSlice_;
            viewDesc.Texture1DArray.FirstArraySlice = desc.tex1DArray_.firstArraySlice_;
            viewDesc.Texture1DArray.ArraySize       = desc.tex1DArray_.arraySize_;
            break;

        case UAVDimension_Texture2D:
            viewDesc.Texture2D.MipSlice = desc.tex2D_.mipSlice_;
            break;

        case UAVDimension_Texture2DArray:
            viewDesc.Texture2DArray.MipSlice        = desc.tex2DArray_.mipSlice_;
            viewDesc.Texture2DArray.FirstArraySlice = desc.tex2DArray_.firstArraySlice_;
            viewDesc.Texture2DArray.ArraySize       = desc.tex2DArray_.arraySize_;
            break;

        case UAVDimension_Texture3D:
            viewDesc.Texture3D.MipSlice        = desc.tex3D_.mipSlice_;
            viewDesc.Texture3D.FirstWSlice = desc.tex3D_.firstWSlice_;
            viewDesc.Texture3D.WSize       = desc.tex3D_.wsize_;
            break;
        };

        ID3D11Device* device = Graphics::getDevice().getD3DDevice();

        ID3D11UnorderedAccessView* view = NULL;
        HRESULT hr = device->CreateUnorderedAccessView(
            resource,
            &viewDesc,
            &view);

        return (SUCCEEDED(hr))? UnorderedAccessViewRef(view) : UnorderedAccessViewRef();
    }


    template<class T>
    inline void TextureRefBase<T>::attachVS(u32 viewIndex, u32 samplerIndex)
    {
        lgraphics::GraphicsDeviceRef& device = Graphics::getDevice();
        ID3D11ShaderResourceView* view = shaderResourceView_.getView();
        device.setVSResources(viewIndex, 1, &view);
        device.setVSSamplers(samplerIndex, 1, sampler_.get());
    }

    template<class T>
    inline void TextureRefBase<T>::attachGS(u32 viewIndex, u32 samplerIndex)
    {
        lgraphics::GraphicsDeviceRef& device = Graphics::getDevice();
        ID3D11ShaderResourceView* view = shaderResourceView_.getView();
        device.setGSResources(viewIndex, 1, &view);
        device.setGSSamplers(samplerIndex, 1, sampler_.get());
    }

    template<class T>
    inline void TextureRefBase<T>::attachPS(u32 viewIndex, u32 samplerIndex)
    {
        lgraphics::GraphicsDeviceRef& device = Graphics::getDevice();
        ID3D11ShaderResourceView* view = shaderResourceView_.getView();
        device.setPSResources(viewIndex, 1, &view);
        device.setPSSamplers(samplerIndex, 1, sampler_.get());
    }


    template<class T>
    inline void TextureRefBase<T>::attachVS(u32 viewIndex)
    {
        lgraphics::GraphicsDeviceRef& device = Graphics::getDevice();
        ID3D11ShaderResourceView* view = shaderResourceView_.getView();
        device.setVSResources(viewIndex, 1, &view);
    }

    template<class T>
    inline void TextureRefBase<T>::attachGS(u32 viewIndex)
    {
        lgraphics::GraphicsDeviceRef& device = Graphics::getDevice();
        ID3D11ShaderResourceView* view = shaderResourceView_.getView();
        device.setGSResources(viewIndex, 1, &view);
    }

    template<class T>
    inline void TextureRefBase<T>::attachPS(u32 viewIndex)
    {
        lgraphics::GraphicsDeviceRef& device = Graphics::getDevice();
        ID3D11ShaderResourceView* view = shaderResourceView_.getView();
        device.setPSResources(viewIndex, 1, &view);
    }

    template<class T>
    inline void TextureRefBase<T>::copy(this_type& src)
    {
        lgraphics::GraphicsDeviceRef& device = Graphics::getDevice();
        device.copyResource(texture_, src.texture_);
    }

    template<class T>
    inline void TextureRefBase<T>::updateSubresource(u32 index, const Box* box, const void* data, u32 rowPitch, u32 depthPitch)
    {
        lgraphics::GraphicsDeviceRef& device = Graphics::getDevice();
        device.updateSubresource(texture_, index, box, data, rowPitch, depthPitch);
    }

    template<class T>
    inline bool TextureRefBase<T>::map(void*& data, u32& rowPitch, u32& depthPitch, s32 type)
    {
        lgraphics::GraphicsDeviceRef& device = Graphics::getDevice();
        return device.map(data, rowPitch, depthPitch, texture_, type);
    }

    template<class T>
    inline void TextureRefBase<T>::unmap()
    {
        lgraphics::GraphicsDeviceRef& device = Graphics::getDevice();
        device.unmap(texture_);
    }

    //--------------------------------------------------------
    //---
    //--- Texture1DRef
    //---
    //--------------------------------------------------------
    class Texture1DRef : public TextureRefBase<ID3D11Texture1D>
    {
    public:
        typedef TextureRefBase<ID3D11Texture1D> parent_type;

        Texture1DRef()
        {}

        Texture1DRef(const Texture1DRef& rhs);

        ~Texture1DRef()
        {
            destroy();
        }

        void destroy();

        Texture1DRef& operator=(const Texture1DRef& rhs)
        {
            Texture1DRef tmp(rhs);
            tmp.swap(*this);
            return *this;
        }

        void swap(Texture1DRef& rhs)
        {
            parent_type::swap(rhs);
        }
    private:
        friend class Texture;

        explicit Texture1DRef(const SamplerStateRef& sampler, ID3D11ShaderResourceView* view, ID3D11Texture1D* texture)
            :parent_type(sampler, view, texture)
        {}
    };

    //--------------------------------------------------------
    //---
    //--- Texture2DRef
    //---
    //--------------------------------------------------------
    class Texture2DRef : public TextureRefBase<ID3D11Texture2D>
    {
    public:
        typedef TextureRefBase<ID3D11Texture2D> parent_type;

        Texture2DRef()
        {}

        Texture2DRef(const Texture2DRef& rhs);

        ~Texture2DRef()
        {
            destroy();
        }

        void destroy();

        Texture2DRef& operator=(const Texture2DRef& rhs)
        {
            Texture2DRef tmp(rhs);
            tmp.swap(*this);
            return *this;
        }

        void swap(Texture2DRef& rhs)
        {
            parent_type::swap(rhs);
        }
    private:
        friend class Texture;

        explicit Texture2DRef(const SamplerStateRef& sampler, ID3D11ShaderResourceView* view, ID3D11Texture2D* texture)
            :parent_type(sampler, view, texture)
        {}
    };


    //--------------------------------------------------------
    //---
    //--- Texture3DRef
    //---
    //--------------------------------------------------------
    class Texture3DRef : public TextureRefBase<ID3D11Texture3D>
    {
    public:
        typedef TextureRefBase<ID3D11Texture3D> parent_type;

        Texture3DRef()
        {}

        Texture3DRef(const Texture3DRef& rhs);

        ~Texture3DRef()
        {
            destroy();
        }

        void destroy();

        Texture3DRef& operator=(const Texture3DRef& rhs)
        {
            Texture3DRef tmp(rhs);
            tmp.swap(*this);
            return *this;
        }

        void swap(Texture3DRef& rhs)
        {
            parent_type::swap(rhs);
        }
    private:
        friend class Texture;

        explicit Texture3DRef(const SamplerStateRef& sampler, ID3D11ShaderResourceView* view, ID3D11Texture3D* texture)
            :parent_type(sampler, view, texture)
        {}
    };


    //--------------------------------------------------------
    //---
    //--- BufferRef
    //---
    //--------------------------------------------------------
    class BufferRef : public TextureRefBase<ID3D11Buffer>
    {
    public:
        typedef TextureRefBase<ID3D11Buffer> parent_type;

        BufferRef()
        {}

        BufferRef(const BufferRef& rhs);

        ~BufferRef()
        {
            destroy();
        }

        void destroy();

        BufferRef& operator=(const BufferRef& rhs)
        {
            BufferRef tmp(rhs);
            tmp.swap(*this);
            return *this;
        }

        void swap(BufferRef& rhs)
        {
            parent_type::swap(rhs);
        }
    private:
        friend class Texture;

        explicit BufferRef(const SamplerStateRef& sampler, ID3D11ShaderResourceView* view, ID3D11Buffer* texture)
            :parent_type(sampler, view, texture)
        {}
    };


    //--------------------------------------------------------
    //---
    //--- Texture
    //---
    //--------------------------------------------------------
    class Texture
    {
    public:
        static Texture1DRef create1D(
            u32 width,
            u32 mipLevels,
            u32 arraySize,
            DataFormat format,
            Usage usage,
            u32 bind,
            CPUAccessFlag access,
            ResourceMisc misc,
            TextureFilterType filter,
            TextureAddress adress,
            CmpFunc compFunc,
            f32 borderColor,
            const SubResourceData* initData,
            const SRVDesc* resourceViewDesc);

        static Texture2DRef create2D(
            u32 width,
            u32 height,
            u32 mipLevels,
            u32 arraySize,
            DataFormat format,
            Usage usage,
            u32 bind,
            CPUAccessFlag access,
            ResourceMisc misc,
            TextureFilterType filter,
            TextureAddress adress,
            CmpFunc compFunc,
            f32 borderColor,
            const SubResourceData* initData,
            const SRVDesc* resourceViewDesc);

        static Texture2DRef create2D(
            u32 width,
            u32 height,
            u32 mipLevels,
            u32 arraySize,
            DataFormat format,
            u32 samleCount, //MSAAサンプル数
            u32 sampleQuality, //MSAA品質
            Usage usage,
            u32 bind,
            CPUAccessFlag access,
            ResourceMisc misc,
            TextureFilterType filter,
            TextureAddress adress,
            CmpFunc compFunc,
            f32 borderColor,
            const SubResourceData* initData,
            const SRVDesc* resourceViewDesc);

        static Texture3DRef create3D(
            u32 width,
            u32 height,
            u32 depth,
            u32 mipLevels,
            DataFormat format,
            Usage usage,
            u32 bind,
            CPUAccessFlag access,
            ResourceMisc misc,
            TextureFilterType filter,
            TextureAddress adress,
            CmpFunc compFunc,
            f32 borderColor,
            const SubResourceData* initData,
            const SRVDesc* resourceViewDesc);
            
        static BufferRef createBuffer(
            u32 size,
            Usage usage,
            u32 bind,
            CPUAccessFlag access,
            ResourceMisc misc,
            u32 structureByteStride,
            const SubResourceData* initData,
            const SRVDesc* resourceViewDesc);
    };
}
#endif //INC_LGRAPHICS_DX11_TEXTUREREF_H__
