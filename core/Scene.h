/*
 * Scene.h
 *
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <vector>
#include "core/LightSource.h"
#include "core/Shape.h"
#include "math/geometry.h"
#include "core/Material.h"


using namespace rapidjson;

namespace rt{

class Scene {
public:

	Scene(){};

	void createScene(Value& scenespecs);

	Vec3f bgcolour;

	std::vector<Shape*> getShapes(){
		return shapes;
	}

	std::vector<LightSource*> getLights() {
		return lightSources;
	}

private:

	std::vector<LightSource*> lightSources;
	std::vector<Shape*> shapes;

};

} //namespace rt



#endif /* SCENE_H_ */
