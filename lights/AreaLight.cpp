/*
 * AreaLight.cpp
 *
 *
 */
#include "AreaLight.h"




namespace rt{

    AreaLight::AreaLight(Vec3f position, Vec3f is, Vec3f id, int cell, float radius) {
        this->position = position;
        this->is = is;
        this->id = id;
        this->type = "arealight";
        this->cell = cell;
        this->radius = radius;        
    }



} //namespace rt
