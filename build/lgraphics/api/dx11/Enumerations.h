#ifndef INC_LGRAPHICS_DX11_ENUMERATIONS_H__
#define INC_LGRAPHICS_DX11_ENUMERATIONS_H__
/**
@file Enumerations.h
@author t-sakai
@date 2010/02/14 create
*/
#include "../../lgraphicsAPIInclude.h"

namespace lgraphics
{
    enum PresentInterval
    {
        PresentInterval_Default = 1,
        PresentInterval_One = 1,
        PresentInterval_Two = 2,
        PresentInterval_Three = 3,
        PresentInterval_Four = 4,
        PresentInterval_Immediate = 0,
    };

    enum DXGISwapEffect
    {
        DXGISwapEffect_Discard = DXGI_SWAP_EFFECT_DISCARD,
        DXGISwapEffect_Sequencial = DXGI_SWAP_EFFECT_SEQUENTIAL,
    };

    enum DriverType
    {
        DriverType_Unknown = D3D_DRIVER_TYPE_UNKNOWN,
        DriverType_Hardware = D3D_DRIVER_TYPE_HARDWARE,
        DriverType_Reference = D3D_DRIVER_TYPE_REFERENCE,
        DriverType_Null = D3D_DRIVER_TYPE_NULL,
        DriverType_Software = D3D_DRIVER_TYPE_SOFTWARE,
        DriverType_Warp = D3D_DRIVER_TYPE_WARP,
    };

    enum FutureLevel
    {
        FutureLevel_9_1 = D3D_FEATURE_LEVEL_9_1,
        FutureLevel_9_2 = D3D_FEATURE_LEVEL_9_2,
        FutureLevel_9_3 = D3D_FEATURE_LEVEL_9_3,
        FutureLevel_10_0 = D3D_FEATURE_LEVEL_10_0,
        FutureLevel_10_1 = D3D_FEATURE_LEVEL_10_1,
        FutureLevel_11_0 = D3D_FEATURE_LEVEL_11_0,
    };

    //--------------------------------------------------------
    enum ClearFlag
    {
        ClearFlag_Depth = D3D11_CLEAR_DEPTH,
        ClearFlag_Stencil = D3D11_CLEAR_STENCIL,
    };

    //--------------------------------------------------------
    enum Usage
    {
        Usage_Default = D3D11_USAGE_DEFAULT,
        Usage_Immutable = D3D11_USAGE_IMMUTABLE,
        Usage_Dynamic = D3D11_USAGE_DYNAMIC,
        Usage_Staging = D3D11_USAGE_STAGING,
    };

    //--------------------------------------------------------
    enum BindFlag
    {
        BindFlag_VertexBuffer = D3D11_BIND_VERTEX_BUFFER,
        BindFlag_IndexBuffer = D3D11_BIND_INDEX_BUFFER,
        BindFlag_ConstantBuffer = D3D11_BIND_CONSTANT_BUFFER,
        BindFlag_ShaderResource = D3D11_BIND_SHADER_RESOURCE,
        BindFlag_StreamOutput = D3D11_BIND_STREAM_OUTPUT,
        BindFlag_RenderTarget = D3D11_BIND_RENDER_TARGET,
        BindFlag_DepthStencil = D3D11_BIND_DEPTH_STENCIL,
        BindFlag_UnorderedAccess = D3D11_BIND_UNORDERED_ACCESS
    };

    //--------------------------------------------------------
    enum CPUAccessFlag
    {
        CPUAccessFlag_None = 0,
        CPUAccessFlag_Write = D3D11_CPU_ACCESS_WRITE,
        CPUAccessFlag_Read = D3D11_CPU_ACCESS_READ,
    };

    //--------------------------------------------------------
    enum ResourceMisc
    {
        ResourceMisc_None = 0,
        ResourceMisc_GenerateMips = D3D11_RESOURCE_MISC_GENERATE_MIPS,
        ResourceMisc_Shared = D3D11_RESOURCE_MISC_SHARED,
        ResourceMisc_TextureCube = D3D11_RESOURCE_MISC_TEXTURECUBE,
        ResourceMisc_DrawDirectArgs = D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS,
        ResourceMisc_BufferAllowRawView = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS,
        ResourceMisc_BufferStructured = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
        ResourceMisc_ResourceClamp = D3D11_RESOURCE_MISC_RESOURCE_CLAMP,
        ResourceMisc_SharedKeyedMutex = D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX,
        ResourceMisc_GDICompatible = D3D11_RESOURCE_MISC_GDI_COMPATIBLE,
        //ResourceMisc_SharedNTHandle = D3D11_RESOURCE_MISC_SHARED_NTHANDLE,
        //ResourceMisc_RestrictedContent = D3D11_RESOURCE_MISC_RESTRICTED_CONTENT,
        //ResourceMisc_RestrictSharedResource = D3D11_RESOURCE_MISC_RESTRICT_SHARED_RESOURCE,
        //ResourceMisc_RestrictSharedResourceDriver = D3D11_RESOURCE_MISC_RESTRICT_SHARED_RESOURCE_DRIVER,
        //ResourceMisc_Guarded = D3D11_RESOURCE_MISC_GUARDED,
    };

    //--------------------------------------------------------
    enum MapType
    {
        MapType_Read = D3D11_MAP_READ,
        MapType_Write = D3D11_MAP_WRITE,
        MapType_ReadWrite = D3D11_MAP_READ_WRITE,
        MapType_WriteDiscard = D3D11_MAP_WRITE_DISCARD,
        MapType_WriteNoOverwrite = D3D11_MAP_WRITE_NO_OVERWRITE,
    };

    //--------------------------------------------------------
    enum ViewSRVDimension
    {
        ViewSRVDimension_Buffer = D3D11_SRV_DIMENSION_BUFFER,
        ViewSRVDimension_Texture1D = D3D11_SRV_DIMENSION_TEXTURE1D,
        ViewSRVDimension_Texture1DArray = D3D11_SRV_DIMENSION_TEXTURE1DARRAY,
        ViewSRVDimension_Texture2D = D3D11_SRV_DIMENSION_TEXTURE2D,
        ViewSRVDimension_Texture2DArray = D3D11_SRV_DIMENSION_TEXTURE2DARRAY,
        //ViewSRVDimension_Texture2DMS = D3D11_SRV_DIMENSION_TEXTURE2DMS,
        //ViewSRVDimension_Texture2DMSArray = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY,
        ViewSRVDimension_Texture3D = D3D11_SRV_DIMENSION_TEXTURE3D,
        ViewSRVDimension_Cube = D3D11_SRV_DIMENSION_TEXTURECUBE,
        ViewSRVDimension_CubeArray = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY,
    };

    //--------------------------------------------------------
    enum ViewRTVDimension
    {
        //ViewRTVDimension_Buffer = D3D11_RTV_DIMENSION_BUFFER,
        ViewRTVDimension_Texture1D = D3D11_RTV_DIMENSION_TEXTURE1D,
        ViewRTVDimension_Texture1DArray = D3D11_RTV_DIMENSION_TEXTURE1DARRAY,
        ViewRTVDimension_Texture2D = D3D11_RTV_DIMENSION_TEXTURE2D,
        ViewRTVDimension_Texture2DArray = D3D11_RTV_DIMENSION_TEXTURE2DARRAY,
        //ViewRTVDimension_Texture2DMS = D3D11_RTV_DIMENSION_TEXTURE2DMS,
        //ViewRTVDimension_Texture2DMSArray = D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY,
        ViewRTVDimension_Texture3D = D3D11_RTV_DIMENSION_TEXTURE3D,
    };

    //--------------------------------------------------------
    enum ViewDSVDimension
    {
        ViewDSVDimension_Texture1D = D3D11_DSV_DIMENSION_TEXTURE1D,
        ViewDSVDimension_Texture1DArray = D3D11_DSV_DIMENSION_TEXTURE1DARRAY,
        ViewDSVDimension_Texture2D = D3D11_DSV_DIMENSION_TEXTURE2D,
        ViewDSVDimension_Texture2DArray = D3D11_DSV_DIMENSION_TEXTURE2DARRAY,
        //ViewDSVDimension_Texture2DMS = D3D11_DSV_DIMENSION_TEXTURE2DMS,
        //ViewDSVDimension_Texture2DMSArray = D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY,
    };

    //--------------------------------------------------------
    enum Primitive
    {
        Primitive_None = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED,
        Primitive_PointList = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
        Primitive_LineList = D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
        Primitive_LineStrip = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
        Primitive_TriangleList = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        Primitive_TriangleStrip = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
        Primitive_LineListADJ = D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ,
        Primitive_LineStripADJ = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ,
        Primitive_TriangleListADJ = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ,
        Primitive_TriangleStripADJ = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ,
        Primitive_PatchList1 = D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST,
        Primitive_PatchList2 = D3D11_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST,
        Primitive_PatchList3 = D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST,
        Primitive_PatchList4 = D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST,
        Primitive_PatchList5 = D3D11_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST,
        Primitive_PatchList6 = D3D11_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST,
        Primitive_PatchList7 = D3D11_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST,
        Primitive_PatchList8 = D3D11_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST,
        Primitive_PatchList9 = D3D11_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST,
        Primitive_PatchList10 = D3D11_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST,
        Primitive_PatchList11 = D3D11_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST,
        Primitive_PatchList12 = D3D11_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST,
        Primitive_PatchList13 = D3D11_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST,
        Primitive_PatchList14 = D3D11_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST,
        Primitive_PatchList15 = D3D11_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST,
        Primitive_PatchList16 = D3D11_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST,
        Primitive_PatchList17 = D3D11_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST,
        Primitive_PatchList18 = D3D11_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST,
        Primitive_PatchList19 = D3D11_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST,
        Primitive_PatchList20 = D3D11_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST,
        Primitive_PatchList21 = D3D11_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST,
        Primitive_PatchList22 = D3D11_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST,
        Primitive_PatchList23 = D3D11_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST,
        Primitive_PatchList24 = D3D11_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST,
        Primitive_PatchList25 = D3D11_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST,
        Primitive_PatchList26 = D3D11_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST,
        Primitive_PatchList27 = D3D11_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST,
        Primitive_PatchList28 = D3D11_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST,
        Primitive_PatchList29 = D3D11_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST,
        Primitive_PatchList30 = D3D11_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST,
        Primitive_PatchList31 = D3D11_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST,
        Primitive_PatchList32 = D3D11_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST,
    };


    //--------------------------------------------------------
    /// データフォーマット
    enum DataFormat
    {
        Data_Unknown = DXGI_FORMAT_UNKNOWN,

        Data_R32G32B32A32_TypeLess = DXGI_FORMAT_R32G32B32A32_TYPELESS,
        Data_R32G32B32A32_Float = DXGI_FORMAT_R32G32B32A32_FLOAT,
        Data_R32G32B32A32_UInt = DXGI_FORMAT_R32G32B32A32_UINT,
        Data_R32G32B32A32_SInt = DXGI_FORMAT_R32G32B32A32_SINT,

        Data_R32G32B32_TypeLess = DXGI_FORMAT_R32G32B32_TYPELESS,
        Data_R32G32B32_Float = DXGI_FORMAT_R32G32B32_FLOAT,
        Data_R32G32B32_UInt = DXGI_FORMAT_R32G32B32_UINT,
        Data_R32G32B32_SInt = DXGI_FORMAT_R32G32B32_SINT,

        Data_R16G16B16A16_TypeLess = DXGI_FORMAT_R16G16B16A16_TYPELESS,
        Data_R16G16B16A16_Float = DXGI_FORMAT_R16G16B16A16_FLOAT,
        Data_R16G16B16A16_UNorm = DXGI_FORMAT_R16G16B16A16_UNORM,
        Data_R16G16B16A16_UInt = DXGI_FORMAT_R16G16B16A16_UINT,
        Data_R16G16B16A16_SNorm = DXGI_FORMAT_R16G16B16A16_SNORM,
        Data_R16G16B16A16_SInt = DXGI_FORMAT_R16G16B16A16_SINT,

        Data_R32G32_TypeLess = DXGI_FORMAT_R32G32_TYPELESS,
        Data_R32G32_Float = DXGI_FORMAT_R32G32_FLOAT,
        Data_R32G32_UInt = DXGI_FORMAT_R32G32_UINT,
        Data_R32G32_SInt = DXGI_FORMAT_R32G32_SINT,

        Data_R32G8X24_TypeLess = DXGI_FORMAT_R32G8X24_TYPELESS,
        Data_D32_Float_S8X24_UInt = DXGI_FORMAT_D32_FLOAT_S8X24_UINT,
        Data_R32_Float_X8X24_TypeLess = DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS,
        Data_X32_TypeLess_G8X24_UInt = DXGI_FORMAT_X32_TYPELESS_G8X24_UINT,

        Data_R10G10B10A2_TypeLess = DXGI_FORMAT_R10G10B10A2_TYPELESS,
        Data_R10G10B10A2_UNorm = DXGI_FORMAT_R10G10B10A2_UNORM,
        Data_R10G10B10A2_UInt = DXGI_FORMAT_R10G10B10A2_UINT,

        Data_R11G11B10_Float = DXGI_FORMAT_R11G11B10_FLOAT,

        Data_R8G8B8A8_TypeLess = DXGI_FORMAT_R8G8B8A8_TYPELESS,
        Data_R8G8B8A8_UNorm = DXGI_FORMAT_R8G8B8A8_UNORM,
        Data_R8G8B8A8_UNorm_SRGB = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
        Data_R8G8B8A8_UInt = DXGI_FORMAT_R8G8B8A8_UINT,
        Data_R8G8B8A8_SNorm = DXGI_FORMAT_R8G8B8A8_SNORM,
        Data_R8G8B8A8_SInt = DXGI_FORMAT_R8G8B8A8_SINT,

        Data_R16G16_TypeLess = DXGI_FORMAT_R16G16_TYPELESS,
        Data_R16G16_Float = DXGI_FORMAT_R16G16_FLOAT,
        Data_R16G16_UNorm = DXGI_FORMAT_R16G16_UNORM,
        Data_R16G16_UInt = DXGI_FORMAT_R16G16_UINT,
        Data_R16G16_SNorm = DXGI_FORMAT_R16G16_SNORM,
        Data_R16G16_SInt = DXGI_FORMAT_R16G16_SINT,

        Data_R32_TypeLess = DXGI_FORMAT_R32_TYPELESS,
        Data_D32_Float = DXGI_FORMAT_D32_FLOAT,
        Data_R32_Float = DXGI_FORMAT_R32_FLOAT,
        Data_R32_UInt = DXGI_FORMAT_R32_UINT,
        Data_R32_SInt = DXGI_FORMAT_R32_SINT,

        Data_R24G8_TypeLess = DXGI_FORMAT_R24G8_TYPELESS,
        Data_D24_UNorm_S8_UInt = DXGI_FORMAT_D24_UNORM_S8_UINT,
        Data_R24_UNorm_X8_TypeLess = DXGI_FORMAT_R24_UNORM_X8_TYPELESS,
        Data_X24_TypeLess_G8_UInt = DXGI_FORMAT_X24_TYPELESS_G8_UINT,

        Data_R8G8_TypeLess = DXGI_FORMAT_R8G8_TYPELESS,
        Data_R8G8_UNorm = DXGI_FORMAT_R8G8_UNORM,
        Data_R8G8_UInt = DXGI_FORMAT_R8G8_UINT,
        Data_R8G8_SNorm = DXGI_FORMAT_R8G8_SNORM,
        Data_R8G8_SInt = DXGI_FORMAT_R8G8_SINT,

        Data_R16_TypeLess = DXGI_FORMAT_R16_TYPELESS,
        Data_R16_Float = DXGI_FORMAT_R16_FLOAT,
        Data_D16_UNorm = DXGI_FORMAT_D16_UNORM,
        Data_R16_UNorm = DXGI_FORMAT_R16_UNORM,
        Data_R16_UInt = DXGI_FORMAT_R16_UINT,
        Data_R16_SNorm = DXGI_FORMAT_R16_SNORM,
        Data_R16_SInt = DXGI_FORMAT_R16_SINT,

        Data_R8_TypeLess = DXGI_FORMAT_R8_TYPELESS,
        Data_R8_UNorm = DXGI_FORMAT_R8_UNORM,
        Data_R8_UInt = DXGI_FORMAT_R8_UINT,
        Data_R8_SNorm = DXGI_FORMAT_R8_SNORM,
        Data_R8_SInt = DXGI_FORMAT_R8_SINT,
        Data_A8_UNorm = DXGI_FORMAT_A8_UNORM,

        Data_R1_UNorm = DXGI_FORMAT_R1_UNORM,

        Data_R9G9B9E5_SharedExp = DXGI_FORMAT_R9G9B9E5_SHAREDEXP,

        Data_R8G8_B8G8_UNorm = DXGI_FORMAT_R8G8_B8G8_UNORM,
        Data_G8R8_G8B8_UNorm = DXGI_FORMAT_G8R8_G8B8_UNORM,

        Data_BC1_TypeLess = DXGI_FORMAT_BC1_TYPELESS,
        Data_BC1_UNorm = DXGI_FORMAT_BC1_UNORM,
        Data_BC1_UNorm_SRGB = DXGI_FORMAT_BC1_UNORM_SRGB,

        Data_BC2_TypeLess = DXGI_FORMAT_BC2_TYPELESS,
        Data_BC2_UNorm = DXGI_FORMAT_BC2_UNORM,
        Data_BC2_UNorm_SRGB = DXGI_FORMAT_BC2_UNORM_SRGB,

        Data_BC3_TypeLess = DXGI_FORMAT_BC3_TYPELESS,
        Data_BC3_UNorm = DXGI_FORMAT_BC3_UNORM,
        Data_BC3_UNorm_SRGB = DXGI_FORMAT_BC3_UNORM_SRGB,

        Data_BC4_TypeLess = DXGI_FORMAT_BC4_TYPELESS,
        Data_BC4_UNorm = DXGI_FORMAT_BC4_UNORM,
        Data_BC4_SNorm = DXGI_FORMAT_BC4_SNORM,

        Data_BC5_TypeLess = DXGI_FORMAT_BC5_TYPELESS,
        Data_BC5_UNorm = DXGI_FORMAT_BC5_UNORM,
        Data_BC5_SNorm = DXGI_FORMAT_BC5_SNORM,

        Data_B5G6R5_UNorm = DXGI_FORMAT_B5G6R5_UNORM,
        Data_B5G5R5A1_UNorm = DXGI_FORMAT_B5G5R5A1_UNORM,

        Data_B8G8R8A8_UNorm = DXGI_FORMAT_B8G8R8A8_UNORM,
        Data_B8G8R8X8_UNorm = DXGI_FORMAT_B8G8R8X8_UNORM,

        Data_R10G10B10_XR_Bias_A2_UNorm = DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM,

        Data_B8G8R8A8_TypeLess = DXGI_FORMAT_B8G8R8A8_TYPELESS,
        Data_B8G8R8A8_UNorm_SRGB = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
        Data_B8G8R8X8_TypeLess = DXGI_FORMAT_B8G8R8X8_TYPELESS,
        Data_B8G8R8X8_UNorm_SRGB = DXGI_FORMAT_B8G8R8X8_UNORM_SRGB,

        Data_BC6H_TypeLess = DXGI_FORMAT_BC6H_TYPELESS,
        Data_BC6H_UF16 = DXGI_FORMAT_BC6H_UF16,
        Data_BC6H_SF16 = DXGI_FORMAT_BC6H_SF16,

        Data_BC7_TypeLess = DXGI_FORMAT_BC7_TYPELESS,
        Data_BC7_UNorm = DXGI_FORMAT_BC7_UNORM,
        Data_BC7_UNorm_SRGB = DXGI_FORMAT_BC7_UNORM_SRGB,

        Data_Force_UInt = DXGI_FORMAT_FORCE_UINT,
    };

    enum DXGIScanlineOrder
    {
        DXGIScanlineOrder_Unspecified = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED ,
        DXGIScanlineOrder_Progressive = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE ,
        DXGIScanlineOrder_UpperFirst  = DXGI_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST ,
        DXGIScanlineOrder_LowerFirst  = DXGI_MODE_SCANLINE_ORDER_LOWER_FIELD_FIRST ,
    };

    enum DXGIScaling
    {
        DXGIScaling_Unspecified = DXGI_MODE_SCALING_UNSPECIFIED,
        DXGIScaling_Centered    = DXGI_MODE_SCALING_CENTERED,
        DXGIScaling_Stretched   = DXGI_MODE_SCALING_STRETCHED,
    };

    enum DXGIUsage
    {
        DXGIUsage_BackBuffer       = DXGI_USAGE_BACK_BUFFER,
        DXGIUsage_DiscardOnPresent = DXGI_USAGE_DISCARD_ON_PRESENT,
        DXGIUsage_ReadOnly         = DXGI_USAGE_READ_ONLY,
        DXGIUsage_RenderTargetOutput = DXGI_USAGE_RENDER_TARGET_OUTPUT,
        DXGIUsage_ShaderInput      = DXGI_USAGE_SHADER_INPUT,
        DXGIUsage_Shared           = DXGI_USAGE_SHARED,
        DXGIUsage_UnorderedAccess  = DXGI_USAGE_UNORDERED_ACCESS,
    };

    enum DXGISwapChainFlag
    {
        DXGISwapChainFlag_Nonprerotated = DXGI_SWAP_CHAIN_FLAG_NONPREROTATED,
        DXGISwapChainFlag_AllowModeSwitch = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH,
        DXGISwapChainFlag_GDICompatible = DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE,
    };


#if 0


    //--------------------------------------------------------
    enum MutiSampleType
    {
        MutiSample_None = D3DMULTISAMPLE_NONE,
        MutiSample_NonMaskable = D3DMULTISAMPLE_NONMASKABLE,
        MutiSample_2 = D3DMULTISAMPLE_2_SAMPLES,
        MutiSample_3 = D3DMULTISAMPLE_3_SAMPLES,
        MutiSample_4 = D3DMULTISAMPLE_4_SAMPLES,
        MutiSample_5 = D3DMULTISAMPLE_5_SAMPLES,
        MutiSample_6 = D3DMULTISAMPLE_6_SAMPLES,
        MutiSample_7 = D3DMULTISAMPLE_7_SAMPLES,
        MutiSample_8 = D3DMULTISAMPLE_8_SAMPLES,
        MutiSample_9 = D3DMULTISAMPLE_9_SAMPLES,
        MutiSample_10 = D3DMULTISAMPLE_10_SAMPLES,
        MutiSample_11 = D3DMULTISAMPLE_11_SAMPLES,
        MutiSample_12 = D3DMULTISAMPLE_12_SAMPLES,
        MutiSample_13 = D3DMULTISAMPLE_13_SAMPLES,
        MutiSample_14 = D3DMULTISAMPLE_14_SAMPLES,
        MutiSample_15 = D3DMULTISAMPLE_15_SAMPLES,
        MutiSample_16 = D3DMULTISAMPLE_16_SAMPLES,
    };
#endif

    //--------------------------------------------------------
    enum TextureAddress
    {
        TexAddress_Wrap = D3D11_TEXTURE_ADDRESS_WRAP,
        TexAddress_Mirror = D3D11_TEXTURE_ADDRESS_MIRROR,
        TexAddress_Clamp = D3D11_TEXTURE_ADDRESS_CLAMP,
        TexAddress_Border = D3D11_TEXTURE_ADDRESS_BORDER,
        TexAddress_Mirroronce = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE, 
    };

    //--------------------------------------------------------
    enum TextureFilterType
    {
        TexFilter_MinMagMipPoint = D3D11_FILTER_MIN_MAG_MIP_POINT,
        TexFilter_MinMagPointMipLinear = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR,
        TexFilter_MinPointMagLinearMipPoint = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,
        TexFilter_MinPointMagMipLinear = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR,
        TexFilter_MinLinearMagMipPoint = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT,
        TexFilter_MinLinearMagPointMipLinear = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
        TexFilter_MinMagLinearMipPoint = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,
        TexFilter_MinMagMipLinear = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
        TexFilter_Anisotropic = D3D11_FILTER_ANISOTROPIC,
        TexFilter_CompMinMagMipPoint = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT,
        TexFilter_CompMinMagPointMipLinear = D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR,
        TexFilter_CompMinPointMagLinearMipPoint = D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT,
        TexFilter_CompMinPointMagMipLinear = D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR,
        TexFilter_CompMinLinearMagMipPoint = D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT,
        TexFilter_CompMinLinearMagPointMipLinear = D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
        TexFilter_CompMinMagLinearMipPoint = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT,
        TexFilter_CompMinMagMipLinear = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR,
        TexFilter_CompAnisotropic = D3D11_FILTER_COMPARISON_ANISOTROPIC,
    };

    //--------------------------------------------------------
    enum CmpFunc
    {
        Cmp_Never = D3D11_COMPARISON_NEVER,
        Cmp_Less = D3D11_COMPARISON_LESS,
        Cmp_Equal = D3D11_COMPARISON_EQUAL,
        Cmp_LessEqual = D3D11_COMPARISON_LESS_EQUAL,
        Cmp_Greater = D3D11_COMPARISON_GREATER,
        Cmp_NotEqual = D3D11_COMPARISON_NOT_EQUAL,
        Cmp_GreaterEqual = D3D11_COMPARISON_GREATER_EQUAL,
        Cmp_Always = D3D11_COMPARISON_ALWAYS,
    };

    //--------------------------------------------------------
    enum BlendType
    {
        Blend_Zero = D3D11_BLEND_ZERO,
        Blend_One = D3D11_BLEND_ONE,
        Blend_SrcAlpha = D3D11_BLEND_SRC_ALPHA,
        Blend_InvSrcAlpha = D3D11_BLEND_INV_SRC_ALPHA,
        Blend_DestAlpha = D3D11_BLEND_DEST_ALPHA,
        Blend_InvDestAlpha = D3D11_BLEND_INV_DEST_ALPHA ,
    };

    //--------------------------------------------------------
    enum BlendOp
    {
        BlendOp_Add = D3D11_BLEND_OP_ADD,
        BlendOp_Subtract = D3D11_BLEND_OP_SUBTRACT,
        BlendOp_RevSubtract = D3D11_BLEND_OP_REV_SUBTRACT,
        BlendOp_Min = D3D11_BLEND_OP_MIN,
        BlendOp_Max = D3D11_BLEND_OP_MAX,
    };

    enum ColorWriteMask
    {
        ColorWrite_R = D3D11_COLOR_WRITE_ENABLE_RED,
        ColorWrite_G = D3D11_COLOR_WRITE_ENABLE_GREEN,
        ColorWrite_B = D3D11_COLOR_WRITE_ENABLE_BLUE,
        ColorWrite_A = D3D11_COLOR_WRITE_ENABLE_ALPHA,
        ColorWrite_All = D3D11_COLOR_WRITE_ENABLE_ALL,
    };

    enum DepthWriteMask
    {
        DepthWrite_Zero = D3D11_DEPTH_WRITE_MASK_ZERO,
        DepthWrite_All = D3D11_DEPTH_WRITE_MASK_ALL,
    };

    enum StencilOp
    {
        StencilOp_Keep = D3D11_STENCIL_OP_KEEP,
        StencilOp_Zero = D3D11_STENCIL_OP_ZERO,
        StencilOp_Replace = D3D11_STENCIL_OP_REPLACE,
        StencilOp_IncrSat = D3D11_STENCIL_OP_INCR_SAT,
        StencilOp_DecrSat = D3D11_STENCIL_OP_DECR_SAT,
        StencilOp_Invert = D3D11_STENCIL_OP_INVERT,
        StencilOp_Incr = D3D11_STENCIL_OP_INCR,
        StencilOp_Decr = D3D11_STENCIL_OP_DECR,
    };

    enum UAVDimension
    {
        UAVDimension_Unknown = D3D11_UAV_DIMENSION_UNKNOWN,
        UAVDimension_Buffer = D3D11_UAV_DIMENSION_BUFFER,
        UAVDimension_Texture1D = D3D11_UAV_DIMENSION_TEXTURE1D,
        UAVDimension_Texture1DArray = D3D11_UAV_DIMENSION_TEXTURE1DARRAY,
        UAVDimension_Texture2D = D3D11_UAV_DIMENSION_TEXTURE2D,
        UAVDimension_Texture2DArray = D3D11_UAV_DIMENSION_TEXTURE2DARRAY,
        UAVDimension_Texture3D = D3D11_UAV_DIMENSION_TEXTURE3D,
    };

    enum UAVFlag
    {
        UAVFlag_RAW = D3D11_BUFFER_UAV_FLAG_RAW,
        UAVFlag_Append = D3D11_BUFFER_UAV_FLAG_APPEND,
        UAVFlag_Counter = D3D11_BUFFER_UAV_FLAG_COUNTER,
    };


    /**
    @brief bits per pixel取得
    */
    u32 getBitsPerPixel(DataFormat format);
}


#endif //INC_LGRAPHICS_DX11_ENUMERATIONS_H__
