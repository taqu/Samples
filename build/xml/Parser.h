#ifndef __XML_PARSER_H__
#define __XML_PARSER_H__
/**
@file Parser.h
@author t-sakai
@date 2009/01/29 create
@data 2009/05/19 lcoreライブラリ用に変更
*/
#include "Core.h"
#include "NodePtrStack.h"

namespace xml
{
    class Node;

    /// XMLパーサ
    class Parser
    {
    public:
        /// コンストラクタ
        Parser()
        {
        }

        /// デストラクタ
        ~Parser()
        {
        }

        /**
        @brief XMLテキストのパース
        @return ルートノード
        @param memory ... XMLテキスト
        */
        Node* parse(const char* memory);

    private:
        /// 文字コード
        enum CharEncode
        {
            CharEncode_SJIS,
            CharEncode_UTF8,
            CharEncode_Num,
        };

        const char* xmlHead(const char* text, CharEncode &encode);
        const char* beginElementTag(const char* text);
        const char* endElementTag(const char* text);
        const char* attribute(const char* text);
        const char* content(const char* text);

        const char* tagName(const char* text);

        const char* BOM(const char* text);

        /// スタックポップ
        void popStack()
        {
            //スタックにひとつしかなければポップしない
            if(_nodeStack.count() > 1){
                _nodeStack.pop();
            }
        }

        /// ノード階層のロールバック用スタック
        NodePtrStack _nodeStack;
    };
}

#endif //__XML_PARSER_H__
