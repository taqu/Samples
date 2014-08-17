#ifndef INC_LPACK_PACK_H__
#define INC_LPACK_PACK_H__
/**
@file lpack.h
@author t-sakai
@date 2012/03/20 create

*/
#include <lcore/lcore.h>
#include <lcore/liostream.h>

namespace lpack
{

    using lcore::s8;
    using lcore::s16;
    using lcore::s32;

    using lcore::u8;
    using lcore::u16;
    using lcore::u32;

    using lcore::f32;
    using lcore::f64;

    using lcore::Char;

    static const u32 MaxFileNameLength = 31;
    static const u32 MaxFileNameBufferSize = MaxFileNameLength + 1;
    static const u32 MaxPathLen = 256;

    /// リトルエンディアンか
    bool isLittleEndian();

    /// バイト反転
    template<class T>
    void byteReverse(T& x)
    {
        u8* l = (u8*)&x;
        u8* h = l + sizeof(T);
        u8 t;

        while(--h>l){
            t = *l;
            *l = *h;
            --l;
            h = t;
        }
    }

    enum Endian
    {
        Endian_Little = 0,
    };

    struct PackHeader
    {
        u8 endian_;
        u8 reserved_;
        u16 numFiles_;
    };

    struct FileEntry
    {
        u32 size_;
        u32 offset_;
        Char name_[MaxFileNameBufferSize];
    };

    //---------------------------------------
    //---
    //--- PackBase
    //---
    //---------------------------------------
    class PackBase
    {
    public:
        /// エントリ数取得
        inline u32 getNumEntries() const;

        /// エントリ取得
        inline const FileEntry& getEntry(u32 index) const;

    protected:
        PackBase();
        PackBase(const PackHeader& header, FileEntry* entries);
        ~PackBase();

        ///
        void swap(PackBase& rhs);

    private:
        friend class PackReader;

        PackBase(const PackBase&);
        PackBase& operator=(const PackBase&);

        PackHeader header_;
        FileEntry* entries_;
    };

    // エントリ数取得
    inline u32 PackBase::getNumEntries() const
    {
        return header_.numFiles_;
    }

    // エントリ取得
    inline const FileEntry& PackBase::getEntry(u32 index) const
    {
        LASSERT(0<=index && index<header_.numFiles_);
        return entries_[index];
    }

    //---------------------------------------
    //---
    //--- PackEntry
    //---
    //---------------------------------------
    class PackEntry : public PackBase
    {
    public:
        PackEntry();
        ~PackEntry();

        ///
        void swap(PackEntry& rhs);
    private:
        friend class PackReader;

        PackEntry(const PackHeader& header, FileEntry* entries);

        PackEntry(const PackEntry&);
        PackEntry& operator=(const PackEntry&);
    };

    //---------------------------------------
    //---
    //--- PackMemory
    //---
    //---------------------------------------
    class PackMemory : public PackBase
    {
    public:
        PackMemory();
        ~PackMemory();

        u32 read(u8* dst, u32 index, u32 offset, u32 size) const;
        const u8* get(u32 index, u32 offset) const;

        ///
        void swap(PackMemory& rhs);
    private:
        friend class PackReader;

        PackMemory(const PackHeader& header, FileEntry* entries, u32 size, u8* data);

        PackMemory(const PackMemory&);
        PackMemory& operator=(const PackMemory&);

        u32 size_;
        u8* data_;
    };

    //---------------------------------------
    //---
    //--- PackFile
    //---
    //---------------------------------------
    class PackFile : public PackBase
    {
    public:
        PackFile();
        ~PackFile();

        u32 read(u8* dst, u32 index, u32 offset, u32 size);

        ///
        void swap(PackFile& rhs);
    private:
        friend class PackReader;

        PackFile(const PackHeader& header, FileEntry* entries, lcore::ifstream& file);

        PackFile(const PackFile&);
        PackFile& operator=(const PackFile&);

        lcore::ifstream file_;
    };
}
#endif //INC_LPACK_PACK_H__
