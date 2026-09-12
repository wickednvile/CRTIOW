#ifndef INTERVAL_H
#define INTERVAL_H

typedef struct interval {
    double min, max;
} interval;

void constructInterval(interval* i) {
    i->min = +infinity;
    i->max = -infinity;
}
void setInterval(interval* i, double min, double max) {
    i->min = min;
    i->max = max;
}
int  intervalContains(interval* i, double x) { return i->min <= x && x <= i->max; }
int intervalSurrounds(interval* i, double x) { return i->min  < x && x <  i->max; }
double  intervalClamp(interval* i, double x) {
    if (x < i->min) return i->min;
    if (x > i->max) return i->max;
    return x;
};

// idk what these are for, in the book I dont think it was used at all
const interval empty    = (interval){+infinity, -infinity};
const interval universe = (interval){-infinity, +infinity};

#endif
