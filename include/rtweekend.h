#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

// Constants
const double infinity = HUGE_VAL;
const double pi = 3.1415926535897932385;

// Utility functions
static inline double degreesToRadians(double degrees)          { return degrees * pi/180.0; }
static inline double randomDouble()                            { return rand() / (RAND_MAX + 1.0); }
static inline double randomDoubleRange(double min, double max) { return min + (max - min) * randomDouble(); }

// Common headers
#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif
