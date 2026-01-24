#ifndef WORLD__H
#define WORLD__H

#include "defines.h"
#include "geometry.h"

struct world {
    struct shape** shapes;
    i32 shapes_count;
    i32 hres;
    i32 vres;
    f32 pixel_scale;
    struct sampler*sampler;
    const char*filename;

};

void world_render(const struct world* world, color3f background_color);

#endif