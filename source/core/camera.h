#ifndef CAMERA__H
#define CAMERA__H

#include "defines.h"
#include "zmath.h"

typedef struct camera camera;
typedef struct hittable_list hittable_list;
typedef struct ray ray;

camera* camera_create(i32 image_width, i32 image_height);

void camera_destroy(camera* cam);

void camera_render(camera* cam, hittable_list* world, const char* file_path,
                   f64 viewport_field_of_view, point3 look_from, point3 look_at, vec3 world_up,
                   i32 samples_per_pixel, i32 depth, color (*background)(ray* r_in));

#endif