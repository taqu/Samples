#ifndef INC_LGRAPHICS_DX9_VERTEXDECLARATIONREF_H__
#define INC_LGRAPHICS_DX9_VERTEXDECLARATIONREF_H__
/**
@file VertexDeclarationRef.h
@author t-sakai
@date 2009/04/30 create
*/
#include "../../lgraphicscore.h"
#include "Enumerations.h"

struct IDirect3DVertexDeclaration9;

namespace lgraphics
{
    struct VertexElement
    {
        u8 getType() const{ return type_;}
        u8 getMethod() const{ return method_;}
        u8 getUsage() const{ return usage_;}
        u8 getUsageIndex() const{ return usageIndex_;}

        u16 stream_;
        u16 offset_;
        u8 type_;
        u8 method_;
        u8 usage_;
        u8 usageIndex_;
    };

    class VertexDeclarationRef
    {
    public:
        static const u32 MAX_ELEMENTS = 16;

        VertexDeclarationRef()
            :vertexSize_(0)
            ,numElements_(0)
            ,declaration_(NULL)
        {
        }

        VertexDeclarationRef(const VertexDeclarationRef& rhs);
        ~VertexDeclarationRef()
        {
            destroy();
        }

        VertexDeclarationRef& operator=(const VertexDeclarationRef& rhs)
        {
            VertexDeclarationRef tmp(rhs);
            tmp.swap(*this);
            return *this;
        }

        void destroy();

        bool valid() const{ return (declaration_ != NULL); }

        void attach() const;

        u32 getVertexSize(u32 ) const{ return vertexSize_;}
        u32 getNumElements() const{ return numElements_;}
        bool getDecl(VertexElement* decl);

        void swap(VertexDeclarationRef& rhs)
        {
            lcore::swap(vertexSize_, rhs.vertexSize_);
            lcore::swap(numElements_, rhs.numElements_);
            lcore::swap(declaration_, rhs.declaration_);
        }

    private:
        friend class VertexDeclCreator;

        VertexDeclarationRef(u32 vertexSize, u32 numElements, IDirect3DVertexDeclaration9* declaration)
            :vertexSize_(vertexSize)
            ,numElements_(numElements)
            ,declaration_(declaration)
        {
        }

        u32 vertexSize_;
        u32 numElements_;
        IDirect3DVertexDeclaration9 *declaration_;
    };


    class VertexDeclCreator
    {
    public:
        VertexDeclCreator(u32 elementNum);
        ~VertexDeclCreator();

        /**
        @return 要素のバイト数を返す
        */
        u16 add(u16 stream, u16 offset, DeclType type, DeclMethod method, DeclUsage usage, u8 usageIndex);
        void end(VertexDeclarationRef& declaration);

    private:
        VertexDeclCreator(const VertexDeclCreator&);
        VertexDeclCreator& operator=(const VertexDeclCreator&);

        void declEnd()
        {
            add(0xFFU, 0, DeclType_UnUsed, DeclMethod_Default, DeclUsage_Position, 0);
        }
        VertexElement *elements_;
        u32 count_;
        u32 size_;
        u32 vertexSize_;
    };
}

//---------------------------------------------------
//---
//--- lcore::swap特殊化
//---
//---------------------------------------------------
namespace lcore
{
    template<>
    inline void swap<lgraphics::VertexDeclarationRef>(lgraphics::VertexDeclarationRef& l, lgraphics::VertexDeclarationRef& r)
    {
        l.swap(r);
    }
}

#endif //INC_LGRAPHICS_DX9_VERTEXDECLARATIONREF_H__
