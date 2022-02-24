/*
 * Scene.cpp
 *
 */
#include "Scene.h"
#include "Shape.h"
#include "parsers/PPMWriter.h"
#include <fstream>

#include "parsers/PPMWriter.h"

namespace rt{

/**
 * Parses json scene object to generate scene to render
 *
 * @param scenespecs the json scene specificatioon
 */
void Scene::createScene(Value& scenespecs){

	bgcolour = (scenespecs["backgroundcolor"][0].GetFloat()*255, scenespecs["backgroundcolor"][1].GetFloat()*255, scenespecs["backgroundcolor"][2].GetFloat()*255);

	Value& light = scenespecs["lightsources"];
	for (int i = 0; i < light.Size(); i++) {
		std::string temp = light[i]["type"].GetString();
		if (temp.compare("pointlight") == 0) {
			Vec3f position;
			float p0 = light[i]["position"][0].GetFloat();
			float p1 = light[i]["position"][1].GetFloat();
			float p2 = light[i]["position"][2].GetFloat();
			Vec3f temp0(p0, p1, p2);
			position = temp0;
			Vec3f is;
			p0 = light[i]["is"][0].GetFloat();
			p1 = light[i]["is"][1].GetFloat();
			p2 = light[i]["is"][2].GetFloat();
			Vec3f temp1(p0, p1, p2);
			is = temp1;
			Vec3f id;
			p0 = light[i]["id"][0].GetFloat();
			p1 = light[i]["id"][1].GetFloat();
			p2 = light[i]["id"][2].GetFloat();
			Vec3f temp2(p0, p1, p2);
			id = temp2;
			lightSources.push_back(LightSource::createPointLight(position, is, id));
		}
		else if (temp.compare("arealight") == 0) {
			Vec3f position;
			float p0 = light[i]["position"][0].GetFloat();
			float p1 = light[i]["position"][1].GetFloat();
			float p2 = light[i]["position"][2].GetFloat();
			Vec3f temp0(p0, p1, p2);
			position = temp0;
			Vec3f is;
			p0 = light[i]["is"][0].GetFloat();
			p1 = light[i]["is"][1].GetFloat();
			p2 = light[i]["is"][2].GetFloat();
			Vec3f temp1(p0, p1, p2);
			is = temp1;
			Vec3f id;
			p0 = light[i]["id"][0].GetFloat();
			p1 = light[i]["id"][1].GetFloat();
			p2 = light[i]["id"][2].GetFloat();
			Vec3f temp2(p0, p1, p2);
			id = temp2;
			int cell = light[i]["cell"].GetInt();
			float radius = light[i]["radius"].GetFloat();
			lightSources.push_back(LightSource::createAreaLight(position, is, id, cell, radius));			
		}
	}

	Value& ss = scenespecs["shapes"];
	for (int i = 0; i < ss.Size(); i++) {
		std::string temp = ss[i]["type"].GetString();
		if (temp.compare("sphere") == 0) {
			float cen0 = ss[i]["center"][0].GetFloat();
            float cen1 = ss[i]["center"][1].GetFloat();
            float cen2 = ss[i]["center"][2].GetFloat();
			Vec3f temp(cen0,cen1,cen2);

			Material* m = new Material();
			m->ks = ss[i]["material"]["ks"].GetFloat();
			m->kd = ss[i]["material"]["kd"].GetFloat();
			m->kr = 0.0;
			float c0 = 255*ss[i]["material"]["diffusecolor"][0].GetFloat();
			float c1 = 255*ss[i]["material"]["diffusecolor"][1].GetFloat();
			float c2 = 255*ss[i]["material"]["diffusecolor"][2].GetFloat();
			Vec3f temp0(c0,c1,c2);
			m->diffusecolor = temp0;
			m->specularexponent = ss[i]["material"]["specularexponent"].GetInt();

			m->hasTexture = false;
			if (ss[i]["material"].HasMember("tPath")) {
				
				m->hasTexture = true;
				Vec3f* colour;
				Vec3f c(0,0,0);
				std::ifstream infile;
				std::string path = ss[i]["material"]["tPath"].GetString();
				infile.open(path.c_str(), std::ios::binary);
				std::string header;
				infile >> header;
				if (strcmp(header.c_str(), "P6") == 0) {
					unsigned char comment;
					unsigned int w, h, p;
					infile >> comment;
					if (comment=='#') {
						infile.ignore(256,'\n');
						infile >> w >> h >> p;
					}
					else {
						infile.seekg(0);
						infile >> header;
						infile >> w >> h >> p;
					}
					m->tWidth = w;
					m->tHeight = h;
					colour = new Vec3f[w*h];
					unsigned char pix[3];
					for (int i = 0; i < w*h ; i++) {
						infile.read(reinterpret_cast<char *>(pix), 3); 
						c.x = pix[1] / 255.f;
						c.y = pix[2] / 255.f;
						c.z = pix[0] / 255.f;
						colour[i] = 255*c;
					}
					infile.close();
					PPMWriter::PPMWriter(w, h, colour, "./debug/sphere_texture.ppm"); //testing texture parsed correctly
				}

				m->hasTexture = true;
				m->texture = colour;
			}
			Shape* s = Shape::createSphere(temp, ss[i]["radius"].GetFloat(), m);
			shapes.push_back(s);
		}
		else if (temp.compare("triangle") == 0) {
			float v00 = ss[i]["v0"][0].GetFloat();
			float v01 = ss[i]["v0"][1].GetFloat();
			float v02 = ss[i]["v0"][2].GetFloat();
			Vec3f t0(v00, v01, v02);
			float v10 = ss[i]["v1"][0].GetFloat();
			float v11 = ss[i]["v1"][1].GetFloat();
			float v12 = ss[i]["v1"][2].GetFloat();
			Vec3f t1(v10, v11, v12);
			float v20 = ss[i]["v2"][0].GetFloat();
			float v21 = ss[i]["v2"][1].GetFloat();
			float v22 = ss[i]["v2"][2].GetFloat();
			Vec3f t2(v20, v21, v22);

			Material* m = new Material();
			m->ks = ss[i]["material"]["ks"].GetFloat();
			m->kd = ss[i]["material"]["kd"].GetFloat();
			if (ss[i]["material"].HasMember("kr")) m->kr = ss[i]["material"]["kr"].GetFloat();
			else m->kr = 0.0;
			m->specularexponent = ss[i]["material"]["specularexponent"].GetInt();
			float c0 = 255*ss[i]["material"]["diffusecolor"][0].GetFloat();
			float c1 = 255*ss[i]["material"]["diffusecolor"][1].GetFloat();
			float c2 = 255*ss[i]["material"]["diffusecolor"][2].GetFloat();
			Vec3f temp0(c0,c1,c2);
			m->diffusecolor = temp0;
			m->hasTexture = false;
			if (ss[i]["material"].HasMember("tPath")) {

				m->hasTexture = true;

				m->tWidth = ss[i]["material"]["tWidth"].GetInt();
				m->tHeight = ss[i]["material"]["tHeight"].GetInt();
			}

			Shape* t = Shape::createTriangle(t0, t1, t2, m);
			shapes.push_back(t);
		}
		else if (temp.compare("plane") == 0)
		{
			float v00 = ss[i]["v0"][0].GetFloat();
			float v01 = ss[i]["v0"][1].GetFloat();
			float v02 = ss[i]["v0"][2].GetFloat();
			Vec3f v0(v00, v01, v02);
			float v10 = ss[i]["v1"][0].GetFloat();
			float v11 = ss[i]["v1"][1].GetFloat();
			float v12 = ss[i]["v1"][2].GetFloat();
			Vec3f v1(v10, v11, v12);
			float v20 = ss[i]["v2"][0].GetFloat();
			float v21 = ss[i]["v2"][1].GetFloat();
			float v22 = ss[i]["v2"][2].GetFloat();
			Vec3f v2(v20, v21, v22);
			float v30 = ss[i]["v3"][0].GetFloat();
			float v31 = ss[i]["v3"][1].GetFloat();
			float v32 = ss[i]["v3"][2].GetFloat();
			Vec3f v3(v30, v31, v32);

			Material* m = new Material();
			m->ks = ss[i]["material"]["ks"].GetFloat();
			m->kd = ss[i]["material"]["kd"].GetFloat();
			if (ss[i]["material"].HasMember("kr")) m->kr = ss[i]["material"]["kr"].GetFloat();
			else m->kr = 0.0;
			m->specularexponent = ss[i]["material"]["specularexponent"].GetInt();
			float c0 = 255*ss[i]["material"]["diffusecolor"][0].GetFloat();
			float c1 = 255*ss[i]["material"]["diffusecolor"][1].GetFloat();
			float c2 = 255*ss[i]["material"]["diffusecolor"][2].GetFloat();
			Vec3f temp0(c0,c1,c2);
			m->diffusecolor = temp0;
			m->hasTexture = false;
			if (ss[i]["material"].HasMember("tPath")) {
				
				m->hasTexture = true;
				Vec3f* colour;
				Vec3f c(0,0,0);
				std::ifstream infile;
				std::string path = ss[i]["material"]["tPath"].GetString();
				infile.open(path.c_str(), std::ios::binary);
				std::string header;
				infile >> header;
				if (strcmp(header.c_str(), "P6") == 0) {
					unsigned char comment;
					unsigned int w, h, p;
					infile >> comment;
					if (comment=='#') {
						infile.ignore(256,'\n');
						infile >> w >> h >> p;
					}
					else {
						infile.seekg(0);
						infile >> header;
						infile >> w >> h >> p;
					}
					m->tWidth = w;
					m->tHeight = h;
					colour = new Vec3f[w*h];
					unsigned char pix[3];
					float x = 0.0, y = 0.0;
					for (int i = 0; i < w*h ; i++) {
						infile.read(reinterpret_cast<char *>(pix), 3); 
						c.x = pix[1] / 255.f;
						c.y = pix[2] / 255.f;
						c.z = pix[0] / 255.f;
						colour[i] = 255*c;
					}
					infile.close();
					PPMWriter::PPMWriter(w, h, colour, "./debug/plane_texture.ppm"); //testing texture parsed correctly
				}
				m->hasTexture = true;
				m->texture = colour;
			}
			Shape* p = Shape::createPlane(v0, v1, v2, v3, m);
			shapes.push_back(p);
		}
		else if (temp.compare("mesh") == 0) {
			int numVertice = ss[i]["vertices"].Size();
			int faces = ss[i]["faces"].GetInt();
			int * numVerticesOnEachFace = new int[ss[i]["numVerticesOnEachFace"].Size()];
			for (int j = 0; j < ss[i]["numVerticesOnEachFace"].Size(); j++) {
				numVerticesOnEachFace[j] = ss[i]["numVerticesOnEachFace"][j].GetInt();
			}
			int * verticesOnEachFace = new int[ss[i]["verticesOnEachFace"].Size()];
			for (int j = 0; j < ss[i]["verticesOnEachFace"].Size(); j++) {
				verticesOnEachFace[j] = ss[i]["verticesOnEachFace"][j].GetInt();
			}
			Vec3f* vertices = new Vec3f[numVertice];
			for (int j = 0; j < numVertice; j++) {
				float v0 = ss[i]["vertices"][j][0].GetFloat();
				float v1 = ss[i]["vertices"][j][1].GetFloat();
				float v2 = ss[i]["vertices"][j][2].GetFloat();
				Vec3f temp0(v0, v1, v2);
				vertices[j] = temp0;
			}

			Material* m = new Material();
			float c0 = 255*ss[i]["material"]["diffusecolor"][0].GetFloat();
			float c1 = 255*ss[i]["material"]["diffusecolor"][1].GetFloat();
			float c2 = 255*ss[i]["material"]["diffusecolor"][2].GetFloat();
			Vec3f temp0(c0,c1,c2);
			m->diffusecolor = temp0;
			m->ks = ss[i]["material"]["ks"].GetFloat();
			m->kd = ss[i]["material"]["kd"].GetFloat();
			if (ss[i]["material"].HasMember("kr")) m->kr = ss[i]["material"]["kr"].GetFloat();
			else m->kr = 0.0;
			m->specularexponent = ss[i]["material"]["specularexponent"].GetInt();
			if (ss[i]["material"].HasMember("tPath")) {	
				m->hasTexture = true;
				Vec3f* colour;
				Vec3f c(0,0,0);
				std::ifstream infile;
				std::string path = ss[i]["material"]["tPath"].GetString();
				infile.open(path.c_str(), std::ios::binary);
				std::string header;
				infile >> header;
				if (strcmp(header.c_str(), "P6") == 0) {
					unsigned char comment;
					unsigned int w, h, p;
					infile >> comment;
					if (comment=='#') {
						infile.ignore(256,'\n');
						infile >> w >> h >> p;
					}
					else {
						infile.seekg(0);
						infile >> header;
						infile >> w >> h >> p;
					}
					m->tWidth = w;
					m->tHeight = h;
					colour = new Vec3f[w*h];
					unsigned char pix[3];
					float x = 0.0, y = 0.0;
					for (int i = 0; i < w*h ; i++) {
						infile.read(reinterpret_cast<char *>(pix), 3); 
						c.x = pix[1] / 255.f;
						c.y = pix[2] / 255.f;
						c.z = pix[0] / 255.f;
						colour[i] = 255*c;
					}
					infile.close();
					PPMWriter::PPMWriter(w, h, colour, "./debug/plane_texture.ppm"); //testing texture parsed correctly
				}
				m->hasTexture = true;
				m->texture = colour;
			}
			Shape* mesh = Shape::createMesh(vertices, faces, numVerticesOnEachFace, verticesOnEachFace, m, numVertice);
			shapes.push_back(mesh);
		}
	}

}



} //namespace rt
