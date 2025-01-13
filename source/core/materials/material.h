#ifndef MATERIAL__H
#define MATERIAL__H

#include "defines.h"
#include "zmath.h"
#include "ray.h"
#include "hittable.h"
#include "texture.h"

//////////////////////////////////////////////////////////////////////////////
//                            __                          __            __  //
//                           /  |                        /  |          /  | //
//  _____  ____    ______   _$$ |_     ______    ______  $$/   ______  $$ | //
// /     \/    \  /      \ / $$   |   /      \  /      \ /  | /      \ $$ | //
// $$$$$$ $$$$  | $$$$$$  |$$$$$$/   /$$$$$$  |/$$$$$$  |$$ | $$$$$$  |$$ | //
// $$ | $$ | $$ | /    $$ |  $$ | __ $$    $$ |$$ |  $$/ $$ | /    $$ |$$ | //
// $$ | $$ | $$ |/$$$$$$$ |  $$ |/  |$$$$$$$$/ $$ |      $$ |/$$$$$$$ |$$ | //
// $$ | $$ | $$ |$$    $$ |  $$  $$/ $$       |$$ |      $$ |$$    $$ |$$ | //
// $$/  $$/  $$/  $$$$$$$/    $$$$/   $$$$$$$/ $$/       $$/  $$$$$$$/ $$/  //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

typedef struct material material;

struct material {
    color (*emitted)(material* mat, hit_record* record);
    bool (*scatter)(material* mat, ray* r_in, hit_record* record, color* out_attenuation, ray* out_scattered);
};

INLINE color default_emitted(material* mat, hit_record* record) {
    return (color){0.0, 0.0, 0.0};
}
INLINE bool default_scatter(material* mat, ray* r_in, hit_record* record, color* out_attenuation, ray* out_scattered) {
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//  __                          __                              __      __                      //
// /  |                        /  |                            /  |    /  |                     //
// $$ |  ______   _____  ____  $$ |____    ______    ______   _$$ |_   $$/   ______   _______   //
// $$ | /      \ /     \/    \ $$      \  /      \  /      \ / $$   |  /  | /      \ /       \  //
// $$ | $$$$$$  |$$$$$$ $$$$  |$$$$$$$  |/$$$$$$  |/$$$$$$  |$$$$$$/   $$ | $$$$$$  |$$$$$$$  | //
// $$ | /    $$ |$$ | $$ | $$ |$$ |  $$ |$$    $$ |$$ |  $$/   $$ | __ $$ | /    $$ |$$ |  $$ | //
// $$ |/$$$$$$$ |$$ | $$ | $$ |$$ |__$$ |$$$$$$$$/ $$ |        $$ |/  |$$ |/$$$$$$$ |$$ |  $$ | //
// $$ |$$    $$ |$$ | $$ | $$ |$$    $$/ $$       |$$ |        $$  $$/ $$ |$$    $$ |$$ |  $$ | //
// $$/  $$$$$$$/ $$/  $$/  $$/ $$$$$$$/   $$$$$$$/ $$/          $$$$/  $$/  $$$$$$$/ $$/   $$/  //
//                                                                                              //
//////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct lambertian {
    material base;
    texture* tex;
    color abledo;
} lambertian;

INLINE bool lambertian_scatter(material* mat, ray* r_in, hit_record* record, color* out_attenuation, ray* out_scattered) {
    lambertian* lamb = (lambertian*)mat;
    vec3 scatter_direction = vec3_add(record->normal, vec3_random_unit_vector());
    if (vec3_compare(scatter_direction, (vec3){0.0, 0.0, 0.0})) {
        scatter_direction = record->normal;
    }
    *out_scattered = (ray){
        .origin = record->point,
        .direction = scatter_direction,
    };
    if (lamb->tex) {
        *out_attenuation = vec3_mul(lamb->tex->value(lamb->tex, record->point, record->u, record->v), lamb->abledo);
    } else {
        *out_attenuation = lamb->abledo;
    }
    return true;
}

INLINE lambertian lambertian_create(color albedo, texture* tex) {
    return (lambertian){
        .base = {
            .emitted = default_emitted,
            .scatter = lambertian_scatter,
        },
        .tex = tex,
        .abledo = albedo,
    };
}

//////////////////////////////////////////////////////
//                            __                __  //
//                           /  |              /  | //
//  _____  ____    ______   _$$ |_     ______  $$ | //
// /     \/    \  /      \ / $$   |   /      \ $$ | //
// $$$$$$ $$$$  |/$$$$$$  |$$$$$$/    $$$$$$  |$$ | //
// $$ | $$ | $$ |$$    $$ |  $$ | __  /    $$ |$$ | //
// $$ | $$ | $$ |$$$$$$$$/   $$ |/  |/$$$$$$$ |$$ | //
// $$ | $$ | $$ |$$       |  $$  $$/ $$    $$ |$$ | //
// $$/  $$/  $$/  $$$$$$$/    $$$$/   $$$$$$$/ $$/  //
//                                                  //
//////////////////////////////////////////////////////

typedef struct metal {
    material base;
    color albedo;
    texture* tex;
    f64 fuzz;
} metal;

INLINE bool metal_scatter(material* mat, ray* r_in, hit_record* record, vec3* out_attenuation, ray* out_scattered) {
    metal* mtal = (metal*)mat;
    vec3 reflected = vec3_reflect(vec3_unit(r_in->direction), record->normal);
    if (mtal->fuzz > DBL_EPSILON) {
        reflected = vec3_add(reflected, vec3_mul_scalar(mtal->fuzz, vec3_random_unit_vector()));
    }
    *out_scattered = (ray){
        .origin = record->point,
        .direction = reflected,
    };
    if (mtal->tex) {
        *out_attenuation = vec3_mul(mtal->albedo, mtal->tex->value(mtal->tex, record->point, record->u, record->v));
    } else {
        *out_attenuation = mtal->albedo;
    }
    return (vec3_dot(out_scattered->direction, record->normal) > 0.0);
}

INLINE metal metal_create(color albedo, texture* tex, f64 fuzz) {
    return (metal){
        .base = {
            .emitted = default_emitted,
            .scatter = metal_scatter,
        },
        .albedo = albedo,
        .tex = tex,
        .fuzz = fuzz,
    };
}

////////////////////////////////////////////////////////////////////////////////////////
//        __  __            __                        __                __            //
//       /  |/  |          /  |                      /  |              /  |           //
//   ____$$ |$$/   ______  $$ |  ______    _______  _$$ |_     ______  $$/   _______  //
//  /    $$ |/  | /      \ $$ | /      \  /       |/ $$   |   /      \ /  | /       | //
// /$$$$$$$ |$$ |/$$$$$$  |$$ |/$$$$$$  |/$$$$$$$/ $$$$$$/   /$$$$$$  |$$ |/$$$$$$$/  //
// $$ |  $$ |$$ |$$    $$ |$$ |$$    $$ |$$ |        $$ | __ $$ |  $$/ $$ |$$ |       //
// $$ \__$$ |$$ |$$$$$$$$/ $$ |$$$$$$$$/ $$ \_____   $$ |/  |$$ |      $$ |$$ \_____  //
// $$    $$ |$$ |$$       |$$ |$$       |$$       |  $$  $$/ $$ |      $$ |$$       | //
//  $$$$$$$/ $$/  $$$$$$$/ $$/  $$$$$$$/  $$$$$$$/    $$$$/  $$/       $$/  $$$$$$$/  //
//                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////

typedef struct dielectric {
    material base;
    color albedo;
    texture* tex;
    f64 refraction_index;
} dielectric;

INLINE bool dielectric_scatter(material* mat, ray* r_in, hit_record* record, vec3* out_attenuation, ray* out_scattered) {
    dielectric* diele = (dielectric*)mat;

    f64 ri = (record->front_face ? (1.0 / diele->refraction_index) : diele->refraction_index);
    vec3 unit = vec3_unit(r_in->direction);
    f64 val = vec3_dot(unit, record->normal) * -1.0;
    f64 cos_theta = (val > 1.0 ? 1.0 : val);
    f64 sin_theta = zsqrt(1.0 - cos_theta * cos_theta);
    bool cannot_refract = (ri * sin_theta > 1.0);

    vec3 dir;
    if (cannot_refract || reflectance(cos_theta, ri) > random_unit()) {
        dir = vec3_reflect(unit, record->normal);
    } else {
        dir = vec3_refract(unit, record->normal, ri);
    }

    *out_scattered = (ray){
        .origin = record->point,
        .direction = dir,
    };

    if (diele->tex) {
        *out_attenuation = vec3_mul(diele->albedo, diele->tex->value(diele->tex, record->point, record->u, record->v));
    } else {
        *out_attenuation = diele->albedo;
    }
    return true;
}

INLINE dielectric dielectric_create(color albedo, texture* tex, f64 refraction_index) {
    return (dielectric){
        .base = {
            .emitted = default_emitted,
            .scatter = dielectric_scatter,
        },
        .albedo = albedo,
        .tex = tex,
        .refraction_index = refraction_index,
    };
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//        __  __   ______    ______                                      __         __  __            __          __      //
//       /  |/  | /      \  /      \                                    /  |       /  |/  |          /  |        /  |     //
//   ____$$ |$$/ /$$$$$$  |/$$$$$$  |__    __   _______   ______    ____$$ |       $$ |$$/   ______  $$ |____   _$$ |_    //
//  /    $$ |/  |$$ |_ $$/ $$ |_ $$//  |  /  | /       | /      \  /    $$ |       $$ |/  | /      \ $$      \ / $$   |   //
// /$$$$$$$ |$$ |$$   |    $$   |   $$ |  $$ |/$$$$$$$/ /$$$$$$  |/$$$$$$$ |       $$ |$$ |/$$$$$$  |$$$$$$$  |$$$$$$/    //
// $$ |  $$ |$$ |$$$$/     $$$$/    $$ |  $$ |$$      \ $$    $$ |$$ |  $$ |       $$ |$$ |$$ |  $$ |$$ |  $$ |  $$ | __  //
// $$ \__$$ |$$ |$$ |      $$ |     $$ \__$$ | $$$$$$  |$$$$$$$$/ $$ \__$$ |       $$ |$$ |$$ \__$$ |$$ |  $$ |  $$ |/  | //
// $$    $$ |$$ |$$ |      $$ |     $$    $$/ /     $$/ $$       |$$    $$ |______ $$ |$$ |$$    $$ |$$ |  $$ |  $$  $$/  //
//  $$$$$$$/ $$/ $$/       $$/       $$$$$$/  $$$$$$$/   $$$$$$$/  $$$$$$$//      |$$/ $$/  $$$$$$$ |$$/   $$/    $$$$/   //
//                                                                         $$$$$$/         /  \__$$ |                     //
//                                                                                         $$    $$/                      //
//                                                                                          $$$$$$/                       //
//                                                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct diffuse_light {
    material base;
    color albedo;
    texture* tex;
} diffuse_light;

INLINE color diffuse_light_emmitted(material* mat, hit_record* record) {
    diffuse_light* light = (diffuse_light*)mat;
    if (light->tex) {
        return vec3_mul(light->tex->value(light->tex, record->point, record->u, record->v), light->albedo);
    }
    return light->albedo;
}

INLINE diffuse_light diffuse_light_create(color albedo, texture* tex) {
    return (diffuse_light){
        .base = {
            .emitted = diffuse_light_emmitted,
            .scatter = default_scatter,
        },
        .albedo = albedo,
        .tex = tex,
    };
}

#endif