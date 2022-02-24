/*
 * Shape.cpp
 *
 */
#include "Shape.h"
#include "shapes/BVH.h"
#include "shapes/Sphere.h"
#include "shapes/Triangle.h"
#include "shapes/TriMesh.h"
#include "Material.h"

namespace rt{

    Shape* Shape::createSphere(Vec3f center, float radius, Material* mat) {
		return new Sphere(center, radius, mat);
    }

    Shape* Shape::createTriangle(Vec3f v0, Vec3f v1, Vec3f v2, Material* mat) {
        return new Triangle(v0, v1, v2, mat);
    }

    Shape* Shape::createPlane(Vec3f v0, Vec3f v1, Vec3f v2, Vec3f v3, Material* mat) {
        return new Triangle(v0, v1, v2, v3, mat);
    }

    Shape* Shape::createMesh(Vec3f* v, int f, int* nvef, int*vef, Material* mat, int n) {
        return new TriMesh(v, f, nvef, vef, mat, n);
    }

} //namespace rt

