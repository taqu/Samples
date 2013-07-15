#ifndef INC_LSCENE_LIGHTENVIRONMENT_H__
#define INC_LSCENE_LIGHTENVIRONMENT_H__
/**
@file LightEnvironment.h
@author t-sakai
@date 2010/11/09 create

*/
#include "DirectionalLight.h"

namespace lscene
{
    class LightEnvironment
    {
    public:
        const DirectionalLight& getDirectionalLight() const{ return directional_;}
        DirectionalLight& getDirectionalLight(){ return directional_;}

    private:
        DirectionalLight directional_;
    };
}

#endif //INC_LSCENE_LIGHTENVIRONMENT_H__
