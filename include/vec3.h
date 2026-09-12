#ifndef VEC3_H
#define VEC3_H

#include "rtweekend.h"

typedef struct vec3 {
    double x, y, z;
} vec3;

typedef vec3 point3;
typedef vec3 color;

static inline vec3 vecAdd(const vec3 u, const vec3 v)   { return (vec3){ u.x + v.x, u.y + v.y, u.z + v.z }; }
static inline vec3 vecSub(const vec3 u, const vec3 v)   { return (vec3){ u.x - v.x, u.y - v.y, u.z - v.z }; }
static inline vec3 vecMul(const vec3 u, const vec3 v)   { return (vec3){ u.x * v.x, u.y * v.y, u.z * v.z }; }
static inline vec3 vecScalarMul(const vec3 v, double k) { return (vec3){ k*v.x, k*v.y, k*v.z }; }
static inline vec3 vecScalarDiv(const vec3 v, double k) { return vecScalarMul(v, 1/k); }
static inline vec3 cross(const vec3 u, const vec3 v)    { return (vec3){ u.y * v.z - u.z * v.y,
                                                                         u.z * v.x - u.x * v.z,
                                                                         u.x * v.y - u.y * v.x }; }

static inline double dot(const vec3 u, const vec3 v)    { return u.x * v.x + u.y * v.y + u.z * v.z; }
static inline double lengthSqr(const vec3 v)            { return v.x * v.x + v.y * v.y + v.z * v.z; }
static inline double length(const vec3 v)               { return sqrt(lengthSqr(v)); }

static inline vec3 unitVector(const vec3 v)             { return vecScalarDiv(v, length(v)); }
static inline vec3 negateVector(const vec3 v)           { return (vec3){ -1.0*v.x, -1.0*v.y, -1.0*v.z }; }

vec3 randomVector() { return (vec3){randomDouble(), randomDouble(), randomDouble()}; }
vec3 randomVectorRange(double min, double max) { return (vec3){randomDoubleRange(min, max), randomDoubleRange(min, max), randomDoubleRange(min, max)}; }

static inline vec3 randomUnitVector() {
    while (1) {
        vec3 p = randomVectorRange(-1.0, 1.0);
        double lensq = lengthSqr(p);
        if (1e-160 < lensq && lensq <= 1.0)
            return unitVector(p);
    }
}

static inline vec3 randomInUnitDiskVector() {
    while (1) {
        vec3 p = (vec3){randomDoubleRange(-1.0, 1.0), randomDoubleRange(-1.0, 1.0), 0.0};
        if (lengthSqr(p) < 1)
            return p;
    }
}

static inline vec3 randomOnHemisphere(const vec3 normal) {
    vec3 onUnitSphere = randomUnitVector();
    if (dot(onUnitSphere, normal) > 0.0)
        return onUnitSphere;
    else
        return negateVector(onUnitSphere);
}

static inline int nearZero(vec3 v)                      { return (fabs(v.x) < 1e-8) && (fabs(v.y) < 1e-8) && (fabs(v.z) < 1e-8); }
static inline vec3 reflect(const vec3 v, const vec3 n)  { return vecSub(v, vecScalarMul(vecScalarMul(n, dot(v, n)), 2.0)); }
static inline vec3 refract(const vec3 uv, const vec3 n, double etaiOverEtat) {
    double cosTheta = fmin(dot(negateVector(uv), n), 1.0);
    vec3 rOutPerp = vecScalarMul(vecAdd(uv, vecScalarMul(n, cosTheta)), etaiOverEtat);
    vec3 rOutParallel = vecScalarMul(negateVector(n), sqrt(fabs(1.0-lengthSqr(rOutPerp))));
    return vecAdd(rOutPerp, rOutParallel);
}

#endif
