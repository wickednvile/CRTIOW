#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec3.h"

static inline double linearToGamma(double linearComponent) {
    if (linearComponent > 0.0)
        return sqrt(linearComponent);

    return 0.0;
}

void writeColor(const color* pixelColor) {
    double r = pixelColor->x;
    double g = pixelColor->y;
    double b = pixelColor->z;

    r = linearToGamma(r);
    g = linearToGamma(g);
    b = linearToGamma(b);

    interval intensity;
    setInterval(&intensity, 0.000, 0.999);
    int rByte = (int)(256 * intervalClamp(&intensity, r));
    int gByte = (int)(256 * intervalClamp(&intensity, g));
    int bByte = (int)(256 * intervalClamp(&intensity, b));

    fprintf(stdout, "%d %d %d\n", rByte, gByte, bByte);
}

#endif
