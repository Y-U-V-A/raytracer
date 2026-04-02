#include "hittable.h"

bool circle_hit(hittable* circle_object, ray* r_in, interval r_t, hit_record* out_record) {
    circle* cir = (circle*)circle_object;
    f64 deno = vec3_dot(cir->normal, r_in->direction);
    /// check the denomenator is not zero (if its zero that means the ray is parllel to plane containing the circle)
    if (zfabs(deno) < DBL_EPSILON) {
        return false;
    }
    /**
     * @brief check if ray intersects the plane of circle
     * plane equ -> Ax+By+Cz = D
     * Ax+By+Cz=D can be rewritten as dot(normal_to_the_plane,any_point_on_the_plane) = D / dot(vec3(A,B,C),vec3(x,y,z)) = D
     * here D can be obtained by taking (D = dot(given_normal_to_circle,given_center_of_circle))
     * point_on_ray = (origin+t*dir)
     * dot(normal,point_on_ray) = D
     * dot(normal,(origin+t*dir)) = D (solve for t )
     * t = (D - dot(normal,origin))/dot(normal,dir)
     */
    f64 t = (cir->D - vec3_dot(cir->normal, r_in->origin)) / deno;
    /// check the t is in valid range (not negitive or near zero)
    if (!interval_surrounds(r_t, t)) {
        return false;
    }
    /**
     * @brief if ray intersects the plane now check if the intersection point is inside the circle or not
     * get the point and check if (length(vec3(point-circle_center)) < circle_radius)
     *
     */
    point3 point = ray_at(r_in, t);
    vec3 dp = vec3_sub(point, cir->center);
    if (vec3_length_squared(dp) > cir->radius * cir->radius) {
        return false;
    }
    out_record->t = t;
    out_record->point = point;
    out_record->mat = cir->mat;
    /// bitangent and tangent unit vectors are perpendicular to normal of the circle , these are used to get the u,v texture coordinates for circle
    out_record->u = NDC_TO_UNIT(vec3_dot(dp, cir->bitangent) / cir->radius);
    out_record->v = NDC_TO_UNIT(vec3_dot(dp, cir->tangent) / cir->radius);
    hit_record_set_face_normal(out_record, r_in, cir->normal);

    return true;
}