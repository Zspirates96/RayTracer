/*
 * Sphere.h
 *
 *
 */

#ifndef SPHERE_H_
#define SPHERE_H_

#include "core/Shape.h"
#include "core/RayHitStructs.h"

namespace rt{

class Sphere:public Shape{

public:

	//
	// Constructors
	//
	Sphere(){};
	Sphere(Vec3f center, float radius, Material* mat);

	virtual ~Sphere();


	//
	// Functions that need to be implemented, since Sphere is a subclass of Shape
	//
	Hit intersect(Ray ray);

	Material* getMaterial() {
		return mat;
	}

	Vec3f* getVertices();

	Vec3f getColour(Vec3f point);

	Vec3f getNormal(Vec3f surface);

	float getRadius() {
		return radius;
	}

	int getVnum() { return 0;}

	Vec3f center;
	float radius;
	Material* mat;
	Vec3f normal;

};



} //namespace rt




#endif /* SPHERE_H_ */
