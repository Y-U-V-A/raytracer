#include "logger.h"
#include "world.h"
#include "sphere.h"
#include "sampler.h"
#include "plane.h"
#include "camera.h"
#include "view_plane.h"

int main() {

    random_seed(83);

    struct sphere_make_info sphere_info = {
        .center = (point3f){.x = 0.0f, .y = 2.0f, .z = -2.0f},
        .radius = 2.0f,
    };

    struct plane_make_info plane_info = {
        .normal = (normal3f){.x = 0.0f, .y = 1.0f, .z = 0.0f},
        .point_on_plane = (point3f){.x = 0.0f, .y = -1.0f, .z = 0.0f},
    };

    struct sphere sphere = sphere_make(&sphere_info);
    struct plane plane = plane_make(&plane_info);

#define SHAPES_COUNT 2
    struct shape* shapes[SHAPES_COUNT] = {
        (struct shape*)(&sphere),
        (struct shape*)(&plane),
    };

    // struct perspective_camera_make_info camera_info = {
    //     .eye = (point3f){.x = 0.0f, .y = 0.0f, .z = 2.0f},
    //     .lookat = (vector3f){.x = 0.0f, .y = 0.0f, .z = -1.0f},
    //     .fov = 90.0f,
    //     .roll = 45.0f,
    //     .zoom = 1.0f,
    // };

    // struct thinlens_camera_make_info camera_info = {
    //     .eye = (point3f){.x = 0.0f, .y = 1.0f, .z = 2.0f},
    //     .lookat = (vector3f){.x = 0.0f, .y = 0.f, .z = -1.0f},
    //     .lens_radius = 0.4f,
    //     .focal_plane_distance = 4.f,
    //     .fov = 90.0f,
    //     .roll = 0.0f,
    //     .zoom = 2.0f,
    // };

    struct fisheye_camera_make_info camera_info = {
        .eye = (point3f){.x = 0.0f, .y = 5.0f, .z = 5.0f},
        .lookat = (vector3f){.x = 0.0f, .y = 0.0f, .z = -2.0f},
        .psi_max = 90.0f,
        .roll = 0.0f,
    };

    // struct spherical_camera_make_info camera_info = {
    //     .eye = (point3f){.x = 0.0f, .y = 0.0f, .z = 0.0f},
    //     .lookat = (vector3f){.x = 0.0f, .y = 0.0f, .z = -2.0f},
    //     .psi_max = 60.0f,
    //     .roll = 0.0f,
    //     .lambda_max=90.0f,
    // };

    struct fisheye_camera camera = fisheye_camera_make(&camera_info);

    struct sampler sampler = sampler_create(100, RANDOM);
    struct view_plane view_plane = {
        .hres = 512,
        .vres = 512,
        .samples_per_pixel = 25,
        .sampler = &sampler,
    };

    struct world world = {
        .camera = (struct camera*)(&camera),
        .output_filename = "scene.ppm",
        .shapes = shapes,
        .shapes_count = SHAPES_COUNT,
        .view_plane = &view_plane,
    };

    world_render_scene(&world);

    sampler_destroy(&sampler);

    if (memory != 0) {
        LOGE("memory leak %u bytes", memory);
        ASSERT(0);
    }

    return 0;
}