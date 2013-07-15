#ifndef INC_LFRAMEWORK_IOUTIL_H__
#define INC_LFRAMEWORK_IOUTIL_H__
/**
@file IOUtil.h
@author t-sakai
@date 2010/05/06 create

*/
#include <lcore/liostream.h>
#include "lframework.h"
#include "Static.h"

namespace lgraphics
{
    class TextureRef;
}

namespace lframework
{
namespace io
{
    /**
    @brief 画像の起点、左上・左下を入れ替える
    @param texture
    */
    void swapOrigin(lgraphics::TextureRef& texture);


    ImageFormat getFormatFromExt(const char* ext);
}
}
#endif //INC_LFRAMEWORK_IOUTIL_H__
