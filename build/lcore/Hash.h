#ifndef INC_LCORE_HASH_H__
#define INC_LCORE_HASH_H__
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
@file Hash.h
@author t-sakai
@date 2009/06/28 modified
*/
#include "utility.h"

namespace lcore
{
    inline size_t calc_hash(const unsigned char* v, size_t count)
    {
        size_t val = 216136261U;
        do{
            --count;
            val = 16777619U * val ^ v[count];
        }while(count != 0);

        return val;
    }

    inline size_t calc_hash_string(const unsigned char* v)
    {
        size_t val = 216136261U;
        do{
            val = 16777619U * val ^ *v;
            ++v;
        }while(*v != '\0');

        return val;
    }

    template<typename T>
    struct hash_type_traits
    {
        static std::size_t count(const T& /*t*/)
        {
            return sizeof(T); //sizeofが0になることはない
        }
    };

    template<typename T>
    struct hasher
    {
        inline static size_t calc(const T& t)
        {
            const unsigned char* v = reinterpret_cast<const unsigned char*>(&t);
            size_t count = hash_type_traits<T>::count(t);
            return calc_hash(v, count);
        }
    };

#define HASH_TYPE_TRAITS_PRIMITIVE_CREATOR(type) \
    template<>\
    struct hasher<type>\
    {\
        inline static size_t calc(const type t)\
        {\
            const unsigned char* v = reinterpret_cast<const unsigned char*>(&t);\
            size_t count = hash_type_traits<type>::count(t);\
            return calc_hash(v, count);\
        }\
    };\

HASH_TYPE_TRAITS_PRIMITIVE_CREATOR(char)
HASH_TYPE_TRAITS_PRIMITIVE_CREATOR(unsigned char)
HASH_TYPE_TRAITS_PRIMITIVE_CREATOR(short)
HASH_TYPE_TRAITS_PRIMITIVE_CREATOR(unsigned short)
HASH_TYPE_TRAITS_PRIMITIVE_CREATOR(int)
HASH_TYPE_TRAITS_PRIMITIVE_CREATOR(unsigned int)
HASH_TYPE_TRAITS_PRIMITIVE_CREATOR(long)
HASH_TYPE_TRAITS_PRIMITIVE_CREATOR(unsigned long)
HASH_TYPE_TRAITS_PRIMITIVE_CREATOR(long long)
HASH_TYPE_TRAITS_PRIMITIVE_CREATOR(unsigned long long)
HASH_TYPE_TRAITS_PRIMITIVE_CREATOR(float)
HASH_TYPE_TRAITS_PRIMITIVE_CREATOR(double)

#undef HASH_TYPE_TRAITS_PRIMITIVE_CREATOR


#define HASH_TYPE_TRAITS_PRIMITIVE_POINTER_CREATOR(type) \
    template<>\
    struct hasher<type*>\
    {\
        inline static size_t calc(const type* t)\
        {\
            const unsigned char* v = reinterpret_cast<const unsigned char*>(t);\
            size_t count = hash_type_traits<type>::count(*t);\
            return calc_hash(v, count);\
        }\
    };\

//HASH_TYPE_TRAITS_PRIMITIVE_POINTER_CREATOR(char)
//HASH_TYPE_TRAITS_PRIMITIVE_POINTER_CREATOR(unsigned char)
HASH_TYPE_TRAITS_PRIMITIVE_POINTER_CREATOR(short)
HASH_TYPE_TRAITS_PRIMITIVE_POINTER_CREATOR(unsigned short)
HASH_TYPE_TRAITS_PRIMITIVE_POINTER_CREATOR(int)
HASH_TYPE_TRAITS_PRIMITIVE_POINTER_CREATOR(unsigned int)
HASH_TYPE_TRAITS_PRIMITIVE_POINTER_CREATOR(long)
HASH_TYPE_TRAITS_PRIMITIVE_POINTER_CREATOR(unsigned long)
HASH_TYPE_TRAITS_PRIMITIVE_POINTER_CREATOR(long long)
HASH_TYPE_TRAITS_PRIMITIVE_POINTER_CREATOR(unsigned long long)
HASH_TYPE_TRAITS_PRIMITIVE_POINTER_CREATOR(float)
HASH_TYPE_TRAITS_PRIMITIVE_POINTER_CREATOR(double)

#undef HASH_TYPE_TRAITS_PRIMITIVE_POINTER_CREATOR

    //template<>
    //struct hasher<std::string>
    //{
    //    inline static std::size_t calc(const std::string& t)
    //    {
    //        const unsigned char* v = reinterpret_cast<const unsigned char*>(t.c_str());
    //        std::size_t count = t.size();
    //        return calc_hash(v, count);
    //    }
    //};

    // char*, unsigned char* は文字列と見なして特殊化
    template<>
    struct hasher<char*>
    {
        inline static size_t calc(const char* t)
        {
            return calc_hash_string(reinterpret_cast<const unsigned char*>(t));
        }
    };

    template<>
    struct hasher<unsigned char*>
    {
        inline static size_t calc(const unsigned char* t)
        {
            return calc_hash_string(t);
        }
    };



    namespace hash_detail
    {
        template<class T>
        struct type_traits
        {
            typedef T* pointer;
            typedef const T* const_pointer;
            typedef T& reference;
            typedef const T& const_reference;
            typedef T& param_tye;
            typedef const T& const_param_tye;

            static void swap(T& lhs, T& rhs)
            {
                lcore::swap(lhs, rhs);
            }
        };

        template<class T>
        struct type_traits<T*>
        {
            typedef T** pointer;
            typedef const T** const_pointer;
            typedef T*& reference;
            typedef const T*& const_reference;
            typedef T* param_tye;
            typedef const T* const_param_tye;

            static void swap(T*& lhs, T*& rhs)
            {
                lcore::swap(lhs, rhs);
            }
        };

        template<typename T> struct PrimeList
        {
            static size_t const value_[];
            static ptrdiff_t const length_;
        };


        template<typename T>
        size_t const PrimeList<T>::value_[] =
        {
            5ul, 11ul, 17ul, 29ul, 37ul, 53ul, 67ul, 79ul,
            97ul, 131ul, 193ul, 257ul, 389ul, 521ul, 769ul,
            1031ul, 1543ul, 2053ul, 3079ul, 6151ul, 12289ul, 24593ul,
            49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
            1572869ul, 3145739ul, 6291469ul, 12582917ul, 25165843ul,
            50331653ul, 100663319ul, 201326611ul, 402653189ul, 805306457ul,
            1610612741ul, 3221225473ul, 4294967291ul,
        };

        template<typename T>
        ptrdiff_t const PrimeList<T>::length_ = 40;

        typedef PrimeList<size_t> prime_list;


        inline size_t next_prime(size_t n)
        {
            const size_t * const prime_list_begin = prime_list::value_;
            const size_t * const prime_list_end = prime_list_begin + prime_list::length_;
            const size_t *bound = lower_bound(prime_list_begin, prime_list_end, n);
            if(bound == prime_list_end){
                --bound;
            }
            return *bound;
        }

        inline size_t prev_prime(size_t n)
        {
            const size_t * const prime_list_begin = prime_list::value_;
            const size_t * const prime_list_end = prime_list_begin + prime_list::length_;
            const size_t *bound = upper_bound(prime_list_begin, prime_list_end, n);
            if(bound == prime_list_begin){
                --bound;
            }
            return *bound;
        }



        //------------------------------------------------------
        /// newによるメモリアロケータ
        template<class T>
        class AllocatorNew
        {
        public:
            typedef size_t size_type;
            typedef ptrdiff_t difference_type;
            typedef T* pointer;
            typedef const T* const_pointer;
            typedef T& reference;
            typedef const T& const_reference;
            typedef T value_type;


            /// Constructor
            AllocatorNew() throw()
            {
            }

            /// Copy constructor
            AllocatorNew(const AllocatorNew& ) throw()
            {
            }

            /// Copy constructor
            template<class OtherType>
            AllocatorNew(const AllocatorNew<OtherType>& ) throw()
            {
            }

            /// Destructor
            ~AllocatorNew() throw()
            {
            }


            pointer address(reference ref) const
            {
                return &ref;
            }

            const_pointer address(const_reference ref) const
            {
                return &ref;
            }

            size_type max_size() const throw()
            {
                size_type count = (size_type)(-1)/sizeof(value_type);
                return ((0<count)? count : 1);
            }

            pointer allocate(size_type size) const
            {
                return allocate(size, (pointer)NULL);
            }

            pointer allocate(size_type size, const void*) const
            {
                return pointer( LIME_OPERATOR_NEW(size * sizeof(value_type)) );
            }

            void deallocate(pointer ptr, size_type = 0) const
            {
                LIME_OPERATOR_DELETE(ptr);
            }

            void construct(pointer ptr) const
            {
                LIME_PLACEMENT_NEW(reinterpret_cast<void*>(ptr)) value_type;
            }

            void construct(pointer ptr, const_reference ref) const
            {
                LIME_PLACEMENT_NEW(reinterpret_cast<void*>(ptr)) value_type(ref);
            }

            void destroy(pointer ptr) const
            {
                if(ptr) ptr->~T();
            }

            template<class OtherType>
            struct rebind
            {
                typedef AllocatorNew<OtherType> other;
            };

        };

        /// newによるメモリアロケータ
        template<class T>
        class AllocatorNew<T*>
        {
        public:
            typedef size_t size_type;
            typedef ptrdiff_t difference_type;
            typedef T** pointer;
            typedef const T** const_pointer;
            typedef T* reference;
            typedef const T* const_reference;
            typedef T* value_type;


            /// Constructor
            AllocatorNew() throw()
            {
            }

            /// Copy constructor
            AllocatorNew(const AllocatorNew& ) throw()
            {
            }

            /// Copy constructor
            template<class OtherType>
            AllocatorNew(const AllocatorNew<OtherType>& ) throw()
            {
            }

            /// Destructor
            ~AllocatorNew() throw()
            {
            }


            pointer address(reference ref) const
            {
                return &ref;
            }

            const_pointer address(const_reference ref) const
            {
                return &ref;
            }

            size_type max_size() const throw()
            {
                size_type count = (size_type)(-1)/sizeof(value_type);
                return ((0<count)? count : 1);
            }

            pointer allocate(size_type size) const
            {
                return allocate(size, (pointer)0);
            }

            pointer allocate(size_type size, const void*) const
            {
                return pointer( ::operator new(size * sizeof(value_type)) );
            }

            void deallocate(pointer ptr, size_type = 0) const
            {
                LIME_OPERATOR_DELETE(ptr);
            }

            void construct(pointer ptr) const
            {
                LIME_PLACEMENT_NEW(reinterpret_cast<void*>(ptr)) value_type;
            }

            void construct(pointer ptr, const_reference ref) const
            {
                LIME_PLACEMENT_NEW(reinterpret_cast<void*>(ptr)) value_type(const_cast<reference>(ref));
            }

            void destroy(pointer) const
            {
                //ptr->~T();
            }

            template<class OtherType>
            struct rebind
            {
                typedef AllocatorNew<OtherType> other;
            };

        };


        template<class Allocator>
        class AllocatorArrayConstructor
        {
        public:
            typedef typename Allocator::pointer pointer;
            typedef typename Allocator::size_type size_type;
            typedef typename Allocator::value_type value_type;


            AllocatorArrayConstructor(Allocator& allocator)
                :allocator_(allocator),
                ptr_(NULL)
            {
            }

            ~AllocatorArrayConstructor()
            {
                allocator_.deallocate(ptr_);
            }

            void construct(size_type count)
            {
                pointer ptr = allocator_.allocate(count * sizeof(value_type));
                for(size_type i=0; i<count; ++i){
                    allocator_.construct(&(ptr[i]));
                }
                ptr_ = ptr;
            }

            pointer get()
            {
                return ptr_;
            }

            pointer release()
            {
                pointer ret = ptr_;
                ptr_ = NULL;
                return ret;
            }

        private:
            AllocatorArrayConstructor(const AllocatorArrayConstructor&);
            AllocatorArrayConstructor& operator=(const AllocatorArrayConstructor&);

            Allocator& allocator_;
            pointer ptr_;
        };
    }
}

#endif //INC_LCORE_HASH_H__
