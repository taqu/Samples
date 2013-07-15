#ifndef INC_LCORE_SHARED_PTR_H__
#define INC_LCORE_SHARED_PTR_H__
/**
@file shared_ptr.h
@author t-sakai
@date 2008/12/14 create

*/

namespace smart_ptr
{
    //---------------------------------------------------
    //---
    //--- void参照のための特性
    //---
    //---------------------------------------------------
    template<class T>
    struct shared_ptr_traits
    {
        typedef T& reference_type;
    };

    template<>
    struct shared_ptr_traits<void>
    {
        typedef void reference_type;
    };


    //---------------------------------------------------
    //---
    //--- default_deleter
    //---
    //---------------------------------------------------
    template<class T>
    class default_deleter
    {
    public:
        default_deleter()
        {
        }

        ~default_deleter()
        {}

        void operator()(void* p) const
        {
            LIME_DELETE_NONULL static_cast<T*>(p);
        }
    };


    //---------------------------------------------------
    //---
    //--- counted_base
    //---
    //---------------------------------------------------
    class counted_base
    {
    public:
        counted_base()
            :_use_count(1)
        {}

        virtual ~counted_base()
        {}

        virtual void dispose() =0;

        void release()
        {
            if(--_use_count == 0){
                dispose();
                LIME_DELETE_NONULL this;
            }
        }

        void addRef()
        {
            ++_use_count;
        }

        long use_count() const
        {
            return _use_count;
        }
    private:
        long _use_count;
    };


    //---------------------------------------------------
    //---
    //--- counted_impl
    //---
    //---------------------------------------------------
    template<class T>
    class counted_impl : public counted_base
    {
    public:
        explicit counted_impl(T* pointer)
            :_pointer(pointer)
        {
        }

        void dispose()
        {
            LIME_DELETE_NONULL _pointer;
        }
    private:
        T *_pointer;
    };

    //---------------------------------------------------
    //---
    //--- counted_impl_del
    //---
    //---------------------------------------------------
    template<class T, class Deleter>
    class counted_impl_del : public counted_base
    {
    public:
        explicit counted_impl_del(T* pointer, Deleter deleter)
            :_pointer(pointer),
            _deleter(deleter)
        {
        }

        void dispose()
        {
            _deleter(_pointer);
        }
    private:
        T *_pointer;
        Deleter _deleter;
    };

    //---------------------------------------------------
    //---
    //--- shared_count
    //---
    //---------------------------------------------------
    class shared_count
    {
    public:
        shared_count()
            :_impl(0)
        {}

        template<class T>
        explicit shared_count(T* ptr)
            :_impl(new counted_impl_del<T, default_deleter<T> >(ptr, default_deleter<T>()) )
        {
        }

        template<class T, class Deleter>
        explicit shared_count(T* ptr, Deleter deleter)
            :_impl(new counted_impl_del<T, Deleter>(ptr, deleter) )
        {
        }

        shared_count(const shared_count& rhs)
            :_impl(rhs._impl)
        {
            if(_impl != 0){
                _impl->addRef();
            }
        }

        ~shared_count()
        {
            if(_impl != 0){
                _impl->release();
            }
        }

        shared_count& operator=(const shared_count& rhs)
        {
            
            if(rhs._impl != _impl){
                counted_base* tmp = rhs._impl;

                if(tmp != 0) tmp->addRef();
                if(_impl != 0) _impl->release();
                _impl = tmp;
            }
            return *this;
        }

        long use_count() const
        {
            return (_impl != 0)? _impl->use_count() : 0;
        }

        friend inline bool operator==(const shared_count& p1, const shared_count& p2)
        {
            return (p1._impl == p2._impl);
        }

        friend inline bool operator<(const shared_count& p1, const shared_count& p2)
        {
            return (p1._impl < p2._impl);
        }

    private:
        counted_base *_impl;
    };

    //---------------------------------------------------
    //---
    //--- shared_ptr
    //---
    //---------------------------------------------------
    template<class T>
    class shared_ptr
    {
    private:
        typedef shared_ptr<T> this_type;

    public:
        typedef T value_type;
        typedef T* pointer_type;
        typedef typename shared_ptr_traits<T>::reference_type reference_type;    

        template<class U> friend class shared_ptr;

        shared_ptr()
            :_pointer(0),
            _count()
        {}

        explicit shared_ptr(pointer_type ptr)
            :_pointer(ptr)
            ,_count(ptr)
        {}

        shared_ptr(const shared_ptr& ptr)
            :_pointer(ptr._pointer),
            _count(ptr._count)
        {
        }

        template<typename U>
        explicit shared_ptr(U* ptr)
            :_pointer(ptr),
            _count(ptr)
        {
        }

        template<typename U>
        shared_ptr( const shared_ptr<U>& ptr)
            :_pointer(ptr._pointer),
            _count(ptr._pointer)
        {
        }

        template<typename U, typename Deleter>
        shared_ptr(U* ptr, Deleter deleter)
            :_pointer(ptr._pointer),
            _count(ptr._pointer, deleter)
        {
        }

        ~shared_ptr()
        {
        }

        reference_type operator*() const
        {
            //assert(_pointer != 0);
            return *_pointer;
        }

        pointer_type operator->() const
        {
            //assert(_pointer != 0);
            return _pointer;
        }

        pointer_type get() const
        {
            //assert(_pointer != 0);
            return _pointer;
        }

        operator bool () const
        {
            return (_pointer != 0);
        }

        bool operator!() const
        {
            return (_pointer == 0);
        }

        long use_count() const
        {
            return _count.use_count();
        }
    private:
        pointer_type _pointer;
        shared_count _count;
    };

    template<class T, class U>
    inline bool operator==(const shared_ptr<T>& p1, const shared_ptr<U>& p2)
    {
        return (p1.get() == p2.get());
    }

    template<class T, class U>
    inline bool operator!=(const shared_ptr<T>& p1, const shared_ptr<U>& p2)
    {
        return !(p1 == p2);
    }

}
#endif //INC_LCORE_SHARED_PTR_H__
