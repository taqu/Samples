#ifndef INC_LGRAPHICS_ES2_GEOMETRYBUFFER_H__
#define INC_LGRAPHICS_ES2_GEOMETRYBUFFER_H__
/**
@file GeometryBuffer.h
@author t-sakai
@date 2010/07/10 create
*/
#include <lcore/smart_ptr/intrusive_ptr.h>
#include "../../lgraphicscore.h"

#include "VertexBufferRef.h"
#include "IndexBufferRef.h"
#include "VertexDeclarationRef.h"

namespace lgraphics
{
    class GeometryBuffer
    {
    public:
        typedef smart_ptr::intrusive_ptr<GeometryBuffer> pointer;

        GeometryBuffer();
        GeometryBuffer(PrimitiveType type, const VertexDeclarationRef& decl, const VertexBufferRef& vb, const IndexBufferRef& ib);
        ~GeometryBuffer();

        PrimitiveType getType() const{ return type_;}

        u32 getNumStreams() const{ return numStreams_;}

        VertexBufferRef& getVertexBuffer(u32 stream)
        {
            LASSERT(0<=stream && stream<LIME_MAX_VERTEX_STREAMS);
            return vertexBuffer_[stream];
        }

        IndexBufferRef& getIndexBuffer(){ return indexBuffer_;}

        VertexDeclarationRef& getDecl() { return decl_;}

        bool addVertexBufferStream(VertexBufferRef& vb);

        /**

        頂点のストライドに頂点宣言の設定を使用する
        */
        void attach();

        void detach();

        bool hasIndex() const{ return indexBuffer_.valid();}
    private:
        friend inline void intrusive_ptr_addref(GeometryBuffer* ptr);
        friend inline void intrusive_ptr_release(GeometryBuffer* ptr);

        void addRef()
        {
            ++refCount_;
        }
        void release()
        {
            if(--refCount_ == 0){
                LIME_DELETE_NONULL this;
            }
        }

        s32 refCount_;

        PrimitiveType type_;
        u32 numStreams_;
        VertexDeclarationRef decl_;
        VertexBufferRef vertexBuffer_[LIME_MAX_VERTEX_STREAMS];
        IndexBufferRef indexBuffer_;
    };

    inline void intrusive_ptr_addref(GeometryBuffer* ptr)
    {
        ptr->addRef();
    }

    inline void intrusive_ptr_release(GeometryBuffer* ptr)
    {
        ptr->release();
    }
}

#endif //INC_LGRAPHICS_ES2_GEOMETRYBUFFER_H__

