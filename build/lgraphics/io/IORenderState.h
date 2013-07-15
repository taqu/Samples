#ifndef INC_LGRAPHICS_IORENDERSTATE_H__
#define INC_LGRAPHICS_IORENDERSTATE_H__
/**
@file IORenderState.h
@author t-sakai
@date 2010/05/06 create

*/
#include <lcore/liostream.h>

namespace lgraphics
{
    class RenderStateRef;

namespace io
{
    class IORenderState
    {
    public:
        IORenderState(){}
        ~IORenderState(){}

        static bool read(lcore::istream& is, RenderStateRef& state);
        static bool write(lcore::ostream& os, RenderStateRef& state);

    };
}
}
#endif //INC_LGRAPHICS_IORENDERSTATE_H__
