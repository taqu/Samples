#ifndef INC_IOSHADERKEY_H__
#define INC_IOSHADERKEY_H__
/**
@file IOShaderKey.h
@author t-sakai
@date 2010/05/06 create

*/
#include <lcore/liostream.h>

namespace lgraphics
{
    class ShaderKey;

namespace io
{
    class IOShaderKey
    {
    public:
        IOShaderKey(){}
        ~IOShaderKey(){}

        static bool read(lcore::istream& is, ShaderKey& key);
        static bool write(lcore::ostream& os, const ShaderKey& key);
    };
}
}
#endif //INC_IOSHADERKEY_H__
