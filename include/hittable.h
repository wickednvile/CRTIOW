#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"

typedef struct material material;

typedef struct hitRecord {
    point3 p;
    vec3 normal;
    material* mat;
    double t;
    int frontFace; // this is a bool but im not using bools in C
} hitRecord;

static void setFaceNormal(hitRecord* h, const ray* r, const vec3 outwardNormal) {
    h->frontFace = dot(r->dir, outwardNormal) < 0;
    h->normal    = h->frontFace ? outwardNormal : negateVector(outwardNormal);
}

#endif 
