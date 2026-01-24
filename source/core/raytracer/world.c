#include "world.h"
#include "logger.h"
#include "shape.h"
#include "ray.h"
#include "surface_interaction.h"
#include "sampler.h"

#define GAMMA 0.5f

void world_render(const struct world* world, color3f background_color) {

    FILE* scene = file_open(world->filename);
    log_file(scene, "P3\n");
    log_file(scene, "%u %u\n", world->hres, world->vres);
    log_file(scene, "255\n");

    f32 inv_num_samples = 1.0f / world->sampler->num_samples;

    for (i32 row = world->vres - 1; row >= 0; --row) {
        log_stdout("\rremaning scanlines  ->  %d            ", row);
        for (i32 column = 0; column < world->hres; ++column) {
            color3f pixel_color = (color3f){.r = 0.0f, .g = 0.0f, .b = 0.0f};

            for (i32 sample = 0; sample < world->sampler->num_samples; ++sample) {

                point2f point = sampler_sample_unit_square(world->sampler);
                point.x = world->pixel_scale * ((column - 0.5f * world->hres) + point.x);
                point.y = world->pixel_scale * ((row - 0.5f * world->vres) + point.y);

                struct ray ray = {
                    .o = (point3f){.x = point.x, .y = point.y, .z = 0.0f},
                    .d = (union vec3f){.x = 0.0f, .y = 0.0f, .z = -1.0f},
                };

                bool hit_anything = FALSE;
                f64 tmin = infinity;
                // struct surface_interaction si={0};
                for (i32 shape = 0; shape < world->shapes_count; ++shape) {
                    f64 shape_tmin = infinity;
                    struct surface_interaction shape_si = {0};
                    if (world->shapes[shape]->hit(world->shapes[shape], &ray, &shape_tmin, &shape_si) == TRUE && shape_tmin < tmin) {
                        tmin = shape_tmin;
                        // si=shape_si;
                        hit_anything = TRUE;
                    }
                }
                if (hit_anything) {
                    f32 xx = lerp_f32(((f32)ray.o.x + (0.5f * world->pixel_scale * world->hres)) / (world->pixel_scale * world->hres), 0.0f, 8.83f);
                    f32 yy = lerp_f32(((f32)ray.o.y + (0.5f * world->pixel_scale * world->vres)) / (world->pixel_scale * world->vres), 0.0f, 8.83f);
                    f32 grey = 0.5f * (1.0f + sin_f32(xx * yy * xx * yy));
                    pixel_color = vec3f_add(pixel_color, (union vec3f){.x = grey, .y = grey, .z = grey});
                } else {
                    pixel_color = vec3f_add(pixel_color, background_color);
                }
            }

            pixel_color = vec3f_scale(inv_num_samples, pixel_color);
            pixel_color.r = clamp_f32(pixel_color.r, 0.0f, 1.0f);
            pixel_color.g = clamp_f32(pixel_color.g, 0.0f, 1.0f);
            pixel_color.b = clamp_f32(pixel_color.b, 0.0f, 1.0f);
            // pixel_color.r = pow_f32(pixel_color.r, GAMMA);
            // pixel_color.g = pow_f32(pixel_color.g, GAMMA);
            // pixel_color.b = pow_f32(pixel_color.b, GAMMA);

            i32 r = (i32)(255.9999 * pixel_color.r);
            i32 g = (i32)(255.9999 * pixel_color.g);
            i32 b = (i32)(255.9999 * pixel_color.b);
            log_file(scene, "%d %d %d\n", r, g, b);
        }
    }
    LOGT("\r%s rendering complete                                 ",world->filename);
    file_close(scene);
}