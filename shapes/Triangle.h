/*
 * Triangle.h
 *
 *
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "core/Shape.h"
#include "rapidjson/document.h"

namespace rt{

class Triangle: public Shape{
public:

    Triangle(){};
    Triangle(Vec3f v0, Vec3f v1, Vec3f v2, Material* mat);

    Triangle(Vec3f v0, Vec3f v1, Vec3f v2, Vec3f v3, Material* mat);

    virtual ~Triangle();

    Hit intersect(Ray ray);

    Material* getMaterial() {
        return mat;
    }

    int getVnum() {return num;}

    Vec3f* getVertices();

    Vec3f getColour(Vec3f point);

    float getRadius() { return 0;}

    Vec3f getNormal(Vec3f surface) {
        return normal;
    }

    int num;
    Vec3f normal;
    Vec3f v0;
    Vec3f v1;
    Vec3f v2;
    Vec3f v3;
    Material* mat;

};



} //namespace rt




#endif /* TRIANGLE_H_ */
