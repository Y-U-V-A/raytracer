#include "hittable.h"

bool constant_medium_hit(hittable* object, ray* r_in, interval r_t, hit_record* out_record) {
    /**
     * @brief the concept of this constant_medium is to get the intersection point of object not on the surface but inside the object
     *
     */
    constant_medium* medium = (constant_medium*)object;
    hit_record rec1 = {0};
    hit_record rec2 = {0};
    /// this will get the lower t
    if (!medium->object->hit(medium->object, r_in, interval_universe(), &rec1))
        return false;
    /// this will get the higher t
    if (!medium->object->hit(medium->object, r_in, (interval){rec1.t + 0.001, INFINITY}, &rec2))
        return false;

    if (rec1.t < r_t.min)
        rec1.t = r_t.min;
    if (rec2.t > r_t.max)
        rec2.t = r_t.max;
    /// ray just passes through the object
    if (rec1.t >= rec2.t)
        return false;
    if (rec1.t < 0)
        rec1.t = 0;

    f64 len = vec3_length(r_in->direction);
    f64 distance_inside_boundary = (rec2.t - rec1.t) * len;
    f64 hit_distance = medium->neg_inv_density * zlog(random_unit());

    /// ray just passes through the object
    if (hit_distance > distance_inside_boundary)
        return false;

    out_record->t = rec1.t + hit_distance / len;
    out_record->point = ray_at(r_in, out_record->t);
    out_record->mat = medium->mat;
    out_record->u = rec2.u;
    out_record->v = rec2.v;
    out_record->normal = rec2.normal;
    out_record->front_face = true;

    return true;
}