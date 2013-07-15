#ifndef INC_LFRAMEWORK_MESSAGEDRIVEN_H__
#define INC_LFRAMEWORK_MESSAGEDRIVEN_H__
/**
@file MessageDriven.h
@author t-sakai
@date 2010/02/21 create
*/
#include "lgraphics/Window.h"

namespace lgraphics
{
    struct InitParam;
}

namespace lframework
{
    class MessageDriven
    {
    public:
        /**
        @brief ウィンドウ作成とグラフィックスドライバ初期化
        @return 成否
        @param param ... グラフィック関係初期化パラメータ
        */
        bool create(lgraphics::Window::InitParam& windowParam, lgraphics::InitParam& param, const char* title);

        /// 処理ループ実行
        void run();

    protected:
        MessageDriven(const MessageDriven&);
        MessageDriven& operator=(const MessageDriven&);

        MessageDriven();
        virtual ~MessageDriven();

        virtual void initialize() =0;
        virtual void terminate() =0;

    private:
        /// ウィンドウクラス。おそらく重くないのでポインタじゃない
        lgraphics::Window window_;
    };
}
#endif //INC_LFRAMEWORK_MESSAGEDRIVEN_H__
