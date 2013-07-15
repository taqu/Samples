#ifndef INC_LCORE_FIXEDARRAY_H__
#define INC_LCORE_FIXEDARRAY_H__
/**
@file FixedArray.h
@author t-sakai
@date 2009/01/05 create
*/
#include "lcore.h"

namespace lcore
{
    /// 動的確保配列の代替
    template<class T>
    class FixedArray
    {
    public:
        typedef u32 size_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T value_type;
        typedef FixedArray<T> this_type;

        typedef pointer iterator;
        typedef const_pointer const_iterator;

        explicit FixedArray(size_type size)
            :size_(size),
            data_(0)
        {
            if(size_ > 0){
                data_ = LIME_NEW T[size_];
            }
        }

        ~FixedArray()
        {
            LIME_DELETE_ARRAY(data_);
        }

        /// サイズ
        size_type size() const
        {
            return size_;
        }

        /// 先頭のイテレータ
        const_iterator begin() const
        {
            return data_;
        }

        /// 先頭のイテレータ
        iterator begin()
        {
            return data_;
        }

        /// 最後の次のイテレータ
        const_iterator end() const
        {
            return (data_ + size_);
        }

        /// 最後の次のイテレータ
        iterator end()
        {
            return (data_ + size_);
        }

        /// インデックスで参照
        reference at(size_type index)
        {
            return getRef(index);
        }

        /// インデックスで参照
        const_reference at(size_type index) const
        {
            return getRef(index);
        }

        /// インデックスで参照
        reference operator [](size_type index)
        {
            return getRef(index);
        }

        /// インデックスで参照
        const_reference operator [](size_type index) const
        {
            return getRef(index);
        }

        ///// 先頭を参照
        //reference front()
        //{
        //    return getRef(0);
        //}

        ///// 先頭を参照
        //const_reference front() const
        //{
        //    return getRef(0);
        //}

        ///// 後尾を参照
        //reference back()
        //{
        //    return getRef(_size-1);
        //}

        ///// 後尾を参照
        //const_reference back() const
        //{
        //    return getRef(_size-1);
        //}

        /// 中身入れ替え
        void swap(this_type& right)
        {
            lcore::swap(size_, right.size_);
            lcore::swap(data_, right.data_);
        }

        /// 代入演算子
        //my_type& operator=(my_type& right)
        //{
        //    eraseAll();
        //    _size = right.size();
        //    if(_size > 0){
        //        _data = _allocator.allocate(_size);
        //        size_type i = 0;
        //        for(iterator itr = begin();
        //            itr != end();
        //            ++itr)
        //        {
        //            new ( itr ) value_type(right[i]);
        //            ++i;
        //        }
        //    }
        //    return *this;
        //}

    private:
        FixedArray(const FixedArray&);
        FixedArray& operator=(const FixedArray&);

        reference getRef(size_type index)
        {
            LASSERT(0 <= index
                && index < size_);
            return data_[index];
        }

        const_reference getRef(size_type index) const
        {
            LASSERT(0 <= index
                && index < size_);
            return data_[index];
        }

        size_type size_;
        pointer data_;
    };
}

#endif //INC_LCORE_FIXEDARRAY_H__
