/*
 * LightSource.h
 *
 */

#ifndef LIGHTSOURCE_H_
#define LIGHTSOURCE_H_

#include "rapidjson/document.h"
#include "math/geometry.h"

namespace rt{

class LightSource{
public:

    LightSource(){};

    virtual ~LightSource(){};

    static LightSource* createPointLight(Vec3f position, Vec3f is, Vec3f id);

    static LightSource* createAreaLight(Vec3f position, Vec3f is, Vec3f id, int cell, float radius);

    Vec3f getPosition() { return this->position;}
    Vec3f getis() { return this->is;}
    Vec3f getid() { return this->id;}
    int getCell() { return this->cell;}
    float getRadius() { return this->radius;}
    std::string getType() { return this->type;}


protected:

    Vec3f position;
    Vec3f is;
    Vec3f id;
    int cell;
    float radius;
    std::string type;

};

} //namespace rt



#endif /* LIGHTSOURCE_H_ */
