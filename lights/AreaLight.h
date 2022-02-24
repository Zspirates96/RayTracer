/*
 * AreaLight.h
 *
 *
 */

#ifndef AREALIGHT_H_
#define AREALIGHT_H_

#include "core/LightSource.h"

namespace rt{

class AreaLight:public LightSource{
public:
    AreaLight(){};
    AreaLight(Vec3f position, Vec3f is, Vec3f id, int cell, float radius);

    virtual ~AreaLight(){};

};



} //namespace rt




#endif /* AREALIGHT_H_ */
