#ifndef ZMATH__H
#define ZMATH__H

#include "defines.h"

f64 zsin(f64 rad);

f64 zcos(f64 rad);

f64 ztan(f64 rad);

f64 zasin(f64 x);

f64 zacos(f64 x);

f64 zatan(f64 x);

f64 zatan2(f64 y, f64 x);

f64 zfabs(f64 val);

f64 zfmod(f64 x, f64 y);

i32 zabs(i32 val);

f64 zfmin(f64 x, f64 y);

f64 zfmax(f64 x, f64 y);

f64 zsqrt(f64 val);

f64 zpow(f64 base, f64 power);

f64 zfloor(f64 val);

f64 zceil(f64 val);

f64 zround(f64 val);

f64 zlog(f64 val);

void random_seed();

f64 random_unit();

f64 random_double(f64 min, f64 max);

i32 random_int(i32 min, i32 max);

typedef int (*qsort_cmp)(const void* a, const void* b);
void quick_sort(void* base, u64 count, u64 stride, int (*cmp)(const void* a, const void* b));

typedef struct vec3 {
    f64 x;
    f64 y;
    f64 z;
} vec3;

typedef struct mat3 {
    f32 data[9];
} mat3;

typedef vec3 point3;
typedef vec3 color;

//////////////////////////////////////////////
//                                 ______   //
//                                /      \  //
//  __     __  ______    _______ /$$$$$$  | //
// /  \   /  |/      \  /       |$$ ___$$ | //
// $$  \ /$$//$$$$$$  |/$$$$$$$/   /   $$<  //
//  $$  /$$/ $$    $$ |$$ |       _$$$$$  | //
//   $$ $$/  $$$$$$$$/ $$ \_____ /  \__$$ | //
//    $$$/   $$       |$$       |$$    $$/  //
//     $/     $$$$$$$/  $$$$$$$/  $$$$$$/   //
//                                          //
//////////////////////////////////////////////

INLINE vec3 vec3_create(f64 x, f64 y, f64 z) {
    return (vec3){x, y, z};
}

INLINE vec3 vec3_zero() {
    return (vec3){0.0, 0.0, 0.0};
}

INLINE vec3 vec3_one() {
    return (vec3){1.0, 1.0, 1.0};
}

INLINE vec3 vec3_add(vec3 vec_1, vec3 vec_2) {
    return (vec3){vec_1.x + vec_2.x, vec_1.y + vec_2.y, vec_1.z + vec_2.z};
}

INLINE vec3 vec3_sub(vec3 vec_1, vec3 vec_2) {
    return (vec3){vec_1.x - vec_2.x, vec_1.y - vec_2.y, vec_1.z - vec_2.z};
}

INLINE vec3 vec3_mul(vec3 vec_1, vec3 vec_2) {
    return (vec3){vec_1.x * vec_2.x, vec_1.y * vec_2.y, vec_1.z * vec_2.z};
}

INLINE vec3 vec3_div(vec3 vec_1, vec3 vec_2) {
    return (vec3){
        vec_2.x != 0.0 ? vec_1.x / vec_2.x : 0.0,
        vec_2.y != 0.0 ? vec_1.y / vec_2.y : 0.0,
        vec_2.z != 0.0 ? vec_1.z / vec_2.z : 0.0};
}

INLINE vec3 vec3_mul_scalar(f64 scale, vec3 vec) {
    return (vec3){vec.x * scale, vec.y * scale, vec.z * scale};
}

INLINE vec3 vec3_cross(vec3 vec_1, vec3 vec_2) {
    return (vec3){vec_1.y * vec_2.z - vec_1.z * vec_2.y,
                  vec_1.z * vec_2.x - vec_1.x * vec_2.z,
                  vec_1.x * vec_2.y - vec_1.y * vec_2.x};
}

INLINE f64 vec3_dot(vec3 vec_1, vec3 vec_2) {
    return (f64)(vec_1.x * vec_2.x + vec_1.y * vec_2.y + vec_1.z * vec_2.z);
}

INLINE f64 vec3_length_squared(vec3 vec) {
    return (f64)(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

INLINE f64 vec3_length(vec3 vec) {
    return (f64)(zsqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z));
}

INLINE void vec3_normalize(vec3* vec) {
    f64 len = vec3_length(*vec);
    if (len > 0.0) {
        vec->x /= len;
        vec->y /= len;
        vec->z /= len;
    }
}

INLINE vec3 vec3_unit(vec3 vec) {
    f64 len = vec3_length(vec);
    if (len > 0.0) {
        vec.x /= len;
        vec.y /= len;
        vec.z /= len;
    }
    return vec;
}

INLINE f64 vec3_distance(vec3 vec_1, vec3 vec_2) {
    vec3 vec = (vec3){vec_1.x - vec_2.x, vec_1.y - vec_2.y, vec_1.z - vec_2.z};
    return vec3_length(vec);
}

INLINE f64 vec3_distance_squared(vec3 vec_1, vec3 vec_2) {
    vec3 vec = (vec3){vec_1.x - vec_2.x, vec_1.y - vec_2.y, vec_1.z - vec_2.z};
    return vec3_length_squared(vec);
}

INLINE bool vec3_compare(vec3 vec_0, vec3 vec_1) {
    return ((zfabs(vec_0.x - vec_1.x) < DBL_EPSILON) &&
            (zfabs(vec_0.y - vec_1.y) < DBL_EPSILON) &&
            (zfabs(vec_0.z - vec_1.z) < DBL_EPSILON));
}

INLINE vec3 vec3_random(f64 min, f64 max) {
    return (vec3){random_double(min, max), random_double(min, max), random_double(min, max)};
}

INLINE vec3 vec3_random_unit_vector() {
    while (1) {
        vec3 v = vec3_random(-1, 1);
        f64 len = vec3_length_squared(v);
        if (1e-160 < len && len <= 1.0) { // if length is near zero then (1.0 / zsqrt(len) -> infinity,so
            return vec3_mul_scalar((1.0 / zsqrt(len)), v);
        }
    }
}

INLINE vec3 vec3_random_on_hemisphere(vec3 normal) {
    vec3 v = vec3_random_unit_vector();
    if (vec3_dot(normal, v) > 0.0) // In the same hemisphere as the normal
    {
        return v;
    }
    return vec3_mul_scalar(-1.0, v);
}

INLINE f64 vec3_axis(vec3 vec, i32 val) {
    if (val == 0)
        return vec.x;
    if (val == 1)
        return vec.y;
    return vec.z;
}

//////////////////////////////////////////////////////////////////////////
//  __              __                                              __  //
// /  |            /  |                                            /  | //
// $$/  _______   _$$ |_     ______    ______   __     __  ______  $$ | //
// /  |/       \ / $$   |   /      \  /      \ /  \   /  |/      \ $$ | //
// $$ |$$$$$$$  |$$$$$$/   /$$$$$$  |/$$$$$$  |$$  \ /$$/ $$$$$$  |$$ | //
// $$ |$$ |  $$ |  $$ | __ $$    $$ |$$ |  $$/  $$  /$$/  /    $$ |$$ | //
// $$ |$$ |  $$ |  $$ |/  |$$$$$$$$/ $$ |        $$ $$/  /$$$$$$$ |$$ | //
// $$ |$$ |  $$ |  $$  $$/ $$       |$$ |         $$$/   $$    $$ |$$ | //
// $$/ $$/   $$/    $$$$/   $$$$$$$/ $$/           $/     $$$$$$$/ $$/  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

typedef struct interval {
    f64 min;
    f64 max;
} interval;

INLINE interval interval_create(interval range1, interval range2) {
    return (interval){(range1.min < range2.min ? range1.min : range2.min),
                      (range1.max > range2.max ? range1.max : range2.max)};
}

INLINE interval interval_expand(interval range, f64 val) {
    return (interval){range.min - val / 2.0, range.max + val / 2.0};
}

INLINE interval interval_displace(interval range, f64 val) {
    return (interval){range.min + val, range.max + val};
}

INLINE interval interval_empty() {
    return (interval){+INFINITY, -INFINITY};
}

INLINE interval interval_universe() {
    return (interval){-INFINITY, +INFINITY};
}

INLINE f64 interval_size(interval range) {
    return (f64)(range.max - range.min);
}

INLINE f64 interval_clamp(interval range, f64 val) {
    return (val > range.max ? range.max : (val < range.min ? range.min : val));
}

INLINE bool interval_contains(interval range, f64 val) {
    return (val >= range.min && val <= range.max);
}

INLINE bool interval_surrounds(interval range, f64 val) {
    return (val > range.min && val < range.max);
}

// norm must be normalized and inc and dot(norm,inc)<0
INLINE vec3 vec3_reflect(vec3 inc, vec3 norm) {
    return vec3_sub(inc, vec3_mul_scalar((2.0 * vec3_dot(inc, norm)), norm));
}
// both must be unit vectors
INLINE vec3 vec3_refract(vec3 inc, vec3 norm, f64 eta) {
    f64 k = vec3_dot(norm, inc);
    return vec3_sub(vec3_mul_scalar(eta, inc),
                    vec3_mul_scalar((eta * k) + zsqrt(1.0 - (eta * eta * (1.0 - k * k))), norm));
}

INLINE f64 lerp(f64 a, f64 b, f64 t) {
    return a + t * (b - a);
}

// TODO:get feel of this
//  Use Schlick's approximation for reflectance.
INLINE f64 reflectance(f64 cosine, f64 refraction_index) {
    // Use Schlick's approximation for reflectance.
    f64 r0 = (1.0 - refraction_index) / (1.0 + refraction_index);
    r0 = r0 * r0;
    return r0 + (1 - r0) * zpow((1 - cosine), 5);
}

//////////////////////////////////////////////////
//                            __       ______   //
//                           /  |     /      \  //
//  _____  ____    ______   _$$ |_   /$$$$$$  | //
// /     \/    \  /      \ / $$   |  $$ ___$$ | //
// $$$$$$ $$$$  | $$$$$$  |$$$$$$/     /   $$<  //
// $$ | $$ | $$ | /    $$ |  $$ | __  _$$$$$  | //
// $$ | $$ | $$ |/$$$$$$$ |  $$ |/  |/  \__$$ | //
// $$ | $$ | $$ |$$    $$ |  $$  $$/ $$    $$/  //
// $$/  $$/  $$/  $$$$$$$/    $$$$/   $$$$$$/   //
//                                              //
//////////////////////////////////////////////////

INLINE mat3 mat3_create(f64 n00, f64 n01, f64 n02,
                        f64 n10, f64 n11, f64 n12,
                        f64 n20, f64 n21, f64 n22) {
    return (mat3){n00, n10, n20,
                  n01, n11, n21,
                  n02, n12, n22};
}

INLINE mat3 mat3_rotation(f32 rad, vec3 axis) {
    vec3 a = vec3_unit(axis);
    f32 COS = zcos(rad);
    f32 SIN = zsin(rad);
    f32 one_minus_C = 1.0f - COS;
    f32 xy = a.x * a.y * one_minus_C;
    f32 xz = a.x * a.z * one_minus_C;
    f32 yz = a.y * a.z * one_minus_C;

    return mat3_create(a.x * a.x * one_minus_C + COS, xy - a.z * SIN, xz + a.y * SIN,
                       xy + a.z * SIN, a.y * a.y * one_minus_C + COS, yz - a.x * SIN,
                       xz - a.y * SIN, yz + a.x * SIN, a.z * a.z * one_minus_C + COS);
}

INLINE vec3 mat3_mul_vec3(mat3 mtx, vec3 vec) {
    return (vec3){
        mtx.data[0 + 0] * vec.x + mtx.data[3 + 0] * vec.y + mtx.data[6 + 0] * vec.z,
        mtx.data[0 + 1] * vec.x + mtx.data[3 + 1] * vec.y + mtx.data[6 + 1] * vec.z,
        mtx.data[0 + 2] * vec.x + mtx.data[3 + 2] * vec.y + mtx.data[6 + 2] * vec.z,
    };
}

#endif