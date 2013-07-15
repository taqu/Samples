#ifndef INC_VECTOR_H__
#define INC_VECTOR_H__
/**
@file Vector.h
@author t-sakai
@date 2010/01/04 create
*/
#include "lcore.h"

namespace lcore
{
    template<class T>
    class vector_arena
    {
    public:
        static const s32 DEFAULT_INCREMENT_SIZE = 64;

        typedef u32 size_type;
        typedef T* iterator;
        typedef const T* const_iterator;

        vector_arena();
        explicit vector_arena(u32 incSize);
        vector_arena(u32 size, u32 incSize);
        vector_arena(const vector_arena<T>& rhs);
        ~vector_arena();

        u32 size() const{ return size_;}
        u32 capacity() const{ return capacity_;}

        T& operator[](u32 index)
        {
            LASSERT(0<=index && index<size_);
            return items_[index];
        }

        const T& operator[](u32 index) const
        {
            LASSERT(0<=index && index<size_);
            return items_[index];
        }

        T& front()
        {
            LASSERT(0<capacity_);
            return items_[0];
        }

        const T& front() const
        {
            LASSERT(0<capacity_);
            return items_[0];
        }

        T& back()
        {
            LASSERT(0<size_);
            return items_[size_-1];
        }

        const T& back() const
        {
            LASSERT(0<size_);
            return items_[size_-1];
        }


        void push_back(const T& t);
        void pop_back();

        iterator begin(){ return items_;}
        const_iterator begin() const{ return items_;}

        iterator end(){ return items_ + size_;}
        const_iterator end() const{ return items_ + size_;}

        void clear();
        void swap(vector_arena& rhs);
        void reserve(u32 capacity);
        void resize(u32 size);

        void removeAt(u32 index);

        inline void setIncSize(u32 size);
    private:
        u32 incSize_;
        u32 capacity_;
        u32 size_;
        T *items_;
    };

    template<class T>
    vector_arena<T>::vector_arena()
        :incSize_(DEFAULT_INCREMENT_SIZE)
        ,capacity_(0)
        ,size_(0)
    {
        items_ = NULL;
    }

    template<class T>
    vector_arena<T>::vector_arena(u32 incSize)
        :incSize_(incSize)
        ,capacity_(0)
        ,size_(0)
    {
        LASSERT(incSize>0);
        items_ = NULL;
    }

    template<class T>
    vector_arena<T>::vector_arena(u32 size, u32 incSize)
        :incSize_(incSize)
        ,capacity_( (size>incSize)? size : incSize )
        ,size_(size)
    {
        LASSERT(incSize>0);

        s8 *buffer = LIME_NEW s8[capacity_*sizeof(T)];

        //items_ = LIME_PLACEMENT_NEW(buffer) T[size_];
        items_ = reinterpret_cast<T*>(buffer);

        for(u32 i=0; i<size_; ++i){
            LIME_PLACEMENT_NEW(&items_[i]) T();
        }
    }

    template<class T>
    vector_arena<T>::vector_arena(const vector_arena<T>& rhs)
        :incSize_(rhs.incSize_)
        ,capacity_(rhs.capacity_)
        ,size_(rhs.size_)
    {
        LASSERT(incSize_>0);

        s8 *buffer = LIME_NEW s8[capacity_*sizeof(T)];

        //items_ = LIME_PLACEMENT_NEW(buffer) T[size_];
        items_ = reinterpret_cast<T*>(buffer);

        for(u32 i=0; i<size_; ++i){
            LIME_PLACEMENT_NEW(&items_[i]) T(rhs.items_[i]);
        }
    }

    template<class T>
    vector_arena<T>::~vector_arena()
    {
        for(u32 i=0; i<size_; ++i){
            items_[i].~T();
        }

        s8 *buffer = reinterpret_cast<s8*>(items_);
        LIME_DELETE_ARRAY(buffer);
        items_ = NULL;
    }

    template<class T>
    void vector_arena<T>::push_back(const T& t)
    {
        if(size_ >= capacity_){
            //新しいバッファ確保
            u32 newCapacity = capacity_ + incSize_;
            T *newItems = reinterpret_cast<T*>( LIME_NEW s8[newCapacity*sizeof(T)] );

            //コピーコンストラクタでコピー。古い要素のデストラクト
            for(u32 i=0; i<size_; ++i){
                LIME_PLACEMENT_NEW(&newItems[i]) T(items_[i]);
                items_[i].~T();
            }
            LIME_PLACEMENT_NEW(&newItems[size_]) T(t);

            //古いバッファ破棄
            s8 *buffer = reinterpret_cast<s8*>(items_);
            LIME_DELETE_ARRAY(buffer);

            items_ = newItems;
            ++size_;
            capacity_ = newCapacity;

        }else{
            LIME_PLACEMENT_NEW(&items_[size_]) T(t);
            ++size_;
        }
    }

    template<class T>
    void vector_arena<T>::pop_back()
    {
        --size_;
        items_[size_].~T();
    }

    template<class T>
    void vector_arena<T>::clear()
    {
        for(u32 i=0; i<size_; ++i){
            items_[i].~T();
        }
        size_ = 0;
    }

    template<class T>
    void vector_arena<T>::swap(vector_arena& rhs)
    {
        lcore::swap(incSize_, rhs.incSize_);
        lcore::swap(capacity_, rhs.capacity_);
        lcore::swap(size_, rhs.size_);
        lcore::swap(items_, rhs.items_);
    }

    template<class T>
    void vector_arena<T>::reserve(u32 capacity)
    {
        if(capacity<=capacity_){
            return;
        }

        //新しいバッファ確保
        T *newItems = reinterpret_cast<T*>( LIME_NEW s8[capacity*sizeof(T)] );

        //コピーコンストラクタでコピー。古い要素のデストラクト
        for(u32 i=0; i<size_; ++i){
            LIME_PLACEMENT_NEW(&newItems[i]) T(items_[i]);
            items_[i].~T();
        }

        //古いバッファ破棄
        s8 *buffer = reinterpret_cast<s8*>(items_);
        LIME_DELETE_ARRAY(buffer);

        items_ = newItems;
        capacity_ = capacity;
    }

    template<class T>
    void vector_arena<T>::resize(u32 size)
    {
        if(size < size_){
            //デストラクト
            for(u32 i=size; i<size_; ++i){
                items_[i].~T();
            }

        }else{
            reserve(size);
            for(u32 i=size_; i<size; ++i){
                LIME_PLACEMENT_NEW(&items_[i]) T;
            }
        }
        size_ = size;
    }

    template<class T>
    void vector_arena<T>::removeAt(u32 index)
    {
        LASSERT(0<=index && index<size_);
        for(u32 i=index+1; i<size_; ++i){
            items_[i-1] = items_[i];
        }
        --size_;
    }

    template<class T>
    inline void vector_arena<T>::setIncSize(u32 size)
    {
        LASSERT(size>0);
        incSize_ = size;
    }



    //-----------------------------------------------------------------
    //---
    //--- vector_arena ポインタ特殊化
    //---
    //-----------------------------------------------------------------
    template<class T>
    class vector_arena<T*>
    {
    public:
        static const s32 DEFAULT_INCREMENT_SIZE = 64;

        typedef u32 size_type;
        typedef T** iterator;
        typedef const T** const_iterator;

        vector_arena();
        explicit vector_arena(u32 incSize);
        vector_arena(u32 size, u32 incSize);
        vector_arena(const vector_arena<T*>& rhs);
        ~vector_arena();

        u32 size() const{ return size_;}
        u32 capacity() const{ return capacity_;}

        T*& operator[](u32 index)
        {
            LASSERT(0<=index && index<size_);
            return items_[index];
        }

        const T* operator[](u32 index) const
        {
            LASSERT(0<=index && index<size_);
            return items_[index];
        }

        T*& front()
        {
            LASSERT(0<size_);
            return items_[0];
        }

        const T*& front() const
        {
            LASSERT(0<size_);
            return items_[0];
        }

        T*& back()
        {
            LASSERT(0<size_);
            return items_[size_-1];
        }

        const T*& back() const
        {
            LASSERT(0<size_);
            return items_[size_-1];
        }


        void push_back(T* const t);
        void pop_back();

        iterator begin(){ return items_;}
        const_iterator begin() const{ return items_;}

        iterator end(){ return items_ + size_;}
        const_iterator end() const{ return items_ + size_;}

        void clear();
        void swap(vector_arena& rhs);
        void reserve(u32 capacity);
        void resize(u32 size);

        void removeAt(u32 index);

        inline void setIncSize(u32 size);
    private:
        u32 incSize_;
        u32 capacity_;
        u32 size_;
        T** items_;
    };

    template<class T>
    vector_arena<T*>::vector_arena()
        :incSize_(DEFAULT_INCREMENT_SIZE)
        ,capacity_(0)
        ,size_(0)
    {
        items_ = NULL;
    }

    template<class T>
    vector_arena<T*>::vector_arena(u32 incSize)
        :incSize_(incSize)
        ,capacity_(0)
        ,size_(0)
    {
        LASSERT(incSize>0);
        items_ = NULL;
    }

    template<class T>
    vector_arena<T*>::vector_arena(u32 size, u32 incSize)
        :incSize_(incSize)
        ,capacity_( (size>incSize)? size : incSize )
        ,size_(size)
    {
        LASSERT(incSize>0);

        items_ = LIME_NEW T*[capacity_];

        for(u32 i=0; i<size_; ++i){
            items_[i] = NULL;
        }
    }

    template<class T>
    vector_arena<T*>::vector_arena(const vector_arena<T*>& rhs)
        :incSize_(rhs.incSize_)
        ,capacity_(rhs.capacity_)
        ,size_(rhs.size_)
    {
        LASSERT(incSize_>0);

        items_ = LIME_NEW T*[capacity_];

        for(u32 i=0; i<size_; ++i){
            items_[i] = rhs.items_[i];
        }
    }

    template<class T>
    vector_arena<T*>::~vector_arena()
    {
        LIME_DELETE_ARRAY(items_);
    }

    template<class T>
    void vector_arena<T*>::push_back(T* const t)
    {
        if(size_ >= capacity_){
            //新しいバッファ確保
            u32 newCapacity = capacity_ + incSize_;
            T** newItems = LIME_NEW T*[newCapacity];

            //コピー
            for(u32 i=0; i<size_; ++i){
                newItems[i] = items_[i];
            }
            //古いバッファ破棄
            LIME_DELETE_ARRAY(items_);

            items_ = newItems;
            capacity_ = newCapacity;

        }
        items_[size_] = t;
        ++size_;
    }

    template<class T>
    void vector_arena<T*>::pop_back()
    {
        LASSERT(size_>0);
        --size_;
    }

    template<class T>
    void vector_arena<T*>::clear()
    {
        size_ = 0;
    }

    template<class T>
    void vector_arena<T*>::swap(vector_arena& rhs)
    {
        lcore::swap(incSize_, rhs.incSize_);
        lcore::swap(capacity_, rhs.capacity_);
        lcore::swap(size_, rhs.size_);
        lcore::swap(items_, rhs.items_);
    }

    template<class T>
    void vector_arena<T*>::reserve(u32 capacity)
    {
        if(capacity<=capacity_){
            return;
        }

        //新しいバッファ確保
        T** newItems = LIME_NEW T*[capacity];

        //コピー
        for(u32 i=0; i<size_; ++i){
            newItems[i] = items_[i];
        }

        //古いバッファ破棄
        LIME_DELETE_ARRAY(items_);

        items_ = newItems;
        capacity_ = capacity;
    }

    template<class T>
    void vector_arena<T*>::resize(u32 size)
    {
        if(size > size_){
            reserve(size);
        }
        size_ = size;
    }

    template<class T>
    void vector_arena<T*>::removeAt(u32 index)
    {
        LASSERT(0<=index && index<size_);
        for(u32 i=index+1; i<size_; ++i){
            items_[i-1] = items_[i];
        }
        --size_;
    }

    template<class T>
    inline void vector_arena<T*>::setIncSize(u32 size)
    {
        LASSERT(size>0);
        incSize_ = size;
    }



    //-----------------------------------------------------------------
    //---
    //--- コンストラクタ・デストラクタを呼ばない簡易版
    //---
    //-----------------------------------------------------------------
    template<class T>
    class simple_vector_arena
    {
    public:
        static const s32 DEFAULT_INCREMENT_SIZE = 64;

        typedef u32 size_type;
        typedef T* iterator;
        typedef const T* const_iterator;

        simple_vector_arena();
        explicit simple_vector_arena(u32 incSize);
        simple_vector_arena(u32 size, u32 incSize);
        simple_vector_arena(const simple_vector_arena<T>& rhs);
        ~simple_vector_arena();

        u32 size() const{ return size_;}
        u32 capacity() const{ return capacity_;}

        T& operator[](u32 index)
        {
            LASSERT(0<=index && index<size_);
            return items_[index];
        }

        const T& operator[](u32 index) const
        {
            LASSERT(0<=index && index<size_);
            return items_[index];
        }

        T& front()
        {
            LASSERT(0<capacity_);
            return items_[0];
        }

        const T& front() const
        {
            LASSERT(0<capacity_);
            return items_[0];
        }

        T& back()
        {
            LASSERT(0<size_);
            return items_[size_-1];
        }

        const T& back() const
        {
            LASSERT(0<size_);
            return items_[size_-1];
        }


        void push_back(const T& t);
        void pop_back();

        iterator begin(){ return items_;}
        const_iterator begin() const{ return items_;}

        iterator end(){ return items_ + size_;}
        const_iterator end() const{ return items_ + size_;}

        void clear();
        void swap(simple_vector_arena& rhs);
        void reserve(u32 capacity);
        void resize(u32 size);

        void removeAt(u32 index);

        inline void setIncSize(u32 size);
    private:
        u32 incSize_;
        u32 capacity_;
        u32 size_;
        T *items_;
    };

    template<class T>
    simple_vector_arena<T>::simple_vector_arena()
        :incSize_(DEFAULT_INCREMENT_SIZE)
        ,capacity_(0)
        ,size_(0)
    {
        items_ = NULL;
    }

    template<class T>
    simple_vector_arena<T>::simple_vector_arena(u32 incSize)
        :incSize_(incSize)
        ,capacity_(0)
        ,size_(0)
    {
        LASSERT(incSize>0);
        items_ = NULL;
    }

    template<class T>
    simple_vector_arena<T>::simple_vector_arena(u32 size, u32 incSize)
        :incSize_(incSize)
        ,capacity_( (size>incSize)? size : incSize )
        ,size_(size)
    {
        LASSERT(incSize>0);
        s8 *buffer = LIME_NEW s8[capacity_*sizeof(T)];
        items_ = reinterpret_cast<T*>(buffer);
    }

    template<class T>
    simple_vector_arena<T>::simple_vector_arena(const simple_vector_arena<T>& rhs)
        :incSize_(rhs.incSize_)
        ,capacity_(rhs.capacity_)
        ,size_(rhs.size_)
    {
        LASSERT(incSize_>0);

        s8 *buffer = LIME_NEW s8[capacity_*sizeof(T)];

        items_ = reinterpret_cast<T*>(buffer);

        for(u32 i=0; i<size_; ++i){
            items_[i] = rhs.items_[i];
        }
    }

    template<class T>
    simple_vector_arena<T>::~simple_vector_arena()
    {
        s8 *buffer = reinterpret_cast<s8*>(items_);
        LIME_DELETE_ARRAY(buffer);
        items_ = NULL;
    }

    template<class T>
    void simple_vector_arena<T>::push_back(const T& t)
    {
        if(size_ >= capacity_){
            //新しいバッファ確保
            u32 newCapacity = capacity_ + incSize_;
            T *newItems = reinterpret_cast<T*>( LIME_NEW s8[newCapacity*sizeof(T)] );

            //コピー
            for(u32 i=0; i<size_; ++i){
                newItems[i] = items_[i];
            }

            //古いバッファ破棄
            s8 *buffer = reinterpret_cast<s8*>(items_);
            LIME_DELETE_ARRAY(buffer);

            items_ = newItems;
            capacity_ = newCapacity;

        }
        items_[size_] = t;
        ++size_;
    }

    template<class T>
    void simple_vector_arena<T>::pop_back()
    {
        LASSERT(size_>0);
        --size_;
    }

    template<class T>
    void simple_vector_arena<T>::clear()
    {
        size_ = 0;
    }

    template<class T>
    void simple_vector_arena<T>::swap(simple_vector_arena& rhs)
    {
        lcore::swap(incSize_, rhs.incSize_);
        lcore::swap(capacity_, rhs.capacity_);
        lcore::swap(size_, rhs.size_);
        lcore::swap(items_, rhs.items_);
    }

    template<class T>
    void simple_vector_arena<T>::reserve(u32 capacity)
    {
        if(capacity<=capacity_){
            return;
        }

        //新しいバッファ確保
        T *newItems = reinterpret_cast<T*>( LIME_NEW s8[capacity*sizeof(T)] );

        //コピー
        for(u32 i=0; i<size_; ++i){
            newItems[i] = items_[i];
        }

        //古いバッファ破棄
        s8 *buffer = reinterpret_cast<s8*>(items_);
        LIME_DELETE_ARRAY(buffer);

        items_ = newItems;
        capacity_ = capacity;
    }

    template<class T>
    void simple_vector_arena<T>::resize(u32 size)
    {
        if(size > size_){
            reserve(size);
        }
        size_ = size;
    }

    template<class T>
    void simple_vector_arena<T>::removeAt(u32 index)
    {
        LASSERT(0<=index && index<size_);
        for(u32 i=index+1; i<size_; ++i){
            items_[i-1] = items_[i];
        }
        --size_;
    }

    template<class T>
    inline void simple_vector_arena<T>::setIncSize(u32 size)
    {
        LASSERT(size>0);
        incSize_ = size;
    }
}

#endif //INC_VECTOR_H__
