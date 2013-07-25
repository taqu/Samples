#ifndef INC_LGRAPHICS_DX11_GRAPHICSDEVICEREF_H__
#define INC_LGRAPHICS_DX11_GRAPHICSDEVICEREF_H__
/**
@file GraphicsDevice.h
@author t-sakai
@date 2010/03/23 create
*/
#include "../../lgraphicscore.h"
#include "../../lgraphicsAPIInclude.h"
#include "Enumerations.h"

struct IDXGISwapChain;

namespace lgraphics
{
    struct InitParam;
    class BlendStateRef;

    //Bufferの場合はバイト単位、テクスチャの場合はテクセル単位
    struct Box
    {
        Box(){}
        Box(u32 left, u32 top, u32 front, u32 right, u32 bottom, u32 back)
            :left_(left)
            ,top_(top)
            ,front_(front)
            ,right_(right)
            ,bottom_(bottom)
            ,back_(back)
        {}

        u32 left_;
        u32 top_;
        u32 front_;
        u32 right_;
        u32 bottom_;
        u32 back_;
    };

    //--------------------------------------
    //---
    //--- DepthStencilStateRef
    //---
    //--------------------------------------
    class DepthStencilStateRef
    {
    public:
        struct StencilOPDesc
        {
            StencilOp failOp_;
            StencilOp depthFailOp_;
            StencilOp passOp_;
            CmpFunc cmpFunc_;
        };

        DepthStencilStateRef();
        DepthStencilStateRef(const DepthStencilStateRef& rhs);
        ~DepthStencilStateRef();

        void destroy();

        bool valid() const;
        DepthStencilStateRef& operator=(const DepthStencilStateRef& rhs);
        void swap(DepthStencilStateRef& rhs);
    private:
        friend class GraphicsDeviceRef;

        explicit DepthStencilStateRef(ID3D11DepthStencilState* state);

        ID3D11DepthStencilState* state_;
    };


    //--------------------------------------
    //---
    //--- GraphicsDeviceRef
    //---
    //--------------------------------------
    /**
    @brief Direct3D11 グラフィクスデバイス
    */
    class GraphicsDeviceRef
    {
    public:
        enum ClearDepthStencilFlag
        {
            ClearDepth = D3D11_CLEAR_DEPTH,
            ClearStencil = D3D11_CLEAR_STENCIL,
        };

        enum RasterizerState
        {
            Rasterizer_FillSolid = 0,
            Rasterizer_FillSolidNoCull,
            Rasterizer_FillWireFrame,
            Rasterizer_FillWireFrameNoCull,
            Rasterizer_DepthMap,
            Rasterizer_Num,
        };

        enum DepthStencilState
        {
            DepthStencil_DEnableWEnable =0,
            DepthStencil_DEnableWDisable,
            DepthStencil_DDisableWEnable,
            DepthStencil_DDisableWDisable,
            DepthStencil_Num,
        };

        enum BlendState
        {
            BlendState_NoAlpha =0,
            BlendState_Alpha,
            BlendState_AlphaAdditive,
            BlendState_Num,
        };

        enum MapType
        {
            MapType_Read = D3D11_MAP_READ,
            MapType_Write = D3D11_MAP_WRITE,
            MapType_ReadWrite = D3D11_MAP_READ_WRITE,
            MapType_WriteDiscard = D3D11_MAP_WRITE_DISCARD,
            MapType_WriteNoOverwrite = D3D11_MAP_WRITE_NO_OVERWRITE,
        };

        static const Char* GSModel;
        static const Char* VSModel;
        static const Char* PSModel;

        static const u32 MinRefreshRate = 15;
        static const u32 MaxRefreshRate = 120;

        u32 getRefreshRate() const{ return refreshRate_;}
        u32 getFeatureLevel() const{ return featureLevel_;}
        ID3D11Device* getD3DDevice(){ return device_;}
        ID3D11DeviceContext* getContext(){ return context_;}


        void onSize(u32 width, u32 height);

        void getRenderTargetDesc(u32& width, u32& height);

        bool isFullScreen();
        void changeScreenMode();

        inline void present();

        inline void setClearColor(const f32* color);
        inline const f32* getClearColor() const;

        inline void setClearDepthStencil(u32 flag, f32 depth, u8 stencil);

        inline void clearRenderTargetView();
        inline void clearDepthStencilView();

        inline void clearRenderTargetView(ID3D11RenderTargetView* view, const f32* color);
        inline void clearDepthStencilView(ID3D11DepthStencilView* view, u32 flags, f32 depth, u8 stencil);
        inline void clearUnorderedAccessView(ID3D11UnorderedAccessView* view, const u32 values[4]);

        inline void draw(u32 numVertices, u32 start);
        inline void drawIndexed(u32 numIndices, u32 start, u32 baseVertex);

        inline void drawInstanced(u32 numVertices, u32 numInstances, u32 startVertex, u32 startInstance);

        void setViewport(s32 x, s32 y, u32 width, u32 height);

        inline void setRasterizerState(RasterizerState state);

        void setBlendState(BlendStateRef& state);
        inline void setBlendState(BlendState state);
        inline void setDepthStencilState(DepthStencilState state);

        inline void setPrimitiveTopology(Primitive topology);
        inline void setInputLayout(ID3D11InputLayout* layout);
        inline void setVertexBuffers(u32 start, u32 num, ID3D11Buffer* const* buffers, const u32* strides, const u32* offsets);
        inline void setIndexBuffer(ID3D11Buffer* buffer, DataFormat format, u32 offset);

        inline void setVSConstantBuffers(u32 start, u32 num, ID3D11Buffer* const* buffers);
        inline void setGSConstantBuffers(u32 start, u32 num, ID3D11Buffer* const* buffers);
        inline void setPSConstantBuffers(u32 start, u32 num, ID3D11Buffer* const* buffers);

        inline void setVSResources(u32 start, u32 num, ID3D11ShaderResourceView* const* views);
        inline void setGSResources(u32 start, u32 num, ID3D11ShaderResourceView* const* views);
        inline void setPSResources(u32 start, u32 num, ID3D11ShaderResourceView* const* views);

        inline void setVSSamplers(u32 start, u32 num, ID3D11SamplerState* const* states);
        inline void setGSSamplers(u32 start, u32 num, ID3D11SamplerState* const* states);
        inline void setPSSamplers(u32 start, u32 num, ID3D11SamplerState* const* states);

        inline void setGeometryShader(ID3D11GeometryShader* shader);
        inline void setVertexShader(ID3D11VertexShader* shader);
        inline void setPixelShader(ID3D11PixelShader* shader);

        inline void setRenderTargets(
            u32 numViews,
            ID3D11RenderTargetView* const* views,
            ID3D11DepthStencilView* depthStencilView);

        inline void getRenderTargets(
            u32 numViews,
            ID3D11RenderTargetView** views,
            ID3D11DepthStencilView** depthStencilView);

        inline void setRenderTargetsAndUAV(
            u32 numViews,
            ID3D11RenderTargetView* const* views,
            ID3D11DepthStencilView* depthStencilView,
            u32 UAVStart,
            u32 numUAVs,
            ID3D11UnorderedAccessView* const* uavs,
            const u32* UAVInitCounts);

        inline void updateSubresource(
            ID3D11Resource* dstResource,
            u32 dstSubresource,
            const Box* dstBox,
            const void* srcData,
            u32 srcRowPitch,
            u32 srcDepthPitch);

        inline void clearVSResources(u32 numResources);
        inline void clearGSResources(u32 numResources);
        inline void clearPSResources(u32 numResources);
        inline void clearRenderTargets(u32 numResources);

        void restoreDefaultRenderTargets();

        inline void copyResource(ID3D11Resource* dst, u32 dstSubResource, u32 dstX, u32 dstY, u32 dstZ, ID3D11Resource* src, u32 srcSubResource, const Box* box);
        inline void copyResource(ID3D11Resource* dst, ID3D11Resource* src);
        inline void copyStructureCount(ID3D11Buffer* dst, u32 dstOffset, ID3D11UnorderedAccessView* view);

        inline bool map(void*& data, u32& rowPitch, u32& depthPitch, ID3D11Resource* resource, s32 type);
        inline void unmap(ID3D11Resource* resource);

        DepthStencilStateRef createDepthStencilState(
            bool depthEnable,
            DepthWriteMask depthWriteMask,
            CmpFunc depthFunc,
            bool stencilEnable,
            u8 stencilReadMask,
            u8 stencilWriteMask,
            const DepthStencilStateRef::StencilOPDesc& frontFace,
            const DepthStencilStateRef::StencilOPDesc& backFace);

        void setDepthStencilState(DepthStencilStateRef& state, u32 stencilRef);

    private:
        static const u32 MaxShaderResources = 32;
        static ID3D11ShaderResourceView* const NULLResources[MaxShaderResources];

        static ID3D11RenderTargetView* const NullTargets[MaxRenderTargets];

        friend class Graphics;

        GraphicsDeviceRef();
        ~GraphicsDeviceRef();

        bool initialize(const InitParam& initParam);
        void terminate();
        bool createBackBuffer(u32 width, u32 height, u32 depthStencilFormat);

        f32 clearColor_[4];
        u32 clearDepthStencilFlag_;
        f32 clearDepth_;
        u8 clearStencil_;
        u32 syncInterval_;
        u32 refreshRate_;
        u32 featureLevel_;

        ID3D11RasterizerState* rasterizerStates_[Rasterizer_Num];

        f32 blendFactors_[MaxRenderTargets];
        ID3D11DepthStencilState* depthStencilStates_[DepthStencil_Num];
        ID3D11BlendState* blendStates_[BlendState_Num];

        ID3D11Device *device_; /// デバイス実態
        ID3D11DeviceContext *context_; /// デバイスコンテキスト
        IDXGISwapChain *swapChain_; /// スワップチェイン
        ID3D11Texture2D* backBuffer_;
        ID3D11RenderTargetView* renderTargetView_;
        ID3D11Texture2D* depthStencil_;
        ID3D11DepthStencilView* depthStencilView_;
    };

    inline void GraphicsDeviceRef::present()
    {
        swapChain_->Present(syncInterval_, 0);
    }

    inline void GraphicsDeviceRef::setClearColor(const f32* color)
    {
        clearColor_[0] = color[0];
        clearColor_[1] = color[1];
        clearColor_[2] = color[2];
        clearColor_[3] = color[3];
    }

    inline const f32* GraphicsDeviceRef::getClearColor() const
    {
        return clearColor_;
    }

    inline void GraphicsDeviceRef::setClearDepthStencil(u32 flag, f32 depth, u8 stencil)
    {
        clearDepthStencilFlag_ = flag;
        clearDepth_ = depth;
        clearStencil_ = stencil;
    }

    inline void GraphicsDeviceRef::clearRenderTargetView()
    {
        context_->ClearRenderTargetView(renderTargetView_, clearColor_);
    }

    inline void GraphicsDeviceRef::clearDepthStencilView()
    {
        context_->ClearDepthStencilView(depthStencilView_, clearDepthStencilFlag_, clearDepth_, clearStencil_);
    }

    inline void GraphicsDeviceRef::clearRenderTargetView(ID3D11RenderTargetView* view, const f32* color)
    {
        context_->ClearRenderTargetView(view, color);
    }

    inline void GraphicsDeviceRef::clearDepthStencilView(ID3D11DepthStencilView* view, u32 flags, f32 depth, u8 stencil)
    {
        context_->ClearDepthStencilView(view, flags, depth, stencil);
    }

    inline void GraphicsDeviceRef::clearUnorderedAccessView(ID3D11UnorderedAccessView* view, const u32 values[4])
    {
        context_->ClearUnorderedAccessViewUint(view, values);
    }

    inline void GraphicsDeviceRef::draw(u32 numVertices, u32 start)
    {
        context_->Draw(numVertices, start);
    }

    inline void GraphicsDeviceRef::drawIndexed(u32 numIndices, u32 start, u32 baseVertex)
    {
        context_->DrawIndexed(numIndices, start, baseVertex);
    }

    inline void GraphicsDeviceRef::drawInstanced(u32 numVertices, u32 numInstances, u32 startVertex, u32 startInstance)
    {
        context_->DrawInstanced(numVertices, numInstances, startVertex, startInstance);
    }

    inline void GraphicsDeviceRef::setRasterizerState(RasterizerState state)
    {
        context_->RSSetState(rasterizerStates_[state]);
    }

    inline void GraphicsDeviceRef::setBlendState(BlendState state)
    {
        context_->OMSetBlendState(blendStates_[state], blendFactors_, 0xFFFFFFFFU);
    }

    inline void GraphicsDeviceRef::setDepthStencilState(DepthStencilState state)
    {
        context_->OMSetDepthStencilState(depthStencilStates_[state], 0);
    }

    inline void GraphicsDeviceRef::setPrimitiveTopology(Primitive topology)
    {
        context_->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(topology));
    }

    inline void GraphicsDeviceRef::setInputLayout(ID3D11InputLayout* layout)
    {
        context_->IASetInputLayout(layout);
    }

    inline void GraphicsDeviceRef::setVertexBuffers(u32 start, u32 num, ID3D11Buffer* const* buffers, const u32* strides, const u32* offsets)
    {
        context_->IASetVertexBuffers(start, num, buffers, strides, offsets);
    }

    inline void GraphicsDeviceRef::setIndexBuffer(ID3D11Buffer* buffer, DataFormat format, u32 offset)
    {
        context_->IASetIndexBuffer(buffer, static_cast<DXGI_FORMAT>(format), offset);
    }

    inline void GraphicsDeviceRef::setVSConstantBuffers(u32 start, u32 num, ID3D11Buffer* const* buffers)
    {
        context_->VSSetConstantBuffers(start, num, buffers);
    }

    inline void GraphicsDeviceRef::setGSConstantBuffers(u32 start, u32 num, ID3D11Buffer* const* buffers)
    {
        context_->GSSetConstantBuffers(start, num, buffers);
    }

    inline void GraphicsDeviceRef::setPSConstantBuffers(u32 start, u32 num, ID3D11Buffer* const* buffers)
    {
        context_->PSSetConstantBuffers(start, num, buffers);
    }

    inline void GraphicsDeviceRef::setVSResources(u32 start, u32 num, ID3D11ShaderResourceView* const* views)
    {
        context_->VSSetShaderResources(start, num, views);
    }

    inline void GraphicsDeviceRef::setGSResources(u32 start, u32 num, ID3D11ShaderResourceView* const* views)
    {
        context_->GSSetShaderResources(start, num, views);
    }

    inline void GraphicsDeviceRef::setPSResources(u32 start, u32 num, ID3D11ShaderResourceView* const* views)
    {
        context_->PSSetShaderResources(start, num, views);
    }

    inline void GraphicsDeviceRef::setVSSamplers(u32 start, u32 num, ID3D11SamplerState* const* states)
    {
        context_->VSSetSamplers(start, num, states);
    }

    inline void GraphicsDeviceRef::setGSSamplers(u32 start, u32 num, ID3D11SamplerState* const* states)
    {
        context_->GSSetSamplers(start, num, states);
    }

    inline void GraphicsDeviceRef::setPSSamplers(u32 start, u32 num, ID3D11SamplerState* const* states)
    {
        context_->PSSetSamplers(start, num, states);
    }

    inline void GraphicsDeviceRef::setGeometryShader(ID3D11GeometryShader* shader)
    {
        context_->GSSetShader(shader, NULL, 0);
    }

    inline void GraphicsDeviceRef::setVertexShader(ID3D11VertexShader* shader)
    {
        context_->VSSetShader(shader, NULL, 0);
    }

    inline void GraphicsDeviceRef::setPixelShader(ID3D11PixelShader* shader)
    {
        context_->PSSetShader(shader, NULL, 0);
    }

    inline void GraphicsDeviceRef::setRenderTargets(
        u32 numViews,
        ID3D11RenderTargetView* const* views,
        ID3D11DepthStencilView* depthStencilView)
    {
        context_->OMSetRenderTargets(numViews, views, depthStencilView);
    }

    inline void GraphicsDeviceRef::getRenderTargets(
        u32 numViews,
        ID3D11RenderTargetView** views,
        ID3D11DepthStencilView** depthStencilView)
    {
        context_->OMGetRenderTargets(numViews, views, depthStencilView);
    }

    inline void GraphicsDeviceRef::setRenderTargetsAndUAV(
        u32 numViews,
        ID3D11RenderTargetView* const* views,
        ID3D11DepthStencilView* depthStencilView,
        u32 UAVStart,
        u32 numUAVs,
        ID3D11UnorderedAccessView* const* uavs,
        const u32* UAVInitCounts)
    {
        context_->OMSetRenderTargetsAndUnorderedAccessViews(numViews, views, depthStencilView, UAVStart, numUAVs, uavs, UAVInitCounts);
    }

    inline void GraphicsDeviceRef::updateSubresource(
        ID3D11Resource* dstResource,
        u32 dstSubresource,
        const Box* dstBox,
        const void* srcData,
        u32 srcRowPitch,
        u32 srcDepthPitch)
    {
        context_->UpdateSubresource(
            dstResource,
            dstSubresource,
            reinterpret_cast<const D3D11_BOX*>(dstBox),
            srcData,
            srcRowPitch,
            srcDepthPitch);
    }

    inline void GraphicsDeviceRef::clearVSResources(u32 numResources)
    {
        LASSERT(numResources<MaxShaderResources);
        context_->VSSetShaderResources(0, numResources, NULLResources);
    }

    inline void GraphicsDeviceRef::clearGSResources(u32 numResources)
    {
        LASSERT(numResources<MaxShaderResources);
        context_->GSSetShaderResources(0, numResources, NULLResources);
    }

    inline void GraphicsDeviceRef::clearPSResources(u32 numResources)
    {
        LASSERT(numResources<MaxShaderResources);
        context_->PSSetShaderResources(0, numResources, NULLResources);
    }

    inline void GraphicsDeviceRef::clearRenderTargets(u32 numTargets)
    {
        LASSERT(numTargets<MaxRenderTargets);
        context_->OMSetRenderTargets(numTargets, NullTargets, NULL);
    }

    inline void GraphicsDeviceRef::copyResource(ID3D11Resource* dst, u32 dstSubResource, u32 dstX, u32 dstY, u32 dstZ, ID3D11Resource* src, u32 srcSubResource, const Box* box)
    {
        context_->CopySubresourceRegion(dst, dstSubResource, dstX, dstY, dstZ, src, srcSubResource, reinterpret_cast<const D3D11_BOX*>(box));
    }

    inline void GraphicsDeviceRef::copyResource(ID3D11Resource* dst, ID3D11Resource* src)
    {
        context_->CopyResource(dst, src);
    }

    inline void GraphicsDeviceRef::copyStructureCount(ID3D11Buffer* dst, u32 dstOffset, ID3D11UnorderedAccessView* view)
    {
        context_->CopyStructureCount(dst, dstOffset, view);
    }

    inline bool GraphicsDeviceRef::map(void*& data, u32& rowPitch, u32& depthPitch, ID3D11Resource* resource, s32 type)
    {
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        HRESULT hr = context_->Map(resource, 0, (D3D11_MAP)type, 0, &mappedResource);
        data = mappedResource.pData;
        rowPitch = mappedResource.RowPitch;
        depthPitch = mappedResource.DepthPitch;
        return SUCCEEDED(hr);
    }

    inline void GraphicsDeviceRef::unmap(ID3D11Resource* resource)
    {
        context_->Unmap(resource, 0);
    }
}
#endif //INC_LGRAPHICS_DX11_GRAPHICSDEVICEREF_H__
