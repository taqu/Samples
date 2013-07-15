#ifndef INC_LGRAPHICS_SECTIONID_H__
#define INC_LGRAPHICS_SECTIONID_H__
/**
@file SectionID.h
@author t-sakai
@date 2010/05/02 create
*/
#include <lgraphics/lgraphicscore.h>

namespace lgraphics
{
namespace format
{
#define MAKE_ID(name, id)\
    static const unsigned int name = id;

    MAKE_ID(GeometryBufferID, LIME_MAKE_FOURCC('g', 'b', 'u', 'f'))
    MAKE_ID(VertexDeclarationID, LIME_MAKE_FOURCC('v', 'd', 'e', 'c'))
    MAKE_ID(VertexBufferID, LIME_MAKE_FOURCC('v', 'b', 'u', 'f'))
    MAKE_ID(IndexBufferID, LIME_MAKE_FOURCC('i', 'b', 'u', 'f'))

    MAKE_ID(ShaderKeyID, LIME_MAKE_FOURCC('s', 'h', 'k', 'y'))

    MAKE_ID(SamplerStateID, LIME_MAKE_FOURCC('s', 'a', 'm', 'p'))
    MAKE_ID(RenderStateID, LIME_MAKE_FOURCC('r', 'e', 'n', 'd'))

#undef MAKE_ID
}
}
#endif //INC_LGRAPHICS_SECTIONID_H__
