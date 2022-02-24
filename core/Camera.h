/*
 * Camera.h
 *
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "rapidjson/document.h"
#include "math/geometry.h"


using namespace rapidjson;

namespace rt{

class Camera{
public:

	//
	// Constructors
	//
	Camera(){};
	Camera(Value& specs):height(height), width(width), fov(fov){};

	//
	// Destructor
	//
	virtual ~Camera();


	//
	// factory function : returns camera instance dynamically based on camera specifications
	//
	static Camera* createCamera(Value& cameraSpecs);


	//
	// print function (to be implemented by the subclasses )
	//
	virtual void printCamera()=0;



	//
	// other camera functions (to complete)
	//












	//
	// Getters and Setters
	//
	int getHeight() const {
		return height;
	}

	void setHeight(int height) {
		this->height = height;
	}

	int getWidth() const {
		return width;
	}

	void setWidth(int width) {
		this->width = width;
	}

	int getFov() const{
		return fov;
	}

	Vec3f getPos() {
		return position;
	}

	Vec3f getLookAt() {
		return lookat;
	}

	Vec3f getUp() {
		return up;
	}

	std::string getType() { 
		return type;
	}

	float getFocal() {
		return focal_length;
	}

	float getAperture() {
		return aperture;
	}

protected:

	//
	//camera members
	//
	int height;
	int width;
	int fov; //field of view
	Vec3f lookat;
	Vec3f up;
	Vec3f position;
	float focal_length;
	float aperture;
	std::string type;

};

} //namespace rt



#endif /* CAMERA_H_ */
