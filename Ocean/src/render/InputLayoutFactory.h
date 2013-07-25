#ifndef INC_RENDER_INPUTLAYOUTFACTORY_H__
#define INC_RENDER_INPUTLAYOUTFACTORY_H__
/**
@file InputLayoutFactory.h
@author t-sakai
@date 2012/07/30 create
*/
#include <lgraphics/api/InputLayoutRef.h>
#include "render.h"
#include "ShaderType.h"

namespace render
{
    enum Layout
    {
        Layout_P =0,
        Layout_PN,
        Layout_PU,
        Layout_PU16,
        Layout_PC,
        Layout_PNTB,
        Layout_PNU,
        Layout_PNCU,
        Layout_PNTBU,
        Layout_PNUBone,
        Layout_Ocean,
        Layout_Num,
    };

    class InputLayoutFactory
    {
    public:
        InputLayoutFactory();
        ~InputLayoutFactory();

        bool initialize();

        lgraphics::InputLayoutRef& get(Layout type){ return layouts_[type];}

    private:
        InputLayoutFactory(const InputLayoutFactory&);
        InputLayoutFactory& operator=(const InputLayoutFactory&);

        lgraphics::InputLayoutRef layouts_[Layout_Num];
    };
}
#endif //INC_RENDER_INPUTLAYOUTFACTORY_H__
