/*
 * BVH.h
 *
 *
 */

#ifndef BVH_H_
#define BVH_H_

#include "core/Shape.h"
#include "rapidjson/document.h"

namespace rt{

class BVH: public Shape{
public:

    struct BoundingVolume{
        std::vector<int> object;
        float* dnear;
        float* dfar;
        Vec3f centroid;
    };

    struct Octree {
        BoundingVolume bv;
        Octree* children[8] = { nullptr };
        bool leaf = false;
        Vec3f bbox_min;
        Vec3f bbox_max;
    };

    BVH(){};
    BVH(std::vector<Shape*> shapes, int depth);
    virtual ~BVH(){};

    Hit intersect(Ray);
    std::tuple<Hit, int> bvhIntersect(Ray ray, Octree node);
    static Vec3f centroid(BoundingVolume bv);
    static void extend(BoundingVolume& bv0, BoundingVolume& bv1);
    void buildTree();
    void insert(Octree& node, int d, BoundingVolume* bv, int bvSize);
    Octree getTree() { return this->tree_root;}

    int depth;
    std::vector<Shape*> shapes;
    Vec3f* normals;
    BoundingVolume* bv;
    BoundingVolume root;
    Octree tree_root;
    int intersectedObject;
    std::vector<int> dont_try;

    float getRadius() { return 0.0;}
    Vec3f getColour(Vec3f point) {Vec3f temp(0,0,0); return temp;}
    int getVnum() { return 0;}
    Vec3f* getVertices() {return nullptr;}
    Material* getMaterial() {return nullptr;}
    Vec3f getNormal(Vec3f surface) {Vec3f temp(0,0,0); return temp;}

};



} //namespace rt



#endif /* BVH_H_ */
