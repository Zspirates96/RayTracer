/*
 * TriMesh.cpp
 *
 *
 */
#include "TriMesh.h"


namespace rt{

    TriMesh::TriMesh(Vec3f* v, int f, int* nvef, int*vef, Material* mat, int numVertice) {
        this->vertices = v;
        this->faces = f;
        this->numVef = nvef;
        this->vef = vef;
        this->mat = mat;
        this->type = "mesh";
        this->numVertice = numVertice;

        int k = 0;
        int n = 0;
        int number = 0;
        for (int i = 0; i < f; i++) {
            number += nvef[i] - 2;
        }
        this->normals = new Vec3f[number];
        this->numTri = number;
        for (int i = 0; i < f; i++) {
            if (nvef[i] < 3) break;
            for (int j = 0; j < nvef[i] - 2; j++) {
                Vec3f* tri = new Vec3f[3];
                tri[0] = vertices[vef[k]];
                tri[1] = vertices[vef[k+j+1]];
                tri[2] = vertices[vef[k+j+2]];
                Vec3f N = (tri[1]-tri[0]).crossProduct(tri[2]-tri[0]);
                this->normals[n] = N.normalize();
                this->triangles.push_back(tri);
                n++;
            }
            k += nvef[i];
        }
    }

    Vec3f TriMesh::getNormal(Vec3f surface) {
        Vec3f N(0,0,0);
        for (int i = 0; i < this->numTri; i++) {
            N = this->normals[i];
            Vec3f* triangle = triangles.at(i);
            Vec3f test = surface - triangle[0];
            if (fabs(test.dotProduct(N)) > 1e-4) continue;
            Vec3f e0 =  triangle[1] -  triangle[0];
            Vec3f e1 =  triangle[2] -  triangle[1];
            Vec3f e2 =  triangle[0] -  triangle[2];
            Vec3f c0 = surface -  triangle[0];
            Vec3f c1 = surface -  triangle[1];
            Vec3f c2 = surface -  triangle[2];
            if (N.dotProduct(e0.crossProduct(c0)) > 0 && N.dotProduct(e1.crossProduct(c1)) > 0 && N.dotProduct(e2.crossProduct(c2)) > 0) return N;
        }
        return N;
    }

    Vec3f TriMesh::getColour(Vec3f point) {
        return this->mat->diffusecolor;
    }

    Hit TriMesh::intersect(Ray ray) {
        Hit h;
        Vec3f big(1e10,1e10,1e10);
        h.point0 = big;
        for (int i = 0; i < this->numTri; i++) {
            Vec3f N = this->normals[i];
            float denom = N.dotProduct(ray.direction);
            if (fabs(denom) < 1e-10) continue;
            Vec3f* triangle = triangles.at(i);
            Vec3f D = ray.origin - triangle[0];
            float t = -N.dotProduct(D) / denom;
            if (t < 0) continue;
            Vec3f P = ray.origin + (t*ray.direction);

            Vec3f e0 =  triangle[1] -  triangle[0];
            Vec3f e1 =  triangle[2] -  triangle[1];
            Vec3f e2 =  triangle[0] -  triangle[2];
            Vec3f c0 = P -  triangle[0];
            Vec3f c1 = P -  triangle[1];
            Vec3f c2 = P -  triangle[2];
            if (N.dotProduct(e0.crossProduct(c0)) > 0 && N.dotProduct(e1.crossProduct(c1)) > 0 && N.dotProduct(e2.crossProduct(c2)) > 0) {
                if ((P - ray.origin).length() < (h.point0 - ray.origin).length()) h.point0 = P;
            }
        }
        if (h.point0.x == big.x && h.point0.y == big.y && h.point0.z == big.z) {
            h.point0.x = 0;
            h.point0.y = 0;
            h.point0.z = 0;
        }
        return h;
    }


} //namespace rt


