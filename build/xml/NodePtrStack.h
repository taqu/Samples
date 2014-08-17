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
        static const s32 INITIAL_STACK_SIZE = 10;
        NodePtrStack()
            :nodes_(INITIAL_STACK_SIZE),
            count_(0)
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
            XML_ASSERT(count_ > 0);
            --count_;
        }

        /**
        @return スタックの先頭
        */
        Node* top()
        {
            XML_ASSERT(count_ > 0);
            return nodes_[count_ - 1];
        }

        /**
        @return 現在スタックに積まれている個数
        */
        u32 count() const
        {
            return count_;
        }

    private:
        typedef lcore::FixedArray<Node*> NodeArray;

        /**
        @brief スタック領域を拡張
        @param newSize ... 新しいサイズ
        */
        void expand(u32 newSize);

        /// スタック
        NodeArray nodes_;

        /// スタック内の数
        u32 count_;
    };
}
#endif //__XML_NODEPTRSTACK_H__
