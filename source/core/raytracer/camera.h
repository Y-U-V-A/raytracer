#ifndef CAMERA__H
#define CAMERA__H

#include "defines.h"
#include "geometry.h"

struct sampler;
struct world;
struct camera {
    point3f eye;
    point3f lookat;
    vector3f u;
    vector3f v;
    vector3f w;
    void (*render_scene)(const struct world*);
};

struct camera camera_make(point3f eye, point3f lookat, f32 roll, void (*render_scene)(const struct world*));

struct orthographic_camera {
    struct camera camera;
    f32 zoom;
};
struct orthographic_camera_make_info {
    point3f eye;
    point3f lookat;
    f32 roll;
    f32 zoom;
};

struct orthographic_camera orthographic_camera_make(struct orthographic_camera_make_info*);

struct perspective_camera {
    struct camera camera;
    f32 fov;
    f32 zoom;
};
struct perspective_camera_make_info {
    point3f eye;
    point3f lookat;
    f32 roll;
    f32 fov;
    f32 zoom;
};

struct perspective_camera perspective_camera_make(struct perspective_camera_make_info*);

struct thinlens_camera {
    struct camera camera;
    f32 lens_radius;
    f32 focal_plane_distance;
    f32 fov;
    f32 zoom;
};
struct thinlens_camera_make_info {
    point3f eye;
    point3f lookat;
    f32 roll;
    f32 lens_radius;
    f32 focal_plane_distance;
    f32 fov;
    f32 zoom;
};

struct thinlens_camera thinlens_camera_make(struct thinlens_camera_make_info*);

struct fisheye_camera {
    struct camera camera;
    f32 psi_max;
    f32 zoom;
};
struct fisheye_camera_make_info {
    point3f eye;
    point3f lookat;
    f32 roll;
    f32 psi_max;
};

struct fisheye_camera fisheye_camera_make(struct fisheye_camera_make_info*);

struct spherical_camera {
    struct camera camera;
    f32 psi_max;
    f32 lambda_max;
};
struct spherical_camera_make_info {
    point3f eye;
    point3f lookat;
    f32 roll;
    f32 psi_max;
    f32 lambda_max;
};

struct spherical_camera spherical_camera_make(struct spherical_camera_make_info*);

// enum stereo_viewing_type {
//     PARLLEL,
//     TRANSVERSE,
// };

// struct stereo_camera {
//     struct camera camera;
//     f32 beta;
//     f32 pixel_gap;
//     struct camera* left_camera;
//     struct camera* right_camera;
//     enum stereo_viewing_type viewing_type;
// };

// struct stereo_camera stereo_camera_make(point3f eye, point3f lookat, f32 roll, f32 beta, f32 pixel_gap,
//                                         enum stereo_viewing_type view_type, struct camera* left_camera,
//                                         struct camera* right_camera);

#endif