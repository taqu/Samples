#ifndef INC_LSOUND_PACK_H__
#define INC_LSOUND_PACK_H__
/**
@file Pack.h
@author t-sakai
@date 2013/02/11 create
*/
#include "../Type.h"

namespace lsound
{
    struct PackHeader
    {
        u8 reserved0_;
        u8 reserved1_;
        u16 numFiles_;
    };

    struct FileEntry
    {
        u32 size_;
        s32 offset_;
    };

    class Pack
    {
    public:
        Pack();
        ~Pack();

        /// ƒtƒ@ƒCƒ‹”æ“¾
        u32 getNumFiles() const;

        u32 getSize(u32 index) const;
        const u8* getData(u32 index) const;

        s32 getFileOffset(u32 index) const;

        void swap(Pack& rhs);
    private:
        friend class PackReader;
        void releaseEntries();
        void releaseData();

        PackHeader header_;
        FileEntry* entries_;
        s32 dataTopOffset_;
        u8* data_;
    };
}
#endif //INC_LSOUND_PACK_H__
