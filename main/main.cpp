/*
 * main.cpp
 *
 */

#include <iostream>
#include <fstream>

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

#include "math/geometry.h"
#include "parsers/PPMWriter.h"

#include "core/RayTracer.h"
#include "core/Camera.h"
#include "core/Scene.h"


using namespace rt;
using namespace rapidjson;

int main(int argc, char* argv[]){
	//parse commandline arguments
	char* inputFile=argv[1];    //first command line argument holds the path to the json input file
	char* outputFile=argv[2];   //second command line argument holds the path to the output image file

	std::printf("Input file: %s\n",inputFile);

	//parse json input file using rapidjson
	std::ifstream ifs(inputFile);
	IStreamWrapper is(ifs);
	Document d;     // d holds the complete json object
	d.ParseStream(is);

	//generate a camera according to the input file
	Camera* camera=Camera::createCamera(d["camera"]);

	//print camera data (based on the input file provided)
	camera->printCamera();
	//generate the scene according to the input file
	Scene* scene=new Scene();
	scene->createScene(d["scene"]);

	//
	// Main function, render scene
	//
	int sampling = 1;
	if (d.HasMember("sampling")) sampling = d["sampling"].GetInt();
	clock_t timeStart = clock();
	Vec3f* pixelbuffer = RayTracer::render(camera, scene, d["nbounces"].GetInt(), sampling);
	std::cout << "render time:" <<(std::clock() - timeStart)/ (double) CLOCKS_PER_SEC << "\n";


	//free resources when rendering is finished
	delete scene;



	//convert linear RGB pixel values [0-1] to range 0-255
	RayTracer::tonemap(pixelbuffer);



	std::printf("Output file: %s\n",outputFile);

	//write rendered scene to file (pixels RGB values must be in range 0255)
	PPMWriter::PPMWriter(camera->getWidth(), camera->getHeight(), pixelbuffer, outputFile);

	delete pixelbuffer;
	delete camera;
}


