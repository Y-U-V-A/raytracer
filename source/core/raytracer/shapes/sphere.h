#ifndef SPHERE__H
#define SPHERE__H

#include "shape.h"
#include "geometry.h"

struct sphere {
    struct shape base;
    f32 radius;
    point3f center;
};

bool sphere_hit(const struct shape* shape, const struct ray* ray, f64* tmin, struct surface_interaction* si);

INLINE struct sphere sphere_make(f32 radius, point3f center) {
    return (struct sphere){
        .base = {
            .hit = sphere_hit,
        },
        .radius = radius,
        .center = center,
    };
}

#endif