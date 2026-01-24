#ifndef RAY__H
#define RAY__H

#include "geometry.h"

struct ray {
    point3f o;
    union vec3f d;
};

INLINE struct ray ray_make(point3f o,union vec3f d){
    return (struct ray){
        .o=o,
        .d=d,
    };
}

INLINE point3f ray_at(const struct ray* ray, f32 t) {
    return (point3f) {
        .x = ray->o.x + ray->d.x * t,
        .y = ray->o.y + ray->d.y * t,
        .z = ray->o.z + ray->d.z * t,
    };
}

#endif