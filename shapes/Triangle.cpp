/*
 * Triangle.cpp
 *
 *
 */
#include "Triangle.h"



namespace rt{

    Triangle::~Triangle(){};

    Triangle::Triangle(Vec3f v0, Vec3f v1, Vec3f v2, Material* mat){
        this->mat = mat;
        this->v0 = v0;
        this->v1 = v1;
        this->v2 = v2;
        Vec3f zero(0,0,0);
        this->v3 = zero;
        this->type = "triangle";
        this->num = 3;
    }

    Triangle::Triangle(Vec3f v0, Vec3f v1, Vec3f v2, Vec3f v3, Material* mat){
        this->mat = mat;
        this->v0 = v0;
        this->v1 = v1;
        this->v2 = v2;
        this->v3 = v3;
        this->type = "plane";
        this->num = 4;
    }

    Vec3f* Triangle::getVertices() {
        Vec3f* v;
        if (this->v3.x != 0 || this->v3.y != 0 || this->v3.z != 0) {
            v = new Vec3f[4];
            v[0] = this->v0;
            v[1] = this->v1;
            v[2] = this->v2;
            v[3] = this->v3;
        }
        else {
            v = new Vec3f[3];
            v[0] = this->v0;
            v[1] = this->v1;
            v[2] = this->v2;
        }
        return v;
    }

    Vec3f Triangle::getColour(Vec3f point) {
        if (!this->mat->hasTexture) return this->mat->diffusecolor;

        Vec3f colour(0,0,0);
        if (this->v3.x != 0 || this->v3.y != 0 || this->v3.z != 0) {
            Vec3f e1 = (v1-v0);
            Vec3f e2 = (v3-v0);
            Vec3f uv = (point - v0);

            int u = mat->tWidth*(uv.dotProduct(e1)/(e1.length()*e1.length()));
            int v = mat->tHeight*(uv.dotProduct(e2)/(e2.length()*e2.length()));

            if (u < 0) u = 0;
            if (v < 0) v = 0;
            if (u > mat->tWidth-1) u = mat->tWidth-1;
            if (v > mat->tHeight-1) v = mat->tHeight-1;

            int n = u + (v*mat->tWidth);
            colour = mat->texture[n];
        }
        return colour;
    }

    Hit Triangle::intersect(Ray ray) {
        
        Hit h;
        Vec3f N;
        N = (this->v1 - this->v0).crossProduct(this->v2 - this->v0);
        if (this->v3.x != 0 || this->v3.y != 0 || this->v3.z != 0) {
            N = (this->v3 - this->v0).crossProduct(this->v1 - this->v0);
        }
        this->normal = N.normalize();
        float denom = N.dotProduct(ray.direction);
        if (fabs(denom) < 1e-10) return h;
        Vec3f D = ray.origin - this->v0;
        float t = -N.dotProduct(D) / denom;
        if (t < 0) return h;
        Vec3f P = ray.origin + (t*ray.direction);

        Vec3f e0 = this->v1 - this->v0;
        Vec3f e1 = this->v2 - this->v1;
        Vec3f e2 = this->v0 - this->v2;
        Vec3f c0 = P - this->v0;
        Vec3f c1 = P - this->v1;
        Vec3f c2 = P - this->v2;
        if (this->v3.x != 0 || this->v3.y != 0 || this->v3.z != 0) {
            e2 = this->v3 - this->v2;
            Vec3f e3 = this->v0 - this->v3;
            Vec3f c3 = P - this->v3;
            if (N.dotProduct(e3.crossProduct(c3)) > 0 || N.dotProduct(e0.crossProduct(c0)) > 0 || N.dotProduct(e1.crossProduct(c1)) > 0 || N.dotProduct(e2.crossProduct(c2)) > 0) return h;
        }
        //plane has vertex in clockwise direction but triangle has vertex in anti-clockwise direction, based on example.json
        else {
            if (N.dotProduct(e0.crossProduct(c0)) < 0 || N.dotProduct(e1.crossProduct(c1)) < 0 || N.dotProduct(e2.crossProduct(c2)) < 0) return h;
        }
        h.point0 = P;
        return h;

    }

} //namespace rt
