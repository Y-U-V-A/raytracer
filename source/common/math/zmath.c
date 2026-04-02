#include "zmath.h"
#include <math.h>
#include <stdlib.h>
#include "platform.h"

f64 zsin(f64 rad) {
    return sin(rad);
}

f64 zcos(f64 rad) {
    return cos(rad);
}

f64 ztan(f64 rad) {
    return tan(rad);
}

f64 zasin(f64 x) {
    return asin(x);
}

f64 zacos(f64 x) {
    return acos(x);
}

f64 zatan(f64 x) {
    return atan(x);
}

f64 zatan2(f64 y, f64 x) {
    return atan2(y, x);
}

f64 zfabs(f64 val) {
    return fabs(val);
}

f64 zfmod(f64 x, f64 y) {
    return fmod(x, y);
}

i32 zabs(i32 val) {
    return abs(val);
}

f64 zfmin(f64 x, f64 y) {
    return fmin(x, y);
}

f64 zfmax(f64 x, f64 y) {
    return fmax(x, y);
}

f64 zsqrt(f64 val) {
    return sqrt(val);
}

f64 zpow(f64 base, f64 power) {
    return pow(base, power);
}

f64 zfloor(f64 val) {
    return floor(val);
}

f64 zceil(f64 val) {
    return ceil(val);
}

f64 zround(f64 val) {
    return round(val);
}

void random_seed() {
    srand(platform_time());
}

f64 random_unit() {
    return rand() / ((f64)RAND_MAX + 1.0); // [0,1)
}

f64 random_double(f64 min, f64 max) {
    return min + random_unit() * (max - min); //[min,max)
}

i32 random_int(i32 min, i32 max) {
    return min + (int)(random_unit() * (max - min)); //[min,max)
}

void quick_sort(void* base, u64 count, u64 stride, int (*cmp)(const void* a, const void* b)) {
    qsort(base, count, stride, cmp);
}

f64 zlog(f64 val) {
    return log(val);
}
