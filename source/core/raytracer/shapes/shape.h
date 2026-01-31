#ifndef SHAPE__H
#define SHAPE__H

#include "defines.h"
struct ray;
struct surface_interaction;
struct shape {
    bool (*hit)(const struct shape*, const struct ray*, f64* tmin, struct surface_interaction*);
};

#endif