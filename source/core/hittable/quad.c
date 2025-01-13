#include "hittable.h"

bool quad_hit(hittable* quad_object, ray* r_in, interval r_t, hit_record* out_record) {
    quad* qu = (quad*)quad_object;
    f64 deno = vec3_dot(qu->normal, r_in->direction); /// if this is zero means -> ray is parllel to plane
    if (zfabs(deno) < DBL_EPSILON) {
        return false;
    }
    /**
     * @brief check if ray intersects the plane contaning quad
     * plane equ -> Ax+By+Cz = D
     * Ax+By+Cz=D can be rewritten as dot(normal_to_the_plane,any_point_on_the_plane) = D / dot(vec3(A,B,C),vec3(x,y,z)) = D
     * here D can be obtained by taking (D = dot(quad_normal,point_on_quad))
     * point_on_ray = (origin+t*dir)
     * dot(normal,point_on_ray) = D
     * dot(normal,(origin+t*dir)) = D (solve for t )
     * t = (D - dot(normal,origin))/dot(normal,dir)
     */
    /// check if t is ouside the ray interval
    f64 t = (qu->D - vec3_dot(qu->normal, r_in->origin)) / deno;
    if (!interval_surrounds(r_t, t)) {
        return false;
    }
    /**
     * @brief if ray intersects the plane now check if the intersection point is inside the quad or not
     * get the point and conditions for point is inside are
     * A = vec3_cross(dp,v) should be in direction of normal (if it is opposite to normal then the point is outside of quad)
     * B = vec3_cross(u,dp) should be in direction of normal (if it is opposite to normal then the point is outside of quad)
     * unit_normal = (normal)/dot(normal,normal);
     * using above vectors we can get the texture coordinates u = dot(unit_normal,A), v = dot(unit_normal,B)
     */
    point3 point = ray_at(r_in, t);
    vec3 dp = vec3_sub(point, qu->Q);
    f64 alpha = vec3_dot(qu->W, vec3_cross(dp, qu->v));
    f64 beta = vec3_dot(qu->W, vec3_cross(qu->u, dp));
    if (alpha < 0.0 || alpha > 1.0 || beta < 0.0 || beta > 1.0) {
        return false;
    }
    out_record->point = point;
    out_record->mat = qu->mat;
    out_record->t = t;
    out_record->u = alpha;
    out_record->v = beta;
    hit_record_set_face_normal(out_record, r_in, qu->normal);

    // this will be true only if u,v are only |_ 90*
    //  record->u = point.x - qu->Q.x;
    //  record->v = point.y - qu->Q.y;
    return true;
}