#ifndef RAY_H
#define RAY_H

#include "vec3.h"

typedef struct ray {
    point3 origin;
    vec3 dir;
} ray;

ray constructRay(const point3* origin, const vec3* dir) { return (ray){ *origin, *dir }; }
point3 rayAt(const ray* r, double t)                    { return vecAdd(r->origin, vecScalarMul(r->dir, t)); }

#endif
