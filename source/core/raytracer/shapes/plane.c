#include "plane.h"
#include "ray.h"
#include "surface_interaction.h"

bool plane_hit(const struct shape* shape, const struct ray* ray, f64* tmin, struct surface_interaction* si) {
    const struct plane* plane = (const struct plane*)shape;
    f64 denom=vec3f_dot(ray->d, plane->normal);
    if(denom==0.0f){
        return FALSE;
    }
    f64 t = vec3f_dot(plane->normal, vec3f_sub(plane->point, ray->o)) / denom;
    if (t > 0.0f) {
        *tmin = t;
        si->color = (color3f){.r = 1.0f, .g = 0.0f, .b = 1.0f};
        si->normal = plane->normal;
        si->point = ray_at(ray, t);
        return TRUE;
    }
    return FALSE;
}