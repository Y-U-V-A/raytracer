#ifndef SANDBOX__H
#define SANDBOX__H

#include "zmemory.h"
#include "logger.h"
#include "hittable_list.h"
#include "camera.h"
#include "material.h"
#include "texture.h"
#include "bvh.h"
#include "darray.h"
#include "box.h"

#define darray_push_back_hittable_ptr(darray, object) \
    {                                                 \
        hittable* __ptr__ = (hittable*)(&object);     \
        darray = _darray_push_back(darray, &__ptr__); \
    }

color background_default(ray* r_in) {
    vec3 dir = vec3_unit(r_in->direction);
    f64 a = NDC_TO_UNIT(dir.y);
    color color1 = (color){1.0, 1.0, 1.0}; // white
    color color2 = (color){0.2, 0.5, 1.0}; // blue
    // (1-t)*A+t*B = A+t*(B-A)
    return vec3_add(color1, vec3_mul_scalar(a, vec3_sub(color2, color1))); // background_default linear interpolation
}

color background_black(ray* r_in) {
    return (color){0, 0, 0};
}

color background_white(ray* r_in) {
    return (color){1, 1, 1};
}

void scene_three_lambertian_spheres(const char* image_name) {

    image_texture* image_tex = image_texture_create("C:/yuva/CG/raytracing/raytracer/source/assets/Buddha.jpg");
    perlin_texture* perlin_tex = perlin_texture_create();

    lambertian ground_mat = lambertian_create((color){0.5, 0.5, 0.5}, (texture*)perlin_tex);
    lambertian left_mat = lambertian_create((color){0., 0.5, 0.8}, 0);
    lambertian center_mat = lambertian_create((color){1.0, 1.0, 1.0}, (texture*)(image_tex));
    lambertian right_mat = lambertian_create((color){0.0, 0.5, 0.8}, (texture*)(image_tex));

    sphere ground = sphere_create((point3){0, -1000, 0}, 1000, (material*)(&ground_mat));
    sphere sphere_object = sphere_create((point3){0.0, 0.0, 0.0}, 0.5, 0);
    translate left = translate_object((hittable*)(&sphere_object), (vec3){-1.0, 0.5, 0.0}, (material*)(&left_mat));
    translate center = translate_object((hittable*)(&sphere_object), (vec3){0.0, 0.5, 0.0}, (material*)(&center_mat));
    translate right = translate_object((hittable*)(&sphere_object), (vec3){1.0, 0.5, 0.0}, (material*)(&right_mat));

    hittable_list* world = hittable_list_create();
    hittable_list_add(world, (hittable*)(&ground));
    hittable_list_add(world, (hittable*)(&left));
    hittable_list_add(world, (hittable*)(&center));
    hittable_list_add(world, (hittable*)(&right));

    camera* cam = camera_create(1000, 600);
    camera_render(cam, world, image_name, 45, (vec3){0, 0.5, 5}, (vec3){0, 0.3, 0}, (vec3){0, 1, 0}, 256, 64, background_default);

    camera_destroy(cam);
    hittable_list_destroy(world);
    perlin_texture_destroy(perlin_tex);
    image_texture_destroy(image_tex);
}

void scene_three_metal_spheres(const char* image_name) {

    image_texture* image_tex = image_texture_create("C:/yuva/CG/raytracing/raytracer/source/assets/Buddha.jpg");
    perlin_texture* perlin_tex = perlin_texture_create();

    lambertian ground_mat = lambertian_create((color){0.5, 0.5, 0.5}, (texture*)perlin_tex);
    metal left_mat = metal_create((color){0., 0.5, 0.8}, 0, 0);
    metal center_mat = metal_create((color){1.0, 1.0, 1.0}, (texture*)(image_tex), 0);
    metal right_mat = metal_create((color){0.0, 0.5, 0.8}, (texture*)(image_tex), 0);

    sphere ground = sphere_create((point3){0, -1000, 0}, 1000, (material*)(&ground_mat));
    sphere sphere_object = sphere_create((point3){0.0, 0.0, 0.0}, 0.5, 0);
    translate left = translate_object((hittable*)(&sphere_object), (vec3){-1.0, 0.5, 0.0}, (material*)(&left_mat));
    translate center = translate_object((hittable*)(&sphere_object), (vec3){0.0, 0.5, 0.0}, (material*)(&center_mat));
    translate right = translate_object((hittable*)(&sphere_object), (vec3){1.0, 0.5, 0.0}, (material*)(&right_mat));

    hittable_list* world = hittable_list_create();
    hittable_list_add(world, (hittable*)(&ground));
    hittable_list_add(world, (hittable*)(&left));
    hittable_list_add(world, (hittable*)(&center));
    hittable_list_add(world, (hittable*)(&right));

    camera* cam = camera_create(1000, 600);
    camera_render(cam, world, image_name, 45, (vec3){0, 0.5, 5}, (vec3){0, 0.3, 0}, (vec3){0, 1, 0}, 256, 64, background_default);

    camera_destroy(cam);
    hittable_list_destroy(world);
    perlin_texture_destroy(perlin_tex);
    image_texture_destroy(image_tex);
}

void scene_three_dielectric_spheres(const char* image_name) {

    image_texture* image_tex = image_texture_create("C:/yuva/CG/raytracing/raytracer/source/assets/Buddha.jpg");
    perlin_texture* perlin_tex = perlin_texture_create();

    lambertian ground_mat = lambertian_create((color){0.5, 0.5, 0.5}, (texture*)perlin_tex);
    dielectric left_mat = dielectric_create((color){0., 0.5, 0.8}, 0, 1.5);
    dielectric center_mat = dielectric_create((color){1.0, 1.0, 1.0}, (texture*)(image_tex), 1.5);
    dielectric right_mat = dielectric_create((color){0.0, 0.5, 0.8}, (texture*)(image_tex), 1.5);

    sphere ground = sphere_create((point3){0, -1000, 0}, 1000, (material*)(&ground_mat));
    sphere sphere_object = sphere_create((point3){0.0, 0.0, 0.0}, 0.5, 0);
    translate left = translate_object((hittable*)(&sphere_object), (vec3){-1.0, 0.5, 0.0}, (material*)(&left_mat));
    translate center = translate_object((hittable*)(&sphere_object), (vec3){0.0, 0.5, 0.0}, (material*)(&center_mat));
    translate right = translate_object((hittable*)(&sphere_object), (vec3){1.0, 0.5, 0.0}, (material*)(&right_mat));

    hittable_list* world = hittable_list_create();
    hittable_list_add(world, (hittable*)(&ground));
    hittable_list_add(world, (hittable*)(&left));
    hittable_list_add(world, (hittable*)(&center));
    hittable_list_add(world, (hittable*)(&right));

    camera* cam = camera_create(1000, 600);
    camera_render(cam, world, image_name, 45, (vec3){0, 0.5, 5}, (vec3){0, 0.3, 0}, (vec3){0, 1, 0}, 256, 64, background_default);

    camera_destroy(cam);
    hittable_list_destroy(world);
    perlin_texture_destroy(perlin_tex);
    image_texture_destroy(image_tex);
}

void scene_three_lambertian_quad(const char* image_name) {

    image_texture* image_tex = image_texture_create("C:/yuva/CG/raytracing/raytracer/source/assets/Buddha.jpg");
    perlin_texture* perlin_tex = perlin_texture_create();

    lambertian ground_mat = lambertian_create((color){0.5, 0.5, 0.5}, (texture*)perlin_tex);
    lambertian left_mat = lambertian_create((color){0., 0.5, 0.8}, 0);
    lambertian center_mat = lambertian_create((color){1.0, 1.0, 1.0}, (texture*)(image_tex));
    lambertian right_mat = lambertian_create((color){0.0, 0.5, 0.8}, (texture*)(image_tex));

    sphere ground = sphere_create((point3){0, -1000, 0}, 1000, (material*)(&ground_mat));
    quad quad_object = quad_create((point3){-0.5, -0.5, 0.0}, (vec3){1.0, 0.0, 0.0}, (vec3){0.0, 1.0, 0.0}, 0);
    rotate quad_object_rotated = rotate_object((hittable*)(&quad_object), (vec3){0.0, 1.0, 0.0}, DEG_TO_RAD(45), 0);
    translate left = translate_object((hittable*)(&quad_object_rotated), (vec3){-1.0, 0.5, 0.0}, (material*)(&left_mat));
    translate center = translate_object((hittable*)(&quad_object_rotated), (vec3){0.0, 0.5, 0.0}, (material*)(&center_mat));
    translate right = translate_object((hittable*)(&quad_object_rotated), (vec3){1.0, 0.5, 0.0}, (material*)(&right_mat));

    hittable_list* world = hittable_list_create();
    hittable_list_add(world, (hittable*)(&ground));
    hittable_list_add(world, (hittable*)(&left));
    hittable_list_add(world, (hittable*)(&center));
    hittable_list_add(world, (hittable*)(&right));

    camera* cam = camera_create(1000, 600);
    camera_render(cam, world, image_name, 45, (vec3){0, 0.5, 5}, (vec3){0, 0.3, 0}, (vec3){0, 1, 0}, 256, 64, background_default);

    camera_destroy(cam);
    hittable_list_destroy(world);
    perlin_texture_destroy(perlin_tex);
    image_texture_destroy(image_tex);
}

void scene_three_metal_quad(const char* image_name) {

    image_texture* image_tex = image_texture_create("C:/yuva/CG/raytracing/raytracer/source/assets/Buddha.jpg");
    perlin_texture* perlin_tex = perlin_texture_create();

    lambertian ground_mat = lambertian_create((color){0.5, 0.5, 0.5}, (texture*)perlin_tex);
    metal left_mat = metal_create((color){0., 0.5, 0.8}, 0, 0);
    metal center_mat = metal_create((color){1.0, 1.0, 1.0}, (texture*)(image_tex), 0);
    metal right_mat = metal_create((color){0.0, 0.5, 0.8}, (texture*)(image_tex), 0);

    sphere ground = sphere_create((point3){0, -1000, 0}, 1000, (material*)(&ground_mat));
    quad quad_object = quad_create((point3){-0.5, -0.5, 0.0}, (vec3){1.0, 0.0, 0.0}, (vec3){0.0, 1.0, 0.0}, 0);
    rotate quad_object_rotated = rotate_object((hittable*)(&quad_object), (vec3){0.0, 1.0, 0.0}, DEG_TO_RAD(45), 0);
    translate left = translate_object((hittable*)(&quad_object_rotated), (vec3){-1.0, 0.5, 0.0}, (material*)(&left_mat));
    translate center = translate_object((hittable*)(&quad_object_rotated), (vec3){0.0, 0.5, 0.0}, (material*)(&center_mat));
    translate right = translate_object((hittable*)(&quad_object_rotated), (vec3){1.0, 0.5, 0.0}, (material*)(&right_mat));

    hittable_list* world = hittable_list_create();
    hittable_list_add(world, (hittable*)(&ground));
    hittable_list_add(world, (hittable*)(&left));
    hittable_list_add(world, (hittable*)(&center));
    hittable_list_add(world, (hittable*)(&right));

    camera* cam = camera_create(1000, 600);
    camera_render(cam, world, image_name, 45, (vec3){0, 0.5, 5}, (vec3){0, 0.3, 0}, (vec3){0, 1, 0}, 256, 64, background_default);

    camera_destroy(cam);
    hittable_list_destroy(world);
    perlin_texture_destroy(perlin_tex);
    image_texture_destroy(image_tex);
}

void scene_three_dielectric_quad(const char* image_name) {

    image_texture* image_tex = image_texture_create("C:/yuva/CG/raytracing/raytracer/source/assets/Buddha.jpg");
    perlin_texture* perlin_tex = perlin_texture_create();

    lambertian ground_mat = lambertian_create((color){0.5, 0.5, 0.5}, (texture*)perlin_tex);
    dielectric left_mat = dielectric_create((color){0., 0.5, 0.8}, 0, 1.5);
    dielectric center_mat = dielectric_create((color){1.0, 1.0, 1.0}, (texture*)(image_tex), 1.5);
    dielectric right_mat = dielectric_create((color){0.0, 0.5, 0.8}, (texture*)(image_tex), 1.5);

    sphere ground = sphere_create((point3){0, -1000, 0}, 1000, (material*)(&ground_mat));
    quad quad_object = quad_create((point3){-0.5, -0.5, 0.0}, (vec3){1.0, 0.0, 0.0}, (vec3){0.0, 1.0, 0.0}, 0);
    rotate quad_object_rotated = rotate_object((hittable*)(&quad_object), (vec3){0.0, 1.0, 0.0}, DEG_TO_RAD(45), 0);
    translate left = translate_object((hittable*)(&quad_object_rotated), (vec3){-1.0, 0.5, 0.0}, (material*)(&left_mat));
    translate center = translate_object((hittable*)(&quad_object_rotated), (vec3){0.0, 0.5, 0.0}, (material*)(&center_mat));
    translate right = translate_object((hittable*)(&quad_object_rotated), (vec3){1.0, 0.5, 0.0}, (material*)(&right_mat));

    hittable_list* world = hittable_list_create();
    hittable_list_add(world, (hittable*)(&ground));
    hittable_list_add(world, (hittable*)(&left));
    hittable_list_add(world, (hittable*)(&center));
    hittable_list_add(world, (hittable*)(&right));

    camera* cam = camera_create(1000, 600);
    camera_render(cam, world, image_name, 45, (vec3){0, 0.5, 5}, (vec3){0, 0.3, 0}, (vec3){0, 1, 0}, 256, 64, background_default);

    camera_destroy(cam);
    hittable_list_destroy(world);
    perlin_texture_destroy(perlin_tex);
    image_texture_destroy(image_tex);
}

void scene_three_lambertian_circle(const char* image_name) {

    image_texture* image_tex = image_texture_create("C:/yuva/CG/raytracing/raytracer/source/assets/Buddha.jpg");
    perlin_texture* perlin_tex = perlin_texture_create();

    lambertian ground_mat = lambertian_create((color){0.5, 0.5, 0.5}, (texture*)perlin_tex);
    lambertian left_mat = lambertian_create((color){0., 0.5, 0.8}, 0);
    lambertian center_mat = lambertian_create((color){1.0, 1.0, 1.0}, (texture*)(image_tex));
    lambertian right_mat = lambertian_create((color){0.0, 0.5, 0.8}, (texture*)(image_tex));

    sphere ground = sphere_create((point3){0, -1000, 0}, 1000, (material*)(&ground_mat));
    circle circle_object = circle_create((point3){0.0, 0.0, 0.0}, 0.5, (vec3){0.0, 0.0, 1.0}, 0);
    rotate circle_object_rotated = rotate_object((hittable*)(&circle_object), (vec3){0.0, 1.0, 0.0}, DEG_TO_RAD(45), 0);
    translate left = translate_object((hittable*)(&circle_object_rotated), (vec3){-1.0, 0.5, 0.0}, (material*)(&left_mat));
    translate center = translate_object((hittable*)(&circle_object_rotated), (vec3){0.0, 0.5, 0.0}, (material*)(&center_mat));
    translate right = translate_object((hittable*)(&circle_object_rotated), (vec3){1.0, 0.5, 0.0}, (material*)(&right_mat));

    hittable_list* world = hittable_list_create();
    hittable_list_add(world, (hittable*)(&ground));
    hittable_list_add(world, (hittable*)(&left));
    hittable_list_add(world, (hittable*)(&center));
    hittable_list_add(world, (hittable*)(&right));

    camera* cam = camera_create(1000, 600);
    camera_render(cam, world, image_name, 45, (vec3){0, 0.5, 5}, (vec3){0, 0.3, 0}, (vec3){0, 1, 0}, 256, 64, background_default);

    camera_destroy(cam);
    hittable_list_destroy(world);
    perlin_texture_destroy(perlin_tex);
    image_texture_destroy(image_tex);
}

void scene_three_metal_circle(const char* image_name) {

    image_texture* image_tex = image_texture_create("C:/yuva/CG/raytracing/raytracer/source/assets/Buddha.jpg");
    perlin_texture* perlin_tex = perlin_texture_create();

    lambertian ground_mat = lambertian_create((color){0.5, 0.5, 0.5}, (texture*)perlin_tex);
    metal left_mat = metal_create((color){0., 0.5, 0.8}, 0, 0);
    metal center_mat = metal_create((color){1.0, 1.0, 1.0}, (texture*)(image_tex), 0);
    metal right_mat = metal_create((color){0.0, 0.5, 0.8}, (texture*)(image_tex), 0);

    sphere ground = sphere_create((point3){0, -1000, 0}, 1000, (material*)(&ground_mat));
    circle circle_object = circle_create((point3){0.0, 0.0, 0.0}, 0.5, (vec3){0.0, 0.0, 1.0}, 0);
    rotate circle_object_rotated = rotate_object((hittable*)(&circle_object), (vec3){0.0, 1.0, 0.0}, DEG_TO_RAD(45), 0);
    translate left = translate_object((hittable*)(&circle_object_rotated), (vec3){-1.0, 0.5, 0.0}, (material*)(&left_mat));
    translate center = translate_object((hittable*)(&circle_object_rotated), (vec3){0.0, 0.5, 0.0}, (material*)(&center_mat));
    translate right = translate_object((hittable*)(&circle_object_rotated), (vec3){1.0, 0.5, 0.0}, (material*)(&right_mat));

    hittable_list* world = hittable_list_create();
    hittable_list_add(world, (hittable*)(&ground));
    hittable_list_add(world, (hittable*)(&left));
    hittable_list_add(world, (hittable*)(&center));
    hittable_list_add(world, (hittable*)(&right));

    camera* cam = camera_create(1000, 600);
    camera_render(cam, world, image_name, 45, (vec3){0, 0.5, 5}, (vec3){0, 0.3, 0}, (vec3){0, 1, 0}, 256, 64, background_default);

    camera_destroy(cam);
    hittable_list_destroy(world);
    perlin_texture_destroy(perlin_tex);
    image_texture_destroy(image_tex);
}

void scene_three_dielectric_circle(const char* image_name) {

    image_texture* image_tex = image_texture_create("C:/yuva/CG/raytracing/raytracer/source/assets/Buddha.jpg");
    perlin_texture* perlin_tex = perlin_texture_create();

    lambertian ground_mat = lambertian_create((color){0.5, 0.5, 0.5}, (texture*)perlin_tex);
    dielectric left_mat = dielectric_create((color){0., 0.5, 0.8}, 0, 1.5);
    dielectric center_mat = dielectric_create((color){1.0, 1.0, 1.0}, (texture*)(image_tex), 1.5);
    dielectric right_mat = dielectric_create((color){0.0, 0.5, 0.8}, (texture*)(image_tex), 1.5);

    sphere ground = sphere_create((point3){0, -1000, 0}, 1000, (material*)(&ground_mat));
    circle circle_object = circle_create((point3){0.0, 0.0, 0.0}, 0.5, (vec3){0.0, 0.0, 1.0}, 0);
    rotate circle_object_rotated = rotate_object((hittable*)(&circle_object), (vec3){0.0, 1.0, 0.0}, DEG_TO_RAD(45), 0);
    translate left = translate_object((hittable*)(&circle_object_rotated), (vec3){-1.0, 0.5, 0.0}, (material*)(&left_mat));
    translate center = translate_object((hittable*)(&circle_object_rotated), (vec3){0.0, 0.5, 0.0}, (material*)(&center_mat));
    translate right = translate_object((hittable*)(&circle_object_rotated), (vec3){1.0, 0.5, 0.0}, (material*)(&right_mat));

    hittable_list* world = hittable_list_create();
    hittable_list_add(world, (hittable*)(&ground));
    hittable_list_add(world, (hittable*)(&left));
    hittable_list_add(world, (hittable*)(&center));
    hittable_list_add(world, (hittable*)(&right));

    camera* cam = camera_create(1000, 600);
    camera_render(cam, world, image_name, 45, (vec3){0, 0.5, 5}, (vec3){0, 0.3, 0}, (vec3){0, 1, 0}, 256, 64, background_default);

    camera_destroy(cam);
    hittable_list_destroy(world);
    perlin_texture_destroy(perlin_tex);
    image_texture_destroy(image_tex);
}

void scene_spheres_world(const char* image_name) {

    hittable_list* world = hittable_list_create();

    lambertian ground_material = lambertian_create((color){0.5, 0.5, 0.5}, 0);
    sphere ground = sphere_create((point3){0, -1000, 0}, 1000, (material*)(&ground_material));
    hittable_list_add(world, (hittable*)(&ground));

    sphere* spheres = zmemory_allocate(sizeof(sphere) * 450);
    lambertian* lamb_mats = zmemory_allocate(sizeof(lambertian) * 200);
    metal* metals = zmemory_allocate(sizeof(metal) * 200);
    dielectric* trans = zmemory_allocate(sizeof(dielectric) * 200);

    i32 lamb = 0, mtal = 0, tra = 0, sph = 0;

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            f64 choose_mat = random_unit();
            point3 center = {a + 0.9 * random_unit(), 0.2, b + 0.9 * random_unit()};
            vec3 temp = vec3_sub(center, (point3){4, 0.2, 0});

            if (vec3_length_squared(temp) > 0.9 * 0.9) {

                if (choose_mat < 0.8 && lamb < 195) {
                    // diffuse
                    vec3 albedo = vec3_mul(vec3_random(0, 1), vec3_random(0, 1));
                    lamb_mats[lamb] = lambertian_create(albedo, 0);
                    spheres[sph] = sphere_create(center, 0.2, (material*)(&lamb_mats[lamb]));
                    hittable_list_add(world, (hittable*)(&spheres[sph]));
                    lamb++;
                    sph++;
                } else if (choose_mat < 0.95 && mtal < 195) {
                    // metal
                    vec3 albedo = vec3_random(0.5, 1);
                    f64 fuzz = random_double(0, 0.5);
                    metals[mtal] = metal_create(albedo, 0, fuzz);
                    spheres[sph] = sphere_create(center, 0.2, (material*)(&metals[mtal]));
                    hittable_list_add(world, (hittable*)(&spheres[sph]));
                    mtal++;
                    sph++;
                } else if (tra < 195) {
                    // glass

                    trans[tra] = dielectric_create((color){1, 1, 1}, 0, 1.5);
                    spheres[sph] = sphere_create(center, 0.2, (material*)(&trans[tra]));
                    hittable_list_add(world, (hittable*)(&spheres[sph]));
                    tra++;
                    sph++;
                }
            }
        }
    }

    trans[tra] = dielectric_create((color){1, 1, 1}, 0, 1.5);
    spheres[sph] = sphere_create((point3){0, 1, 0}, 1.0, (material*)(&trans[tra]));
    hittable_list_add(world, (hittable*)(&spheres[sph]));
    tra++;
    sph++;

    lamb_mats[lamb] = lambertian_create((color){0.4, 0.2, 0.1}, 0);
    spheres[sph] = sphere_create((point3){-4, 1, 0}, 1.0, (material*)(&lamb_mats[lamb]));
    hittable_list_add(world, (hittable*)(&spheres[sph]));
    lamb++;
    sph++;

    metals[mtal] = metal_create((color){0.7, 0.6, 0.5}, 0, 0.0);
    spheres[sph] = sphere_create((point3){4, 1, 0}, 1.0, (material*)(&metals[mtal]));
    hittable_list_add(world, (hittable*)(&spheres[sph]));
    mtal++;
    sph++;

    camera* cam = camera_create(1200, 1000);

    camera_render(cam, world, image_name,
                  90, (point3){13, 2, 3}, (point3){0, 0, 0}, (point3){0, 1, 0}, 1024, 128, background_default);

    camera_destroy(cam);
    hittable_list_destroy(world);

    zmemory_free(spheres, sizeof(sphere) * 450);
    zmemory_free(lamb_mats, sizeof(lambertian) * 200);
    zmemory_free(metals, sizeof(metal) * 200);
    zmemory_free(trans, sizeof(dielectric) * 200);
}

void scene_spheres_world_using_bvh(const char* image_name) {

    hittable** world_array = darray_create(hittable*);

    lambertian ground_material = lambertian_create((color){0.5, 0.5, 0.5}, 0);
    sphere ground = sphere_create((point3){0, -1000, 0}, 1000, (material*)(&ground_material));
    darray_push_back_hittable_ptr(world_array, ground);

    sphere* spheres = zmemory_allocate(sizeof(sphere) * 600);
    lambertian* lamb_mats = zmemory_allocate(sizeof(lambertian) * 200);
    metal* metals = zmemory_allocate(sizeof(metal) * 200);
    dielectric* trans = zmemory_allocate(sizeof(dielectric) * 200);

    i32 lamb = 0, mtal = 0, tra = 0, sph = 0;

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            f64 choose_mat = random_unit();
            point3 center = {a + 0.9 * random_unit(), 0.2, b + 0.9 * random_unit()};
            vec3 temp = vec3_sub(center, (point3){4, 0.2, 0});

            if (vec3_length_squared(temp) > 0.9 * 0.9) {

                if (choose_mat < 0.8 && lamb < 195) {
                    // diffuse
                    vec3 albedo = vec3_mul(vec3_random(0, 1), vec3_random(0, 1));
                    lamb_mats[lamb] = lambertian_create(albedo, 0);
                    spheres[sph] = sphere_create(center, 0.2, (material*)(&lamb_mats[lamb]));
                    darray_push_back_hittable_ptr(world_array, spheres[sph]);
                    lamb++;
                    sph++;
                } else if (choose_mat < 0.95 && mtal < 195) {
                    // metal
                    vec3 albedo = vec3_random(0.5, 1);
                    f64 fuzz = random_double(0, 0.5);
                    metals[mtal] = metal_create(albedo, 0, fuzz);
                    spheres[sph] = sphere_create(center, 0.2, (material*)(&metals[mtal]));
                    darray_push_back_hittable_ptr(world_array, spheres[sph]);
                    mtal++;
                    sph++;
                } else if (tra < 195) {
                    // glass
                    trans[tra] = dielectric_create((color){1, 1, 1}, 0, 1.5);
                    spheres[sph] = sphere_create(center, 0.2, (material*)(&trans[tra]));
                    darray_push_back_hittable_ptr(world_array, spheres[sph]);
                    tra++;
                    sph++;
                }
            }
        }
    }

    trans[tra] = dielectric_create((color){1, 1, 1}, 0, 1.5);
    spheres[sph] = sphere_create((point3){0, 1, 0}, 1.0, (material*)(&trans[tra]));
    darray_push_back_hittable_ptr(world_array, spheres[sph]);
    tra++;
    sph++;

    lamb_mats[lamb] = lambertian_create((color){0.4, 0.2, 0.1}, 0);
    spheres[sph] = sphere_create((point3){-4, 1, 0}, 1.0, (material*)(&lamb_mats[lamb]));
    darray_push_back_hittable_ptr(world_array, spheres[sph]);
    lamb++;
    sph++;

    metals[mtal] = metal_create((color){0.7, 0.6, 0.5}, 0, 0.0);
    spheres[sph] = sphere_create((point3){4, 1, 0}, 1.0, (material*)(&metals[mtal]));
    darray_push_back_hittable_ptr(world_array, spheres[sph]);
    mtal++;
    sph++;

    hittable* bvh = bvh_create(world_array);
    hittable_list* world = hittable_list_create();
    hittable_list_add(world, bvh);

    camera* cam = camera_create(1200, 1000);

    camera_render(cam, world, image_name,
                  90, (point3){13, 2, 3}, (point3){0, 0, 0}, (point3){0, 1, 0}, 1024, 128, background_default);

    camera_destroy(cam);
    hittable_list_destroy(world);
    bvh_destroy(bvh);
    zmemory_free(spheres, sizeof(sphere) * 600);
    zmemory_free(lamb_mats, sizeof(lambertian) * 200);
    zmemory_free(metals, sizeof(metal) * 200);
    zmemory_free(trans, sizeof(dielectric) * 200);

    darray_destroy(world_array);
}

void scene_lights(const char* image_name) {
    perlin_texture* tex = perlin_texture_create();
    lambertian mat = lambertian_create((color){1, 1, 1}, (texture*)(tex));
    diffuse_light light = diffuse_light_create((color){15, 15, 15}, 0);

    sphere ground = sphere_create((point3){0.0, -1000.0, 0}, 1000, (material*)(&mat));
    sphere ball = sphere_create((point3){0.0, 2.0, 0}, 2, (material*)(&mat));
    quad square = quad_create((point3){3, 1, 0}, (vec3){2, 0, 0}, (vec3){0, 2, 0}, (material*)(&light));
    sphere ball2 = sphere_create((point3){0, 3, 0}, 0.5, (material*)(&light));

    hittable_list* world = hittable_list_create();
    hittable_list_add(world, (hittable*)(&ground));
    hittable_list_add(world, (hittable*)(&ball));
    hittable_list_add(world, (hittable*)(&square));
    hittable_list_add(world, (hittable*)(&ball2));

    // render
    camera* cam = camera_create(1000, 800);
    camera_render(cam, world, image_name, 90, (vec3){5, 2, 15}, (vec3){0, 0.5, 0}, (vec3){0, 1, 0}, 512, 64, background_black);

    camera_destroy(cam);
    hittable_list_destroy(world);
    perlin_texture_destroy(tex);
}

void scene_10000_rays(const char* image_name) {

    lambertian ground = lambertian_create((color){0.48, 0.83, 0.53}, 0);

    int boxes_per_side = 20;
    box** boxs = zmemory_allocate(sizeof(box*) * boxes_per_side * boxes_per_side);
    hittable** bvh_array = darray_create(hittable*);

    int idx = 0;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            f64 w = 100.0;
            f64 x0 = -1000.0 + i * w;
            f64 z0 = -1000.0 + j * w;
            f64 y0 = 0.0;
            f64 x1 = x0 + w;
            f64 y1 = random_double(1, 101);
            f64 z1 = z0 + w;

            boxs[idx] = box_create((point3){x0, y0, z0}, (point3){x1, y1, z1}, (material*)(&ground));
            bvh_array = _darray_push_back(bvh_array, &(boxs[idx]));
            idx++;
        }
    }

    diffuse_light light = diffuse_light_create((color){7, 7, 7}, 0);
    quad quad_light = quad_create((point3){123, 554, 147}, (vec3){300, 0, 0}, (vec3){0, 0, 265}, (material*)(&light));
    darray_push_back_hittable_ptr(bvh_array, quad_light);

    lambertian sphere1_material = lambertian_create((color){0.7, 0.3, 0.1}, 0);
    dielectric sphere2_material = dielectric_create((color){1, 1, 1}, 0, 1.5);
    metal sphere3_material = metal_create((color){0.8, 0.8, 0.9}, 0, 1.0);
    sphere sphere1 = sphere_create((point3){400, 400, 200}, 50, (material*)(&sphere1_material));
    sphere sphere2 = sphere_create((point3){260, 150, 45}, 50, (material*)(&sphere2_material));
    sphere sphere3 = sphere_create((point3){0, 150, 145}, 50, (material*)(&sphere3_material));
    darray_push_back_hittable_ptr(bvh_array, sphere1);
    darray_push_back_hittable_ptr(bvh_array, sphere2);
    darray_push_back_hittable_ptr(bvh_array, sphere3);

    dielectric boundry_material = dielectric_create((color){1, 1, 1}, 0, 1.5);
    sphere boundary = sphere_create((point3){360, 150, 145}, 70, (material*)(&boundry_material));
    darray_push_back_hittable_ptr(bvh_array, boundary);

    image_texture* image = image_texture_create("C:/yuva/repos/raytracer/source/assets/sky.jpg");
    lambertian sphere4_material = lambertian_create((color){1, 1, 1}, (texture*)(image));
    sphere sphere4 = sphere_create((point3){400, 200, 400}, 100, (material*)(&sphere4_material));
    darray_push_back_hittable_ptr(bvh_array, sphere4);

    perlin_texture* perlin = perlin_texture_create();
    lambertian sphere5_material = lambertian_create((color){1, 1, 1}, (texture*)(perlin));
    sphere sphere5 = sphere_create((point3){220, 280, 300}, 80, (material*)(&sphere5_material));
    darray_push_back_hittable_ptr(bvh_array, sphere5);

    int ns = 1000;
    sphere* spheres = zmemory_allocate(sizeof(sphere) * ns);
    hittable** mini_bvh_array = darray_create(hittable*);
    for (int j = 0; j < ns; j++) {
        spheres[j] = sphere_create(vec3_random(-82.5, 82.5), 10, 0);
        darray_push_back_hittable_ptr(mini_bvh_array, spheres[j]);
    }
    lambertian white = lambertian_create((color){.73, .73, .73}, 0);
    hittable* mini_bvh = bvh_create(mini_bvh_array);
    rotate rotate_bvh = rotate_object(mini_bvh, (vec3){0, 1, 0}, DEG_TO_RAD(15), 0);
    translate trans = translate_object((hittable*)(&rotate_bvh), (vec3){-100, 270, 395}, (material*)(&white));
    darray_push_back_hittable_ptr(bvh_array, trans);

    hittable* bvh = bvh_create(bvh_array);

    hittable_list* world = hittable_list_create();
    hittable_list_add(world, bvh);

    // render
    camera* cam = camera_create(1200, 1000);

    camera_render(cam, world, image_name, 60, (vec3){478, 278, -600}, (vec3){278, 278, 0}, (vec3){0, 1, 0}, 1024, 64, background_black);

    camera_destroy(cam);
    hittable_list_destroy(world);
    bvh_destroy(bvh);
    zmemory_free(spheres, sizeof(sphere) * ns);
    darray_destroy(mini_bvh_array);
    bvh_destroy(mini_bvh);
    perlin_texture_destroy(perlin);
    image_texture_destroy(image);
    darray_destroy(bvh_array);
    for (int i = 0; i < idx; i++) {
        box_destroy(boxs[i]);
    }
    zmemory_free(boxs, sizeof(box*) * boxes_per_side * boxes_per_side);
}

void scene_cornell(const char* image_name) {

    lambertian red = lambertian_create((color){.65, .05, .05}, 0);
    lambertian white = lambertian_create((color){.73, .73, .73}, 0);
    lambertian green = lambertian_create((color){.12, .45, .15}, 0);
    diffuse_light light = diffuse_light_create((color){15, 15, 15}, 0);

    // Cornell box sides
    quad q1 = quad_create((point3){0, 0, 0}, (vec3){0, 0, -555}, (vec3){0, 555, 0}, (material*)(&green));
    quad q2 = quad_create((point3){555, 0, 0}, (vec3){0, 0, -555}, (vec3){0, 555, 0}, (material*)(&red));
    quad q3 = quad_create((point3){0, 555, 0}, (vec3){555, 0, 0}, (vec3){0, 0, -555}, (material*)(&white)); // top
    quad q4 = quad_create((point3){0, 0, 0}, (vec3){555, 0, 0}, (vec3){0, 0, -555}, (material*)(&white));   // bottom
    quad q5 = quad_create((point3){0, 0, -555}, (vec3){555, 0, 0}, (vec3){0, 555, 0}, (material*)(&white)); // back
    quad q6_light = quad_create((point3){225, 554, -225}, (vec3){100, 0, 0}, (vec3){0, 0, -100}, (material*)(&light));

    // // Box 1
    box* box1 = box_create((point3){-82.5, -165, -82.5}, (point3){82.5, 165, 82.5}, 0);
    rotate box1_rotated = rotate_object((hittable*)box1, (vec3){0.0, 1.0, 0.0}, DEG_TO_RAD(15), 0);
    translate box1_translated = translate_object((hittable*)(&box1_rotated), (vec3){(265 + 82.5) / 2.0, 165, -(295 + 82.5)}, (material*)(&white));
    // // Box 2
    box* box2 = box_create((point3){-82.5, -82.5, -82.5}, (point3){82.5, 82.5, 82.5}, 0);
    rotate box2_rotated = rotate_object((hittable*)box2, (vec3){0.0, 1.0, 0.0}, DEG_TO_RAD(-18), 0);
    translate box2_translated = translate_object((hittable*)(&box2_rotated), (vec3){265 + 82.5, 82.5, -(295 + 82.5) / 2.0}, (material*)(&white));

    hittable** world_array = darray_create(hittable*);

    darray_push_back_hittable_ptr(world_array, q1);
    darray_push_back_hittable_ptr(world_array, q2);
    darray_push_back_hittable_ptr(world_array, q3);
    darray_push_back_hittable_ptr(world_array, q4);
    darray_push_back_hittable_ptr(world_array, q5);
    darray_push_back_hittable_ptr(world_array, q6_light);
    darray_push_back_hittable_ptr(world_array, box1_translated);
    darray_push_back_hittable_ptr(world_array, box2_translated);

    hittable* bvh = bvh_create(world_array);

    hittable_list* world = hittable_list_create();
    hittable_list_add(world, bvh);

    camera* cam = camera_create(800, 800);
    camera_render(cam, world, image_name, 45, (vec3){278, 278, 1500}, (vec3){278, 278, 0}, (vec3){0, 1, 0}, 128, 64, background_black);

    camera_destroy(cam);
    hittable_list_destroy(world);
    box_destroy(box1);
    box_destroy(box2);
    bvh_destroy(bvh);
    darray_destroy(world_array);
}

void scene_render(const char* file_name) {
    // scene_three_lambertian_spheres(file_name);
    // scene_three_metal_spheres(file_name);
    // scene_three_dielectric_spheres(file_name);
    // scene_three_lambertian_quad(file_name);
    // scene_three_metal_quad(file_name);
    // scene_three_dielectric_quad(file_name);
    // scene_three_lambertian_circle(file_name);
    // scene_three_metal_circle(file_name);
    // scene_three_dielectric_circle(file_name);
    // scene_lights(file_name);
    // scene_cornell(file_name);
    scene_10000_rays(file_name);
}

#endif