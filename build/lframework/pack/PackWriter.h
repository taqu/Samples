#ifndef INC_LPACK_PACKWRITER_H__
#define INC_LPACK_PACKWRITER_H__
/**
@file PackWriter.h
@author t-sakai
@date 2012/03/20 create

*/
#include <lcore/liostream.h>
#include <lcore/vector.h>
#include "lpack.h"

namespace lpack
{
    class PackWriter
    {
    public:
        PackWriter();
        ~PackWriter();

        bool push_back(const Char* name, u32 size, const void* buffer);

        bool write(const Char* path);
        bool writeList(const Char* path);
    private:
        typedef lcore::vector_arena<FileEntry> FileEntryArray;
        typedef lcore::vector_arena<void*> MemPtrArray;

        /// �t�@�C���I�[�v��
        inline bool open(const Char* path);

        /// �t�@�C���N���[�Y
        inline void close();

        lcore::ofstream stream_;
        PackHeader header_;
        FileEntryArray entries_;
        MemPtrArray mempts_;
    };

    // �t�@�C���I�[�v��
    inline bool PackWriter::open(const Char* path)
    {
        LASSERT(NULL != path);
        return stream_.open(path, lcore::ios::binary);
    }

    // �t�@�C���N���[�Y
    inline void PackWriter::close()
    {
        stream_.close();
    }
}
#endif //INC_LPACK_PACKWRITER_H__
