#ifndef WORLD__H
#define WORLD__H

#include "geometry.h"

struct ray;
struct world {
    struct view_plane* view_plane;
    struct camera* camera;
    const char* output_filename;
    struct shape** shapes;
    i32 shapes_count;
};

void world_render_scene(const struct world*);

color world_trace_ray(const struct world*, const struct ray*);

#endif