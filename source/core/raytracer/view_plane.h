#ifndef VIEW_PLANE__H
#define VIEW_PLANE__H

#include "defines.h"

struct view_plane {
    struct sampler* sampler;
    i32 samples_per_pixel;
    i32 hres;
    i32 vres;
};

#endif