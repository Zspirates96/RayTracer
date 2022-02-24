/*
 * Sphere.cpp
 *
 *
 */
#include "Sphere.h"
#include <math.h>

#define PI 3.14159265

namespace rt{
	Sphere::~Sphere(){};

	Sphere::Sphere(Vec3f center, float radius, Material* mat){
		this->center = center;
		this->radius = radius;
		this->mat = mat;
		this->type = "sphere";
	}

	Vec3f* Sphere::getVertices() {
		Vec3f* v = new Vec3f[1];
		v[0] = this->center;
		return v;
	}

	Vec3f Sphere::getColour(Vec3f point) {
		if (!mat->hasTexture) return mat->diffusecolor;
		Vec3f dir = (point - center).normalize();
		float phi = atan2(dir.z, dir.x) + PI;
		float theta = asin(dir.y) + (PI/2);
		Vec3f colour(0,0,0);
		
		float u = phi/(2*PI);
		float v = theta/PI;
		int i = u * (mat->tWidth);
		int j = (1-v) * (mat->tHeight-0.001);

		if (i < 0) i = 0;
		if (j < 0) j = 0;

		if (i > mat->tWidth - 1) i = mat->tWidth - 1;
		if (j > mat->tHeight - 1) j = mat->tHeight - 1;


		int n = i + (j*mat->tWidth);

		//if (n > mat->tWidth * mat->tHeight) std::cout << n << "\n";

		colour = mat->texture[n];

		return colour;
	}

	/**
	 * Computes whether a ray hit the specific instance of a sphere shape and returns the hit data
	 *
	 * @param ray cast ray to check for intersection with shape
	 *
	 * @return hit struct containing intersection information
	 *
	 */
	Hit Sphere::intersect(Ray ray){

		Hit h;
		float intersect0, intersect1;
		Vec3f L = this->center - ray.origin;
		float intersectca = L.dotProduct(ray.direction);
		if (intersectca < 0) return h;
		float d2 = L.dotProduct(L) - (intersectca*intersectca);
		float r2 = this->radius * this-> radius;
		if (d2 > r2) return h;
		float intersecthc = sqrt(r2 - d2);
		intersect0 = intersectca - intersecthc;
		intersect1 = intersectca + intersecthc;
		if (intersect0 > intersect1) std::swap(intersect0, intersect1);
		if (intersect0 < 0) {
			if (intersect1 < 0) {
				return h;
			}
			h.point0 = ray.origin + intersect1*ray.direction;
		}
		else {
			h.point0 = ray.origin + intersect0*ray.direction;
			h.point1 = ray.origin + intersect1*ray.direction;
		}
		return h;

	}

	Vec3f Sphere::getNormal(Vec3f surface) {
		return (surface - center).normalize();
	}

} //namespace rt


