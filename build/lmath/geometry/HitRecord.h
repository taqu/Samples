#ifndef INC_LMATH_HITRECORD_H__
#define INC_LMATH_HITRECORD_H__
/**
@file HitRecord.h
@author t-sakai
@date 2010/01/15 create

*/
#include "../lmathcore.h"

namespace lmath
{
    struct HitRecord
    {
        f32 t_; //光線の半直線距離パラメータ
        Vector3 position_;
        Vector3 normal_;
    };
}

#endif //INC_LMATH_HITRECORD_H__
