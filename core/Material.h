/*
 * Material.h
 *
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "rapidjson/document.h"
#include "math/geometry.h"
#include <vector>

namespace rt{

class Material{
public:

    Material(){};

    virtual~Material();

    float ks;
    float kd;
    float kr;
    int specularexponent;
    Vec3f diffusecolor;
    int tWidth;
    int tHeight;
    bool hasTexture;

    Vec3f* texture;

private:
};


} //namespace rt



#endif /* MATERIAL_H_ */
