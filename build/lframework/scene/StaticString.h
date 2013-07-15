#ifndef INC_LGRAPHICS_STATICSTRING_H__
#define INC_LGRAPHICS_STATICSTRING_H__
/**
@file StaticString.h
@author t-sakai
@date 2010/05/02 create
*/
namespace lgraphics
{
namespace format
{

#define LGRAPHICS_STATIC_STRING(name) \
        extern const char name[];\
        extern const int name##Length;

    LGRAPHICS_STATIC_STRING(ModelFileExt)

#undef LGRAPHICS_STATIC_STRING

}
}
#endif //INC_LGRAPHICS_STATICSTRING_H__
