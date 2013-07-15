#ifndef INC_LFRAMEWORK_APPLICATION_H__
#define INC_LFRAMEWORK_APPLICATION_H__
/**
@file Application.h
@author t-sakai
@date 2010/02/14 create
*/
#include "lgraphics/Window.h"

namespace lgraphics
{
    struct InitParam;
}

namespace lframework
{
    /// 処理ループを持ったアプリケーションフレームワーク
    class Application
    {
    public:
        /**
        @brief ウィンドウ作成とグラフィックスドライバ初期化
        @return 成否
        @param param ... グラフィック関係初期化パラメータ
        */
        bool create(lgraphics::InitParam& param, const char* title, WNDPROC wndProc);

        /// 処理ループ実行
        void run();

#if defined(_WIN32)
        /// 処理ループ、モードレスダイアログの処理
        void runWithDialog();
        void setDialog(HWND hDlg){ hDlg_ = hDlg;}
#endif

    protected:
        Application(const Application&);
        Application& operator=(const Application&);

        Application();
        virtual ~Application();

        void setTerminate(){ window_.destroy();}

        virtual void initialize() =0;
        virtual void update() =0;
        virtual void terminate() =0;

        /// ウィンドウクラス。おそらく重くないのでポインタじゃない
        lgraphics::Window window_;

#if defined(_WIN32)
        HWND hDlg_; //モードレスダイアログ
#endif
    };
}
#endif //INC_LFRAMEWORK_APPLICATION_H__
