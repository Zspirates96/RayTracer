/*
 * RayTracer.cpp
 *
 */
#include "RayTracer.h"
#include "RayHitStructs.h"
#include "math/geometry.h"
#include <math.h>
#include <random>

#define PI 3.14159265
#define albedo 0.18

namespace rt{


/**
 * Performs ray tracing to render a photorealistic scene
 *
 * @param camera the camera viewing the scene
 * @param scene the scene to render, including objects and lightsources
 * @param nbounces the number of bounces to consider for raytracing
 *
 * @return a pixel buffer containing pixel values in linear RGB format
 */

int numberOfIntersectionTest=0;
int numberofTrueIntersection=0;

Vec3f* RayTracer::render(Camera* camera, Scene* scene, int nbounces, int sampling){

	Vec3f* pixelbuffer=new Vec3f[camera->getWidth()*camera->getHeight()];

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> d(0.0, 1.0);
	std::uniform_real_distribution<float> dof(-1.0, 1.0);

	std::vector<Matrix44f> l2w;
	std::vector<LightSource*> lights = scene->getLights();
	std::vector<Ray> lightRays;
	std::vector<Ray> rays;

	//creating camera rays, light rays
	for (int i = 0; i < camera->getWidth(); i++) {
		for (int j = 0; j < camera->getHeight(); j++) {
			Ray r;
			r.raytype = PRIMARY;
			r.origin = camera->getPos();
			float imageAspectRatio = camera->getWidth() / camera->getHeight();
			double Px = ((2 * (i + 0.5) / camera->getWidth()) - 1) * tan((camera->getFov() / 2.0) * (PI / 180)) * imageAspectRatio;
			double Py = ((2 * (j + 0.5) / camera->getHeight()) - 1) * tan((camera->getFov() / 2.0) * (PI / 180));
			Vec3f temp(Px, Py, -1);
			r.direction = temp;
			Vec3f D = camera->getLookAt();
			D = -D.normalize();
			Vec3f U = camera->getUp();
			U = U.normalize();
			Vec3f R = -U.crossProduct(D);
			R = -R.normalize();
			U = R.crossProduct(D);
			U = U.normalize();
			Matrix44f c2w(R.x, R.y, R.z, 0, U.x, U.y, U.z, 0, D.x, D.y, D.z, 0, 0, 0, 0, 1);
			Matrix44f ca(1, 0, 0, -r.origin.x, 0, 1, 0, -r.origin.y, 0, 0, 1, -r.origin.z, 0, 0, 0, 1);
			c2w = c2w*ca;
			c2w.multDirMatrix(r.direction, r.direction);
			r.direction = r.direction.normalize();
			if (camera->getType() == "pinhole") rays.push_back(r);
			else if (camera->getType() == "thinlens") {
				Vec3f focal_point = r.origin + camera->getFocal()*r.direction;
				for (int nsampling = 0; nsampling < sampling; nsampling++) {
					float vv0 = dof(gen);
					float vv1 = dof(gen);
					float vv2 = dof(gen);
					Vec3f one(vv0,vv1,vv2);
					r.origin = r.origin + (one*(camera->getAperture()/sampling));
				}
				r.direction = (focal_point - r.origin).normalize();
				rays.push_back(r);
			}

			int size = lights.size();
			for (int k = 0; k < lights.size(); k++) {
				Vec3f L_U(0,0,1);
				Vec3f L_R(1,0,0);
				Vec3f L_D(0,1,0);
				Matrix44f lw(L_R.x, L_R.y, L_R.z, 0, L_U.x, L_U.y, L_U.z, 0, L_D.x, L_D.y, L_D.z, 0, 0, 0, 0, 1);
				Matrix44f la(1, 0, 0, -lights.at(k)->getPosition().x, 0, 1, 0, -lights.at(k)->getPosition().y, 0, 0, 1, -lights.at(k)->getPosition().z, 0, 0, 0, 1);
				l2w.push_back(lw*la);
				Ray ra;
				ra.raytype = SHADOW;
				ra.origin = lights.at(k)->getPosition();
				lightRays.push_back(ra);
			}
		}
	}
	std::vector<Shape*> ss;
	std::vector<Hit> intersection;
	std::vector<int> hitObject;
    //adding diffuse colour and specular to each pixel 
	for (int i = 0; i < camera->getWidth(); i++) {
		for (int j = 0; j < camera->getHeight(); j++) {
			Vec3f colour;
			Ray ray = rays.at(i*(camera->getHeight()) + j);
			ss = scene->getShapes();
			Vec3f closest(1e10,1e10,1e10);
			colour = scene->bgcolour;
			Hit h_temp;
			int object_temp = -1;
			for (int k = 0; k < ss.size(); k++) {
				Hit h = ss.at(k)->intersect(ray);
				numberOfIntersectionTest++;
				if (h.point0.x != 0 || h.point0.y != 0 || h.point0.z != 0) {
					if ((camera->getPos() - h.point0).length() < closest.length()) {
                        Vec3f c(0,0,0);
                        Vec3f specular(0,0,0);
                        //considering multiple light sources
						for (int b = 0; b < lights.size(); b++) {
							Ray lightRay = lightRays.at(b);
							lightRay.direction = h.point0 - lightRay.origin;
							float distance = lightRay.direction.length();
							lightRay.direction = lightRay.direction.normalize();
							Vec3f halfway;
							float blinn_phong = 0.0;
							float cosine = 0.0;
							Vec3f normal = ss.at(k)->getNormal(h.point0);
							normal = normal.normalize();
                            //check if it is under a shadow
							float shadow;
							if (lights.at(b)->getType() == "pointlight") {
								halfway = -(lightRay.direction + ray.direction).normalize();
								blinn_phong = halfway.dotProduct(normal);
								cosine = normal.dotProduct(-lightRay.direction);
								shadow = 1.0;
								for (int haha = 0; haha < ss.size(); haha++) {
									if (k == haha) continue;
									Hit blockedH = ss.at(haha)->intersect(lightRay);
									numberOfIntersectionTest++;
									if (blockedH.point0.x != 0 || blockedH.point0.y != 0 || blockedH.point0.z != 0) {
										Vec3f path = blockedH.point0 - lightRay.origin;
										if (distance > path.length()) {
											numberofTrueIntersection++;
											shadow = 0.0;
											haha+=ss.size();
										}
									}
								}
							}
							else if (lights.at(b)->getType() == "arealight") {
								int cell = lights.at(b)->getCell();
								shadow = float(cell)*float(cell);
								float cellSize = lights.at(b)->getRadius();
								if (cell % 2 == 0) {
									cellSize /= cell;
								}
								else cellSize /= (cell-0.5);
								Vec3f right(1,0,0);
								if (right.y == lightRay.direction.y && right.z == lightRay.direction.z) {
									if (right.x == lightRay.direction.x) {
										right.x = 0;
										right.y = -1;
									}
									else if (right.x == -lightRay.direction.x) {
										right.x = 0;
										right.y = 1;
									}
								}
								Vec3f up = right.crossProduct(lightRay.direction).normalize();
								right = up.crossProduct(lightRay.direction).normalize();
								Vec3f bottomLeft = lights.at(b)->getPosition();
								if (cell % 2 == 0) {
									bottomLeft = bottomLeft - ((cell/2.0)*right*cellSize) - ((cell/2.0)*up*cellSize);
								}
								else {
									bottomLeft = bottomLeft - (((cell-0.5)/2.0)*right*cellSize) - (((cell-0.5)/2.0)*up*cellSize);
								}
								Ray areaRay;
								for (int ii = 0; ii < cell; ii++) {
									for (int jj = 0; jj < cell; jj++) {
										int nsampling = 0;
										float shadow_sampling = sampling/2;
										while (nsampling < sampling) {
											float di = d(gen);
											float dj = d(gen);
											areaRay.origin = bottomLeft + (jj*right*cellSize) + (ii*up*cellSize) + (di*up*cellSize) + (dj*right*cellSize);
											areaRay.direction = h.point0 - areaRay.origin;
											distance = areaRay.direction.length();
											areaRay.direction = areaRay.direction.normalize();
											halfway = -(areaRay.direction + ray.direction).normalize();
											blinn_phong += halfway.dotProduct(normal);
											cosine += normal.dotProduct(-areaRay.direction);
											for (int haha = 0; haha < ss.size(); haha++) {
												if (k==haha) continue;
												Hit blockedH = ss.at(haha)->intersect(areaRay);
												numberOfIntersectionTest++;
												if (blockedH.point0.x != 0 || blockedH.point0.y != 0 || blockedH.point0.z != 0) {
													Vec3f path = blockedH.point0 - areaRay.origin;
													if (distance > path.length()){
														numberofTrueIntersection++;
														shadow_sampling -= 1.0;
														haha+=ss.size();
													}
												}
											}
											nsampling++;
										}
										if (shadow_sampling < 0) shadow-=1.0;
									}
								}
								cosine /= float(cell*cell*sampling);
								blinn_phong /= float(cell*cell*sampling);
								shadow /= float(cell*cell);
							}
                            //adding diffuse colour or texture colour
							Vec3f intensity = lights.at(b)->getid();
							intensity.x /= (4.0*PI*distance*distance);
							intensity.y /= (4.0*PI*distance*distance);
							intensity.z /= (4.0*PI*distance*distance);
							if (cosine < 0) {
								cosine = 0;
							}
							c = c + ss.at(k)->getMaterial()->kd*intensity*shadow*ss.at(k)->getColour(h.point0)*cosine;
							//blin-phong shading below
							if (blinn_phong < 0) blinn_phong = 0.0;
                            Vec3f intensitys = lights.at(b)->getis();
							intensitys.x /= (4.0*PI*distance*distance);
							intensitys.y /= (4.0*PI*distance*distance);
							intensitys.z /= (4.0*PI*distance*distance);
                            specular = specular + 255*ss.at(k)->getMaterial()->ks*shadow*intensitys*std::pow(blinn_phong, ss.at(k)->getMaterial()->specularexponent);
						}
						closest = camera->getPos() - h.point0;
						colour = c + specular;
						h_temp = h;
						object_temp = k;
					}
				}
			}
			numberofTrueIntersection++;
			hitObject.push_back(object_temp);
			intersection.push_back(h_temp);
			pixelbuffer[j*(camera->getWidth()) + i] = colour;	
		}
	}
    //adding reflection to each pixel
	for (int i = 0; i < camera->getWidth(); i++) {
		for (int j = 0; j < camera->getHeight(); j++) {
			Ray ray = rays.at(i*(camera->getHeight()) + j);
			if (hitObject.at(i*(camera->getHeight()) + j) == -1) continue;
			float KR = ss.at(hitObject.at(i*(camera->getHeight()) + j))->getMaterial()->kr;
            //check if kr is bigger than 0
			if (fabs(KR) > 1e-6) {
				Vec3f colour = scene->bgcolour;
                //considering multiple light sources
				for (int b = 0; b < lights.size(); b++) {
					int n = 1;
					Vec3f intensity = lights.at(b)->getid();
					Ray lightRay = lightRays.at(b);
					lightRay.direction = intersection.at(i*(camera->getHeight()) + j).point0 - lightRay.origin;
					lightRay.direction = lightRay.direction.normalize();
					Vec3f reflectedC(0,0,0);
					Vec3f reflectedPoint = intersection.at(i*(camera->getHeight()) + j).point0;
					Vec3f reflectedNormal = ss.at(hitObject.at(i*(camera->getHeight()) + j))->getNormal(reflectedPoint).normalize();
					Vec3f Incident = ray.direction.normalize();
					Ray reflectedRay;
					int reflectedObject = hitObject.at(i*(camera->getHeight()) + j);
					int lastReflectedObject = reflectedObject;
                    int lastHitObject = hitObject.at(i*(camera->getHeight()) + j);
                    //start calculating reflection with nbounces
					while (n <= nbounces) {
						Vec3f reflectedRayDir = Incident - (2*(Incident.dotProduct(reflectedNormal)*reflectedNormal));
						Ray reflectedRay;
						reflectedRay.origin = reflectedPoint;
						reflectedRay.direction = reflectedRayDir;
						float KR_temp = 0.0;
						Vec3f diffuse(0,0,0);
						for (int k = 0; k < ss.size(); k++) {
							if (k == reflectedObject) continue;
							Hit h = ss.at(k)->intersect(reflectedRay);
							if (h.point0.x != 0 || h.point0.y != 0 || h.point0.z != 0) {
								Ray rrRay;
								rrRay.origin = h.point0;
								rrRay.direction = h.point0 - reflectedPoint;
								float dd = rrRay.direction.length();
								rrRay.direction = -rrRay.direction.normalize();
								bool blocked = false;
								for (int kk = 0; kk < ss.size(); kk++) {
									if (kk == reflectedObject || kk == k) continue;
									Hit hh = ss.at(kk)->intersect(rrRay);
									numberOfIntersectionTest++;
									if ((hh.point0.x != 0 || hh.point0.y != 0 || hh.point0.z != 0) && dd > (hh.point0 - reflectedPoint).length()) {
										numberofTrueIntersection++;
										blocked = true;
										kk += ss.size();
									}
								}
								if (!blocked) {
									float distance = (h.point0 - lightRay.origin).length();
									Incident = reflectedRay.direction;
									reflectedNormal = ss.at(k)->getNormal(h.point0);
									reflectedNormal = reflectedNormal.normalize();
									reflectedPoint = h.point0;
									lastReflectedObject = reflectedObject;
									reflectedObject = k;
									float cosine = reflectedNormal.dotProduct(-Incident);
									if (cosine < 0) cosine = 0.0;
                                    float shadow = 1.0;
                                    Ray shadowray = lightRay;
                                    shadowray.direction = shadowray.origin - reflectedPoint;
                                    shadowray.direction = shadowray.direction.normalize();
                                    shadowray.origin = reflectedPoint;
                                    //check if reflection is under a shadow
									if (lights.at(b)->getType() == "pointlight") {
										for (int l = 0; l < ss.size(); l++) {
											if (l == k || l == lastHitObject) continue;
											Hit hshadow = ss.at(l)->intersect(shadowray);
											numberOfIntersectionTest++;
											if (hshadow.point0.x != 0 || hshadow.point0.y != 0 || hshadow.point0.z != 0) {
												shadow = 0.0;
												numberofTrueIntersection++;
												l += ss.size();
											}
										}
									}
									else if (lights.at(b)->getType() == "arealight") {
										int cell = lights.at(b)->getCell();
										shadow = float(cell)*float(cell);
										float cellSize = lights.at(b)->getRadius();
										if (cell % 2 == 0) {
											cellSize /= cell;
										}
										else cellSize /= (cell-0.5);
										Vec3f right(1,0,0);
										if (right.y == shadowray.direction.y && right.z == shadowray.direction.z) {
											if (right.x == shadowray.direction.x) {
												right.x = 0;
												right.y = -1;
											}
											else if (right.x == -shadowray.direction.x) {
												right.x = 0;
												right.y = 1;
											}
										}
										Vec3f up = right.crossProduct(shadowray.direction).normalize();
										right = up.crossProduct(shadowray.direction).normalize();
										Vec3f bottomLeft = lights.at(b)->getPosition();
										if (cell % 2 == 0) {
											bottomLeft = bottomLeft - ((cell/2.0)*right*cellSize) - ((cell/2.0)*up*cellSize);
										}
										else {
											bottomLeft = bottomLeft - (((cell-0.5)/2.0)*right*cellSize) - (((cell-0.5)/2.0)*up*cellSize);
										}
										Ray areaRay;
										for (int ii = 0; ii < cell; ii++) {
											for (int jj = 0; jj < cell; jj++) {
												int nsampling = 0;
												float shadow_sampling = sampling/2;
												while (nsampling < sampling) {
													float di = d(gen);
													float dj = d(gen);
													areaRay.origin = bottomLeft + (jj*right*cellSize) + (ii*up*cellSize) + (di*up*cellSize) + (dj*right*cellSize);
													areaRay.direction = h.point0 - areaRay.origin;
													distance = areaRay.direction.length();
													areaRay.direction = areaRay.direction.normalize();
													for (int haha = 0; haha < ss.size(); haha++) {
														if (k==haha) continue;
														Hit blockedH = ss.at(haha)->intersect(areaRay);
														numberOfIntersectionTest++;
														if (blockedH.point0.x != 0 || blockedH.point0.y != 0 || blockedH.point0.z != 0) {
															Vec3f path = blockedH.point0 - areaRay.origin;
															if (distance > path.length()){
																numberofTrueIntersection++;
																shadow_sampling -= 1.0;
																haha+=ss.size();
															}
														}
													}
													nsampling++;
												}
												if (shadow_sampling < 0) shadow -= 1.0;
											}
										}
										shadow /= float(cell*cell);
									}
									diffuse = diffuse + KR*shadow*ss.at(reflectedObject)->getMaterial()->kd*intensity*ss.at(reflectedObject)->getColour(h.point0)*(cosine/(4.0*PI*distance*distance));
									KR = ss.at(reflectedObject)->getMaterial()->kr;
									lastHitObject = k;
								}
							}
						}
						reflectedC = reflectedC + diffuse;
						if (reflectedObject == lastReflectedObject || fabs(KR) < 1e-6) n+= nbounces;
						else {
							n++;
						}
					}
					colour = colour + reflectedC;
				}
				if (lights.size() == 0) return pixelbuffer;
				colour /= lights.size();
				pixelbuffer[j*(camera->getWidth()) + i] = pixelbuffer[j*(camera->getWidth()) + i] + colour;
			}
		}
	}
    //255 is the max value so set it to 255 if more than 255
	for (int i = 0; i < camera->getWidth(); i++) {
		for (int j = 0; j < camera->getHeight(); j++) {
			if (pixelbuffer[j*(camera->getWidth()) + i].x > 255) pixelbuffer[j*(camera->getWidth()) + i].x = 255;
			if (pixelbuffer[j*(camera->getWidth()) + i].y > 255) pixelbuffer[j*(camera->getWidth()) + i].y = 255;
			if (pixelbuffer[j*(camera->getWidth()) + i].z > 255) pixelbuffer[j*(camera->getWidth()) + i].z = 255;
		}
	}		
	std::cout << "numberofTrueIntersection:" << numberofTrueIntersection << ", " << "numberOfIntersectionTest:" << numberOfIntersectionTest << "\n";		
	return pixelbuffer;
}

/**
 * Tonemaps the rendered image (conversion of linear RGB values [0-1] to low dynamic range [0-255]
 *
 * @param pixelbuffer the array of size width*height containing linear RGB pixel values
 *
 * @return the tonemapped image
 */
Vec3f* RayTracer::tonemap(Vec3f* pixelbuffer){



    //not used because everything is done in render()



	return pixelbuffer;

}





} //namespace rt


