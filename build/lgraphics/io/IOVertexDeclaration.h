#ifndef INC_LGRAPHICS_IOVERTEXDECLARATION_H__
#define INC_LGRAPHICS_IOVERTEXDECLARATION_H__
/**
@file IOVertexDeclaration.h
@author t-sakai
@date 2010/05/06 create
*/
#include <lcore/liostream.h>

namespace lgraphics
{
    class VertexDeclarationRef;

namespace io
{
    class IOVertexDeclaration
    {
    public:
        IOVertexDeclaration(){}
        ~IOVertexDeclaration(){}

        static bool read(lcore::istream& is, VertexDeclarationRef& decl);
        static bool write(lcore::ostream& os, VertexDeclarationRef& decl);
    };
}
}

#endif //INC_LGRAPHICS_IOVERTEXDECLARATION_H__
