#ifndef INC_LCORE_INTRUSIVE_LIST_H__
#define INC_LCORE_INTRUSIVE_LIST_H__
/**
@file List.h
@author t-sakai
@date 2011/03/09 create
*/
#include "../lcore.h"

namespace lcore
{
namespace intrusive
{
    template<class T> class List;

    template<class Derived>
    class ListNodeBase
    {
    public:
        typedef ListNodeBase<Derived> this_type;
        typedef this_type* this_poitner_type;
        typedef Derived value_type;
        typedef Derived* pointer_type;
        typedef const Derived* const_pointer_type;

        pointer_type getPrev() { return static_cast<pointer_type>(prev_);}
        const_pointer_type getPrev() const{ return static_cast<pointer_type>(prev_);}
        pointer_type getNext() { return static_cast<pointer_type>(next_);}
        const_pointer_type getNext() const{ return static_cast<pointer_type>(next_);}

    protected:
        friend class List<Derived>;

        ListNodeBase()
            :prev_(this)
            ,next_(this)
        {
        }

        ~ListNodeBase()
        {}

    private:
        void reset()
        {
            prev_ = this;
            next_ = this;
        }

        void unlink()
        {
            next_->prev_ = prev_;
            prev_->next_ = next_;
            prev_ = NULL;
            next_ = NULL;
        }

        void link(this_type* node)
        {
            prev_ = node->prev_;
            next_ = node;
            node->prev_ = this;
            prev_->next_ = this;
        }

        this_poitner_type prev_;
        this_poitner_type next_;
    };

    template<class T>
    class ListNodeContainer : public ListNodeBase< ListNodeContainer<T> >
    {
    public:
        typedef T value_type;
        typedef T* pointer_type;
        typedef ListNodeContainer<T> this_type;
        typedef ListNodeBase<this_type> base_type;
        typedef T& reference_type;

        ListNodeContainer()
        {}

        ListNodeContainer(const T& value)
            :value_(value)
        {}

        const reference_type operator*() const
        {
            return value_;
        }

        reference_type operator*()
        {
            return value_;
        }

        const pointer_type operator->() const
        {
            return &value_;
        }

        pointer_type operator->()
        {
            return &value_;
        }
    private:
        T value_;
    };

    template<class T>
    class ListNodeContainer<T*> : public ListNodeBase< ListNodeContainer<T*> >
    {
    public:
        typedef T* value_type;
        typedef T** pointer_type;
        typedef ListNodeContainer<value_type> this_type;
        typedef ListNodeBase<this_type> base_type;
        typedef T& reference_type;

        ListNodeContainer()
            :value_(NULL)
        {}

        ListNodeContainer(T* value)
            :value_(value)
        {}

        const reference_type operator*() const
        {
            return *value_;
        }

        reference_type operator*()
        {
            return *value_;
        }

        const pointer_type operator->() const
        {
            return value_;
        }

        pointer_type operator->()
        {
            return value_;
        }
    private:
        T* value_;
    };

    /**
    @brief リンクポインタ埋め込み型リスト

    汎用より簡潔を優先。外部から渡されるポインタがリストにあるかどうか、チェックしない。
    使い方を誤ると安全でない。
    */
    template<class T>
    class List
    {
    public:
        typedef T value_type;
        typedef T* pointer_type;

        typedef List<T> this_type;
        typedef typename T::base_type node_type;

        inline List();
        inline ~List();

        inline u32 size() const;
        inline pointer_type begin();
        inline pointer_type rbegin();
        inline pointer_type end();

        void push_front(pointer_type node);
        void push_back(pointer_type node);
        void remove(pointer_type node);

        void swap(this_type& rhs);
    private:
        u32 size_;
        node_type top_;
    };

    template<class T>
    inline List<T>::List()
        :size_(0)
    {
        top_.reset();
    }

    template<class T>
    inline List<T>::~List()
    {
    }

    template<class T>
    inline u32 List<T>::size() const
    {
        return size_;
    }

    template<class T>
    inline typename List<T>::pointer_type List<T>::begin()
    {
        return top_.getNext();
    }

    template<class T>
    inline typename List<T>::pointer_type List<T>::rbegin()
    {
        return top_.getPrev();
    }

    template<class T>
    inline typename List<T>::pointer_type List<T>::end()
    {
        return static_cast<pointer_type>(&top_);
    }

    template<class T>
    void List<T>::remove(pointer_type node)
    {
        LASSERT(NULL != node);
        LASSERT(size_>0);

        node->unlink();
        --size_;
    }

    template<class T>
    void List<T>::push_front(pointer_type node)
    {
        LASSERT(NULL != node);
        node->link(top_.getNext());
        ++size_;
    }

    template<class T>
    void List<T>::push_back(pointer_type node)
    {
        LASSERT(NULL != node);
        node->link(static_cast<pointer_type>(&top_));
        ++size_;
    }

    template<class T>
    void List<T>::swap(this_type& rhs)
    {
        lcore::swap(size_, rhs.size_);
        lcore::swap(top_, rhs.top_);
    }
}
}
#endif //INC_LCORE_INTRUSIVE_LIST_H__
