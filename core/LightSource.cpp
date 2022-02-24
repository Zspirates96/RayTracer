/*
 * LightSource.cpp
 *
 */
#include "LightSource.h"
#include "lights/PointLight.h"
#include "lights/AreaLight.h"

namespace rt{

    LightSource* LightSource::createPointLight(Vec3f position, Vec3f is, Vec3f id) {
        return new PointLight(position, is, id);
    }

    LightSource* LightSource::createAreaLight(Vec3f position, Vec3f is, Vec3f id, int cell, float radius) {
        return new AreaLight(position, is, id, cell, radius);
    }

} //namespace rt

