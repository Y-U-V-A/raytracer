#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "logger.h"
#include "zmemory.h"

typedef struct image_texture {
    texture base;
    f32* data;
    i32 width;
    i32 height;
    i32 bytes_per_pixel;
} image_texture;

image_texture* image_texture_create(const char* image_file) {
    image_texture* tex = zmemory_allocate(sizeof(image_texture));
    tex->base.value = image_texture_value;
    stbi_set_flip_vertically_on_load(1);
    tex->data = stbi_loadf(image_file, &tex->width, &tex->height, &tex->bytes_per_pixel, 0); // pass zero for default
    if (!tex->data) {
        LOGE("%s image failed to load , using default texture", image_file);
        tex->base.value = default_value;
    }
    return tex;
}

void image_texture_destroy(image_texture* tex) {
    if (tex) {
        if (tex->data)
            stbi_image_free(tex->data);
        zmemory_free(tex, sizeof(image_texture));
    }
}

// UV coordinates (u, v) are normalized texture coordinates ranging from [0, 0] to [1, 1]
// using BILINEAR interpolation
color image_texture_value(texture* tex, point3 point, f64 u, f64 v) {
    image_texture* image_tex = (image_texture*)tex;

    f64 x = u * (image_tex->width - 1);
    f64 y = v * (image_tex->height - 1);

    // Find neighboring indices
    i32 x0 = (i32)zfloor(x);
    i32 y0 = (i32)zfloor(y);
    i32 x1 = x0 + 1;
    i32 y1 = y0 + 1;

    // Clamp indices to texture boundaries
    x1 = (x1 < image_tex->width) ? x1 : image_tex->width - 1;
    y1 = (y1 < image_tex->height) ? y1 : image_tex->height - 1;

    // Fractional parts
    f64 u_frac = x - x0;
    f64 v_frac = y - y0;

    i32 f00_idx = (x0 + y0 * image_tex->width) * image_tex->bytes_per_pixel;
    color f00 = {
        .x = image_tex->data[f00_idx + 0],
        .y = image_tex->data[f00_idx + 1],
        .z = image_tex->data[f00_idx + 2],
    };

    i32 f10_idx = (x1 + y0 * image_tex->width) * image_tex->bytes_per_pixel;
    color f10 = {
        .x = image_tex->data[f10_idx + 0],
        .y = image_tex->data[f10_idx + 1],
        .z = image_tex->data[f10_idx + 2],
    };
    i32 f11_idx = (x1 + y1 * image_tex->width) * image_tex->bytes_per_pixel;
    color f11 = {
        .x = image_tex->data[f11_idx + 0],
        .y = image_tex->data[f11_idx + 1],
        .z = image_tex->data[f11_idx + 2],
    };
    i32 f01_idx = (x0 + y1 * image_tex->width) * image_tex->bytes_per_pixel;
    color f01 = {
        .x = image_tex->data[f01_idx + 0],
        .y = image_tex->data[f01_idx + 1],
        .z = image_tex->data[f01_idx + 2],
    };

    // Compute weights
    f64 w00 = (1 - u_frac) * (1 - v_frac);
    f64 w10 = u_frac * (1 - v_frac);
    f64 w01 = (1 - u_frac) * v_frac;
    f64 w11 = u_frac * v_frac;

    // Perform bilinear interpolation
    return vec3_add(vec3_add(vec3_mul_scalar(w00, f00), vec3_mul_scalar(w10, f10)),
                    vec3_add(vec3_mul_scalar(w01, f01), vec3_mul_scalar(w11, f11)));
}
