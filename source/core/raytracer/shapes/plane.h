#ifndef PLANE__H
#define PLANE__H

#include "shape.h"
#include "geometry.h"

struct plane {
    struct shape base;
    normal3f normal;
    point3f point;
};

bool plane_hit(const struct shape* shape, const struct ray* ray, f64* tmin, struct surface_interaction* si);

INLINE struct plane plane_make(normal3f normal, point3f point) {
    return (struct plane){
        .base = {
            .hit = plane_hit,
        },
        .normal = vec3f_normalize(normal),
        .point = point,
    };
}

#endif