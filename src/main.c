#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittableList.h"
#include "material.h"
#include "sphere.h"

int main(void) {
    // world or scene
    hittableList world = {0};

    material materialGround   = {0};
    materialGround.lambert    = (matLambertian){ 1, (color){0.5, 0.5, 0.5}};
    sphere ground = constructSphere( (point3){0.0, -1000.0, 0.0}, 1000.0, &materialGround );
    addHitObj(&world, &ground);

    int size = 0;
    for (int a = -11; a < 11; a++)
        for (int b = -11; b < 11; b++)
            size++;

    material* sphereMaterial = calloc(size, sizeof(material));
    sphere* spheres          = calloc(size, sizeof(sphere));

    int i = 0;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++, i++) {
            double chooseMat = randomDouble();
            point3 center = (point3){a + 0.9*randomDouble(), 0.2, b + 0.9*randomDouble()};

            if (length(vecSub(center, (point3){4.0, 0.2, 0.0})) > 0.9) {
                if (chooseMat < 0.8) {
                    // lamberian diffuse
                    color albedo = vecMul(randomVector(), randomVector());
                    sphereMaterial[i].lambert = (matLambertian){1, albedo};
                    spheres[i] = constructSphere( center, 0.2, &sphereMaterial[i] );
                    addHitObj(&world, &spheres[i]);
                } else if (chooseMat < 0.95) {
                    // metal
                    color albedo = randomVectorRange(0.5, 1.0);
                    double fuzz = randomDoubleRange(0.0, 0.5);
                    sphereMaterial[i].metal = (matMetal){1, albedo, fuzz};
                    spheres[i] = constructSphere( center, 0.2, &sphereMaterial[i]);
                    addHitObj(&world, &spheres[i]);
                } else {
                    // dielectric glass
                    sphereMaterial[i].dielectric = (matDielectric){1, 1.5};
                    spheres[i] = constructSphere( center, 0.2, &sphereMaterial[i]);
                    addHitObj(&world, &spheres[i]);
                }
            }

            // fprintf(stderr, "%p\n", (void*)&sphereMaterial[i]);
        }
    }

    material material1   = {0}, material2 = {0}, material3 = {0};
    material1.dielectric = (matDielectric){1, 1.5};
    sphere s1            = constructSphere( (point3){0.0, 1.0, 0.0}, 1.0, &material1 );
    addHitObj(&world, &s1);

    material2.lambert    = (matLambertian){1, (color){0.4, 0.2, 0.1}};
    sphere s2            = constructSphere( (point3){-4.0, 1.0, 0.0}, 1.0, &material2 );
    addHitObj(&world, &s2);

    material3.metal      = (matMetal)     {1, (color){0.7, 0.6, 0.5}, 0.0};
    sphere s3            = constructSphere( (point3){4.0, 1.0, 0.0}, 1.0, &material3 );
    addHitObj(&world, &s3);

    // camera settigs
    camera cam          = {0};
    cam.aspect          = 16.0/9.0;
    cam.imageWidth      = 2048;
    cam.samplesPerPixel = 500;
    cam.maxDepth        = 50;

    cam.vFOV            = 20.0;
    cam.lookFrom        = (point3){ 13.0, 2.0, 3.0};
    cam.lookAt          = (point3){  0.0, 0.0, 0.0};
    cam.vUp             = (vec3)  {  0.0, 1.0, 0.0};

    cam.defocusAngle    = 0.6;
    cam.focusDist       = 10.0;

    cameraRender(&cam, &world);

    free(sphereMaterial);
    free(spheres);

    return 0;
}
