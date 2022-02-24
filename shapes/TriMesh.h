/*
 * TriMesh.h
 *
 *
 */

#ifndef TRIMESH_H_
#define TRIMESH_H_

#include "core/Shape.h"
#include "rapidjson/document.h"
#include <vector>

namespace rt{

class TriMesh: public Shape{
public:

    TriMesh(){};
    TriMesh(Vec3f* v, int f, int* nvef, int*vef, Material* mat, int numVertice);

    virtual ~TriMesh(){};

    Hit intersect(Ray ray);

    Material* getMaterial() {
        return mat;
    }

    Vec3f getColour(Vec3f point);    

    Vec3f getNormal(Vec3f surface);

    Vec3f* getVertices() {
        return vertices;
    }

    float getRadius() {return 0;}

    int getVnum() {
        return numVertice;
    }

    std::vector<Vec3f*> getTri() { return triangles;}

    int numVertice;
    Vec3f* normals;
    Vec3f* vertices;
    int faces;
    int* numVef;
    int* vef;
    Material* mat;
    std::vector<Vec3f*> triangles;
    int numTri;

};



} //namespace rt




#endif /* TRIMESH_H_ */
