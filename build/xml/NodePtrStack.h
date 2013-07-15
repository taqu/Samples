#ifndef __XML_NODEPTRSTACK_H__
#define __XML_NODEPTRSTACK_H__
/**
@file NodePtrStack.h
@author t-sakai
@date 2009/02/02 create
@data 2009/05/19 lcoreライブラリ用に変更
*/
#include "Core.h"
#include <lcore/FixedArray.h>

namespace xml
{
    class Node;

    class NodePtrStack
    {
    public:
        static const int INITIAL_STACK_SIZE = 10;
        NodePtrStack()
            :_nodes(INITIAL_STACK_SIZE),
            _count(0)
        {
        }

        /**
        @brief プッシュ
        @param node ... XMLノード
        */
        void push(Node* node);

        /**
        @brief ポップ
        */
        void pop()
        {
            XML_ASSERT(_count > 0);
            --_count;
        }

        /**
        @return スタックの先頭
        */
        Node* top()
        {
            XML_ASSERT(_count > 0);
            return _nodes[_count - 1];
        }

        /**
        @return 現在スタックに積まれている個数
        */
        size_t count() const
        {
            return _count;
        }

    private:
        typedef lcore::FixedArray<Node*> NodeArray;

        /**
        @brief スタック領域を拡張
        @param newSize ... 新しいサイズ
        */
        void expand(size_t newSize);

        /// スタック
        NodeArray _nodes;

        /// スタック内の数
        size_t _count;
    };
}
#endif //__XML_NODEPTRSTACK_H__
