#include "logger.h"
#include "world.h"
#include "sphere.h"
#include "sampler.h"

#define SHAPES_COUNT 1

void random_100_spp() {
    struct sphere sphere = sphere_make(40, (point3f){.x = 0.0f, .y = 0.0f, .z = -50.0f});
    // struct plane plane = plane_make((normal3f){.x = 1.0f, .y = 0.0f, .z = -1.0f}, (point3f){.x = 0.0f, .y = 0.0f, .z = -10.0f});

    struct shape* shapes[SHAPES_COUNT] = {
        (struct shape*)(&sphere),
    };

    struct sampler sampler = sampler_create_random(100);

    struct world world = {
        .shapes = shapes,
        .shapes_count = SHAPES_COUNT,
        .hres = 512,
        .vres = 512,
        .pixel_scale = 0.16f,
        .sampler = &sampler,
        .filename = "random_100_spp.ppm",
    };

    world_render(&world, (color3f){.r = 0.0f, .g = 0.f, .b = 0.0f});

    sampler_destroy(&sampler);
}

void random_9_spp() {
    struct sphere sphere = sphere_make(40, (point3f){.x = 0.0f, .y = 0.0f, .z = -50.0f});
    // struct plane plane = plane_make((normal3f){.x = 1.0f, .y = 0.0f, .z = -1.0f}, (point3f){.x = 0.0f, .y = 0.0f, .z = -10.0f});

    struct shape* shapes[SHAPES_COUNT] = {
        (struct shape*)(&sphere),
    };

    struct sampler sampler = sampler_create_random(9);

    struct world world = {
        .shapes = shapes,
        .shapes_count = SHAPES_COUNT,
        .hres = 512,
        .vres = 512,
        .pixel_scale = 0.16f,
        .sampler = &sampler,
        .filename = "random_9_spp.ppm",
    };

    world_render(&world, (color3f){.r = 0.0f, .g = 0.f, .b = 0.0f});

    sampler_destroy(&sampler);
}

void regular_100_spp() {
    struct sphere sphere = sphere_make(40, (point3f){.x = 0.0f, .y = 0.0f, .z = -50.0f});
    // struct plane plane = plane_make((normal3f){.x = 1.0f, .y = 0.0f, .z = -1.0f}, (point3f){.x = 0.0f, .y = 0.0f, .z = -10.0f});

    struct shape* shapes[SHAPES_COUNT] = {
        (struct shape*)(&sphere),
    };

    struct sampler sampler = sampler_create_regular(100);

    struct world world = {
        .shapes = shapes,
        .shapes_count = SHAPES_COUNT,
        .hres = 512,
        .vres = 512,
        .pixel_scale = 0.16f,
        .sampler = &sampler,
        .filename = "regular_100_spp.ppm",
    };

    world_render(&world, (color3f){.r = 0.0f, .g = 0.f, .b = 0.0f});

    sampler_destroy(&sampler);
}

void regular_9_spp() {
    struct sphere sphere = sphere_make(40, (point3f){.x = 0.0f, .y = 0.0f, .z = -50.0f});
    // struct plane plane = plane_make((normal3f){.x = 1.0f, .y = 0.0f, .z = -1.0f}, (point3f){.x = 0.0f, .y = 0.0f, .z = -10.0f});

    struct shape* shapes[SHAPES_COUNT] = {
        (struct shape*)(&sphere),
    };

    struct sampler sampler = sampler_create_regular(9);

    struct world world = {
        .shapes = shapes,
        .shapes_count = SHAPES_COUNT,
        .hres = 512,
        .vres = 512,
        .pixel_scale = 0.16f,
        .sampler = &sampler,
        .filename = "regular_9_spp.ppm",
    };

    world_render(&world, (color3f){.r = 0.0f, .g = 0.f, .b = 0.0f});

    sampler_destroy(&sampler);
}

void jittered_100_spp() {
    struct sphere sphere = sphere_make(40, (point3f){.x = 0.0f, .y = 0.0f, .z = -50.0f});
    // struct plane plane = plane_make((normal3f){.x = 1.0f, .y = 0.0f, .z = -1.0f}, (point3f){.x = 0.0f, .y = 0.0f, .z = -10.0f});

    struct shape* shapes[SHAPES_COUNT] = {
        (struct shape*)(&sphere),
    };

    struct sampler sampler = sampler_create_jittered(100);

    struct world world = {
        .shapes = shapes,
        .shapes_count = SHAPES_COUNT,
        .hres = 512,
        .vres = 512,
        .pixel_scale = 0.16f,
        .sampler = &sampler,
        .filename = "jittered_100_spp.ppm",
    };

    world_render(&world, (color3f){.r = 0.0f, .g = 0.f, .b = 0.0f});

    sampler_destroy(&sampler);
}

void jittered_9_spp() {
    struct sphere sphere = sphere_make(40, (point3f){.x = 0.0f, .y = 0.0f, .z = -50.0f});
    // struct plane plane = plane_make((normal3f){.x = 1.0f, .y = 0.0f, .z = -1.0f}, (point3f){.x = 0.0f, .y = 0.0f, .z = -10.0f});

    struct shape* shapes[SHAPES_COUNT] = {
        (struct shape*)(&sphere),
    };

    struct sampler sampler = sampler_create_jittered(9);

    struct world world = {
        .shapes = shapes,
        .shapes_count = SHAPES_COUNT,
        .hres = 512,
        .vres = 512,
        .pixel_scale = 0.16f,
        .sampler = &sampler,
        .filename = "jittered_9_spp.ppm",
    };

    world_render(&world, (color3f){.r = 0.0f, .g = 0.f, .b = 0.0f});

    sampler_destroy(&sampler);
}

void n_rooks_100_spp() {
    struct sphere sphere = sphere_make(40, (point3f){.x = 0.0f, .y = 0.0f, .z = -50.0f});
    // struct plane plane = plane_make((normal3f){.x = 1.0f, .y = 0.0f, .z = -1.0f}, (point3f){.x = 0.0f, .y = 0.0f, .z = -10.0f});

    struct shape* shapes[SHAPES_COUNT] = {
        (struct shape*)(&sphere),
    };

    struct sampler sampler = sampler_create_n_rooks(100);

    struct world world = {
        .shapes = shapes,
        .shapes_count = SHAPES_COUNT,
        .hres = 512,
        .vres = 512,
        .pixel_scale = 0.16f,
        .sampler = &sampler,
        .filename = "n_rooks_100_spp.ppm",
    };

    world_render(&world, (color3f){.r = 0.0f, .g = 0.f, .b = 0.0f});

    sampler_destroy(&sampler);
}

void n_rooks_9_spp() {
    struct sphere sphere = sphere_make(40, (point3f){.x = 0.0f, .y = 0.0f, .z = -50.0f});
    // struct plane plane = plane_make((normal3f){.x = 1.0f, .y = 0.0f, .z = -1.0f}, (point3f){.x = 0.0f, .y = 0.0f, .z = -10.0f});

    struct shape* shapes[SHAPES_COUNT] = {
        (struct shape*)(&sphere),
    };

    struct sampler sampler = sampler_create_n_rooks(9);

    struct world world = {
        .shapes = shapes,
        .shapes_count = SHAPES_COUNT,
        .hres = 512,
        .vres = 512,
        .pixel_scale = 0.16f,
        .sampler = &sampler,
        .filename = "n_rooks_9_spp.ppm",
    };

    world_render(&world, (color3f){.r = 0.0f, .g = 0.f, .b = 0.0f});

    sampler_destroy(&sampler);
}

void multi_jittered_100_spp() {
    struct sphere sphere = sphere_make(40, (point3f){.x = 0.0f, .y = 0.0f, .z = -50.0f});
    // struct plane plane = plane_make((normal3f){.x = 1.0f, .y = 0.0f, .z = -1.0f}, (point3f){.x = 0.0f, .y = 0.0f, .z = -10.0f});

    struct shape* shapes[SHAPES_COUNT] = {
        (struct shape*)(&sphere),
    };

    struct sampler sampler = sampler_create_multi_jittered(100);

    struct world world = {
        .shapes = shapes,
        .shapes_count = SHAPES_COUNT,
        .hres = 512,
        .vres = 512,
        .pixel_scale = 0.16f,
        .sampler = &sampler,
        .filename = "multi_jittered_100_spp.ppm",
    };

    world_render(&world, (color3f){.r = 0.0f, .g = 0.f, .b = 0.0f});

    sampler_destroy(&sampler);
}

void multi_jittered_9_spp() {
    struct sphere sphere = sphere_make(40, (point3f){.x = 0.0f, .y = 0.0f, .z = -50.0f});
    // struct plane plane = plane_make((normal3f){.x = 1.0f, .y = 0.0f, .z = -1.0f}, (point3f){.x = 0.0f, .y = 0.0f, .z = -10.0f});

    struct shape* shapes[SHAPES_COUNT] = {
        (struct shape*)(&sphere),
    };

    struct sampler sampler = sampler_create_multi_jittered(9);

    struct world world = {
        .shapes = shapes,
        .shapes_count = SHAPES_COUNT,
        .hres = 512,
        .vres = 512,
        .pixel_scale = 0.16f,
        .sampler = &sampler,
        .filename = "multi_jittered_9_spp.ppm",
    };

    world_render(&world, (color3f){.r = 0.0f, .g = 0.f, .b = 0.0f});

    sampler_destroy(&sampler);
}

void hammersley_100_spp() {
    struct sphere sphere = sphere_make(40, (point3f){.x = 0.0f, .y = 0.0f, .z = -50.0f});
    // struct plane plane = plane_make((normal3f){.x = 1.0f, .y = 0.0f, .z = -1.0f}, (point3f){.x = 0.0f, .y = 0.0f, .z = -10.0f});

    struct shape* shapes[SHAPES_COUNT] = {
        (struct shape*)(&sphere),
    };

    struct sampler sampler = sampler_create_hammersley(100);

    struct world world = {
        .shapes = shapes,
        .shapes_count = SHAPES_COUNT,
        .hres = 512,
        .vres = 512,
        .pixel_scale = 0.16f,
        .sampler = &sampler,
        .filename = "hammersley_100_spp.ppm",
    };

    world_render(&world, (color3f){.r = 0.0f, .g = 0.f, .b = 0.0f});

    sampler_destroy(&sampler);
}

void hammersley_9_spp() {
    struct sphere sphere = sphere_make(40, (point3f){.x = 0.0f, .y = 0.0f, .z = -50.0f});
    // struct plane plane = plane_make((normal3f){.x = 1.0f, .y = 0.0f, .z = -1.0f}, (point3f){.x = 0.0f, .y = 0.0f, .z = -10.0f});

    struct shape* shapes[SHAPES_COUNT] = {
        (struct shape*)(&sphere),
    };

    struct sampler sampler = sampler_create_hammersley(9);

    struct world world = {
        .shapes = shapes,
        .shapes_count = SHAPES_COUNT,
        .hres = 512,
        .vres = 512,
        .pixel_scale = 0.16f,
        .sampler = &sampler,
        .filename = "hammersley_9_spp.ppm",
    };

    world_render(&world, (color3f){.r = 0.0f, .g = 0.f, .b = 0.0f});

    sampler_destroy(&sampler);
}

int main() {

    random_seed(17);
    LOGE("raytracer");
    LOGW("raytracer");
    LOGI("raytracer");
    LOGD("raytracer");
    LOGT("raytracer");

    random_100_spp();
    random_9_spp();
    regular_100_spp();
    regular_9_spp();
    jittered_100_spp();
    jittered_9_spp();
    n_rooks_100_spp();
    n_rooks_9_spp();
    multi_jittered_100_spp();
    multi_jittered_9_spp();
    hammersley_100_spp();
    hammersley_9_spp();

    if (memory != 0) {
        LOGE("memory leak %u bytes", memory);
        ASSERT(0);
    }

    return 0;
}