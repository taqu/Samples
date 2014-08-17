#ifndef INC_LPACK_PACKREADER_H__
#define INC_LPACK_PACKREADER_H__
/**
@file PackReader.h
@author t-sakai
@date 2012/03/20 create

*/
#include "lpack.h"

namespace lpack
{
    class PackReader
    {
    public:
        static bool readEntry(PackEntry& pack, const Char* path);
        static bool readMemory(PackMemory& pack, const Char* path);
        static bool readFile(PackFile& pack, const Char* path);
    };
}
#endif //INC_LPACK_PACKREADER_H__
