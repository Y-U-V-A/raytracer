#ifndef AABB__H
#define AABB__H

#include "defines.h"
#include "zmath.h"
#include "ray.h"

typedef struct aabb {
    interval x_range;
    interval y_range;
    interval z_range;
} aabb;

INLINE void aabb_pad_to_minimums(aabb* box) {
    f64 delta = 0.0001;
    if (interval_size(box->x_range) < delta)
        interval_expand(box->x_range, delta);
    if (interval_size(box->y_range) < delta)
        interval_expand(box->y_range, delta);
    if (interval_size(box->z_range) < delta)
        interval_expand(box->z_range, delta);
}

INLINE aabb aabb_create(point3 point1, point3 point2) {
    aabb temp = (aabb){(point1.x < point2.x ? (interval){point1.x, point2.x} : (interval){point2.x, point1.x}),
                       (point1.y < point2.y ? (interval){point1.y, point2.y} : (interval){point2.y, point1.y}),
                       (point1.z < point2.z ? (interval){point1.z, point2.z} : (interval){point2.z, point1.z})};
    aabb_pad_to_minimums(&temp);
    return temp;
}

INLINE aabb aabb_create_empty() {
    return (aabb){
        interval_empty(),
        interval_empty(),
        interval_empty(),
    };
}

INLINE aabb aabb_merge(aabb box1, aabb box2) {
    return (aabb){interval_create(box1.x_range, box2.x_range),
                  interval_create(box1.y_range, box2.y_range),
                  interval_create(box1.z_range, box2.z_range)};
}

INLINE interval aabb_axis_interval(aabb* box, i32 axis) {
    switch (axis) {
    case 0:
        return box->x_range;
    case 1:
        return box->y_range;
    case 2:
        return box->z_range;
    }
    return interval_empty();
}

INLINE i32 aabb_longest_axis(aabb* box) {
    f64 x = interval_size(box->x_range);
    f64 y = interval_size(box->y_range);
    f64 z = interval_size(box->z_range);
    return (x > y && x > z) ? 0 : (y > z) ? 1
                                          : 2;
}

INLINE aabb aabb_translate(aabb* box, vec3 offset) {
    return (aabb){
        interval_displace(box->x_range, offset.x),
        interval_displace(box->y_range, offset.y),
        interval_displace(box->z_range, offset.z),
    };
}

INLINE bool aabb_hit(aabb* box, ray* r, interval r_t) {

    point3 origin = r->origin;
    vec3 dir = r->direction;

    for (i32 i = 0; i < 3; ++i) {
        interval axis_range = aabb_axis_interval(box, i);
        f64 inv_dir = 1.0 / vec3_axis(dir, i);
        f64 t0 = (axis_range.min - vec3_axis(origin, i)) * inv_dir;
        f64 t1 = (axis_range.max - vec3_axis(origin, i)) * inv_dir;

        if (t0 < t1) {
            if (t0 > r_t.min)
                r_t.min = t0;
            if (t1 < r_t.max)
                r_t.max = t1;
        } else {
            if (t0 < r_t.min)
                r_t.min = t0;
            if (t1 > r_t.max)
                r_t.max = t1;
        }

        if (r_t.max <= r_t.min) {
            return false;
        }
    }
    return true;
}

#endif