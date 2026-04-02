#include "camera.h"
#include "logger.h"
#include "zmemory.h"
#include "hittable_list.h"
#include "platform.h"
#include "zthread.h"
#include "material.h"
#include "clock.h"

/////////////////////////////////////////////////////////////////////
//   _______   ______   _____  ____    ______    ______   ______   //
//  /       | /      \ /     \/    \  /      \  /      \ /      \  //
// /$$$$$$$/  $$$$$$  |$$$$$$ $$$$  |/$$$$$$  |/$$$$$$  |$$$$$$  | //
// $$ |       /    $$ |$$ | $$ | $$ |$$    $$ |$$ |  $$/ /    $$ | //
// $$ \_____ /$$$$$$$ |$$ | $$ | $$ |$$$$$$$$/ $$ |     /$$$$$$$ | //
// $$       |$$    $$ |$$ | $$ | $$ |$$       |$$ |     $$    $$ | //
//  $$$$$$$/  $$$$$$$/ $$/  $$/  $$/  $$$$$$$/ $$/       $$$$$$$/  //
//                                                                 //
/////////////////////////////////////////////////////////////////////

#define BYTES_PER_PIXEL 12 // "255 255 255" (including spaces)
#define PIXEL_DATA_SIZE (BYTES_PER_PIXEL + 4)
#define GAMMA 0.5

typedef struct camera {
    i32 image_width;
    i32 image_height;
    point3 pixel_00;
    vec3 delta_x;
    vec3 delta_y;
    vec3 origin;
    f64 inv_sqrt_spp;
    color (*background)(ray* r_in);
} camera;

typedef struct camera_thread_params {
    char* buffer;
    i32 buffer_size;
    camera* cam;
    hittable_list* world;
    i32 width;
    i32 height_start;
    i32 height_end;
    i32 depth;
    i32 sqrt_spp;
} camera_thread_params;

ray generate_ray(camera* cam, i32 width, i32 height, i32 row_s, i32 col_s);
color get_pixel_color(camera* cam, ray* r, i32 depth, hittable_list* world);
zthread_func_return_type camera_thread_start_func(void* params);

camera* camera_create(i32 image_width, i32 image_height) {
    if (image_height <= 0 || image_width <= 0) {
        LOGE("camera_create: invalid params");
        return 0;
    }
    camera* cam = zmemory_allocate(sizeof(camera));
    cam->image_width = image_width;
    cam->image_height = image_height;
    return cam;
}

void camera_destroy(camera* cam) {
    if (!cam) {
        LOGE("camera_destroy: invalid params");
        return;
    }
    zmemory_free(cam, sizeof(camera));
}

void camera_render(camera* cam, hittable_list* world, const char* file_path,
                   f64 viewport_field_of_view, point3 look_from, point3 look_at, vec3 world_up,
                   i32 samples_per_pixel, i32 depth, color (*background)(ray* r_in)) {

    char ppm[100] = {0};
    i32 i;
    for (i = 0; file_path[i] != '\0' && file_path[i] != '.'; ++i) {
        ppm[i] = file_path[i];
    }
    ppm[i++] = '.';
    ppm[i++] = 'p';
    ppm[i++] = 'p';
    ppm[i++] = 'm';

    FILE* file = file_open(ppm, 0, FILE_MODE_READ_AND_WRITE);

    clock clk;
    clock_set(&clk);

    if (cam == 0 || file == 0 || world == 0 || viewport_field_of_view <= 0.0 | samples_per_pixel <= 0 || depth <= 0) {
        LOGE("camera_render: invalid params");
        return;
    }

    // get the view port dimensions
    f64 focal_length = vec3_length(vec3_sub(look_at, look_from));
    f64 aspect = cam->image_width / (f64)cam->image_height;
    f64 viewport_height = ztan(DEG_TO_RAD(viewport_field_of_view * 0.5)) * focal_length * 2;
    f64 viewport_width = viewport_height * aspect;
    f64 delta_x = viewport_width / cam->image_width;
    f64 delta_y = viewport_height / cam->image_height;

    // calculate the u,v,w unit basis vectors for the camera coordinate frame.
    vec3 axis_z = vec3_unit(vec3_sub(look_from, look_at));
    vec3 axis_x = vec3_unit(vec3_cross(world_up, axis_z));
    vec3 axis_y = vec3_unit(vec3_cross(axis_z, axis_x));

    // get the top_left pixel coordinate
    vec3 temp = vec3_mul_scalar(viewport_width * -0.5, axis_x);
    temp = vec3_add(temp, vec3_mul_scalar(viewport_height * 0.5, axis_y));
    temp = vec3_add(temp, vec3_mul_scalar(focal_length * -1.0, axis_z));
    vec3 top_left = vec3_add(look_at, temp);

    cam->delta_x = vec3_mul_scalar(delta_x, axis_x);
    cam->delta_y = vec3_mul_scalar(delta_y * -1.0, axis_y);
    cam->pixel_00 = vec3_add(top_left, vec3_mul_scalar(0.5, vec3_add(cam->delta_x, cam->delta_y)));
    cam->origin = look_from;
    cam->background = background;
    i32 sqrt_spp = (i32)zsqrt(samples_per_pixel);
    cam->inv_sqrt_spp = 1.0 / sqrt_spp;

#ifndef MULTITHREADING
    // using single thread
    LOGI("generating image data on single thread...");

    f64 pixel_sample_scale = 1.0f / (sqrt_spp * sqrt_spp);

    for (i32 height = 0; height < cam->image_height; ++height) {
        LOG_STDOUT("\rremaning scanlines %u                      ", cam->image_height - height);

        for (i32 width = 0; width < cam->image_width; ++width) {
            color pixel_color = {0.0, 0.0, 0.0};
            for (i32 row_s = 0; row_s < sqrt_spp; ++row_s) {
                for (i32 col_s = 0; col_s < sqrt_spp; ++col_s) {
                    ray r = generate_ray(cam, width, height, row_s, col_s);
                    color c = get_pixel_color(cam, &r, depth, world);
                    pixel_color = vec3_add(pixel_color, c);
                }
            }
            pixel_color = vec3_mul_scalar(pixel_sample_scale, pixel_color); // take the average of all samples
            // gamma correction
            pixel_color.x = zpow(pixel_color.x, GAMMA);
            pixel_color.y = zpow(pixel_color.y, GAMMA);
            pixel_color.z = zpow(pixel_color.z, GAMMA);
            i32 r = (i32)(255.9999 * pixel_color.x);
            i32 g = (i32)(255.9999 * pixel_color.y);
            i32 b = (i32)(255.9999 * pixel_color.z);
            LOG_FILE(file, "%d %d %d\n", r, g, b);
        }
    }

#else
    // using multithread
    LOGI("generating image data on multi threads...");

    i32 cpu_count = platform_get_processor_count();
    if (cpu_count <= 0) {
        LOGE("Invalid processor count");
        return;
    }
    i32 thread_height = (cam->image_height / cpu_count);
    camera_thread_params params[cpu_count];
    zthread threads[cpu_count];

    for (i32 i = 0; i < cpu_count; ++i) {
        params[i].cam = cam;
        params[i].sqrt_spp = sqrt_spp;
        params[i].depth = depth;
        params[i].world = world;
        params[i].width = cam->image_width;
        params[i].height_start = i * thread_height;
        // last thread must include remaining lines
        if (i == cpu_count - 1) {
            params[i].height_end = cam->image_height;
        } else {
            params[i].height_end = (i + 1) * thread_height;
        }
        // width*height*no_of_char_in(255 255 255\n)+extra_padding
        params[i].buffer_size = (cam->image_width * (params[i].height_end - params[i].height_start) * PIXEL_DATA_SIZE);
        params[i].buffer = zmemory_allocate(params[i].buffer_size);
        if (params[i].buffer == 0) {
            LOGE("camera_render: failed to allocate memory");
            for (i32 j = 0; j < i; ++j) {
                zmemory_free(params[j].buffer, params[j].buffer_size);
            }
            return;
        }

        if (!zthread_create(camera_thread_start_func, &params[i], &threads[i])) {
            LOGE("camera_render: failed to create a zthread");
            for (i32 j = 0; j <= i; ++j) {
                zmemory_free(params[j].buffer, params[j].buffer_size);
            }
            return;
        }
    }

    if (!zthread_wait_on_all(threads, cpu_count)) {
        LOGE("camera_render: threads wait failed");
    }

    for (i32 i = 0; i < cpu_count; ++i) {
        zthread_destroy(&threads[i]); // does nothing important
    }

    LOGD("writing image data into file...");

    LOG_FILE(file, "P3\n%d %d\n%d\n", cam->image_width, cam->image_height, 255);

    for (i32 i = 0; i < cpu_count; ++i) {
        LOG_FILE(file, "%s", params[i].buffer);
    }
    // dealloc memory
    for (i32 i = 0; i < cpu_count; ++i) {
        zmemory_free(params[i].buffer, params[i].buffer_size);
    }

#endif

    file_close(file);

    clock_update(&clk);

    LOGT("\r DONE :)                                                            ");
    if (clk.elapsed < 60) {
        LOGD("time taken to render = %lf sec", clk.elapsed);
    } else if (clk.elapsed < 3600) {
        LOGD("time taken to render = %lf mins", clk.elapsed / 60);
    } else {
        LOGD("time taken to render = %lf hrs", clk.elapsed / 3600);
    }

#ifdef BMP
    char bmp[100] = {0};
    for (i = 0; file_path[i] != '\0'; ++i) {
        bmp[i] = file_path[i];
    }
    bmp[i++] = '.';
    bmp[i++] = 'b';
    bmp[i++] = 'm';
    bmp[i++] = 'p';
    convert_ppm_to_bmp(ppm, bmp);
#endif
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

ray generate_ray(camera* cam, i32 width, i32 height, i32 row_s, i32 col_s) {
    f64 dx = (col_s + random_unit()) * cam->inv_sqrt_spp;
    f64 dy = (row_s + random_unit()) * cam->inv_sqrt_spp;
    point3 x = vec3_mul_scalar((width + dx - 0.5), cam->delta_x);
    point3 y = vec3_mul_scalar((height + dy - 0.5), cam->delta_y);
    point3 pixel_sample = vec3_add(cam->pixel_00, vec3_add(x, y));
    return (ray){
        .origin = cam->origin,
        .direction = vec3_sub(pixel_sample, cam->origin),
    };
}

color get_pixel_color(camera* cam, ray* r, i32 depth, hittable_list* world) {
    if (depth <= 0) {
        return vec3_zero();
    }
    hit_record record = {0};
    if (!hittable_list_hit(world, r, (interval){0.0001, INFINITY}, &record)) {
        return cam->background(r);
    }
    color attenuation = {0};
    ray scattered = {0};
    color color_from_emmision = record.mat->emitted(record.mat, &record);
    if (!record.mat->scatter(record.mat, r, &record, &attenuation, &scattered)) {
        return color_from_emmision;
    }
    vec3 color_from_scatter = vec3_mul(attenuation, get_pixel_color(cam, &scattered, depth - 1, world));
    return vec3_add(color_from_emmision, color_from_scatter);
}

zthread_func_return_type camera_thread_start_func(void* params) {
    camera_thread_params* parameters = (camera_thread_params*)params;

    u64 offset = 0;
    f64 pixel_sample_scale = 1.0f / (parameters->sqrt_spp * parameters->sqrt_spp);

    for (i32 height = parameters->height_start; height < parameters->height_end; ++height) {

        for (i32 width = 0; width < parameters->width; ++width) {
            color pixel_color = {0.0, 0.0, 0.0};

            for (i32 row_s = 0; row_s < parameters->sqrt_spp; ++row_s) {
                for (i32 col_s = 0; col_s < parameters->sqrt_spp; ++col_s) {

                    ray r = generate_ray(parameters->cam, width, height, row_s, col_s);
                    color c = get_pixel_color(parameters->cam, &r, parameters->depth, parameters->world);
                    pixel_color = vec3_add(pixel_color, c);
                }
            }

            pixel_color = vec3_mul_scalar(pixel_sample_scale, pixel_color); // take the average of all samples
            pixel_color.x = zpow(pixel_color.x, GAMMA);
            pixel_color.y = zpow(pixel_color.y, GAMMA);
            pixel_color.z = zpow(pixel_color.z, GAMMA);
            i32 r = (i32)(255.9999 * pixel_color.x);
            i32 g = (i32)(255.9999 * pixel_color.y);
            i32 b = (i32)(255.9999 * pixel_color.z);
            offset += LOG_BUFFER(parameters->buffer + offset, parameters->buffer_size - offset, "%d %d %d\n", r, g, b);
        }
    }
    return 0;
}
