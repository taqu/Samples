#ifndef INC_LGRAPHICS_DX11_BLOBREF_H__
#define INC_LGRAPHICS_DX11_BLOBREF_H__
/**
@file BlobRef.h
@author t-sakai
@date 2012/07/21 create
*/
#include "../../lgraphicscore.h"
struct ID3D10Blob;

namespace lgraphics
{
    class BlobRef
    {
    public:
        BlobRef();
        BlobRef(const BlobRef& rhs);
        explicit BlobRef(ID3D10Blob* blob);
        ~BlobRef();

        ID3D10Blob* get(){ return blob_;}
        void* getPointer();
        u32 getSize();

        BlobRef& operator=(const BlobRef& rhs)
        {
            BlobRef tmp(rhs);
            tmp.swap(*this);
            return *this;
        }

        bool valid() const{ return (blob_!=NULL);}

        void swap(BlobRef& rhs)
        {
            lcore::swap(blob_, rhs.blob_);
        }
    private:
        ID3D10Blob* blob_;
    };
}
#endif //INC_LGRAPHICS_DX11_BLOBREF_H__
