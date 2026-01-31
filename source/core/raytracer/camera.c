#include "camera.h"
#include "sampler.h"
#include "ray.h"
#include "view_plane.h"
#include "world.h"
#include "logger.h"
#include "memory.h"

#define LOG_PPM_HEADER(scene, hres, vres) log_file(scene, "P3\n%d %d\n255\n", hres, vres)
#define LOG_PPM_COLOR(scene, red, green, blue) log_file(scene, "%d %d %d\n", (i32)(red * 255.9999), (i32)(green * 255.9999), (i32)(blue * 255.9999))

/**
 *     ██████  █████  ███    ███ ███████ ██████   █████
 *    ██      ██   ██ ████  ████ ██      ██   ██ ██   ██
 *    ██      ███████ ██ ████ ██ █████   ██████  ███████
 *    ██      ██   ██ ██  ██  ██ ██      ██   ██ ██   ██
 *     ██████ ██   ██ ██      ██ ███████ ██   ██ ██   ██
 *
 *
 */

struct camera camera_make(point3f eye, point3f lookat, f32 roll, void (*render_scene)(const struct world*)) {
    struct camera camera = {
        .eye = eye,
        .lookat = lookat,
        .render_scene = render_scene,
    };
    camera.w = f32x3_normalize(f32x3_sub(camera.eye, camera.lookat));
    vector3f world_up = {
        .x = 0.0f,
        .y = 1.0f,
        .z = 0.0f,
    };
    if (camera.w.x == 0.0f && camera.w.z == 0.0f && (camera.w.y == 1.0f || camera.w.y == -1.0f)) {
        world_up.z = -1.0f;
        world_up.y = 0.0f;
    }
    camera.u = f32x3_normalize(f32x3_cross(world_up, camera.w));
    camera.v = f32x3_normalize(f32x3_cross(camera.w, camera.u));
    if (roll != 0.0f) {
        struct transform roll_mtx = rotate(roll, camera.w);
        vector3f u = transform_vec3f(&roll_mtx, camera.u);
        vector3f v = transform_vec3f(&roll_mtx, camera.v);
        camera.u = u;
        camera.v = v;
    }

    // LOGI("u %f %f %f", camera.u.x, camera.u.y, camera.u.z);
    // LOGI("v %f %f %f", camera.v.x, camera.v.y, camera.v.z);
    // LOGI("w %f %f %f", camera.w.x, camera.w.y, camera.w.z);
    return camera;
}

/**
 *     ██████  ██████  ████████ ██   ██  ██████   ██████  ██████   █████  ██████  ██   ██ ██  ██████
 *    ██    ██ ██   ██    ██    ██   ██ ██    ██ ██       ██   ██ ██   ██ ██   ██ ██   ██ ██ ██
 *    ██    ██ ██████     ██    ███████ ██    ██ ██   ███ ██████  ███████ ██████  ███████ ██ ██
 *    ██    ██ ██   ██    ██    ██   ██ ██    ██ ██    ██ ██   ██ ██   ██ ██      ██   ██ ██ ██
 *     ██████  ██   ██    ██    ██   ██  ██████   ██████  ██   ██ ██   ██ ██      ██   ██ ██  ██████
 *
 *
 */

void orthographic_camera_render_scene(const struct world* world) {
    const struct orthographic_camera* ortho = (const struct orthographic_camera*)world->camera;

    struct view_plane* view_plane = world->view_plane;
    f32 inv_samples_per_pixel = 1.0f / view_plane->samples_per_pixel;
    f32 zoom = ortho->zoom;

    FILE* scene = file_open(world->output_filename);
    LOG_PPM_HEADER(scene, view_plane->hres, view_plane->vres);

    for (i32 r = view_plane->vres - 1; r >= 0; --r) {
        for (i32 c = 0; c < view_plane->hres; ++c) {
            color pixel_color = BLACK;
            for (i32 i = 0; i < view_plane->samples_per_pixel; ++i) {
                point2f sp = sampler_sample_unit_square(view_plane->sampler);
                point2f pp = {
                    .x = zoom * (c - 0.5f * view_plane->hres + sp.x),
                    .y = zoom * (r - 0.5f * view_plane->vres + sp.y),
                };
                struct ray ray = {
                    .o = f32x3_add(ortho->camera.eye, f32x3_add(f32x3_scale(pp.x, ortho->camera.u),
                                                                f32x3_scale(pp.y, ortho->camera.v))),
                    .d = f32x3_neg(ortho->camera.w),
                };
                pixel_color = f32x3_add(pixel_color, world_trace_ray(world, &ray));
            }
            pixel_color = f32x3_scale(inv_samples_per_pixel, pixel_color);
            f32x3_clamp(pixel_color, 0.0f, 1.0f);
            LOG_PPM_COLOR(scene, pixel_color.r, pixel_color.g, pixel_color.b);
        }
    }
    LOGT("\n%s complete\n", world->output_filename);
    file_close(scene);
}

struct orthographic_camera orthographic_camera_make(struct orthographic_camera_make_info* info) {
    return (struct orthographic_camera){
        .camera = camera_make(info->eye, info->lookat, info->roll, orthographic_camera_render_scene),
        .zoom = info->zoom,
    };
}

/**
 *    ██████  ███████ ██████  ███████ ██████  ███████  ██████ ████████ ██ ██    ██ ███████
 *    ██   ██ ██      ██   ██ ██      ██   ██ ██      ██         ██    ██ ██    ██ ██
 *    ██████  █████   ██████  ███████ ██████  █████   ██         ██    ██ ██    ██ █████
 *    ██      ██      ██   ██      ██ ██      ██      ██         ██    ██  ██  ██  ██
 *    ██      ███████ ██   ██ ███████ ██      ███████  ██████    ██    ██   ████   ███████
 *
 *
 */

void perspective_camera_render_scene(const struct world* world) {
    const struct perspective_camera* persp = (const struct perspective_camera*)world->camera;

    struct view_plane* view_plane = world->view_plane;
    f32 inv_samples_per_pixel = 1.0f / view_plane->samples_per_pixel;
    f32 zoom = 1.0f / persp->zoom;
    f32 view_plane_distance = (world->view_plane->vres * 0.5f) /
                              tan_f32(radians(persp->fov * 0.5f));

    FILE* scene = file_open(world->output_filename);
    LOG_PPM_HEADER(scene, view_plane->hres, view_plane->vres);

    for (i32 r = view_plane->vres - 1; r >= 0; --r) {
        for (i32 c = 0; c < view_plane->hres; ++c) {
            color pixel_color = BLACK;
            for (i32 i = 0; i < view_plane->samples_per_pixel; ++i) {
                point2f sp = sampler_sample_unit_square(view_plane->sampler);
                point2f pp = {
                    .x = zoom * (c - 0.5f * view_plane->hres + sp.x),
                    .y = zoom * (r - 0.5f * view_plane->vres + sp.y),
                };
                struct ray ray = {
                    .o = persp->camera.eye,
                    .d = f32x3_normalize(f32x3_sub(f32x3_add(f32x3_scale(pp.x, persp->camera.u),
                                                             f32x3_scale(pp.y, persp->camera.v)),
                                                   f32x3_scale(view_plane_distance, persp->camera.w))),
                };
                pixel_color = f32x3_add(pixel_color, world_trace_ray(world, &ray));
            }
            pixel_color = f32x3_scale(inv_samples_per_pixel, pixel_color);
            f32x3_clamp(pixel_color, 0.0f, 1.0f);
            LOG_PPM_COLOR(scene, pixel_color.r, pixel_color.g, pixel_color.b);
        }
    }
    LOGT("\n%s complete\n", world->output_filename);
    file_close(scene);
}

struct perspective_camera perspective_camera_make(struct perspective_camera_make_info* info) {
    return (struct perspective_camera){
        .camera = camera_make(info->eye, info->lookat, info->roll, perspective_camera_render_scene),
        .fov = info->fov,
        .zoom = info->zoom,
    };
}

/**
 *    ████████ ██   ██ ██ ███    ██         ██      ███████ ███    ██ ███████
 *       ██    ██   ██ ██ ████   ██         ██      ██      ████   ██ ██
 *       ██    ███████ ██ ██ ██  ██         ██      █████   ██ ██  ██ ███████
 *       ██    ██   ██ ██ ██  ██ ██         ██      ██      ██  ██ ██      ██
 *       ██    ██   ██ ██ ██   ████ ███████ ███████ ███████ ██   ████ ███████
 *
 *
 */

void thinlens_camera_render_scene(const struct world* world) {
    const struct thinlens_camera* thinlens = (const struct thinlens_camera*)world->camera;

    sampler_map_samples_to_unit_disk(world->view_plane->sampler);

    struct view_plane* view_plane = world->view_plane;
    f32 inv_samples_per_pixel = 1.0f / view_plane->samples_per_pixel;
    f32 zoom = 1.0f / thinlens->zoom;
    f32 view_plane_distance = (world->view_plane->vres * 0.5f) /
                              tan_f32(radians(thinlens->fov * 0.5f));
    f32 inv_view_plane_distance = 1.0f / view_plane_distance;

    FILE* scene = file_open(world->output_filename);
    LOG_PPM_HEADER(scene, view_plane->hres, view_plane->vres);

    for (i32 r = view_plane->vres - 1; r >= 0; --r) {
        for (i32 c = 0; c < view_plane->hres; ++c) {
            color pixel_color = BLACK;
            for (i32 i = 0; i < view_plane->samples_per_pixel; ++i) {
                point2f sp = sampler_sample_unit_square(view_plane->sampler);
                point2f pp = {
                    .x = zoom * (c - 0.5f * view_plane->hres + sp.x),
                    .y = zoom * (r - 0.5f * view_plane->vres + sp.y),
                };
                point2f fpp = {
                    .x = pp.x * thinlens->focal_plane_distance * inv_view_plane_distance,
                    .y = pp.y * thinlens->focal_plane_distance * inv_view_plane_distance,
                };
                vector2f dp = f32x2_scale(thinlens->lens_radius, sampler_sample_unit_disk(view_plane->sampler));
                struct ray ray = {
                    .o = f32x3_add(thinlens->camera.eye,
                                   f32x3_add(f32x3_scale(dp.x, thinlens->camera.u),
                                             f32x3_scale(dp.y, thinlens->camera.v))),
                    .d = f32x3_normalize(f32x3_sub(f32x3_add(f32x3_scale(fpp.x - dp.x, thinlens->camera.u),
                                                             f32x3_scale(fpp.y - dp.y, thinlens->camera.v)),
                                                   f32x3_scale(thinlens->focal_plane_distance, thinlens->camera.w))),
                };
                pixel_color = f32x3_add(pixel_color, world_trace_ray(world, &ray));
            }
            pixel_color = f32x3_scale(inv_samples_per_pixel, pixel_color);
            f32x3_clamp(pixel_color, 0.0f, 1.0f);
            LOG_PPM_COLOR(scene, pixel_color.r, pixel_color.g, pixel_color.b);
        }
    }
    LOGT("\n%s complete\n", world->output_filename);
    file_close(scene);
}

struct thinlens_camera thinlens_camera_make(struct thinlens_camera_make_info* info) {
    return (struct thinlens_camera){
        .camera = camera_make(info->eye, info->lookat, info->roll, thinlens_camera_render_scene),
        .lens_radius = info->lens_radius,
        .focal_plane_distance = info->focal_plane_distance,
        .fov = info->fov,
        .zoom = info->zoom,
    };
}

/**
 *    ███████ ██ ███████ ██   ██ ███████ ██    ██ ███████
 *    ██      ██ ██      ██   ██ ██       ██  ██  ██
 *    █████   ██ ███████ ███████ █████     ████   █████
 *    ██      ██      ██ ██   ██ ██         ██    ██
 *    ██      ██ ███████ ██   ██ ███████    ██    ███████
 *
 *
 */

void fisheye_camera_render_scene(const struct world* world) {

    const struct fisheye_camera* fisheye = (const struct fisheye_camera*)world->camera;

    struct view_plane* view_plane = world->view_plane;
    f32 inv_samples_per_pixel = 1.0f / view_plane->samples_per_pixel;

    FILE* scene = file_open(world->output_filename);
    LOG_PPM_HEADER(scene, view_plane->hres, view_plane->vres);

    for (i32 r = view_plane->vres - 1; r >= 0; --r) {
        for (i32 c = 0; c < view_plane->hres; ++c) {
            color pixel_color = BLACK;
            for (i32 i = 0; i < view_plane->samples_per_pixel; ++i) {
                point2f sp = sampler_sample_unit_square(view_plane->sampler);
                point2f pp = {
                    .x = (c - 0.5f * view_plane->hres + sp.x),
                    .y = (r - 0.5f * view_plane->vres + sp.y),
                };
                point2f ndc = {
                    .x = pp.x / (view_plane->hres * 0.5f),
                    .y = pp.y / (view_plane->vres * 0.5f),
                };
                f32 r_squared = (ndc.x * ndc.x + ndc.y * ndc.y);
                if (r_squared <= 1.0f) {
                    f32 r = sqrt_f32(r_squared);
                    f32 psi = radians(r * fisheye->psi_max);
                    f32 cos_psi = cos_f32(psi);
                    f32 sin_psi = sin_f32(psi);
                    f32 cos_alpha;
                    f32 sin_alpha;
                    if (r > 0.0001f) {
                        cos_alpha = ndc.x / r;
                        sin_alpha = ndc.y / r;
                    } else {
                        cos_alpha = 0.0f;
                        sin_alpha = 0.0f;
                    }
                    struct ray ray = {
                        .o = fisheye->camera.eye,
                        .d = f32x3_add(f32x3_scale(sin_psi * cos_alpha, fisheye->camera.u),
                                       f32x3_add(f32x3_scale(sin_psi * sin_alpha, fisheye->camera.v),
                                                 f32x3_scale(cos_psi, f32x3_neg(fisheye->camera.w)))),
                    };
                    pixel_color = f32x3_add(pixel_color, world_trace_ray(world, &ray));
                }
            }
            pixel_color = f32x3_scale(inv_samples_per_pixel, pixel_color);
            f32x3_clamp(pixel_color, 0.0f, 1.0f);
            LOG_PPM_COLOR(scene, pixel_color.r, pixel_color.g, pixel_color.b);
        }
    }
    LOGT("\n%s complete\n", world->output_filename);
    file_close(scene);
}

struct fisheye_camera fisheye_camera_make(struct fisheye_camera_make_info* info) {
    return (struct fisheye_camera){
        .camera = camera_make(info->eye, info->lookat, info->roll, fisheye_camera_render_scene),
        .psi_max = info->psi_max,
    };
}

/**
 *    ███████ ██████  ██   ██ ███████ ██████  ██  ██████  █████  ██
 *    ██      ██   ██ ██   ██ ██      ██   ██ ██ ██      ██   ██ ██
 *    ███████ ██████  ███████ █████   ██████  ██ ██      ███████ ██
 *         ██ ██      ██   ██ ██      ██   ██ ██ ██      ██   ██ ██
 *    ███████ ██      ██   ██ ███████ ██   ██ ██  ██████ ██   ██ ███████
 *
 *
 */

void spherical_camera_render_scene(const struct world* world) {
    const struct spherical_camera* spherical = (const struct spherical_camera*)world->camera;

    struct view_plane* view_plane = world->view_plane;
    f32 inv_samples_per_pixel = 1.0f / view_plane->samples_per_pixel;

    FILE* scene = file_open(world->output_filename);
    LOG_PPM_HEADER(scene, view_plane->hres, view_plane->vres);

    for (i32 r = view_plane->vres - 1; r >= 0; --r) {
        for (i32 c = 0; c < view_plane->hres; ++c) {
            color pixel_color = BLACK;
            for (i32 i = 0; i < view_plane->samples_per_pixel; ++i) {
                point2f sp = sampler_sample_unit_square(view_plane->sampler);
                point2f pp = {
                    .x = (c - 0.5f * view_plane->hres + sp.x),
                    .y = (r - 0.5f * view_plane->vres + sp.y),
                };
                point2f ndc = {
                    .x = pp.x / (view_plane->hres * 0.5f),
                    .y = pp.y / (view_plane->vres * 0.5f),
                };
                f32 psi = radians(ndc.y * spherical->psi_max);
                f32 lambda = radians(ndc.x * spherical->lambda_max);
                f32 phi = PI - lambda;
                f32 theta = (PI / 2.0f) - psi;

                f32 sin_phi = sin_f32(phi);
                f32 cos_phi = cos_f32(phi);
                f32 sin_theta = sin_f32(theta);
                f32 cos_theta = cos_f32(theta);

                struct ray ray = {
                    .o = spherical->camera.eye,
                    .d = f32x3_add(f32x3_scale(sin_theta * sin_phi, spherical->camera.u),
                                   f32x3_add(f32x3_scale(cos_theta, spherical->camera.v),
                                             f32x3_scale(sin_theta * cos_phi, spherical->camera.w))),
                };

                pixel_color = f32x3_add(pixel_color, world_trace_ray(world, &ray));
            }
            pixel_color = f32x3_scale(inv_samples_per_pixel, pixel_color);
            f32x3_clamp(pixel_color, 0.0f, 1.0f);
            LOG_PPM_COLOR(scene, pixel_color.r, pixel_color.g, pixel_color.b);
        }
    }
    LOGT("\n%s complete\n", world->output_filename);
    file_close(scene);
}

struct spherical_camera spherical_camera_make(struct spherical_camera_make_info* info) {
    return (struct spherical_camera){
        .camera = camera_make(info->eye, info->lookat, info->roll, spherical_camera_render_scene),
        .psi_max = info->psi_max,
        .lambda_max = info->lambda_max,
    };
}

/**
 *    ███████ ████████ ███████ ██████  ███████  ██████  ███████  ██████  ██████  ██████  ██    ██
 *    ██         ██    ██      ██   ██ ██      ██    ██ ██      ██      ██    ██ ██   ██  ██  ██
 *    ███████    ██    █████   ██████  █████   ██    ██ ███████ ██      ██    ██ ██████    ████
 *         ██    ██    ██      ██   ██ ██      ██    ██      ██ ██      ██    ██ ██         ██
 *    ███████    ██    ███████ ██   ██ ███████  ██████  ███████  ██████  ██████  ██         ██
 *
 *
 */

// void stereo_camera_render_scene(const struct world* world) {
// }

// struct stereo_camera stereo_camera_make(point3f eye, point3f lookat, f32 roll, f32 beta, f32 pixel_gap,
//                                         enum stereo_viewing_type view_type, struct camera* left_camera,
//                                         struct camera* right_camera) {
//     struct stereo_camera stereo = {
//         .camera = camera_make(eye, lookat, roll, stereo_camera_render_scene),
//         .beta = beta,
//         .pixel_gap = pixel_gap,
//         .viewing_type = view_type,
//     };
//     f32 l = f32x3_distance(lookat, eye);
//     f32 x = l * tan_f32(radians(beta * 0.5f));
//     *left_camera = camera_make(f32x3_sub(eye, f32x3_scale(x, stereo.camera.u)), f32x3_sub(lookat, f32x3_scale(x, stereo.camera.u)), 0, left_camera->render_scene);
//     *right_camera = camera_make(f32x3_add(eye, f32x3_scale(x, stereo.camera.u)), f32x3_add(lookat, f32x3_scale(x, stereo.camera.u)), 0, right_camera->render_scene);
//     stereo.left_camera = left_camera;
//     stereo.right_camera = right_camera;
//     return stereo;
// }