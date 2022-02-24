/*
 * Shape.h
 *
 */

#ifndef SHAPE_H_
#define SHAPE_H_

#include "rapidjson/document.h"
#include "core/RayHitStructs.h"
#include "core/Material.h"

namespace rt{

class Shape{
public:

	//
	// Constructors
	//
	Shape(){};

	//
	// Destructor (must be overriden in subclass)
	//
	virtual ~Shape(){};

	//
	// Shape abstract methods (to be implemented by subclasses)
	//
	virtual Hit intersect(Ray)=0;

	static Shape* createSphere(Vec3f center, float radius, Material* mat);

	static Shape* createTriangle(Vec3f v0, Vec3f v1, Vec3f v2, Material* mat);

	static Shape* createPlane(Vec3f v0, Vec3f v1, Vec3f v2, Vec3f v3, Material* mat);

	static Shape* createMesh(Vec3f* v, int f, int* nvef, int*vef, Material* mat, int n);

	virtual Vec3f* getVertices()=0;

	virtual Material* getMaterial()=0;

	virtual int getVnum()=0;

	virtual Vec3f getColour(Vec3f point)=0;

	virtual float getRadius()=0;

	std::string getType() {
		return type;
	}

	virtual Vec3f getNormal(Vec3f surface)=0;

protected:

	std::string type;
	Material * material;

};


} //namespace rt


#endif /* SHAPE_H_ */
