#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"

struct hitRecord;

typedef struct matLambertian {
    int use;
    color albedo;
} matLambertian;

typedef struct matMetal {
    int use;
    color albedo;
    double fuzz;
} matMetal;

typedef struct matDielectric {
    int use;
    double refractionIndex;
} matDielectric;

typedef struct material {
    matLambertian lambert;
    matMetal metal;
    matDielectric dielectric;
} material;

int lambertianScatter(matLambertian mat, const ray* rIn, const hitRecord rec, color* attenuation, ray* scattered) {
    vec3 scatterDirection = vecAdd(rec.normal, randomUnitVector());

    if (nearZero(scatterDirection))
        scatterDirection = rec.normal;

    *scattered = constructRay(&rec.p, &scatterDirection);
    *attenuation = mat.albedo;
    return 1;
}

int metallicScatter(matMetal mat, const ray* rIn, const hitRecord rec, color* attenuation, ray* scattered) {
    mat.fuzz = (mat.fuzz <= 1.0) ? mat.fuzz : 1.0;
    vec3 reflected = reflect(rIn->dir, rec.normal);
    reflected = vecAdd(unitVector(reflected), (vecScalarMul(randomUnitVector(), mat.fuzz)));
    *scattered = constructRay(&rec.p, &reflected);
    *attenuation = mat.albedo;
    return (dot(scattered->dir, rec.normal) > 0);
}

double reflectance(double cosine, double refractionIndex) {
    double r0 = (1.0 - refractionIndex) / (1.0 + refractionIndex);
    r0 = r0*r0;
    return r0 + (1.0 - r0)*pow((1.0-cosine), 5.0);
}

int dielectricScatter(matDielectric mat, const ray* rIn, const hitRecord rec, color* attenuation, ray* scattered) {
    *attenuation = (color){1.0, 1.0, 1.0};
    double ri = (rec.frontFace) ? (1.0/mat.refractionIndex) : mat.refractionIndex; 

    vec3 unitDirection = unitVector(rIn->dir);
    double cosTheta = fmin(dot(negateVector(unitDirection), rec.normal), 1.0);
    double sinTheta = sqrt(1.0 - cosTheta*cosTheta);

    int cannotRefract = ri*sinTheta > 1.0;
    vec3 direction;

    if (cannotRefract || reflectance(cosTheta, ri) > randomDouble())
        direction = reflect(unitDirection, rec.normal);
    else
        direction = refract(unitDirection, rec.normal, ri);

    *scattered = constructRay(&rec.p, &direction);
    return 1;
}

int materialScatter(const ray* rIn, const hitRecord rec, color* attenuation, ray* scattered) {
    if (rec.mat->lambert.use)
        if (lambertianScatter(rec.mat->lambert, rIn, rec, attenuation, scattered))
            return 1;
    if (rec.mat->metal.use)
        if (metallicScatter(rec.mat->metal, rIn, rec, attenuation, scattered))
            return 1;
    if (rec.mat->dielectric.use)
        if (dielectricScatter(rec.mat->dielectric, rIn, rec, attenuation, scattered))
            return 1;

    return 0;
}

#endif
