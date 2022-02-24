/*
 * Pinhole.cpp
 *
 *
 */
#include <iostream>

#include "Pinhole.h"



namespace rt{

	//
	// Pinhole constructor (example)
	//
	Pinhole::Pinhole(Value& specs):Camera(specs){


		width = specs["width"].GetInt();
		height = specs["height"].GetInt();
		fov = specs["fov"].GetInt();
		Vec3f temp0(specs["lookat"][0].GetFloat(), specs["lookat"][1].GetFloat(), specs["lookat"][2].GetFloat());
		this->lookat = temp0;
		Vec3f temp1(specs["position"][0].GetFloat(), specs["position"][1].GetFloat(), specs["position"][2].GetFloat());
		this->position = temp1;
		Vec3f temp2(specs["up"][0].GetFloat(), specs["up"][1].GetFloat(), specs["up"][2].GetFloat());
		this->up = temp2;
		this->focal_length = 0.0;
		this->aperture = 0.0;
		this->type = "pinhole";

	}


	/**
	 * Prints camera data
	 * this function implements the abstract function of the base Camera class
	 */
	void Pinhole::printCamera(){
		printf("I am a pinhole camera! \n");
		printf("width: %dpx, height: %dpx, fov:%d \n", width, height, fov);
		std::cout << "Position, x: " << position.x << ", y: " << position.y << ", z: " << position.z << "\n";
	}



} //namespace rt

