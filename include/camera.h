#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "hittableList.h"
#include "material.h"

typedef struct camera {
// public: You can set in main
    double aspect;
    int imageWidth;
    int imageHeight;
    int samplesPerPixel;
    int maxDepth;
    double vFOV;
    point3 lookFrom;
    point3 lookAt;
    vec3 vUp;

    double defocusAngle;
    double focusDist;

// private: Don't use in main
    double pixelSamplesScale;
    point3 center;
    point3 pixel00Loc;
    vec3 pixelDeltaU;
    vec3 pixelDeltaV;
    vec3 u, v, w;
    vec3 defocusDiskU;
    vec3 defocusDiskV;
} camera;

void cameraInitialize(camera* cam) {
    // image settings, calculate height given a width
    cam->imageHeight = cam->imageWidth/cam->aspect;
    cam->imageHeight = (cam->imageHeight < 1) ? 1 : cam->imageHeight;

    cam->pixelSamplesScale = 1.0 / cam->samplesPerPixel;

    // viewport and camera
    cam->center = cam->lookFrom;

    double theta = degreesToRadians(cam->vFOV);
    double h = tan(theta/2.0);

    double viewportHeight = 2.0 * h * cam->focusDist;
    double viewportWidth  = viewportHeight * ((double)cam->imageWidth/cam->imageHeight);

    cam->w = unitVector(vecSub(cam->lookFrom, cam->lookAt));
    cam->u = unitVector(cross(cam->vUp, cam->w));
    cam->v = cross(cam->w, cam->u);

    vec3 viewportU = vecScalarMul(cam->u, viewportWidth);
    vec3 viewportV = vecScalarMul(negateVector(cam->v), viewportHeight);

    cam->pixelDeltaU = vecScalarDiv(viewportU, cam->imageWidth);
    cam->pixelDeltaV = vecScalarDiv(viewportV, cam->imageHeight);

    vec3 viewportUpperLeft = vecSub(vecSub(vecSub(cam->center, vecScalarMul(cam->w, cam->focusDist)), vecScalarDiv(viewportU, 2.0)), vecScalarDiv(viewportV, 2.0));
    cam->pixel00Loc = vecAdd(viewportUpperLeft, vecScalarMul(vecAdd(cam->pixelDeltaU, cam->pixelDeltaV), 0.5));

    double defocusRadius = cam->focusDist * tan(degreesToRadians(cam->defocusAngle/2.0));
    cam->defocusDiskU = vecScalarMul(cam->u, defocusRadius);
    cam->defocusDiskV = vecScalarMul(cam->v, defocusRadius);
}

color rayColor(const ray* r, int depth, hittableList* world) {
    if (depth <= 0)
        return (color){0.0, 0.0, 0.0};

    hitRecord rec = {0};
    if (hitAny(world, r, (interval){0.001, infinity}, &rec)) {
        ray scattered;
        color attenuation;
        
        if (materialScatter(r, rec, &attenuation, &scattered))
            return vecMul(attenuation, rayColor(&scattered, depth-1, world));
        return (color){0.0, 0.0, 0.0};
    }

    vec3 unitDirection = unitVector(r->dir);
    double a = 0.5 * (unitDirection.y + 1.0);
    return vecAdd(vecScalarMul((color){1.0, 1.0, 1.0}, (1.0-a)), vecScalarMul((color){0.3, 0.5, 1.0}, a));
}

static inline vec3 sampleSquare() {
    return (vec3){randomDouble() - 0.5, randomDouble() - 0.5, 0.0};
}

point3 defocusDiskSample(camera* cam) {
    vec3 p = randomInUnitDiskVector();
    return vecAdd(vecAdd(cam->center, vecScalarMul(cam->defocusDiskU, p.x)), vecScalarMul(cam->defocusDiskV, p.y));
}

ray getRay(camera* cam, int i, int j) {
    vec3 offset = sampleSquare();
    
    vec3 pixelSample = vecAdd(
            vecAdd(cam->pixel00Loc, vecScalarMul(cam->pixelDeltaU, i + offset.x)), 
            vecScalarMul(cam->pixelDeltaV, j + offset.y));

    point3 rayOrigin = (cam->defocusAngle <= 0) ? cam->center : defocusDiskSample(cam);
    vec3 rayDirection = vecSub(pixelSample, rayOrigin);

    return constructRay(&rayOrigin, &rayDirection);
}

void cameraRender(camera* cam, hittableList* world) {
    cameraInitialize(cam);

    printf("P3\n%d %d\n%d\n", cam->imageWidth, cam->imageHeight, 255);

    for (int j = 0; j < cam->imageHeight; ++j) {

        fprintf(stderr, "\rScanlines remaining: %d ", cam->imageHeight-j);

        for (int i = 0; i < cam->imageWidth; ++i) {

            color pixelColor = (color){0.0, 0.0, 0.0};

            for (int sample = 0; sample < cam->samplesPerPixel; sample++) {
                ray r = getRay(cam, i, j);
                pixelColor = vecAdd(pixelColor, rayColor(&r, cam->maxDepth, world));
            }

            color result = vecScalarMul(pixelColor, cam->pixelSamplesScale);
            writeColor(&result);
        }
    }
    fprintf(stderr, "\rFinished.                                  \n");
}


#endif
