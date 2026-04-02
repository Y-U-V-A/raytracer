#include "hittable.h"

bool sphere_hit(hittable* sphere_object, ray* r_in, interval r_t, hit_record* out_record) {

    /**
     * @brief explains how intersection point is found for sphere
     * sphere equ -> (Cx-x)*(Cx-x)+(Cy-y)*(Cy-y)+(Cz-z)*(Cz-z) = radius*radius // this can be proved using pythogroas therom
     * it can be rewritten as dot((center-point),(center-point)) = radius*radius
     * point_on_ray = (origin+t*dir)
     * dot((center-(point_on_ray)),(center-(point_on_ray))) = radius*radius
     * dot((center-(origin+t*dir)),(center-(origin+t*dir))) = radius*radius (solve for t)
     * (t*t*dot(dir,dir))-(t*2*dot(dir,(center-origin)))+(dot((center-origin),(center-origin))-radius*radius) = 0 // get the roots for t
     * a = dot(dir,dir) (or) vec3_length_squared(dir)
     * b = 2*dot(dir,(center-origin))
     * c = dot((center-origin),(center-origin))-radius*radius
     * use ((-b +- sqrt(b*b-4*a*c))/2*a) to get roots
     * if discriminant < 0 (no intersection)
     * if discriminant = 0 (tangent intersection)
     * if discriminant > 0 (two different intersections one front and other back)
     */

    sphere* object = (sphere*)(sphere_object);
    vec3 oc = vec3_sub(object->center, r_in->origin); // Origin_to_CircleCenter vector
    f32 a = vec3_length_squared(r_in->direction);
    f32 half_b = vec3_dot(oc, r_in->direction);
    f32 c = vec3_length_squared(oc) - (object->radius * object->radius);
    f32 discriminant = half_b * half_b - a * c;
    if (discriminant < 0) {
        return false;
    }
    f32 sqrtd = zsqrt(discriminant);
    f32 root = (half_b - sqrtd) / a;      // first get the smallest root
    if (!interval_surrounds(r_t, root)) { // to stop multiple bounces which will result in dark color
        root = (half_b + sqrtd) / a;      // second get the largest root
        if (!interval_surrounds(r_t, root)) {
            return false;
        }
    }
    vec3 point = ray_at(r_in, root);
    vec3 outward_normal = vec3_mul_scalar((1.0 / object->radius), vec3_sub(point, object->center)); // normalize the normal
    f32 hori_angle = zacos(-outward_normal.y);
    f32 vert_angle = zatan2(outward_normal.z, outward_normal.x) + PI;

    out_record->t = root;
    out_record->point = point;
    out_record->mat = object->mat;
    out_record->u = vert_angle / (PI * 2.0);
    out_record->v = hori_angle / PI;
    hit_record_set_face_normal(out_record, r_in, outward_normal);
    return true;
}