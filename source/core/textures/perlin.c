#include "texture.h"
#include "zmemory.h"

#define POINT_COUNT 256

typedef struct perlin_texture {
    texture base;
    vec3 gradients[POINT_COUNT];
    i32 perm_x[POINT_COUNT];
    i32 perm_y[POINT_COUNT];
    i32 perm_z[POINT_COUNT];
} perlin_texture;

f64 generate_perlin_noise(perlin_texture* lin, point3 point);

perlin_texture* perlin_texture_create() {
    perlin_texture* lin = zmemory_allocate(sizeof(perlin_texture));
    lin->base.value = perlin_texture_value;
    for (i32 i = 0; i < POINT_COUNT; ++i) {
        lin->gradients[i] = vec3_unit(vec3_random(-1, 1));
        lin->perm_x[i] = i;
        lin->perm_y[i] = i;
        lin->perm_z[i] = i;
    }
    // just shuffle
    for (i32 i = POINT_COUNT - 1; i > 0; --i) {
        i32 idx = random_int(0, i);
        i32 idy = random_int(0, i);
        i32 idz = random_int(0, i);

        i32 temp = lin->perm_x[i];
        lin->perm_x[i] = lin->perm_x[idx];
        lin->perm_x[idx] = temp;

        temp = lin->perm_y[i];
        lin->perm_y[i] = lin->perm_y[idy];
        lin->perm_y[idy] = temp;

        temp = lin->perm_z[i];
        lin->perm_z[i] = lin->perm_z[idz];
        lin->perm_z[idz] = temp;
    }
    return lin;
}

void perlin_texture_destroy(perlin_texture* lin) {
    if (lin) {
        zmemory_free(lin, sizeof(perlin_texture));
    }
}

color perlin_texture_value(texture* tex, point3 point, f64 u, f64 v) {
    // turbulance
    f64 final = 0;
    point3 temp = point;
    f64 weight = 1;
    perlin_texture* lin = (perlin_texture*)tex;
    for (i32 i = 0; i < 7; ++i) {
        final += weight * generate_perlin_noise(lin, temp);
        weight *= 0.5;
        temp = vec3_mul_scalar(2, temp);
    }
    final = (1 + zsin(4 * point.x + 10 * zfabs(final))) * 0.5;
    return (color){final, final, final};
}

//////////////////////////////////////////////////////////////////////
//  __                  __                                          //
// /  |                /  |                                         //
// $$ |____    ______  $$ |  ______    ______    ______    _______  //
// $$      \  /      \ $$ | /      \  /      \  /      \  /       | //
// $$$$$$$  |/$$$$$$  |$$ |/$$$$$$  |/$$$$$$  |/$$$$$$  |/$$$$$$$/  //
// $$ |  $$ |$$    $$ |$$ |$$ |  $$ |$$    $$ |$$ |  $$/ $$      \  //
// $$ |  $$ |$$$$$$$$/ $$ |$$ |__$$ |$$$$$$$$/ $$ |       $$$$$$  | //
// $$ |  $$ |$$       |$$ |$$    $$/ $$       |$$ |      /     $$/  //
// $$/   $$/  $$$$$$$/ $$/ $$$$$$$/   $$$$$$$/ $$/       $$$$$$$/   //
//                         $$ |                                     //
//                         $$ |                                     //
//                         $$/                                      //
//                                                                  //
//////////////////////////////////////////////////////////////////////

f64 generate_perlin_noise(perlin_texture* lin, point3 point) {

    // get the point in the unit cube
    f64 dx = point.x - zfloor(point.x);
    f64 dy = point.y - zfloor(point.y);
    f64 dz = point.z - zfloor(point.z);

    // get the cube`s lowest corner point
    i32 x = (i32)zfloor(point.x);
    i32 y = (i32)zfloor(point.y);
    i32 z = (i32)zfloor(point.z);

    // Smooth the fractional parts (apply smooth step function x*x(3-2*x))
    f64 dx_smooth = dx * dx * (3 - 2 * dx);
    f64 dy_smooth = dy * dy * (3 - 2 * dy);
    f64 dz_smooth = dz * dz * (3 - 2 * dz);

    vec3 eight_gradients[2][2][2];
    f64 eight_dot_products[2][2][2];

    for (i32 i = 0; i < 2; ++i) {
        for (i32 j = 0; j < 2; ++j) {
            for (i32 k = 0; k < 2; ++k) {
                // get all the 8 corners gradient
                eight_gradients[i][j][k] = lin->gradients[lin->perm_x[(i + x) & (POINT_COUNT - 1)] ^
                                                          lin->perm_y[(j + y) & (POINT_COUNT - 1)] ^
                                                          lin->perm_z[(k + z) & (POINT_COUNT - 1)]];
                // get all the 8 corners dot product
                eight_dot_products[i][j][k] = vec3_dot(eight_gradients[i][j][k], (vec3){dx - i, dy - j, dz - k});
            }
        }
    }

    // trilinear interpolation
    f64 edge1_lerp = lerp(eight_dot_products[0][0][0], eight_dot_products[0][0][1], dz_smooth); // left down edge
    f64 edge2_lerp = lerp(eight_dot_products[0][1][0], eight_dot_products[0][1][1], dz_smooth); // left up edge
    f64 edge3_lerp = lerp(eight_dot_products[1][0][0], eight_dot_products[1][0][1], dz_smooth); // right down edge
    f64 edge4_lerp = lerp(eight_dot_products[1][1][0], eight_dot_products[1][1][1], dz_smooth); // right up edge

    f64 lerp_edge1_edge2 = lerp(edge1_lerp, edge2_lerp, dy_smooth); // left face
    f64 lerp_edge3_edge4 = lerp(edge3_lerp, edge4_lerp, dy_smooth); // right face

    f64 final_lerp = lerp(lerp_edge1_edge2, lerp_edge3_edge4, dx_smooth);

    return final_lerp;
}
