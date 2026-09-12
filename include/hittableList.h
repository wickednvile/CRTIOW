#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "rtweekend.h"
#include "sphere.h"

typedef struct hittableList {
    int n;
    sphere* objects[10000];
} hittableList;

void addHitObj(hittableList* hl, sphere* obj) {
    int i = (hl->n += 1);
    hl->objects[i-1] = obj;
}

int hitAny(hittableList* hl, const ray* r, interval rayT, hitRecord* rec) {
    hitRecord tempRec;
    tempRec.t = 0.0;
    int hitAnything = 0;
    double closestSoFar = rayT.max;

    for (int i = 0; i < hl->n; i++) {
        if (hitSphere(*(hl->objects[i]), r, (interval){rayT.min, closestSoFar}, &tempRec)) {
            hitAnything = 1;
            closestSoFar = tempRec.t;
            *rec = tempRec;
        }
    }

    return hitAnything;
}

#endif
