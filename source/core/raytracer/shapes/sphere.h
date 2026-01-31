#ifndef SPHERE__H
#define SPHERE__H

#include "shape.h"
#include "geometry.h"

struct sphere {
    struct shape base;
    f32 radius;
    point3f center;
};

struct sphere_make_info {
    f32 radius;
    point3f center;
};

bool sphere_hit(const struct shape*, const struct ray*, f64* tmin, struct surface_interaction*);

struct sphere sphere_make(struct sphere_make_info*);

#endif