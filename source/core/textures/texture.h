#ifndef TEXTURE__H
#define TEXTURE__H

#include "defines.h"
#include "zmath.h"

////////////////////////////////////////////////////////////////////////////
//    __                            __                                    //
//   /  |                          /  |                                   //
//  _$$ |_     ______   __    __  _$$ |_    __    __   ______    ______   //
// / $$   |   /      \ /  \  /  |/ $$   |  /  |  /  | /      \  /      \  //
// $$$$$$/   /$$$$$$  |$$  \/$$/ $$$$$$/   $$ |  $$ |/$$$$$$  |/$$$$$$  | //
//   $$ | __ $$    $$ | $$  $$<    $$ | __ $$ |  $$ |$$ |  $$/ $$    $$ | //
//   $$ |/  |$$$$$$$$/  /$$$$  \   $$ |/  |$$ \__$$ |$$ |      $$$$$$$$/  //
//   $$  $$/ $$       |/$$/ $$  |  $$  $$/ $$    $$/ $$ |      $$       | //
//    $$$$/   $$$$$$$/ $$/   $$/    $$$$/   $$$$$$/  $$/        $$$$$$$/  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

typedef struct texture texture;
struct texture {
    color (*value)(texture* tex, point3 point, f64 u, f64 v);
};

#define DEFAULT_TEXTURE_SCALE 5
// TODO: add default checker texture
INLINE color default_value(texture* tex, point3 point, f64 u, f64 v) {
    i32 x = (i32)(zfloor(DEFAULT_TEXTURE_SCALE * point.x));
    i32 y = (i32)(zfloor(DEFAULT_TEXTURE_SCALE * point.y));
    i32 z = (i32)(zfloor(DEFAULT_TEXTURE_SCALE * point.z));
    return ((x + y + z) % 2 == 0) ? (color){1, 0, 0} : (color){1, 1, 1};
}

////////////////////////////////////////////////////////////////////////////
//            __                            __                            //
//           /  |                          /  |                           //
//   _______ $$ |____    ______    _______ $$ |   __   ______    ______   //
//  /       |$$      \  /      \  /       |$$ |  /  | /      \  /      \  //
// /$$$$$$$/ $$$$$$$  |/$$$$$$  |/$$$$$$$/ $$ |_/$$/ /$$$$$$  |/$$$$$$  | //
// $$ |      $$ |  $$ |$$    $$ |$$ |      $$   $$<  $$    $$ |$$ |  $$/  //
// $$ \_____ $$ |  $$ |$$$$$$$$/ $$ \_____ $$$$$$  \ $$$$$$$$/ $$ |       //
// $$       |$$ |  $$ |$$       |$$       |$$ | $$  |$$       |$$ |       //
//  $$$$$$$/ $$/   $$/  $$$$$$$/  $$$$$$$/ $$/   $$/  $$$$$$$/ $$/        //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

typedef struct checker {
    texture base;
    color even;
    color odd;
    f64 scale;
} checker;

INLINE color checker_value(texture* tex, point3 point, f64 u, f64 v) {
    checker* check = (checker*)tex;
    i32 x = (i32)(zfloor(check->scale * point.x));
    i32 y = (i32)(zfloor(check->scale * point.y));
    i32 z = (i32)(zfloor(check->scale * point.z));
    return ((x + y + z) % 2 == 0) ? check->even : check->odd;
}

INLINE checker checker_create(color even, color odd, f64 scale) {
    return (checker){
        .base = {
            .value = checker_value,
        },
        .even = even,
        .odd = odd,
        .scale = scale,
    };
}

//////////////////////////////////////////////////////
//  __                                              //
// /  |                                             //
// $$/  _____  ____    ______    ______    ______   //
// /  |/     \/    \  /      \  /      \  /      \  //
// $$ |$$$$$$ $$$$  | $$$$$$  |/$$$$$$  |/$$$$$$  | //
// $$ |$$ | $$ | $$ | /    $$ |$$ |  $$ |$$    $$ | //
// $$ |$$ | $$ | $$ |/$$$$$$$ |$$ \__$$ |$$$$$$$$/  //
// $$ |$$ | $$ | $$ |$$    $$ |$$    $$ |$$       | //
// $$/ $$/  $$/  $$/  $$$$$$$/  $$$$$$$ | $$$$$$$/  //
//                             /  \__$$ |           //
//                             $$    $$/            //
//                              $$$$$$/             //
//                                                  //
//////////////////////////////////////////////////////

typedef struct image_texture image_texture;

image_texture* image_texture_create(const char* image_file);

void image_texture_destroy(image_texture* tex);

color image_texture_value(texture* tex, point3 point, f64 u, f64 v);

//////////////////////////////////////////////////////
//                                __  __            //
//                               /  |/  |           //
//   ______    ______    ______  $$ |$$/  _______   //
//  /      \  /      \  /      \ $$ |/  |/       \  //
// /$$$$$$  |/$$$$$$  |/$$$$$$  |$$ |$$ |$$$$$$$  | //
// $$ |  $$ |$$    $$ |$$ |  $$/ $$ |$$ |$$ |  $$ | //
// $$ |__$$ |$$$$$$$$/ $$ |      $$ |$$ |$$ |  $$ | //
// $$    $$/ $$       |$$ |      $$ |$$ |$$ |  $$ | //
// $$$$$$$/   $$$$$$$/ $$/       $$/ $$/ $$/   $$/  //
// $$ |                                             //
// $$ |                                             //
// $$/                                              //
//                                                  //
//////////////////////////////////////////////////////

typedef struct perlin_texture perlin_texture;

perlin_texture* perlin_texture_create();

void perlin_texture_destroy(perlin_texture* lin);

color perlin_texture_value(texture* tex, point3 point, f64 u, f64 v);

#endif