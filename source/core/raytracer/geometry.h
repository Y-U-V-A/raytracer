#ifndef GEOMETRY__H
#define GEOMETRY__H

#include "defines.h"

f64 sqrt_f64(f64 val);
f32 sqrt_f32(f32 val);
f32 pow_f32(f32 val, f32 exp);
f32 sin_f32(f32 rad);
void random_seed(u32 seed);
i32 random_i32();
f32 random_f32();

INLINE f32 clamp_f32(f32 val, f32 min, f32 max) {
    return (val < min) ? min : (val > max ? max : val);
}

union vec2f {
    f32 data[2];
    struct {
        f32 x;
        f32 y;
    };
    struct {
        f32 u;
        f32 v;
    };
};

typedef union vec2f point2f;

INLINE union vec2f vec2f_scale(f32 scale, union vec2f v) {
    return (union vec2f){.x = v.x * scale, .y = v.y * scale};
}

INLINE union vec2f vec2f_sub(union vec2f v0, union vec2f v1) {
    return (union vec2f){
        .x = v0.x - v1.x,
        .y = v0.y - v1.y,
    };
}

INLINE union vec2f vec2f_add(union vec2f v0, union vec2f v1) {
    return (union vec2f){
        .x = v0.x + v1.x,
        .y = v0.y + v1.y,
    };
}

union vec3f {
    f32 data[3];
    struct {
        f32 x;
        f32 y;
        f32 z;
    };
    struct {
        f32 r;
        f32 g;
        f32 b;
    };
};

typedef union vec3f point3f;
typedef union vec3f color3f;
typedef union vec3f normal3f;

INLINE f32 vec3f_dot(union vec3f v0, union vec3f v1) {
    return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
}

INLINE union vec3f vec3f_scale(f32 scale, union vec3f v) {
    return (union vec3f){.x = v.x * scale, .y = v.y * scale, .z = v.z * scale};
}

INLINE union vec3f vec3f_sub(union vec3f v0, union vec3f v1) {
    return (union vec3f){
        .x = v0.x - v1.x,
        .y = v0.y - v1.y,
        .z = v0.z - v1.z,
    };
}

INLINE union vec3f vec3f_add(union vec3f v0, union vec3f v1) {
    return (union vec3f){
        .x = v0.x + v1.x,
        .y = v0.y + v1.y,
        .z = v0.z + v1.z,
    };
}

INLINE union vec3f vec3f_normalize(union vec3f v) {
    f32 len = sqrt_f32(v.x * v.x + v.y * v.y + v.z * v.z);
    if (len != 0.0f) {
        f32 inv_len = 1.0f / len;
        v.x *= inv_len;
        v.y *= inv_len;
        v.z *= inv_len;
    }
    return v;
}

INLINE f32 lerp_f32(f32 t, f32 a, f32 b) {
    return a + (b - a) * t;
}

#endif