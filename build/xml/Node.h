#ifndef __XML_NODE_H__
#define __XML_NODE_H__
/**
@file Node.h
@author t-sakai
@date 2009/02/05 create
@data 2009/05/19 lcoreライブラリ用に変更
*/
#include "Core.h"
#include <lcore/FixedArray.h>

namespace xml
{
    template<class FirstType, class SecondType>
    struct Pair
    {
        typedef FirstType first_type;
        typedef SecondType second_type;

        Pair()
            :_first(first_type()),
            _second(second_type())
        {
        }

        Pair(const first_type& first, const second_type& second)
            :_first(first), _second(second)
        {
        }

        Pair(const Pair<first_type, second_type>& pair)
            :_first(pair._first),
            _second(pair._second)
        {

        }

        first_type _first;
        second_type _second;
    };

    // Nodeクラスのプライベートにアクセスするためのクラス
    class NodeAccess;

    /// XMLノードクラス
    class Node
    {
    public:
        typedef std::string string_type;
        typedef Pair<string_type, string_type> string_pair;
        typedef lcore::FixedArray<string_pair*> AttrList;
        typedef lcore::FixedArray<Node*> NodeList;

        /// コンストラクタ
        Node();

        /// デストラクタ
        ~Node();

        /**
        @return ノード名
        */
        const string_type& getName() const
        {
            return _name;
        }

        /**
        @return ノード名
        */
        string_type& getName()
        {
            return _name;
        }

        /**
        @return ノードの内容の文字列
        */
        const string_type& getText() const
        {
            return _text;
        }

        /**
        @return ノードの内容の文字列
        */
        string_type& getText()
        {
            return _text;
        }

        /**
        @return 属性数
        */
        size_t attributeNum() const
        {
            return _attributes.size();
        }

        /**
        @brief 属性をキー名で検索
        @return 属性の文字列。なければNULL
        @param key ... キー文字列
        */
        const string_type* getAttributeByName(const string_type& key) const;

        /**
        @brief 属性をインデックスで検索
        @return 属性の文字列
        @param index ... インデックス
        @notice インデックスは0以上属性数未満でなければならない
        */
        const string_pair* getAttribute(size_t index) const
        {
            XML_ASSERT(0<=index
                && index < attributeNum());

            return _attributes[index];
        }

        /**
        @return 子の数
        */
        size_t childNum() const
        {
            return _childs.size();
        }

        /**
        @brief 子をインデックスで検索
        @return 子ノード
        @param index ... インデックス
        @notice インデックスは0以上子の数未満でなければならない
        */
        const Node* getChild(size_t index) const
        {
            XML_ASSERT(0 <= index && index < (s32)_childs.size());
            return _childs[index];
        }

        /**
        @brief 子を追加
        @param child ... 子ノード
        */
        void addChild(Node* child);

        /**
        @brief 属性を追加
        @param key ... キー文字列
        @param value ... 値文字列
        */
        void addAttribute(const string_type& key, const string_type& value);
    private:
        // このクラスを通じてだけアクセスさせる
        friend class NodeAccess;

        /**
        @brief ノードの名前を設定
        @param nameBegin ... 名前文字列の先頭
        @param nameEnd ... 名前文字列の末尾のひとつ後
        */
        void setName(const char* nameBegin, const char* nameEnd)
        {
            _name.assign<const char*>(nameBegin, nameEnd);
        }

        /**
        @brief ノードの内容文字列に、文字列を追加
        @param beginText ... 追加文字列の先頭
        @param endText ... 追加文字列の末尾のひとつ後
        */
        void appendText(const char* beginText, const char* endText)
        {
            _text.append<const char*>(beginText, endText);
        }

        /// ノード名
        string_type _name;

        /// 属性キー名と属性文字列のペアのリスト
        AttrList _attributes;

        /// ノードの内容文字列
        string_type _text;

        /// 子ノードのリスト
        NodeList _childs;
    };
}
#endif //__XML_NODE_H__
