#include "plane.h"
#include "ray.h"
#include "surface_interaction.h"

struct plane plane_make(struct plane_make_info* info) {
    return (struct plane){
        .base = {
            .hit = plane_hit,
        },
        .normal = f32x3_normalize(info->normal),
        .point_on_plane = info->point_on_plane,
    };
}

bool plane_hit(const struct shape* shape, const struct ray* ray, f64* tmin, struct surface_interaction* si) {
    const struct plane* plane = (const struct plane*)shape;
    f64 denom = f32x3_dot(ray->d, plane->normal);
    if (denom == 0.0f) {
        return FALSE;
    }
    f64 t = f32x3_dot(plane->normal, f32x3_sub(plane->point_on_plane, ray->o)) / denom;
    if (t > 0.0f) {
        *tmin = t;
        si->color = (color){.r = 1.0f, .g = 0.0f, .b = 1.0f};
        si->normal = plane->normal;
        si->point = ray_at(ray, t);
        return TRUE;
    }
    return FALSE;
}