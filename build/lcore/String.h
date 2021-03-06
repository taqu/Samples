﻿#ifndef INC_LCORE_STRING_H__
#define INC_LCORE_STRING_H__
/**
@file String.h
@author t-sakai
@date 2009/09/10 create
*/
#include <stdarg.h>
#include "lcore.h"
#include "Hash.h"
#include "liostream.h"

namespace lcore
{
    //---------------------------------------------
    //---
    //--- String
    //---
    //---------------------------------------------
    /// 固定長マルチバイト文字列
    template<u32 SIZE>
    class String
    {
    public:
        static const u32 Size = SIZE;

        typedef String<SIZE> this_type;
        static const u32 MAX_LENGTH = SIZE-1;

        String()
            :size_(0)
        {
            LASSERT(MAX_LENGTH > 0);
            buffer_[0] = '\0';
        }

        String(const this_type& rhs)
            :size_(rhs.size())
        {
            lcore::memcpy(buffer_, rhs.buffer_, SIZE);
        }

        String(const Char* str, u32 length)
        {
            LASSERT(MAX_LENGTH > 0);
            assign(str, length);
        }

        ~String()
        {
        }

        const Char* c_str() const
        {
            return buffer_;
        }

        Char* c_str()
        {
            return buffer_;
        }

        /// 文字列長を返す
        u32 size() const
        {
            return size_;
        }

        void clear()
        {
            size_ = 0;
            buffer_[0] = '\0';
        }

        void assign(const Char* str, u32 length);

        void assignMemory(const Char* str, u32 size);

        void swap(this_type& rhs);

        bool operator==(const this_type& rhs) const;

        bool operator!=(const this_type& rhs) const
        {
            return !(*this == rhs);
        }

        bool operator==(const Char* str) const;

        bool operator!=(const Char* str) const
        {
            return !(*this == str);
        }

        String& operator=(const String& rhs);

        u32 push_back(const Char* str, u32 len);

        lcore::istream& read(lcore::istream& is);
        lcore::ostream& write(lcore::ostream& os);

        Char& operator[](u32 index)
        {
            LASSERT(0<=index && index=<size_);
            return buffer_[index];
        }

        const Char& operator[](u32 index) const
        {
            LASSERT(0<=index && index=<size_);
            return buffer_[index];
        }

        /// 書式付出力
        int print(const Char* format, ... );
    protected:

        u32 size_; //文字列長
        Char buffer_[SIZE];
    };

    //----------------------------------------------------------
    template<u32 SIZE>
    void String<SIZE>::assign(const Char* str, u32 length)
    {
        LASSERT(str != NULL);
        u32 count = (length>=SIZE)? MAX_LENGTH : length;
        u32 i=0;
        for(; i<count; ++i){
            buffer_[i] = str[i];
            if(str[i] == '\0'){
                break;
            }
        }
        size_ = i;
        buffer_[i] = '\0';
    }

    //----------------------------------------------------------
    template<u32 SIZE>
    void String<SIZE>::assignMemory(const Char* str, u32 size)
    {
        LASSERT(str != NULL);
        u32 count = (size>=SIZE)? MAX_LENGTH : size;
        u32 i=0;
        for(; i<count; ++i){
            buffer_[i] = str[i];
            if(str[i] == '\0'){
                break;
            }
        }
        size_ = i;
        buffer_[i] = '\0';
    }

    //----------------------------------------------------------
    template<u32 SIZE>
    void String<SIZE>::swap(this_type& rhs)
    {
        Char buffer[SIZE];
        memcpy(buffer, rhs.buffer_, rhs.size_ + 1);
        memcpy(rhs.buffer_, buffer_, size_ + 1);
        memcpy(buffer_, buffer, rhs.size_ + 1);

        lcore::swap(size_, rhs.size_);
    }

    //----------------------------------------------------------
    template<u32 SIZE>
    bool String<SIZE>::operator==(const this_type& rhs) const
    {
        if(size_ != rhs.size()){
            return false;
        }
        return (strncmp(buffer_, rhs.buffer_, size_) == 0);
    }

    //----------------------------------------------------------
    template<u32 SIZE>
    bool String<SIZE>::operator==(const Char* str) const
    {
        LASSERT(str != NULL);
        return (0 == lcore::strncmp(buffer_, str, size_));
    }

    //----------------------------------------------------------
    template<u32 SIZE>
    lcore::istream& String<SIZE>::read(lcore::istream& is)
    {
        is.read(buffer_, SIZE);

        u32 i=0;
        for(; i<String<SIZE>::MAX_LENGTH; ++i){
            if(buffer_[i] == '\0'){
                size_ = i;
                return is;
            }
        }
        buffer_[MAX_LENGTH] = '\0';
        size_ = MAX_LENGTH;
        return is;
    }

    //----------------------------------------------------------
    template<u32 SIZE>
    String<SIZE>& String<SIZE>::operator=(const String<SIZE>& rhs)
    {
        if(this != &rhs){
            memcpy(buffer_, rhs.buffer_, SIZE);
            size_ = rhs.size_;
        }
        return *this;
    }

    //----------------------------------------------------------
    template<u32 SIZE>
    u32 String<SIZE>::push_back(const Char* str, u32 len)
    {
        u32 j=0;
        len += size_;
        if(len>MAX_LENGTH){
            len = MAX_LENGTH;
        }

        while(size_<len){
            buffer_[size_] = str[j];
            ++size_;
            ++j;
        }
        buffer_[size_] = '\0';
        return j;
    }

    //----------------------------------------------------------
    template<u32 SIZE>
    lcore::ostream& String<SIZE>::write(lcore::ostream& os)
    {
        os.write(buffer_, SIZE);
        return os;
    }

    //----------------------------------------------------------
    template<u32 SIZE>
    int String<SIZE>::print(const Char* format, ... )
    {
        LASSERT(format != NULL);

        va_list ap;
        va_start(ap, format);

        int ret = vsnprintf(buffer_, SIZE-1, format, ap);

        va_end(ap);

        if(ret<0){
            size_ = 0;
        }else{
            size_ = ret;
        }
        buffer_[size_] = '\0';

        return ret;
    }



    //----------------------------------------------------------
    template<u32 SIZE>
    struct hasher<String<SIZE> >
    {
        inline static u32 calc(const String<SIZE>& t)
        {
            const u8* v = reinterpret_cast<const u8*>(t.c_str());
            u32 count = t.size();
            return calc_hash(v, count);
        }
    };

    template<u32 SIZE>
    inline lcore::istream& operator>>(lcore::istream& is, String<SIZE>& rhs)
    {
        return rhs.read(is);
    }

    template<u32 SIZE>
    inline lcore::ostream& operator<<(lcore::ostream& os, String<SIZE>& rhs)
    {
        return rhs.write(os);
    }

    //---------------------------------------------
    //---
    //--- StringWithHash
    //---
    //---------------------------------------------
    /// 固定長マルチバイト文字列
    template<u32 SIZE>
    class StringWithHash
    {
    public:
        static const u32 Size = SIZE;

        typedef StringWithHash<SIZE> this_type;
        static const u32 MAX_LENGTH = SIZE-1;

        StringWithHash()
            :hash_(0)
            ,size_(0)
        {
            LASSERT(MAX_LENGTH > 0);
            buffer_[0] = '\0';
        }

        StringWithHash(const this_type& rhs)
            :hash_(0)
            ,size_(rhs.size())
        {
            lcore::memcpy(buffer_, rhs.buffer_, SIZE);
        }

        StringWithHash(const Char* str, u32 length)
            :hash_(0)
        {
            LASSERT(MAX_LENGTH > 0);
            assign(str, length);
        }

        ~StringWithHash()
        {
        }

        const Char* c_str() const
        {
            return buffer_;
        }

        Char* c_str()
        {
            return buffer_;
        }

        /// 文字列長を返す
        u32 size() const
        {
            return size_;
        }

        void clear()
        {
            hash_ = 0;
            size_ = 0;
            buffer_[0] = '\0';
        }

        void assign(const Char* str, u32 length);

        void assignMemory(const Char* str, u32 size);

        void swap(this_type& rhs);

        bool operator==(const this_type& rhs) const;

        bool operator!=(const this_type& rhs) const
        {
            return !(*this == rhs);
        }

        bool operator==(const Char* str) const;

        bool operator!=(const Char* str) const
        {
            return !(*this == str);
        }

        StringWithHash& operator=(const StringWithHash& rhs);

        u32 push_back(const Char* str, u32 len);

        lcore::istream& read(lcore::istream& is);
        lcore::ostream& write(lcore::ostream& os);

        Char& operator[](u32 index)
        {
            LASSERT(0<=index && index=<size_);
            return buffer_[index];
        }

        const Char& operator[](u32 index) const
        {
            LASSERT(0<=index && index=<size_);
            return buffer_[index];
        }

        /// 書式付出力
        int print(const Char* format, ... );

        u32 getHash() const{ return hash_;}
        void calcHash();
    protected:
        u32 hash_;
        u32 size_; //文字列長
        Char buffer_[SIZE];
    };

    //----------------------------------------------------------
    template<u32 SIZE>
    void StringWithHash<SIZE>::assign(const Char* str, u32 length)
    {
        LASSERT(str != NULL);
        u32 count = (length>=SIZE)? MAX_LENGTH : length;
        u32 i=0;
        for(; i<count; ++i){
            buffer_[i] = str[i];
            if(str[i] == '\0'){
                break;
            }
        }
        size_ = i;
        buffer_[i] = '\0';
        calcHash();
    }

    //----------------------------------------------------------
    template<u32 SIZE>
    void StringWithHash<SIZE>::assignMemory(const Char* str, u32 size)
    {
        LASSERT(str != NULL);
        u32 count = (size>=SIZE)? MAX_LENGTH : size;
        u32 i=0;
        for(; i<count; ++i){
            buffer_[i] = str[i];
            if(str[i] == '\0'){
                break;
            }
        }
        size_ = i;
        buffer_[i] = '\0';
        calcHash();
    }

    //----------------------------------------------------------
    template<u32 SIZE>
    void StringWithHash<SIZE>::swap(this_type& rhs)
    {
        Char buffer[SIZE];
        memcpy(buffer, rhs.buffer_, rhs.size_ + 1);
        memcpy(rhs.buffer_, buffer_, size_ + 1);
        memcpy(buffer_, buffer, rhs.size_ + 1);

        lcore::swap(hash_, rhs.hash_);
        lcore::swap(size_, rhs.size_);
    }

    //----------------------------------------------------------
    template<u32 SIZE>
    bool StringWithHash<SIZE>::operator==(const this_type& rhs) const
    {
        if(hash_ != rhs.getHash()){
            return false;
        }
        return (strncmp(buffer_, rhs.buffer_, size_) == 0);
    }

    //----------------------------------------------------------
    template<u32 SIZE>
    bool StringWithHash<SIZE>::operator==(const Char* str) const
    {
        LASSERT(str != NULL);
        return (0 == lcore::strncmp(buffer_, str, size_));
    }

    //----------------------------------------------------------
    template<u32 SIZE>
    lcore::istream& StringWithHash<SIZE>::read(lcore::istream& is)
    {
        is.read(buffer_, SIZE);

        u32 i=0;
        for(; i<String<SIZE>::MAX_LENGTH; ++i){
            if(buffer_[i] == '\0'){
                size_ = i;
                calcHash();
                return is;
            }
        }
        buffer_[MAX_LENGTH] = '\0';
        size_ = MAX_LENGTH;
        calcHash();
        return is;
    }

    //----------------------------------------------------------
    template<u32 SIZE>
    StringWithHash<SIZE>& StringWithHash<SIZE>::operator=(const StringWithHash<SIZE>& rhs)
    {
        if(this != &rhs){
            memcpy(buffer_, rhs.buffer_, SIZE);
            size_ = rhs.size_;
            calcHash();
        }
        return *this;
    }

    //----------------------------------------------------------
    template<u32 SIZE>
    u32 StringWithHash<SIZE>::push_back(const Char* str, u32 len)
    {
        u32 j=0;
        len += size_;
        if(len>MAX_LENGTH){
            len = MAX_LENGTH;
        }

        while(size_<len){
            buffer_[size_] = str[j];
            ++size_;
            ++j;
        }
        buffer_[size_] = '\0';
        calcHash();
        return j;
    }

    //----------------------------------------------------------
    template<u32 SIZE>
    lcore::ostream& StringWithHash<SIZE>::write(lcore::ostream& os)
    {
        os.write(buffer_, SIZE);
        return os;
    }

    //----------------------------------------------------------
    template<u32 SIZE>
    int StringWithHash<SIZE>::print(const Char* format, ... )
    {
        LASSERT(format != NULL);

        va_list ap;
        va_start(ap, format);

        int ret = vsnprintf(buffer_, SIZE-1, format, ap);

        va_end(ap);

        if(ret<0){
            size_ = 0;
        }else{
            size_ = ret;
        }
        buffer_[size_] = '\0';

        calcHash();
        return ret;
    }

    //----------------------------------------------------------
    template<u32 SIZE>
    void StringWithHash<SIZE>::calcHash()
    {
        const u8* v = reinterpret_cast<const u8*>(c_str());
        u32 count = size();

        hash_ = 0;
        for(u32 i=0; i<count; ++i){
            hash_ = 33*hash_ + v[i]; //((hash_<<5)+hash_) + v[i];
        }
    }

    //----------------------------------------------------------
    template<u32 SIZE>
    struct hasher<StringWithHash<SIZE> >
    {
        inline static u32 calc(const StringWithHash<SIZE>& t)
        {
            return t.getHash();
        }
    };

    template<u32 SIZE>
    inline lcore::istream& operator>>(lcore::istream& is, StringWithHash<SIZE>& rhs)
    {
        return rhs.read(is);
    }

    template<u32 SIZE>
    inline lcore::ostream& operator<<(lcore::ostream& os, StringWithHash<SIZE>& rhs)
    {
        return rhs.write(os);
    }
}

#endif //INC_LCORE_STRING_H__
