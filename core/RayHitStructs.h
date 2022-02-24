/*
 * RayHitStructs.h
 */

#ifndef CORE_RAYHITSTRUCTS_H_
#define CORE_RAYHITSTRUCTS_H_

#include "math/geometry.h"

namespace rt{

/*
 * Ray structure definition
 */
enum RayType {PRIMARY, SECONDARY, SHADOW};

struct Ray{

	RayType raytype;

	Vec3f origin;
	Vec3f direction;
};


struct Hit{

	Vec3f point0; //point where ray hits a shape
	Vec3f point1; //unrelevant for now

};

}


#endif /* CORE_RAYHITSTRUCTS_H_ */
