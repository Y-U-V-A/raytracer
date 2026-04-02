#ifndef BVH__H
#define BVH__H

#include "defines.h"
#include "zmath.h"
#include "ray.h"
#include "aabb.h"
#include "hittable.h"

hittable* bvh_create(hittable** darray);

void bvh_destroy(hittable* bvh);

bool bvh_hit(hittable* bvh_object, ray* r_in, interval r_t, hit_record* record);

#endif