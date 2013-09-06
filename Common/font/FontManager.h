#ifndef INC_FONTMANAGER_H__
#define INC_FONTMANAGER_H__
/**
@file FontManager.h
@author t-sakai
@date 2011/09/24 create
*/
#include "FontType.h"

namespace lcore
{
    class istream;
}

namespace font
{
    class FontPack;

    class FontManager
    {
    public:
        FontManager();
        ~FontManager();

        /**
        */
        void initialize();

        /**
        */
        void terminate();

        /**
        @brief 文字描画仮想レイヤーの解像度を設定

        この解像度と一致するフォント画像は、ほぼDotByDotで描画される
        */
        void setResolution(u32 width, u32 height);

        /**
        */
        bool load(lcore::istream& is);

        /**
        */
        u32 getNumFonts() const;

        /**
        */
        FontPack& getFontPack(u32 index);

        /**
        */
        void print(u32 fontID, u32 x, u32 y, const char* str);

        /**
        */
        void print(u32 fontID, f32 x, f32 y, f32 scale, const char* str);

        /**
        */
        void format(u32 fontID, u32 x, u32 y, const char* str, ...);

        /**
        */
        void format(u32 fontID, f32 x, f32 y, f32 scale, const char* str, ...);

        /**
        */
        void draw();

        /**
        */
        void draw(u32 fontID);
    private:
        class Impl;

        FontManager(const FontManager&);
        FontManager& operator=(const FontManager&);

        Impl* impl_;
    };
}
#endif //INC_FONTMANAGER_H__
