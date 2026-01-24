#include "sphere.h"
#include "ray.h"
#include "surface_interaction.h"

bool sphere_hit(const struct shape* shape, const struct ray* ray, f64* tmin, struct surface_interaction* si) {
    const struct sphere* sphere = (const struct sphere*)shape;
    union vec3f oc = vec3f_sub(ray->o, sphere->center);
    f64 a = vec3f_dot(ray->d, ray->d);
    f64 b = 2.0f * vec3f_dot(oc, ray->d);
    f64 c = vec3f_dot(oc, oc) - sphere->radius * sphere->radius;

    f64 discrim = b * b - 4.0f * a * c;
    if (discrim < 0.0f) {
        return FALSE;
    }
    f64 sqrt_discrim = sqrt_f64(discrim);
    f64 denom = 2.0 * a;

    f64 t = (-b - sqrt_discrim) / denom;
    if (t > 0.0f) {
        *tmin = t;
        si->color = (color3f){.r = 0.0f, .g = 1.0f, .b = 1.0f};
        si->point = ray_at(ray, t);
        si->normal = vec3f_scale(1.0f / sphere->radius, vec3f_sub(si->point, sphere->center));
        return TRUE;
    }
    t = (-b + sqrt_discrim) / denom;
    if (t > 0.0f) {
        *tmin = t;
        si->color = (color3f){.r = 0.0f, .g = 1.0f, .b = 1.0f};
        si->point = ray_at(ray, t);
        si->normal = vec3f_scale(1.0f / sphere->radius, vec3f_sub(si->point, sphere->center));
        return TRUE;
    }
    return FALSE;
}