#ifndef INC_LMATH_DEBUGOUT_H__
#define INC_LMATH_DEBUGOUT_H__
/**
@file DebugOut.h
@author t-sakai
@date 2010/01/13 create
*/
#include <iosfwd>

namespace lmath
{
    class Vector3;

    std::ostream& operator<<(std::ostream& os, const Vector3& v);
}

#endif //INC_LMATH_DEBUGOUT_H__
