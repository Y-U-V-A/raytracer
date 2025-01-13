#include "hittable.h"

bool triangle_hit(hittable* triangle_object, ray* r_in, interval r_t, hit_record* out_record) {
    /// everything is similar to quad so check quad for formula proof
    triangle* tri = (triangle*)triangle_object;
    f64 deno = vec3_dot(r_in->direction, tri->normal); // if this is zero means -> ray is parllel to plane
    if (zfabs(deno) < DBL_EPSILON) {
        return false;
    }
    // check if t is ouside the ray interval
    f64 t = (tri->D - vec3_dot(tri->normal, r_in->origin)) / deno;
    if (!interval_surrounds(r_t, t)) {
        return false;
    }

    point3 point = ray_at(r_in, t);
    vec3 dp = vec3_sub(point, tri->Q);
    f64 alpha = vec3_dot(tri->W, vec3_cross(dp, tri->v));
    f64 beta = vec3_dot(tri->W, vec3_cross(tri->u, dp));
    if (alpha < 0.0 || alpha > 1.0 || beta < 0.0 || beta > 1.0 || alpha + beta > 1.0) { /// only triangle check is (a + b) > 1
        return false;
    }
    out_record->point = point;
    out_record->mat = tri->mat;
    out_record->t = t;
    out_record->u = alpha;
    out_record->v = beta;
    hit_record_set_face_normal(out_record, r_in, tri->normal);

    return true;
}