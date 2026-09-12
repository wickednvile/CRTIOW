#ifndef SPHERE_H
#define SPHERE_H

#include "rtweekend.h"
#include "hittable.h"

typedef struct sphere {
    point3 center;
    double radius;
    material* mat;
} sphere;

sphere constructSphere(const point3 center, double radius, material* mat) {
    return (sphere){center, fmax(0, radius), mat};
}

int hitSphere(sphere s, const ray* r, interval rayT, hitRecord* rec) {
    vec3 oc = vecSub(s.center, r->origin);
    double a = lengthSqr(r->dir);
    double h = dot(r->dir, oc);
    double c = lengthSqr(oc) - s.radius * s.radius;
    double discriminant = h*h - a*c;

    if (discriminant < 0.0)
        return 0;

    double sqrtD = sqrt(discriminant);

    double root = (h - sqrtD) / a;
    if (!intervalSurrounds(&rayT, root)) {
        root = (h + sqrtD) / a;
        if (!intervalSurrounds(&rayT, root))
            return 0;
    }

    rec->t = root;
    rec->p = rayAt(r, rec->t);
    vec3 outwardNormal = vecScalarDiv(vecSub(rec->p, s.center), s.radius);
    setFaceNormal(rec, r, outwardNormal);
    rec->mat = s.mat;

    return 1;
}

#endif
