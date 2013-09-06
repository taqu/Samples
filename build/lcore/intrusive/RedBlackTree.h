#ifndef INC_LCORE_REDBLACKTREE_H__
#define INC_LCORE_REDBLACKTREE_H__
/**
@file RedBlackTree.h
@author t-sakai
@date 2008/11/13 create
*/
#include "../lcore.h"

namespace lcore
{
    template<class ValueType> class RedBlackTree;

    enum RBColor
    {
        RBColor_Black = 0,
        RBColor_Red = 1,
    };

    //---------------------------------------------------------------
    //---
    //--- AVLNodeBase
    //---
    //---------------------------------------------------------------
    /// ノード基底
    template<class T>
    class RedBlackTreeNodeBase
    {
    private:
        friend class RedBlackTree<T>;
        typedef RedBlackTreeNodeBase<T> this_type;

        /// コンストラクタ
        RedBlackTreeNodeBase(this_type* parent, this_type* left, this_type* right, s32 color, const T& value)
            :parent_(parent)
            ,left_(left)
            ,right_(right)
            ,color_(color)
            ,value_(value)
        {}

        /// デストラクタ
        ~RedBlackTreeNodeBase()
        {}

        bool isRed() const{ return color_ == RBColor_Red;}
        bool isBloack() const{ return color_ == RBColor_Black;}

        void setRed(){ color_ = RBColor_Red;}
        void setBlack(){ color_ = RBColor_Black;}

        this_type* getParent(){ return parent_;}
        void setParent(this_type* parent){ parent_ = parent;}

        s32 getColor() const{ return color_;}
        void setColor(s32 color){ color_ = color;}

        /// 親ノード
        this_type* parent_;

        /// 左部分木
        this_type *left_;

        /// 右部分木
        this_type *right_;

        /// 色
        s32 color_;

        T value_;
    };


    //---------------------------------------------------------------
    //---
    //--- RedBlackTree
    //---
    //---------------------------------------------------------------
    /// 赤黒木
    template<class ValueType>
    class RedBlackTree
    {
    public:
        typedef u32 size_type;
        typedef ValueType* pointer;
        typedef const ValueType* const_pointer;
        typedef ValueType& reference;
        typedef const ValueType& const_reference;
        typedef ValueType value_type;
        typedef RedBlackTree<ValueType> this_type;
        typedef RedBlackTreeNodeBase<ValueType> node_type;

        RedBlackTree()
            :root_(NULL)
        {
        }

        ~RedBlackTree()
        {
            if(NULL != root_){
                eraseAll();
            }
        }

        bool isEmpty() const{ return (NULL == root_);}

        bool insert(const value_type& value);

        void erase(const value_type& value)
        {
            root_ = erase(root_, value);
        }

        void clear()
        {
            eraseNode(root_);
            root_ = NULL;
        }

        const value_type* find(const value_type& value)
        {
            node_type* node = searchNode(value);
            return (NULL != node)? &(node->value_) : NULL;
        }

    private:
        void balance(node_type* node);
        node_type* searchNode(const value_type& value);
        void erase(node_type* node);

        void eraseNode(node_type* node);

        /// 右回転
        node_type* rotateRight(node_type* node);

        /// 左回転
        node_type* rotateLeft(node_type* node);

        inline node_type* getGrandParent(node_type* node)
        {
            LASSERT(NULL != node);
            LASSERT(NULL != node->parent_);

            return node->parent_->parent_;
        }

        inline node_type* getUncle(node_type* node)
        {
            if(NULL != node && NULL != node->parent_){

                node_type* grand = node->parent_->parent_;
                if(NULL == grand){
                    return NULL;
                }
            }

            return (node->parent_ == grand->left_)? grand->right_ : grand->left_;
        }

        inline node_type* getSibling(node_type* node)
        {
            LASSERT(NULL != node);
            if(node == node->parent_->left_){
                return node->parent_->right_;
            }else{
                return node->parent_->left_;
            }
        }

        node_type *root_;
    };

    //---------------------------------------------------------------
    // 右回転
    template<class ValueType>
    typename RedBlackTree<ValueType>::node_type*
        RedBlackTree<ValueType>::rotateRight(node_type* node)
    {
        LASSERT(NULL != node);

        node_type* left = node->left_;
        LASSERT(NULL != left); //右回転する場合必ず存在

        node->left_ = left->right_;
        
        left->right_ = node;
        left->setColor(node->getColor());
        node->setRed();
        return left;
    }


    //---------------------------------------------------------------
    // 左回転
    template<class ValueType>
    typename RedBlackTree<ValueType>::node_type*
        RedBlackTree<ValueType>::rotateLeft(node_type* node)
    {
        LASSERT(NULL != node);

        node_type* right = node->right_;
        LASSERT(NULL != right); //左回転する場合必ず存在

        node->right_ = right->left_;

        right->left_ = node;
        right->setColor(node->getColor());
        node->setRed();
        return right;
    }


    //---------------------------------------------------------------
    template<class ValueType>
    bool RedBlackTree<ValueType>::insert(const value_type& value)
    {
        node_type* node = root_;
        node_type* prev = root_;
        while(NULL != node){
            prev = node;
            if(value == node->value_){
                return false;

            }else if(value < node->value_){
                node = node->left_;

            }else{
                node = node->right_;
            }
        }

        if(NULL == prev){
            root_ = LIME_NEW node_type(NULL, NULL, NULL, BLACK, value);
            return true;
        }

        node_type* newNode = LIME_NEW node_type(prev, NULL, NULL, RED, value);
        if(value < prev->value_){
            prev->left_ = newNode;

        }else{
            prev->right_ = newNode;
        }
        balance(newNode);
        return true;
    }

    template<class ValueType>
    void RedBlackTree<ValueType>::balance(node_type* node)
    {
        if(node->parent_->color_ == BLACK){
            return;
        }
        node_type* grand;
        node_type* uncle = getUncle(node);
        if(NULL != uncle && uncle->color_ == RED){
            node->parent_->color_ = BLACK;
            uncle->color_ = BLACK;
            grand = getGrandParent(node);
            grand->color_ = RED;
            if(NULL == grand->parent_){
                grand->color_ = BLACK;
            }else{
                balance(grand);
            }

        }else{ //if(NULL != uncle
            grand = getGrandParent(node);
            if((node == node->parent_->right_)
                && (node->parent_ == grand->left_))
            {
                rotateLeft(node->parent_);
                node = node->left_;

            }else if((node == node->parent_->left_)
                && (node->parent_ == grand->right_))
            {
                rotateRight(node->parent_);
                node = node->right_;
            }

            grand = getGrandParent(node);
            node->parent_->color_ = BLACK;
            grand->color_ = RED;
            if(node == node->parent_->left_){
                rotateRight(grand);
            }else{
                rotateLeft(grand);
            }
        }//if(NULL != uncle
    }

    //---------------------------------------------------------------
    template<class ValueType>
    typename RedBlackTree<ValueType>::node_type*
        RedBlackTree<ValueType>::searchNode(const value_type& key)
    {
        node_type* node = root_;
        while(NULL != node){
            const value_type& nodeVal = node->value_;
            s32 ret = predicate_(key, nodeVal);
            if( ret==0 ){
                break;
            }else if( ret<0 ){
                node = node->left_;
            }else{
                node = node->right_;
            }
        }
        return node;
    }

    //---------------------------------------------------------------
    template<class ValueType>
    void RedBlackTree<ValueType>::eraseNode(node_type* node)
    {
        if(NULL != node){
            eraseNode(node->left_);
            eraseNode(node->right_);
            LIME_DELETE(node);
        }
    }

    //---------------------------------------------------------------
    template<class ValueType>
    void RedBlackTree<ValueType>::erase(node_type* node)
    {
    }
}
#endif //INC_LCORE_REDBLACKTREE_H__
