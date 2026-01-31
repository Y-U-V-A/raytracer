#include "geometry.h"
#include <math.h>
#include <stdlib.h>

f32 infinity = INFINITY;

f64 sqrt_f64(f64 val) {
    return sqrt(val);
}

f32 sqrt_f32(f32 val) {
    return sqrtf(val);
}

f32 pow_f32(f32 val, f32 exp) {
    return powf(val, exp);
}

f32 sin_f32(f32 rad) {
    return sinf(rad);
}

f32 cos_f32(f32 rad) {
    return cosf(rad);
}

f32 tan_f32(f32 rad) {
    return tanf(rad);
}

void random_seed(u32 seed) {
    srand(seed);
}

i32 random_i32() {
    return rand();
}

f32 random_f32() {
    return rand() / ((f32)RAND_MAX + 1.0f);
}

vector3f transform_vec3f(const struct transform* transform, vector3f v) {
    return (vector3f){
        .x = transform->data[0][0] * v.x + transform->data[0][1] * v.y + transform->data[0][2] * v.z,
        .y = transform->data[1][0] * v.x + transform->data[1][1] * v.y + transform->data[1][2] * v.z,
        .z = transform->data[2][0] * v.x + transform->data[2][1] * v.y + transform->data[2][2] * v.z,
    };
}

struct transform rotate(f32 theta, vector3f axis) {
    vector3f a = f32x3_normalize(axis);
    f32 COS = cos_f32(radians(theta));
    f32 SIN = sin_f32(radians(theta));
    f32 one_minus_C = 1.0f - COS;
    f32 xy = a.x * a.y * one_minus_C;
    f32 xz = a.x * a.z * one_minus_C;
    f32 yz = a.y * a.z * one_minus_C;

    return (struct transform){
        .data = {
            {a.x * a.x * one_minus_C + COS, xy - a.z * SIN, xz + a.y * SIN, 0.0f},
            {xy + a.z * SIN, a.y * a.y * one_minus_C + COS, yz - a.x * SIN, 0.0f},
            {xz - a.y * SIN, yz + a.x * SIN, a.z * a.z * one_minus_C + COS, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f},
        }};
}