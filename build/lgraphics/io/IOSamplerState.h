#ifndef INC_IOSAMPLERSTATE_H__
#define INC_IOSAMPLERSTATE_H__
/**
@file IOSamplerState.h
@author t-sakai
@date 2010/05/06 create

*/
#include <lcore/liostream.h>

namespace lgraphics
{
    class SamplerState;

namespace io
{
    class IOSamplerState
    {
    public:
        IOSamplerState(){}
        ~IOSamplerState(){}

        static bool read(lcore::istream& is, SamplerState& state);
        static bool write(lcore::ostream& os, const SamplerState& state);

    };
}
}
#endif //INC_IOSAMPLERSTATE_H__
