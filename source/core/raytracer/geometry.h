#ifndef GEOMETRY__H
#define GEOMETRY__H

#include "defines.h"

#define BLACK (color){.r = 0.0f, .g = 0.0f, .b = 0.0f}
#define WHITE (color){.r = 1.0f, .g = 1.0f, .b = 1.0f}
#define RED (color){.r = 1.0f, .g = 0.0f, .b = 0.0f}
#define GREEN (color){.r = 0.0f, .g = 1.0f, .b = 0.0f}
#define BLUE (color){.r = 0.0f, .g = 0.0f, .b = 1.0f}
#define YELLOW (color){.r = 1.0f, .g = 1.0f, .b = 0.0f}
#define CYAN (color){.r = 0.0f, .g = 1.0f, .b = 1.0f}
#define MAGENTA (color){.r = 1.0f, .g = 0.0f, .b = 1.0f}

f64 sqrt_f64(f64 val);
f32 sqrt_f32(f32 val);
f32 pow_f32(f32 val, f32 exp);
f32 sin_f32(f32 rad);
f32 cos_f32(f32 rad);
void random_seed(u32 seed);
i32 random_i32();
f32 random_f32();
f32 tan_f32(f32 rad);

INLINE f32 clamp_f32(f32 val, f32 min, f32 max) {
    return (val < min) ? min : (val > max ? max : val);
}

INLINE f32 lerp_f32(f32 t, f32 a, f32 b) {
    return a + (b - a) * t;
}

INLINE f32 radians(f32 val) {
    return (val) * (PI / 180.0f);
}

union f32x2 {
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

typedef union f32x2 point2f;
typedef union f32x2 vector2f;

INLINE vector2f f32x2_scale(f32 val, vector2f v) {
    return (vector2f){.x = v.x * val, .y = v.y * val};
}

INLINE vector2f f32x2_sub(vector2f v0, vector2f v1) {
    return (vector2f){
        .x = v0.x - v1.x,
        .y = v0.y - v1.y,
    };
}

INLINE vector2f f32x2_add(vector2f v0, vector2f v1) {
    return (vector2f){
        .x = v0.x + v1.x,
        .y = v0.y + v1.y,
    };
}

union f32x3 {
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

typedef union f32x3 color;
typedef union f32x3 point3f;
typedef union f32x3 vector3f;
typedef union f32x3 normal3f;

INLINE vector3f f32x3_neg(vector3f v) {
    return (vector3f){.x = -v.x, .y = -v.y, .z = -v.z};
}

INLINE vector3f f32x3_cross(vector3f v0, vector3f v1) {
    return (vector3f){
        .x = v0.y * v1.z - v0.z * v1.y,
        .y = v0.z * v1.x - v0.x * v1.z,
        .z = v0.x * v1.y - v0.y * v1.x,
    };
}

INLINE f32 f32x3_dot(vector3f v0, vector3f v1) {
    return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
}

INLINE vector3f f32x3_scale(f32 val, vector3f v) {
    return (vector3f){.x = v.x * val, .y = v.y * val, .z = v.z * val};
}

INLINE vector3f f32x3_sub(vector3f v0, vector3f v1) {
    return (vector3f){
        .x = v0.x - v1.x,
        .y = v0.y - v1.y,
        .z = v0.z - v1.z,
    };
}

INLINE vector3f f32x3_add(vector3f v0, vector3f v1) {
    return (vector3f){
        .x = v0.x + v1.x,
        .y = v0.y + v1.y,
        .z = v0.z + v1.z,
    };
}

INLINE vector3f f32x3_normalize(vector3f v) {
    f32 len = sqrt_f32(v.x * v.x + v.y * v.y + v.z * v.z);
    if (len != 0.0f) {
        f32 inv_len = 1.0f / len;
        v.x *= inv_len;
        v.y *= inv_len;
        v.z *= inv_len;
    }
    return v;
}

INLINE f32 f32x3_distance(point3f p0, point3f p1) {
    vector3f diff = f32x3_sub(p0, p1);
    return sqrt_f32(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
}

INLINE vector3f f32x3_clamp(vector3f v, f32 min, f32 max) {
    return (vector3f){
        .x = clamp_f32(v.x, min, max),
        .y = clamp_f32(v.y, min, max),
        .z = clamp_f32(v.z, min, max),
    };
}

struct transform {
    f32 data[4][4];
};

vector3f transform_vec3f(const struct transform* transform, vector3f v);

struct transform rotate(f32 theta, vector3f axis);

#endif