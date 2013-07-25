#ifndef INC_RENDER_RECTANGLE_H__
#define INC_RENDER_RECTANGLE_H__
/**
@file Rectangle.h
@author t-sakai
@date 2012/04/24 create

*/
#include "render.h"

namespace lgraphics
{
    class Texture2DRef;
}

namespace render
{
    class RectangleVS;
    class RectanglePS;

    class Rectangle
    {
    public:
        Rectangle()
            :vs_(NULL)
            ,ps_(NULL)
        {}
        ~Rectangle(){}

        bool create();

        void draw();
        void draw(lgraphics::Texture2DRef& texture);
        void drawNoPS(lgraphics::Texture2DRef& texture);

        void draw(
            f32 minx,
            f32 maxx,
            f32 miny,
            f32 maxy,
            lgraphics::Texture2DRef& texture);

    private:
        Rectangle(const Rectangle&);
        Rectangle& operator=(const Rectangle&);
        void setFullScreenQuad();

        RectangleVS* vs_;
        RectanglePS* ps_;
        lmath::Vector4 constants_[4];
    };
}
#endif //INC_RENDER_RECTANGLE_H__
