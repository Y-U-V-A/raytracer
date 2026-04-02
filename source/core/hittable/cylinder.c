#include "hittable.h"

bool cylinder_hit_help(hittable* cylinder_object, ray* r_in, interval r_t, hit_record* out_record);

bool cylinder_hit(hittable* cylinder_object, ray* r_in, interval r_t, hit_record* out_record) {
    hit_record temp_record = {0};
    bool hit_any = 0;
    f64 closest_so_far = r_t.max;
    cylinder* shape = (cylinder*)cylinder_object;

    // check for top_circle
    if (shape->top_circle.base.hit((hittable*)(&shape->top_circle), r_in, (interval){r_t.min, closest_so_far}, &temp_record)) {
        *out_record = temp_record;
        closest_so_far = temp_record.t;
        hit_any = 1;
    }
    // check for bottom_circle
    if (shape->bottom_circle.base.hit((hittable*)(&shape->bottom_circle), r_in, (interval){r_t.min, closest_so_far}, &temp_record)) {
        *out_record = temp_record;
        closest_so_far = temp_record.t;
        hit_any = 1;
    }
    // check for cylinder
    if (cylinder_hit_help(cylinder_object, r_in, (interval){r_t.min, closest_so_far}, &temp_record)) {
        *out_record = temp_record;
        closest_so_far = temp_record.t;
        hit_any = 1;
    }

    return hit_any;
}

////////////////////////////////////////////////////////////
//  __                  __                                //
// /  |                /  |                               //
// $$ |____    ______  $$ |  ______    ______    ______   //
// $$      \  /      \ $$ | /      \  /      \  /      \  //
// $$$$$$$  |/$$$$$$  |$$ |/$$$$$$  |/$$$$$$  |/$$$$$$  | //
// $$ |  $$ |$$    $$ |$$ |$$ |  $$ |$$    $$ |$$ |  $$/  //
// $$ |  $$ |$$$$$$$$/ $$ |$$ |__$$ |$$$$$$$$/ $$ |       //
// $$ |  $$ |$$       |$$ |$$    $$/ $$       |$$ |       //
// $$/   $$/  $$$$$$$/ $$/ $$$$$$$/   $$$$$$$/ $$/        //
//                         $$ |                           //
//                         $$ |                           //
//                         $$/                            //
//                                                        //
////////////////////////////////////////////////////////////

bool cylinder_hit_help(hittable* cylinder_object, ray* r_in, interval r_t, hit_record* out_record) {
    cylinder* shape = (cylinder*)cylinder_object;

    vec3 E = vec3_cross(vec3_sub(r_in->origin, shape->bottom_center), shape->direction);
    vec3 F = vec3_cross(r_in->direction, shape->direction);
    f64 a = vec3_dot(F, F);
    f64 half_b = vec3_dot(E, F);
    f64 c = vec3_dot(E, E) - (shape->radius * shape->radius);
    f64 discriminant = half_b * half_b - a * c;

    if (discriminant < 0.0) {
        return false;
    }

    f64 sqrt_discriminant = zsqrt(discriminant);
    // getting smallest positive t
    f64 t = (-half_b - sqrt_discriminant) / a;
    if (!interval_surrounds(r_t, t)) {         // to stop multiple bounces which will result in dark color
        t = (-half_b + sqrt_discriminant) / a; // second get the largest root
        if (!interval_surrounds(r_t, t)) {
            return false;
        }
    }

    vec3 point = ray_at(r_in, t);
    vec3 inner_vec = vec3_sub(point, shape->bottom_center);
    f64 height = vec3_dot(inner_vec, shape->direction);
    if (height < 0.0 || height > shape->height) {
        return false;
    }

    vec3 outward_normal = vec3_mul_scalar((1 / shape->radius),
                                          vec3_sub(inner_vec, vec3_mul_scalar(height, shape->direction)));
    out_record->point = point;
    out_record->mat = shape->mat;
    out_record->t = t;
    out_record->u = (zatan2(outward_normal.z, outward_normal.x) + PI) / (2 * PI);
    out_record->v = height / shape->height;
    hit_record_set_face_normal(out_record, r_in, outward_normal);

    return true;
}