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

void random_seed(u32 seed) {
    srand(seed);
}

i32 random_i32() {
    return rand();
}

f32 random_f32() {
    return rand() / ((f32)RAND_MAX+1.0f);
}