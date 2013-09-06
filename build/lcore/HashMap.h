#ifndef INC_LCORE_HASHMAP_H__
#define INC_LCORE_HASHMAP_H__
/*
Copyright (c) 2005, Google Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

    * Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
copyright notice, this list of conditions and the following disclaimer
in the documentation and/or other materials provided with the
distribution.
    * Neither the name of Google Inc. nor the names of its
contributors may be used to endorse or promote products derived from
this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/
/**
@file HashMap.h
@author t-sakai
@date 2009/06/28 modified
*/
#include "lcore.h"
#include "clibrary.h"
#include "Hash.h"

namespace lcore
{
    namespace hash_detail
    {
        template<class Key, class Value, class Hasher>
        class HashTable
        {
        public:
            typedef Key key_type;
            typedef Value value_type;

            typedef HashTable<Key, Value, Hasher> this_type;

            typedef size_t size_type;
            typedef ptrdiff_t difference_type;

            typedef typename type_traits<Value>::pointer pointer;
            typedef typename type_traits<Value>::const_pointer const_pointer;
            typedef typename type_traits<Value>::reference reference;
            typedef typename type_traits<Value>::const_reference const_reference;
            typedef typename type_traits<Value>::param_tye value_param_type;
            typedef typename type_traits<Value>::const_param_tye const_value_param_type;

            typedef value_type* value_pointer;

            typedef key_type* key_pointer;
            typedef const key_type* const_key_pointer;
            typedef key_type& key_reference;
            typedef const key_type& const_key_reference;
            typedef typename type_traits<Key>::param_tye key_param_type;
            typedef typename type_traits<Key>::const_param_tye const_key_param_type;

            typedef type_traits<Key> key_traits;

            typedef Hasher hasher;

            typedef AllocatorNew<value_type> value_allocator;
            typedef AllocatorNew<key_type> key_allocator;

            HashTable()
                :bucketCount_(0),
                keys_(NULL),
                values_(NULL),
                size_(0)
            {
            }

            explicit HashTable(size_type n)
                :size_(0),
                bucketCount_(next_prime(n))
            {
                createBuckets(bucketCount_);
            }

            HashTable(size_type n, const_key_param_type emptyKey)
                :empty_(emptyKey),
                size_(0),
                bucketCount_(next_prime(n))
            {
                createBuckets(bucketCount_);
            }

            ~HashTable()
            {
                destroyBuckets(0, bucketCount_);
            }

            size_type bucket_count() const
            {
                return bucketCount_;
            }

            size_type max_bucket_count(size_type max_size) const
            {
                return prev_prime(max_size);
            }

            size_type calcHash(const_key_param_type key) const
            {
                return hasher::calc(key) % bucketCount_;
            }

            void createBuckets(size_type count)
            {
                AllocatorArrayConstructor<value_allocator>
                    value_constructor(value_alloc_);
                value_constructor.construct(count);

                AllocatorArrayConstructor<key_allocator>
                    key_constructor(key_alloc_);
                key_constructor.construct(count);

                for(size_type i=0; i<count; ++i){
                    setKey(empty_, key_constructor.get() + i);
                }

                values_ = value_constructor.release();
                keys_ = key_constructor.release();
            }

            void destroyBuckets(size_type first, size_type last)
            {
                while(first != last){
                    value_alloc_.destroy(values_ + first);
                    key_alloc_.destroy(keys_ + first);
                    ++first;
                }
                value_alloc_.deallocate(values_);
                key_alloc_.deallocate(keys_);
                values_ = NULL;
                keys_ = NULL;
            }

            void clear()
            {
                size_type i=0;
                while(i < bucketCount_){
                    value_alloc_.destroy(values_ + i);
                    key_alloc_.destroy(keys_ + i);
                    ++i;
                }

                size_ = 0;
            }

            void setValue(const_value_param_type src, pointer dst)
            {
                value_alloc_.destroy(dst);
                value_alloc_.construct(dst, src);
            }

            void setKey(const_key_param_type src, key_pointer dst)
            {
                key_alloc_.destroy(dst);
                key_alloc_.construct(dst, src);
            }

            bool isEmpty(size_type pos) const
            {
                return (keys_[pos] == empty_);
            }

            bool isValid(size_type pos) const
            {
                return (0<=pos && pos<=bucketCount_);
            }

            void setEmptyKey(const_key_param_type key)
            {
                empty_ = key;
            }

            size_type findEmptyPosition(const_key_param_type key) const
            {
                size_type pos = calcHash(key);
                size_type n = pos;

                do{
                    if(isEmpty(n)){
                        return n;
                    }
                    ++n;
                    if(n>=bucketCount_){
                        n-=bucketCount_;
                        LASSERT(n<bucketCount_);
                    }
                }while(n != pos);

                return end();
            }

            size_type find(const_key_param_type key) const
            {
                size_type pos = calcHash(key);
                size_type n = pos;

                do{
                    if(key == keys_[n]){
                        return n;
                    }

                    if(isEmpty(n)){
                        return end();
                    }
                    ++n;
                    if(n>=bucketCount_){
                        n-=bucketCount_;
                        LASSERT(n<bucketCount_);
                    }
                }while(n != pos);

                return end();
            }

            bool insert(const_key_param_type key, const_value_param_type value)
            {
                size_type pos = find(key);
                if(pos != end()){
                    return false;
                }
                if((size_<<1)>=bucketCount_){
                    expand();
                }
                
                pos = findEmptyPosition(key);
                if(pos != end()){
                    setValue(value, values_ + pos);
                    setKey(key, keys_ + pos);
                    ++size_;
                    return true;
                }
                return false;
            }

            void erase(size_type pos)
            {
                assert(isValid(pos));
                if(isEmpty(pos)){
                    return;
                }
                setKey(empty_, keys_ + pos);
                value_alloc_.destroy(values_ + pos);
                --size_;
            }

            void swap(HashTable& hashTable)
            {
                lcore::swap(values_, hashTable.values_);
                lcore::swap(keys_, hashTable.keys_);
                lcore::swap(bucketCount_, hashTable.bucketCount_);
                lcore::swap(size_, hashTable.size_);

                key_traits::swap(empty_, hashTable.empty_);
            }

            size_type size() const
            {
                return size_;
            }

            size_type begin() const
            {
                for(size_type i=0; i!=end(); ++i){
                    if(!isEmpty(i)){
                        return i;
                    }
                }
                return end();
            }

            size_type end() const
            {
                return bucketCount_;
            }

            size_type next(size_type pos) const
            {
                for(size_type i=pos+1; i!=end(); ++i){
                    if(!isEmpty(i)){
                        return i;
                    }
                }
                return end();
            }

            void expand()
            {
                this_type tmp(bucketCount_+1, empty_);

                size_type itr = begin();
                while(itr != end()){
                    key_type& key = keys_[itr];
                    value_type& value = values_[itr];

                    if(false == tmp.insert(key, value)){
                        LASSERT(false);
                    }

                    itr = next(itr);
                }
                tmp.swap(*this);
            }

            key_type empty_;
            value_pointer values_;
            key_pointer keys_;
            size_type size_;
            size_type bucketCount_;
            value_allocator value_alloc_;
            key_allocator key_alloc_;
        };
    }


    template<class Key, class Value, class Hasher = hasher<Key> >
    class HashMap
    {
        typedef hash_detail::HashTable<Key, Value, Hasher> hash_table;
    public:
        typedef HashMap<Key, Value, Hasher> this_type;
        typedef typename hash_table::size_type size_type;

        typedef Key key_type;
        typedef Value value_type;

        explicit HashMap(size_type size)
            :table_(size)
        {}

        HashMap(size_type size, const key_type& emptyKey)
            :table_(size, emptyKey)
        {
        }

        ~HashMap()
        {
        }

        size_type capacity() const
        {
            return table_.bucket_count();
        }

        size_type size() const
        {
            return table_.size();
        }

        void clear()
        {
            table_.clear();
        }

        const value_type& getValue(size_type pos) const
        {
            assert(table_.isValid(pos));
            assert(!table_.isEmpty(pos));
            return table_.values_[pos];
        }

        value_type& getValue(size_type pos)
        {
            assert(table_.isValid(pos));
            assert(!table_.isEmpty(pos));
            return table_.values_[pos];
        }

        const key_type& getKey(size_type pos) const
        {
            assert(table_.isValid(pos));
            assert(!table_.isEmpty(pos));
            return table_.keys_[pos];
        }

        key_type& getKey(size_type pos)
        {
            assert(table_.isValid(pos));
            assert(!table_.isEmpty(pos));
            return table_.keys_[pos];
        }

        bool insert(const key_type& key, const value_type& value)
        {
            return table_.insert(key, value);
        }

        size_type find(const key_type& key) const
        {
            return table_.find(key);
        }

        bool isValid(size_type pos) const
        {
            return table_.isValid();
        }

        bool isEnd(size_type pos) const
        {
            return (pos == table_.end());
        }

        void erase(size_type pos)
        {
            table_.erase(pos);
        }

        void swap(this_type& hashMap)
        {
            table_.swap(hashMap.table_);
        }

        void setEmptyKey(const key_type& key)
        {
            table_.setEmptyKey(key);
        }

        size_type begin() const
        {
            return table_.begin();
        }

        size_type end() const
        {
            return table_.end();
        }

        size_type next(size_type pos) const
        {
            return table_.next(pos);
        }
    private:

        hash_table table_;
    };


    struct StringWrapper
    {
        StringWrapper()
            :str_(NULL)
            ,size_(0)
        {}

        StringWrapper(const Char* str, u32 size)
            :str_(str)
            ,size_(size)
        {}

        bool operator==(const StringWrapper& rhs) const
        {
            if(size_ != rhs.size_){
                return false;
            }
            return (lcore::memcmp(str_, rhs.str_, size_) == 0);
        }

        bool operator!=(const StringWrapper& rhs) const
        {
            if(size_ != rhs.size_){
                return true;
            }
            return (lcore::memcmp(str_, rhs.str_, size_) != 0);
        }

        operator const Char*() const{ return str_;}

        static StringWrapper empty_;

        const Char* str_;
        u32 size_;
    };

    template<>
    struct hasher<StringWrapper>
    {
        inline static size_t calc(const StringWrapper& t)
        {
            const unsigned char* v = reinterpret_cast<const unsigned char*>(t.str_);
            return calc_hash(v, t.size_);
        }
    };

    //----------------------------------------------------------------
    /**
    @brief 文字列のためのハッシュマップ
    */
    template<class Value>
    class HashMapCharArray
    {
    private:

    public:
        typedef StringWrapper key_type;
        typedef hasher<key_type> hasher_type;
        typedef Value value_type;
        typedef const Char* string_type;

    private:
        typedef hash_detail::HashTable<key_type, Value, hasher_type > hash_table;

    public:
        typedef HashMapCharArray<Value> this_type;
        typedef typename hash_table::size_type size_type;

        HashMapCharArray()
            :table_(0, StringWrapper::empty_)
        {
        }

        HashMapCharArray(size_type size)
            :table_(size, StringWrapper::empty_)
        {
        }

        ~HashMapCharArray()
        {
        }

        size_type capacity() const
        {
            return table_.bucket_count();
        }

        size_type size() const
        {
            return table_.size();
        }

        const value_type& getValue(size_type pos) const
        {
            assert(table_.isValid(pos));
            assert(!table_.isEmpty(pos));
            return table_.values_[pos];
        }

        value_type& getValue(size_type pos)
        {
            assert(table_.isValid(pos));
            assert(!table_.isEmpty(pos));
            return table_.values_[pos];
        }

        string_type getKey(size_type pos) const
        {
            assert(table_.isValid(pos));
            assert(!table_.isEmpty(pos));
            return table_.keys_[pos].str_;
        }

        string_type getKey(size_type pos)
        {
            assert(table_.isValid(pos));
            assert(!table_.isEmpty(pos));
            return table_.keys_[pos].str_;
        }

        bool insert(string_type key, const value_type& value)
        {
            LASSERT(key != NULL);
            u32 length = lcore::strlen(key);
            return table_.insert(StringWrapper(key, length+1), value);
        }

        size_type find(string_type key) const
        {
            LASSERT(key != NULL);
            u32 length = lcore::strlen(key);
            return table_.find(StringWrapper(key, length+1));
        }

        bool insert(string_type key, u32 length, const value_type& value)
        {
            LASSERT(key != NULL);
            LASSERT(lcore::strlen(key) == length);
            return table_.insert(StringWrapper(key, length+1), value);
        }

        size_type find(string_type key, u32 length) const
        {
            LASSERT(key != NULL);
            LASSERT(lcore::strlen(key) == length);
            return table_.find(StringWrapper(key, length+1));
        }


        bool isValid(size_type pos) const
        {
            return table_.isValid();
        }

        bool isEnd(size_type pos) const
        {
            return (pos == table_.end());
        }

        void erase(size_type pos)
        {
            table_.erase(pos);
        }

        void erase(string_type key)
        {
            size_type pos = find(key);
            if(pos != table_.end()){
                erase(pos);
            }
        }

        void erase(string_type key, u32 length)
        {
            size_type pos = find(key, length);
            if(pos != table_.end()){
                erase(pos);
            }
        }

        void swap(this_type& hashMap)
        {
            table_.swap(hashMap.table_);
        }

        void setEmptyKey(const key_type& key)
        {
            table_.setEmptyKey(key);
        }

        size_type begin() const
        {
            return table_.begin();
        }

        size_type end() const
        {
            return table_.end();
        }

        size_type next(size_type pos) const
        {
            return table_.next(pos);
        }
    private:

        hash_table table_;
    };

}
#endif //INC_LCORE_HASHMAP_H__
