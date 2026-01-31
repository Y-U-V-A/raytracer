#include "world.h"
#include "shape.h"
#include "ray.h"
#include "surface_interaction.h"
#include "camera.h"

void world_render_scene(const struct world* world) {
    world->camera->render_scene(world);
}

color world_trace_ray(const struct world* world, const struct ray* ray) {
    f64 tmin = infinity;
    struct surface_interaction si = {0};
    bool hit_anything = FALSE;
    for (i32 i = 0; i < world->shapes_count; ++i) {
        f64 shape_tmin = infinity;
        struct surface_interaction shape_si = {0};
        if (world->shapes[i]->hit(world->shapes[i], ray, &shape_tmin, &shape_si) && shape_tmin < tmin) {
            tmin = shape_tmin;
            si = shape_si;
            hit_anything = TRUE;
        }
    }
    if (hit_anything) {
        return f32x3_scale(0.5f, f32x3_add(si.normal, (vector3f){.x = 1.0f, .y = 1.0f, .z = 1.0f}));
    }
    return BLACK;
}