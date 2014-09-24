/**
@file InputLayoutFactory.cpp
@author t-sakai
@date 2012/07/31 create
*/
#include "InputLayoutFactory.h"
#include <lgraphics/lgraphicsAPIInclude.h>
namespace render
{
    namespace
    {
        static const u8 LayoutData_P[] =
        {
#include "shader/Layout_P.byte"
        };

        static const u8 LayoutData_PN[] =
        {
#include "shader/Layout_PN.byte"
        };

        static const u8 LayoutData_PU[] =
        {
#include "shader/Layout_PU.byte"
        };

        static const u8 LayoutData_PC[] =
        {
#include "shader/Layout_PC.byte"
        };

        static const u8 LayoutData_PNTB[] =
        {
#include "shader/Layout_PNTB.byte"
        };

        static const u8 LayoutData_PNU[] =
        {
#include "shader/Layout_PNU.byte"
        };

        static const u8 LayoutData_PNCU[] =
        {
#include "shader/Layout_PNCU.byte"
        };

        static const u8 LayoutData_PNTBU[] =
        {
#include "shader/Layout_PNTBU.byte"
        };

        static const u8 LayoutData_PNUBone[] =
        {
#include "shader/Layout_PNUBone.byte"
        };
    }

    InputLayoutFactory::InputLayoutFactory()
    {
    }

    InputLayoutFactory::~InputLayoutFactory()
    {
    }

    bool InputLayoutFactory::initialize()
    {
        Compiler compiler;

        {//Layout_P
            D3D11_INPUT_ELEMENT_DESC layout[] =
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            };

            compiler.resize(sizeof(LayoutData_P));
            u32 size = compiler.decompress(sizeof(LayoutData_P), LayoutData_P);
            if(size>0){
                layouts_[Layout_P] = lgraphics::InputLayout::create(layout, 1, compiler.getData(), size);
            }

        }

        {//Layout_PN
            D3D11_INPUT_ELEMENT_DESC layout[] =
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "NORMAL", 0, DXGI_FORMAT_R16G16B16A16_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            };

            compiler.resize(sizeof(LayoutData_PN));
            u32 size = compiler.decompress(sizeof(LayoutData_PN), LayoutData_PN);
            if(size>0){
                layouts_[Layout_PN] = lgraphics::InputLayout::create(layout, 2, compiler.getData(), size);
            }
        }

        {//Layout_PU
            D3D11_INPUT_ELEMENT_DESC layout[] =
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "TEXCOORD", 0, DXGI_FORMAT_R16G16_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            };

            compiler.resize(sizeof(LayoutData_PU));
            u32 size = compiler.decompress(sizeof(LayoutData_PU), LayoutData_PU);
            if(size>0){
                layouts_[Layout_PU] = lgraphics::InputLayout::create(layout, 2, compiler.getData(), size);
            }
        }

        {//Layout_PC
            D3D11_INPUT_ELEMENT_DESC layout[] =
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            };

            compiler.resize(sizeof(LayoutData_PC));
            u32 size = compiler.decompress(sizeof(LayoutData_PC), LayoutData_PC);
            if(size>0){
                layouts_[Layout_PC] = lgraphics::InputLayout::create(layout, 2, compiler.getData(), size);
            }
        }

        {//Layout_PNTB
            D3D11_INPUT_ELEMENT_DESC layout[] =
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "NORMAL", 0, DXGI_FORMAT_R16G16B16A16_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "TANGENT", 0, DXGI_FORMAT_R16G16B16A16_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "BINORMAL", 0, DXGI_FORMAT_R16G16B16A16_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            };

            compiler.resize(sizeof(LayoutData_PNTB));
            u32 size = compiler.decompress(sizeof(LayoutData_PNTB), LayoutData_PNTB);
            if(size>0){
                layouts_[Layout_PNTB] = lgraphics::InputLayout::create(layout, 4, compiler.getData(), size);
            }
        }

        {//Layout_PNU
            D3D11_INPUT_ELEMENT_DESC layout[] =
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "NORMAL", 0, DXGI_FORMAT_R16G16B16A16_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "TEXCOORD", 0, DXGI_FORMAT_R16G16_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            };

            compiler.resize(sizeof(LayoutData_PNU));
            u32 size = compiler.decompress(sizeof(LayoutData_PNU), LayoutData_PNU);
            if(size>0){
                layouts_[Layout_PNU] = lgraphics::InputLayout::create(layout, 3, compiler.getData(), size);
            }
        }

        {//Layout_PNCU
            D3D11_INPUT_ELEMENT_DESC layout[] =
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "NORMAL", 0, DXGI_FORMAT_R16G16B16A16_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "TEXCOORD", 0, DXGI_FORMAT_R16G16_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            };

            compiler.resize(sizeof(LayoutData_PNCU));
            u32 size = compiler.decompress(sizeof(LayoutData_PNCU), LayoutData_PNCU);
            if(size>0){
                layouts_[Layout_PNCU] = lgraphics::InputLayout::create(layout, 4, compiler.getData(), size);
            }
        }

        {//Layout_PNTBU
            D3D11_INPUT_ELEMENT_DESC layout[] =
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "NORMAL", 0, DXGI_FORMAT_R16G16B16A16_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "TANGENT", 0, DXGI_FORMAT_R16G16B16A16_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "BINORMAL", 0, DXGI_FORMAT_R16G16B16A16_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "TEXCOORD", 0, DXGI_FORMAT_R16G16_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            };

            compiler.resize(sizeof(LayoutData_PNTBU));
            u32 size = compiler.decompress(sizeof(LayoutData_PNTBU), LayoutData_PNTBU);
            if(size>0){
                layouts_[Layout_PNTBU] = lgraphics::InputLayout::create(layout, 5, compiler.getData(), size);
            }
        }

        {//Layout_PNUBone
            D3D11_INPUT_ELEMENT_DESC layout[] =
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "NORMAL", 0, DXGI_FORMAT_R16G16B16A16_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "TEXCOORD", 0, DXGI_FORMAT_R16G16_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "BLENDINDICES", 0, DXGI_FORMAT_R16G16_UINT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "BLENDWEIGHT", 0, DXGI_FORMAT_R16G16_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            };

            compiler.resize(sizeof(LayoutData_PNUBone));
            u32 size = compiler.decompress(sizeof(LayoutData_PNUBone), LayoutData_PNUBone);
            if(size>0){
                layouts_[Layout_PNUBone] = lgraphics::InputLayout::create(layout, 5, compiler.getData(), size);
            }
        }
        return true;
    }
}
