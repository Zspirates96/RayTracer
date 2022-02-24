/*
 * BVH.cpp
 *
 *
 */
#include "BVH.h"
#include "core/Shape.h"
#include <tuple>


namespace rt{

    Vec3f BVH::centroid(BoundingVolume bv) {
        Vec3f temp(0,0,0);
        temp.x = bv.dnear[0] + bv.dfar[0];
        temp.y = bv.dnear[1] + bv.dfar[1];
        temp.z = bv.dnear[2] + bv.dfar[2];
        return 0.5*temp;
    }

    void BVH::extend(BoundingVolume& bv0, BoundingVolume& bv1) {
        for (int i = 0; i < bv1.object.size(); i++) {
            bv0.object.push_back(bv1.object.at(i));
        }
        for (int i = 0; i < 7; i++) {
            bv0.dnear[i] = std::min(bv0.dnear[i], bv1.dnear[i]);
            bv0.dfar[i] = std::max(bv0.dfar[i], bv1.dfar[i]);
        }
        bv0.centroid = centroid(bv0);
    }

    void BVH::buildTree() {
        this->tree_root.bv = root;
        float xDiff = root.dfar[0] - root.dnear[0]; 
        float yDiff = root.dfar[1] - root.dnear[1]; 
        float zDiff = root.dfar[2] - root.dnear[2]; 
        float maxDiff = std::max(xDiff, std::max(yDiff, zDiff));
        float x = root.dnear[0] + root.dfar[0];
        float y = root.dnear[1] + root.dfar[1];
        float z = root.dnear[2] + root.dfar[2];
        this->tree_root.bbox_max = Vec3f(0.5*(x+maxDiff),0.5*(y+maxDiff),0.5*(z+maxDiff));
        this->tree_root.bbox_min = Vec3f(0.5*(x-maxDiff),0.5*(y-maxDiff),0.5*(z-maxDiff));
        this->insert(this->tree_root, 1, this->bv, shapes.size());
    }

    void BVH::insert(Octree& node, int d, BoundingVolume* bbvv, int bvSize) {
        if (d < this->depth && bvSize > 1) {
            std::vector<int> boundedObject = node.bv.object;
            for (int i = 0; i < bvSize; i++) {
                int childIndex = 0;
                Vec3f objectCentroid = bbvv[i].centroid;
                Vec3f nodeCentroid = 0.5*(node.bbox_min + node.bbox_max);
                Octree* child = new Octree;
                if (objectCentroid.x > nodeCentroid.x) {
                    childIndex = 4;
                    child->bbox_min.x = nodeCentroid.x;
                    child->bbox_max.x = node.bbox_max.x;
                }
                else {
                    child->bbox_min.x = node.bbox_min.x;
                    child->bbox_max.x = nodeCentroid.x;
                }
                if (objectCentroid.y > nodeCentroid.y) {
                    childIndex += 2;
                    child->bbox_min.y = nodeCentroid.y;
                    child->bbox_max.y = node.bbox_max.y;
                }
                else {
                    child->bbox_min.y = node.bbox_min.y;
                    child->bbox_max.y = nodeCentroid.y;
                }
                if (objectCentroid.z > nodeCentroid.z) {
                    childIndex += 1;
                    child->bbox_min.z = nodeCentroid.z;
                    child->bbox_max.z = node.bbox_max.z;
                }
                else {
                    child->bbox_min.z = node.bbox_min.z;
                    child->bbox_max.z = nodeCentroid.z;
                }
                if (node.children[childIndex] == nullptr) {
                    node.children[childIndex] = new Octree;
                    node.children[childIndex]->bv = bbvv[i];
                    node.children[childIndex]->bbox_min = child->bbox_min;
                    node.children[childIndex]->bbox_max = child->bbox_max;
                }
                else {
                    extend(node.children[childIndex]->bv, bbvv[i]);
                }
            }
            for (int i = 0; i < 8; i++) {
                if (node.children[i] == nullptr) continue;
                int newBVSize = node.children[i]->bv.object.size();
                BoundingVolume* bvs = new BoundingVolume[newBVSize];
                for (int j = 0; j < newBVSize; j++) {
                    bvs[j] = this->bv[node.children[i]->bv.object.at(j)];
                }
                insert(*(node.children[i]), d+1, bvs, newBVSize);
            }
        }
        else {
            node.leaf = true;
        }
    }

    BVH::BVH(std::vector<Shape*> shapes, int depth) {
        this->depth = depth;
        this->shapes = shapes;
        this->normals = new Vec3f[7];
        this->normals[0] = Vec3f(1,0,0);
        this->normals[1] = Vec3f(0,1,0);
        this->normals[2] = Vec3f(0,0,1);
        this->normals[3] = Vec3f(sqrt(3)/3, sqrt(3)/3, sqrt(3)/3);
        this->normals[4] = Vec3f(-sqrt(3)/3, sqrt(3)/3, sqrt(3)/3);
        this->normals[5] = Vec3f(-sqrt(3)/3, -sqrt(3)/3, sqrt(3)/3);
        this->normals[6] = Vec3f(sqrt(3)/3, -sqrt(3)/3, sqrt(3)/3);
        this->root.dnear = new float[7];
        this->root.dfar = new float[7];
        for (int i = 0; i < 7; i++) {
            this->root.dnear[i] = 1e10;
            this->root.dfar[i] = -1e10;
        }
        this->bv = new BoundingVolume[shapes.size()];
        for (int i = 0; i < shapes.size(); i++) {
            this->root.object.push_back(i);
            this->bv[i].object.push_back(i);
            this->bv[i].dnear = new float[7];
            this->bv[i].dfar = new float[7];
            int numVertice = 0;
            std::string type = shapes.at(i)->getType();
            if (type == "sphere") numVertice = 1;
            else if (type == "mesh") numVertice = shapes.at(i)->getVnum();
            else if (type == "triangle") numVertice = 3;
            else if (type == "plane") numVertice = 4;
            Vec3f* v = shapes.at(i)->getVertices();
            for (int j = 0; j < 7; j++) {
                if (type != "sphere") {
                    this->bv[i].dnear[j] = 1e10;
                    this->bv[i].dfar[j] = -1e10;
                    for (int k = 0; k < numVertice; k++) {
                        this->bv[i].dnear[j] = std::min(this->bv[i].dnear[j], normals[j].dotProduct(v[k]));
                        this->bv[i].dfar[j] = std::max(this->bv[i].dfar[j], normals[j].dotProduct(v[k]));
                    }
                }
                else {
                    float d = normals[j].dotProduct(v[0]);
                    this->bv[i].dnear[j] = d - shapes.at(i)->getRadius();
                    this->bv[i].dfar[j] = d + shapes.at(i)->getRadius();
                }
                this->root.dnear[j] = std::min(this->bv[i].dnear[j], this->root.dnear[j]);
                this->root.dfar[j] = std::max(this->bv[i].dfar[j], this->root.dfar[j]);
            }
            this->bv[i].centroid = centroid(bv[i]);
        }
        this->root.centroid = centroid(this->root);

    }

    Hit BVH::intersect(Ray ray) {
        std::tuple<Hit, int> h;
        h = bvhIntersect(ray, this->tree_root);
        this->intersectedObject = std::get<1>(h);
        this->dont_try.clear();
        return std::get<0>(h);
    }

    std::tuple<Hit, int> BVH::bvhIntersect(Ray ray, Octree node) {
        Hit* h = new Hit[9];
        int object = -1;
        for (int i = 0; i < 9; i++) {
            Hit h_temp;
            h_temp.point0 = Vec3f(1e10,1e10,1e10);
            h[i] = h_temp;
        }
        int closest = 8;
        float closest_length = 1e8;
        if (!node.leaf) {
            float tnear = -1e10;
            float tfar = 1e10;
            for (int i = 0; i < 7; i++) {
                float NO = this->normals[i].dotProduct(ray.origin);
                float NR = this->normals[i].dotProduct(ray.direction);
                if (fabs(NR) < 1e-6) continue;
                float dfar = node.bv.dfar[i];
                float dnear = node.bv.dnear[i];
                if (NR < 0) {
                    tnear = std::max(tnear, (dfar - NO) / NR);
                    tfar = std::min(tfar, (dnear - NO) / NR);
                }
                else {
                    tnear = std::max(tnear, (dnear - NO) / NR);
                    tfar = std::min(tfar, (dfar - NO) / NR);
                }
                if (tnear > tfar) break;
            }
            if ((tfar > tnear) && fabs(1e10 - tfar) > 1e-6) {
                for (int i = 0; i < 8; i++) {
                    std::tuple<Hit, int> answer_temp;
                    if (node.children[i] == nullptr) continue;
                    else answer_temp = bvhIntersect(ray, *(node.children[i]));
                    if (std::get<0>(answer_temp).point0.x < 1e9 && std::get<0>(answer_temp).point0.y < 1e9 && std::get<0>(answer_temp).point0.z < 1e9) {
                        if ((std::get<0>(answer_temp).point0 - ray.origin).length() < closest_length) {
                            closest = i;
                            closest_length = (std::get<0>(answer_temp).point0 - ray.origin).length();
                            object = std::get<1>(answer_temp);
                            h[i] = std::get<0>(answer_temp);
                        }
                    }
                }
            }
            else {
                Hit h_failed;
                h_failed.point0 = Vec3f(1e10,1e10,1e10);
                std::tuple<Hit, int> failed(h_failed, -1);
                return failed;
            }
        }
        else {
            int numObject = node.bv.object.size();
            for (int i = 0; i < numObject; i++) {
                int obj = node.bv.object.at(i);
                bool skip = false;
                for (int j = 0; j < this->dont_try.size(); j++) {
                    if (obj == this->dont_try.at(j)) {
                        skip = true;
                        break;
                    }
                }
                if (skip) continue;
                h[i] = this->shapes.at(obj)->intersect(ray);
                if (h[i].point0.x != 0 || h[i].point0.y != 0 || h[i].point0.z != 0) {
                    if ((h[i].point0 - ray.origin).length() < closest_length) {
                        closest = i;
                        closest_length = (h[i].point0 - ray.origin).length();
                        object = obj;
                    }
                }
            }
        }
        std::tuple<Hit, int> answer(h[closest], object);
        return answer;
    }

} //namespace rt


