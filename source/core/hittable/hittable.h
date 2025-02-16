#ifndef HITTABLE__H
#define HITTABLE__H

#include "defines.h"
#include "zmath.h"
#include "ray.h"
#include "aabb.h"

////////////////////////////////////////////////////////////////////////
//  __        __    __      __                __        __            //
// /  |      /  |  /  |    /  |              /  |      /  |           //
// $$ |____  $$/  _$$ |_  _$$ |_     ______  $$ |____  $$ |  ______   //
// $$      \ /  |/ $$   |/ $$   |   /      \ $$      \ $$ | /      \  //
// $$$$$$$  |$$ |$$$$$$/ $$$$$$/    $$$$$$  |$$$$$$$  |$$ |/$$$$$$  | //
// $$ |  $$ |$$ |  $$ | __ $$ | __  /    $$ |$$ |  $$ |$$ |$$    $$ | //
// $$ |  $$ |$$ |  $$ |/  |$$ |/  |/$$$$$$$ |$$ |__$$ |$$ |$$$$$$$$/  //
// $$ |  $$ |$$ |  $$  $$/ $$  $$/ $$    $$ |$$    $$/ $$ |$$       | //
// $$/   $$/ $$/    $$$$/   $$$$/   $$$$$$$/ $$$$$$$/  $$/  $$$$$$$/  //
//                                                                    //
////////////////////////////////////////////////////////////////////////

typedef struct material material;

typedef struct hit_record {
    point3 point;    /// intersection point
    vec3 normal;     /// surface normal at intersection point (normal is always opposite to normal's dir)
    f64 t;           /// ray's t
    f64 u;           /// texture coordinate u [0,1]  along x axis
    f64 v;           /// texture coordinate v [0,1] along y axis
    bool front_face; /// if normal is opposite to ray's dir this is true else false
    material* mat;   /// material of object at intersection point
} hit_record;

typedef struct hittable hittable;
struct hittable {
    bool (*hit)(hittable* object, ray* r_in, interval r_t, hit_record* out_record);
    aabb box;
};

INLINE bool default_hit(hittable* object, ray* r_in, interval r_t, hit_record* out_record) {
    return false;
}

INLINE void hit_record_set_face_normal(hit_record* record, ray* r_in, vec3 outward_normal) {
    /// this will ensure the normal is always opposite to ray's dir
    record->front_face = (vec3_dot(r_in->direction, outward_normal) < 0.0);
    record->normal = record->front_face ? outward_normal : vec3_mul_scalar(-1.0, outward_normal);
}

///////////////////////////////////////////////////////////////////////////
//   ______   _______      _____  ________   ______   ________  ______   //
//  /      \ /       \    /     |/        | /      \ /        |/      \  //
// /$$$$$$  |$$$$$$$  |   $$$$$ |$$$$$$$$/ /$$$$$$  |$$$$$$$$//$$$$$$  | //
// $$ |  $$ |$$ |__$$ |      $$ |$$ |__    $$ |  $$/    $$ |  $$ \__$$/  //
// $$ |  $$ |$$    $$<  __   $$ |$$    |   $$ |         $$ |  $$      \  //
// $$ |  $$ |$$$$$$$  |/  |  $$ |$$$$$/    $$ |   __    $$ |   $$$$$$  | //
// $$ \__$$ |$$ |__$$ |$$ \__$$ |$$ |_____ $$ \__/  |   $$ |  /  \__$$ | //
// $$    $$/ $$    $$/ $$    $$/ $$       |$$    $$/    $$ |  $$    $$/  //
//  $$$$$$/  $$$$$$$/   $$$$$$/  $$$$$$$$/  $$$$$$/     $$/    $$$$$$/   //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//                      __                                      //
//                     /  |                                     //
//   _______   ______  $$ |____    ______    ______    ______   //
//  /       | /      \ $$      \  /      \  /      \  /      \  //
// /$$$$$$$/ /$$$$$$  |$$$$$$$  |/$$$$$$  |/$$$$$$  |/$$$$$$  | //
// $$      \ $$ |  $$ |$$ |  $$ |$$    $$ |$$ |  $$/ $$    $$ | //
//  $$$$$$  |$$ |__$$ |$$ |  $$ |$$$$$$$$/ $$ |      $$$$$$$$/  //
// /     $$/ $$    $$/ $$ |  $$ |$$       |$$ |      $$       | //
// $$$$$$$/  $$$$$$$/  $$/   $$/  $$$$$$$/ $$/        $$$$$$$/  //
//           $$ |                                               //
//           $$ |                                               //
//           $$/                                                //
//                                                              //
//////////////////////////////////////////////////////////////////

typedef struct sphere {
    hittable base;
    material* mat;
    point3 center;
    f64 radius;
} sphere;

bool sphere_hit(hittable* sphere_object, ray* r_in, interval r_t, hit_record* out_record);

INLINE sphere sphere_create(point3 center, f64 radius, material* mat) {
    return (sphere){
        .base = {
            .hit = sphere_hit,
            .box = aabb_create(vec3_sub(center, (vec3){radius, radius, radius}),
                               vec3_add(center, (vec3){radius, radius, radius})),
        },
        .mat = mat,
        .center = center,
        .radius = radius,
    };
}

//////////////////////////////////////////////
//                                      __  //
//                                     /  | //
//   ______   __    __   ______    ____$$ | //
//  /      \ /  |  /  | /      \  /    $$ | //
// /$$$$$$  |$$ |  $$ | $$$$$$  |/$$$$$$$ | //
// $$ |  $$ |$$ |  $$ | /    $$ |$$ |  $$ | //
// $$ \__$$ |$$ \__$$ |/$$$$$$$ |$$ \__$$ | //
// $$    $$ |$$    $$/ $$    $$ |$$    $$ | //
//  $$$$$$$ | $$$$$$/   $$$$$$$/  $$$$$$$/  //
//       $$ |                               //
//       $$ |                               //
//       $$/                                //
//                                          //
//////////////////////////////////////////////

typedef struct quad {
    hittable base;
    material* mat;
    point3 Q;    /// point on quad
    vec3 u;      /// edge vector of quad
    vec3 v;      /// edge vector of quad
    vec3 normal; /// normal to the quad
    vec3 W;      /// this is used to get the (alpha,beta) in P=Q+(alpha)*u+(beta)*v
                 /// (where w=(unnormilized_norm)/dot(unnormilized_norm,unnormilized_norm))
    f64 D;       /// dot(normal,any point on quad plane )
} quad;

bool quad_hit(hittable* quad_object, ray* r_in, interval r_t, hit_record* out_record);

INLINE quad quad_create(point3 Q, vec3 u, vec3 v, material* mat) {
    vec3 n = vec3_cross(u, v);
    vec3 normal = vec3_unit(n);
    return (quad){
        .base = {
            .hit = quad_hit,
            .box = aabb_merge(aabb_create(Q, vec3_add(Q, vec3_add(u, v))), // the quad maybe tilted along the diagnoal so;
                              aabb_create(vec3_add(Q, u), vec3_add(Q, v))),
        },
        .mat = mat,
        .Q = Q,
        .u = u,
        .v = v,
        .normal = normal,
        .D = vec3_dot(normal, Q),
        .W = vec3_mul_scalar(1.0 / vec3_dot(n, n), n),
    };
}

//////////////////////////////////////////////////////////////////////////
//    __                __                                __            //
//   /  |              /  |                              /  |           //
//  _$$ |_     ______  $$/   ______   _______    ______  $$ |  ______   //
// / $$   |   /      \ /  | /      \ /       \  /      \ $$ | /      \  //
// $$$$$$/   /$$$$$$  |$$ | $$$$$$  |$$$$$$$  |/$$$$$$  |$$ |/$$$$$$  | //
//   $$ | __ $$ |  $$/ $$ | /    $$ |$$ |  $$ |$$ |  $$ |$$ |$$    $$ | //
//   $$ |/  |$$ |      $$ |/$$$$$$$ |$$ |  $$ |$$ \__$$ |$$ |$$$$$$$$/  //
//   $$  $$/ $$ |      $$ |$$    $$ |$$ |  $$ |$$    $$ |$$ |$$       | //
//    $$$$/  $$/       $$/  $$$$$$$/ $$/   $$/  $$$$$$$ |$$/  $$$$$$$/  //
//                                             /  \__$$ |               //
//                                             $$    $$/                //
//                                              $$$$$$/                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

typedef struct triangle {
    hittable base;
    material* mat;
    point3 Q;    /// point on triangle
    vec3 u;      /// edge vector of triangle
    vec3 v;      /// edge vector of triangle
    vec3 normal; /// normal to the triangle
    vec3 W;      /// this is used to get the (alpha,beta) in P=Q+(alpha)*u+(beta)*v
                 /// (where w=(unnormilized_norm)/dot(unnormilized_norm,unnormilized_norm))
    f64 D;       /// dot(normal,any point on triangle plane )
} triangle;

bool triangle_hit(hittable* triangle_object, ray* r_in, interval r_t, hit_record* out_record);

INLINE triangle triangle_create(point3 Q, vec3 u, vec3 v, material* mat) {
    vec3 n = vec3_cross(u, v);
    vec3 normal = vec3_unit(n);
    return (triangle){
        .base = {
            .hit = triangle_hit,
            .box = aabb_merge(aabb_create(Q, vec3_add(Q, vec3_add(u, v))), // the triangle maybe tilted along the diagnoal so;
                              aabb_create(vec3_add(Q, u), vec3_add(Q, v))),
        },
        .mat = mat,
        .Q = Q,
        .u = u,
        .v = v,
        .normal = normal,
        .D = vec3_dot(normal, Q),
        .W = vec3_mul_scalar(1.0 / vec3_dot(n, n), n),
    };
}

//////////////////////////////////////////////////////
//            __                      __            //
//           /  |                    /  |           //
//   _______ $$/   ______    _______ $$ |  ______   //
//  /       |/  | /      \  /       |$$ | /      \  //
// /$$$$$$$/ $$ |/$$$$$$  |/$$$$$$$/ $$ |/$$$$$$  | //
// $$ |      $$ |$$ |  $$/ $$ |      $$ |$$    $$ | //
// $$ \_____ $$ |$$ |      $$ \_____ $$ |$$$$$$$$/  //
// $$       |$$ |$$ |      $$       |$$ |$$       | //
//  $$$$$$$/ $$/ $$/        $$$$$$$/ $$/  $$$$$$$/  //
//                                                  //
//////////////////////////////////////////////////////

typedef struct circle {
    hittable base;
    material* mat;
    point3 center; /// center of circle
    vec3 normal;   /// normal to circle
    vec3 tangent;
    vec3 bitangent;
    f64 radius; /// radius of circle
    f64 D;
} circle;

bool circle_hit(hittable* circle_object, ray* r_in, interval r_t, hit_record* out_record);

INLINE circle circle_create(point3 center, f64 radius, vec3 normal, material* mat) {
    normal = vec3_unit(normal);
    vec3 tangent = vec3_cross(normal, (vec3){1, 0, 0});
    if (vec3_length(tangent) < 1e-8) {
        tangent = vec3_cross(normal, (vec3){0, 1, 0});
    }
    tangent = vec3_unit(tangent);
    vec3 bitangent = vec3_cross(tangent, normal);
    vec3 extent1 = vec3_mul_scalar(radius, tangent);
    vec3 extent2 = vec3_mul_scalar(radius, bitangent);
    return (circle){
        .base = {
            .hit = circle_hit,
            .box = aabb_create(vec3_sub(center, vec3_add(extent1, extent2)),
                               vec3_add(center, vec3_add(extent1, extent2))),
        },
        .mat = mat,
        .center = center,
        .normal = normal,
        .tangent = tangent,
        .bitangent = bitangent,
        .radius = radius,
        .D = vec3_dot(center, normal),
    };
}

//////////////////////////////////////////////////////////////////////////
//                      __  __                  __                      //
//                     /  |/  |                /  |                     //
//   _______  __    __ $$ |$$/  _______    ____$$ |  ______    ______   //
//  /       |/  |  /  |$$ |/  |/       \  /    $$ | /      \  /      \  //
// /$$$$$$$/ $$ |  $$ |$$ |$$ |$$$$$$$  |/$$$$$$$ |/$$$$$$  |/$$$$$$  | //
// $$ |      $$ |  $$ |$$ |$$ |$$ |  $$ |$$ |  $$ |$$    $$ |$$ |  $$/  //
// $$ \_____ $$ \__$$ |$$ |$$ |$$ |  $$ |$$ \__$$ |$$$$$$$$/ $$ |       //
// $$       |$$    $$ |$$ |$$ |$$ |  $$ |$$    $$ |$$       |$$ |       //
//  $$$$$$$/  $$$$$$$ |$$/ $$/ $$/   $$/  $$$$$$$/  $$$$$$$/ $$/        //
//           /  \__$$ |                                                 //
//           $$    $$/                                                  //
//            $$$$$$/                                                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

typedef struct cylinder {
    hittable base;
    material* mat;
    vec3 direction;
    f64 height;
    f64 radius;
    point3 bottom_center;
    point3 top_center;
    circle bottom_circle;
    circle top_circle;
} cylinder;

bool cylinder_hit(hittable* cylinder_object,
                  ray* r_in,
                  interval r_t,
                  hit_record* out_record);

INLINE cylinder cylinder_create(point3 cylinder_bottom_center,
                                vec3 cylinder_direction,
                                f64 cylinder_height,
                                f64 cylinder_radius,
                                material* mat) {
    cylinder_direction = vec3_unit(cylinder_direction);
    point3 cylinder_top_center = vec3_add(cylinder_bottom_center, vec3_mul_scalar(cylinder_height, cylinder_direction));
    return (cylinder){
        .base = {
            .hit = cylinder_hit,
            .box = aabb_expand(aabb_create(cylinder_bottom_center, cylinder_top_center), 2 * cylinder_radius),
        },
        .mat = mat,
        .direction = cylinder_direction,
        .height = cylinder_height,
        .radius = cylinder_radius,
        .bottom_center = cylinder_bottom_center,
        .top_center = cylinder_top_center,
        .bottom_circle = circle_create(cylinder_bottom_center, cylinder_radius, vec3_mul_scalar(-1.0, cylinder_direction), mat),
        .top_circle = circle_create(cylinder_top_center, cylinder_radius, cylinder_direction, mat),
    };
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//   ______   _______   ________  _______    ______   ________  ______   ______   __    __   ______   //
//  /      \ /       \ /        |/       \  /      \ /        |/      | /      \ /  \  /  | /      \  //
// /$$$$$$  |$$$$$$$  |$$$$$$$$/ $$$$$$$  |/$$$$$$  |$$$$$$$$/ $$$$$$/ /$$$$$$  |$$  \ $$ |/$$$$$$  | //
// $$ |  $$ |$$ |__$$ |$$ |__    $$ |__$$ |$$ |__$$ |   $$ |     $$ |  $$ |  $$ |$$$  \$$ |$$ \__$$/  //
// $$ |  $$ |$$    $$/ $$    |   $$    $$< $$    $$ |   $$ |     $$ |  $$ |  $$ |$$$$  $$ |$$      \  //
// $$ |  $$ |$$$$$$$/  $$$$$/    $$$$$$$  |$$$$$$$$ |   $$ |     $$ |  $$ |  $$ |$$ $$ $$ | $$$$$$  | //
// $$ \__$$ |$$ |      $$ |_____ $$ |  $$ |$$ |  $$ |   $$ |    _$$ |_ $$ \__$$ |$$ |$$$$ |/  \__$$ | //
// $$    $$/ $$ |      $$       |$$ |  $$ |$$ |  $$ |   $$ |   / $$   |$$    $$/ $$ | $$$ |$$    $$/  //
//  $$$$$$/  $$/       $$$$$$$$/ $$/   $$/ $$/   $$/    $$/    $$$$$$/  $$$$$$/  $$/   $$/  $$$$$$/   //
//                                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
//    __                                             __              __                //
//   /  |                                           /  |            /  |               //
//  _$$ |_     ______   ______   _______    _______ $$ |  ______   _$$ |_     ______   //
// / $$   |   /      \ /      \ /       \  /       |$$ | /      \ / $$   |   /      \  //
// $$$$$$/   /$$$$$$  |$$$$$$  |$$$$$$$  |/$$$$$$$/ $$ | $$$$$$  |$$$$$$/   /$$$$$$  | //
//   $$ | __ $$ |  $$/ /    $$ |$$ |  $$ |$$      \ $$ | /    $$ |  $$ | __ $$    $$ | //
//   $$ |/  |$$ |     /$$$$$$$ |$$ |  $$ | $$$$$$  |$$ |/$$$$$$$ |  $$ |/  |$$$$$$$$/  //
//   $$  $$/ $$ |     $$    $$ |$$ |  $$ |/     $$/ $$ |$$    $$ |  $$  $$/ $$       | //
//    $$$$/  $$/       $$$$$$$/ $$/   $$/ $$$$$$$/  $$/  $$$$$$$/    $$$$/   $$$$$$$/  //
//                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////

typedef struct translate {
    hittable base;
    hittable* object;
    vec3 offset;
    material* mat;
} translate;

INLINE bool translate_hit(hittable* object, ray* r_in, interval r_t, hit_record* record) {
    translate* trans = (translate*)object;
    ray r = {
        .origin = vec3_sub(r_in->origin, trans->offset),
        .direction = r_in->direction,
    };
    if (!trans->object->hit(trans->object, &r, r_t, record)) {
        return false;
    }
    record->point = vec3_add(record->point, trans->offset);
    record->mat = trans->mat;
    return true;
}

INLINE translate translate_object(hittable* object, vec3 offset, material* mat) {
    return (translate){
        .base = {
            .hit = translate_hit,
            .box = aabb_translate(&object->box, offset),
        },
        .object = object,
        .offset = offset,
        .mat = mat,
    };
}

//////////////////////////////////////////////////////////////////
//                        __                  __                //
//                       /  |                /  |               //
//   ______    ______   _$$ |_     ______   _$$ |_     ______   //
//  /      \  /      \ / $$   |   /      \ / $$   |   /      \  //
// /$$$$$$  |/$$$$$$  |$$$$$$/    $$$$$$  |$$$$$$/   /$$$$$$  | //
// $$ |  $$/ $$ |  $$ |  $$ | __  /    $$ |  $$ | __ $$    $$ | //
// $$ |      $$ \__$$ |  $$ |/  |/$$$$$$$ |  $$ |/  |$$$$$$$$/  //
// $$ |      $$    $$/   $$  $$/ $$    $$ |  $$  $$/ $$       | //
// $$/        $$$$$$/     $$$$/   $$$$$$$/    $$$$/   $$$$$$$/  //
//                                                              //
//////////////////////////////////////////////////////////////////

typedef struct rotate {
    hittable base;
    hittable* object;
    vec3 axis;
    f64 rad;
    material* mat;
} rotate;

INLINE bool rotate_hit(hittable* object, ray* r_in, interval r_t, hit_record* record) {
    rotate* rota = (rotate*)object;
    mat3 mtx = mat3_rotation(rota->rad * -1.0, rota->axis);
    ray r = {
        .origin = mat3_mul_vec3(mtx, r_in->origin),
        .direction = mat3_mul_vec3(mtx, r_in->direction),
    };
    if (!rota->object->hit(rota->object, &r, r_t, record)) {
        return false;
    }
    record->point = mat3_mul_vec3(mat3_rotation(rota->rad, rota->axis), record->point);
    record->mat = rota->mat;
    return true;
}

// NOTE: make sure the object's is symmetrical aligned with axis before rotating the object (to get correct results)
INLINE rotate rotate_object(hittable* object, vec3 axis, f64 rad, material* mat) {
    return (rotate){
        .base = {
            .hit = rotate_hit,
            .box = object->box,
        },
        .object = object,
        .axis = axis,
        .rad = rad,
        .mat = mat,
    };
}

////////////////////////////////////////////////////////////////////
//                                __  __                          //
//                               /  |/  |                         //
//  _____  ____    ______    ____$$ |$$/  __    __  _____  ____   //
// /     \/    \  /      \  /    $$ |/  |/  |  /  |/     \/    \  //
// $$$$$$ $$$$  |/$$$$$$  |/$$$$$$$ |$$ |$$ |  $$ |$$$$$$ $$$$  | //
// $$ | $$ | $$ |$$    $$ |$$ |  $$ |$$ |$$ |  $$ |$$ | $$ | $$ | //
// $$ | $$ | $$ |$$$$$$$$/ $$ \__$$ |$$ |$$ \__$$ |$$ | $$ | $$ | //
// $$ | $$ | $$ |$$       |$$    $$ |$$ |$$    $$/ $$ | $$ | $$ | //
// $$/  $$/  $$/  $$$$$$$/  $$$$$$$/ $$/  $$$$$$/  $$/  $$/  $$/  //
//                                                                //
////////////////////////////////////////////////////////////////////

typedef struct constant_medium {
    hittable base;
    hittable* object;
    f64 neg_inv_density;
    material* mat;
} constant_medium;

bool constant_medium_hit(hittable* object, ray* r_in, interval r_t, hit_record* record);

INLINE constant_medium constant_medium_create(hittable* object, f64 density, material* mat) {
    return (constant_medium){
        .base = {
            .hit = constant_medium_hit,
            .box = object->box,
        },
        .object = object,
        .neg_inv_density = (-1.0 / density),
        .mat = mat,
    };
}

#endif