/*
 * PointLight.cpp
 *
 *
 */
#include "PointLight.h"
#include "math/geometry.h"


namespace rt{

    PointLight::PointLight(Vec3f position, Vec3f is, Vec3f id) {
        this->position = position;
        this->is = is;
        this->id = id;
        this->type = "pointlight";
        this->cell = 0;
        this->radius = 0.0;
    }



} //namespace rt
