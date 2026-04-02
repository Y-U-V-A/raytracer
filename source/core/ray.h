#ifndef RAY__H
#define RAY__H

#include "defines.h"
#include "zmath.h"

typedef struct ray {
    point3 origin;
    vec3 direction;
} ray;

INLINE ray ray_create(point3 point, vec3 direction) {
    return (ray){
        .origin = point,
        .direction = direction,
    };
}

INLINE point3 ray_at(ray* r, f32 t) {
    return (point3){
        r->origin.x + r->direction.x * t,
        r->origin.y + r->direction.y * t,
        r->origin.z + r->direction.z * t,
    };
}

#endif