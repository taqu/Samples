#ifndef INC_LSOUND_PACKREADER_H__
#define INC_LSOUND_PACKREADER_H__
/**
@file PackReader.h
@author t-sakai
@date 2013/02/12 create
*/
#include <lcore/liostream.h>
#include "Pack.h"

namespace lsound
{
    class PackReader
    {
    public:
        PackReader();
        ~PackReader();

        /// ファイルオープン
        bool open(const Char* path);

        /// ファイルクローズ
        void close();

        bool readEntries(Pack& pack);
        bool readAll(Pack& pack);
    private:
        static const u32 BlockSize = 4096;

        lcore::ifstream stream_;
        u32 dataSize_;
        Pack pack_;
    };
}
#endif //INC_LSOUND_PACKREADER_H__
