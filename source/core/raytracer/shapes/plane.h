#ifndef PLANE__H
#define PLANE__H

#include "shape.h"
#include "geometry.h"

struct plane {
    struct shape base;
    normal3f normal;
    point3f point_on_plane;
};

struct plane_make_info {
    normal3f normal;
    point3f point_on_plane;
};

bool plane_hit(const struct shape*, const struct ray*, f64* tmin, struct surface_interaction*);

struct plane plane_make(struct plane_make_info*);

#endif