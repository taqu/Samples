#ifndef INC_LGRAPHICS_DX11_INPUTLAYOUTREF_H__
#define INC_LGRAPHICS_DX11_INPUTLAYOUTREF_H__
/**
@file InputLayoutRef.h
@author t-sakai
@date 2012/07/15 create
*/
#include "../../lgraphicscore.h"

struct D3D11_INPUT_ELEMENT_DESC;
struct ID3D11InputLayout;

namespace lgraphics
{
    class InputLayoutRef
    {
    public:
        static InputLayoutRef create(
            D3D11_INPUT_ELEMENT_DESC* elements,
            u32 numElements,
            const void* data,
            u32 size);

        InputLayoutRef()
            :layout_(NULL)
        {}

        InputLayoutRef(const InputLayoutRef& rhs);

        ~InputLayoutRef()
        {
            destroy();
        }

        void destroy();

        bool valid() const{ return (NULL != layout_);}

        InputLayoutRef& operator=(const InputLayoutRef& rhs)
        {
            InputLayoutRef tmp(rhs);
            tmp.swap(*this);
            return *this;
        }

        void swap(InputLayoutRef& rhs)
        {
            lcore::swap(layout_, rhs.layout_);
        }

        void attach();
    private:
        friend class InputLayout;

        InputLayoutRef(ID3D11InputLayout* layout)
            :layout_(layout)
        {}

        ID3D11InputLayout* layout_;
    };

    class InputLayout
    {
    public:
        static InputLayoutRef create(
            D3D11_INPUT_ELEMENT_DESC* elements,
            u32 numElements,
            const void* data,
            u32 size);
    };
}
#endif //INC_LGRAPHICS_DX11_INPUTLAYOUTREF_H__
