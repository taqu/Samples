#ifndef INC_LCORE_INTRUSIVE_PTR_H__
#define INC_LCORE_INTRUSIVE_PTR_H__
/**
@file intrusive_ptr.h
@author t-sakai
@date 2008/12/18 create

*/
#include "../LangSpec.h" //for NULL define
namespace smart_ptr
{
    //---------------------------------------------------
    //---
    //--- void参照のための特性
    //---
    //---------------------------------------------------
    template<class T>
    struct intrusive_ptr_traits
    {
        typedef T& reference_type;
    };

    template<>
    struct intrusive_ptr_traits<void>
    {
        typedef void reference_type;
    };

    template<class T>
    class intrusive_ptr
    {
    public:
        typedef T value_type;
        typedef T* pointer_type;
        typedef intrusive_ptr<T> this_type;
        typedef typename intrusive_ptr_traits<T>::reference_type reference_type;

        intrusive_ptr()
            :_pointer(NULL)
        {
        }

        explicit intrusive_ptr(T* pointer, bool add_ref = true)
            :_pointer(pointer)
        {
            if(_pointer != NULL
                && add_ref)
            {
                intrusive_ptr_addref(_pointer);
            }
        }

        intrusive_ptr(const intrusive_ptr& rhs)
            :_pointer(rhs._pointer)
        {
            if(_pointer != NULL){
                intrusive_ptr_addref(_pointer);
            }
        }

        template<class U>
        explicit intrusive_ptr(U* ptr)
            :_pointer(ptr)
        {
            if(_pointer != NULL){
                intrusive_ptr_addref(_pointer);
            }
        }

        template<class U>
        intrusive_ptr(const intrusive_ptr<U>& rhs)
            :_pointer(rhs.get())
        {
            if(_pointer != NULL){
                intrusive_ptr_addref(_pointer);
            }
        }

        ~intrusive_ptr()
        {
            if(_pointer != NULL){
                intrusive_ptr_release(_pointer);
            }
        }

        pointer_type get() const
        {
            return _pointer;
        }

        //operator pointer_type()
        //{
        //    return _pointer;
        //}

        //operator const pointer_type() const
        //{
        //    return _pointer;
        //}

        reference_type operator*() const
        {
            return *_pointer;
        }

        pointer_type operator->() const
        {
            return _pointer;
        }

        operator bool() const
        {
            return (_pointer != NULL);
        }

        bool operator!() const
        {
            return (_pointer == NULL);
        }

        intrusive_ptr& operator=(const intrusive_ptr& rhs)
        {
            this_type(rhs).swap(*this);
            return *this;
        }

        intrusive_ptr& operator=(T* rhs)
        {
            this_type(rhs).swap(*this);
            return *this;
        }

        void swap(intrusive_ptr& rhs)
        {
            T* tmp = _pointer;
            _pointer = rhs._pointer;
            rhs._pointer = tmp;
        }
    private:

        T *_pointer;
    };

    template<class T, class U>
    inline bool operator==(const intrusive_ptr<T>& left, const intrusive_ptr<U>& right)
    {
        return left.get() == right.get();
    }

    template<class T, class U>
    inline bool operator!=(const intrusive_ptr<T>& left, const intrusive_ptr<U>& right)
    {
        return left.get() != right.get();
    }

    template<class T, class U>
    inline bool operator==(const intrusive_ptr<T>& left, const U* right)
    {
        return left.get() == right;
    }

    template<class T, class U>
    inline bool operator!=(const intrusive_ptr<T>& left, const U* right)
    {
        return left.get() != right;
    }

    template<class T, class U>
    inline bool operator==(const T* left, const intrusive_ptr<U>& right)
    {
        return left == right.get();
    }

    template<class T, class U>
    inline bool operator!=(const T* left, const intrusive_ptr<U>& right)
    {
        return left != right.get();
    }


    template<class T>
    inline bool operator<(const intrusive_ptr<T>& left, const intrusive_ptr<T>& right)
    {
        return left.get() < right.get();
    }

    template<class Dst, class Src>
    inline intrusive_ptr<Dst> static_pointer_cast(const intrusive_ptr<Src>& pointer)
    {
        return static_cast<Dst*>(pointer.get());
    }

    template<class Dst, class Src>
    inline intrusive_ptr<Dst> const_pointer_cast(const intrusive_ptr<Src>& pointer)
    {
        return const_cast<Dst*>(pointer.get());
    }

    template<class Dst, class Src>
    inline intrusive_ptr<Dst> dynamic_pointer_cast(const intrusive_ptr<Src>& pointer)
    {
        return dynamic_cast<Dst*>(pointer.get());
    }

}
#endif //INC_LCORE_INTRUSIVE_PTR_H__
