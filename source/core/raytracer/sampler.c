#include "sampler.h"
#include "memory.h"
#include "logger.h"

/**
 *    ███████  █████  ███    ███ ██████  ██      ███████ ██████
 *    ██      ██   ██ ████  ████ ██   ██ ██      ██      ██   ██
 *    ███████ ███████ ██ ████ ██ ██████  ██      █████   ██████
 *         ██ ██   ██ ██  ██  ██ ██      ██      ██      ██   ██
 *    ███████ ██   ██ ██      ██ ██      ███████ ███████ ██   ██
 *
 *
 */

void sampler_setup_shuffled_indices(struct sampler* sampler) {
    i32* indices = malloc(sizeof(i32) * sampler->num_samples);
    for (i32 i = 0; i < sampler->num_samples; ++i) {
        indices[i] = i;
    }
    for (i32 set = 0; set < NUM_SETS; ++set) {
        for (i32 i = sampler->num_samples - 1; i >= 0; --i) {
            i32 idx = random_i32() % (i + 1);
            i32 tmp = indices[i];
            indices[i] = indices[idx];
            indices[idx] = tmp;
        }
        for (i32 i = 0; i < sampler->num_samples; ++i) {
            sampler->shuffled_indices[i + set * sampler->num_samples] = indices[i];
        }
    }
    free(indices, sizeof(i32) * sampler->num_samples);
}

void sampler_destroy(struct sampler* sampler) {
    free(sampler->samples, (sizeof(point2f) + sizeof(i32)) * sampler->num_samples * NUM_SETS);
}

/**
 *    ██████   █████  ███    ██ ██████   ██████  ███    ███
 *    ██   ██ ██   ██ ████   ██ ██   ██ ██    ██ ████  ████
 *    ██████  ███████ ██ ██  ██ ██   ██ ██    ██ ██ ████ ██
 *    ██   ██ ██   ██ ██  ██ ██ ██   ██ ██    ██ ██  ██  ██
 *    ██   ██ ██   ██ ██   ████ ██████   ██████  ██      ██
 *
 *
 */

void generate_samples_random(struct sampler* sampler) {
    for (i32 set = 0; set < NUM_SETS; ++set) {
        for (i32 i = 0; i < sampler->num_samples; ++i) {
            sampler->samples[i + set * sampler->num_samples].x = random_f32();
            sampler->samples[i + set * sampler->num_samples].y = random_f32();
        }
    }
}

struct sampler sampler_create_random(i32 num_samples) {
    void* block = malloc((sizeof(point2f) + sizeof(i32)) * num_samples * NUM_SETS);
    struct sampler sampler = {
        .samples = (point2f*)block,
        .shuffled_indices = (i32*)(sizeof(point2f) * num_samples * NUM_SETS + (u8*)block),
        .num_samples = num_samples,
        .count = 0,
        .jump = 0,
    };
    generate_samples_random(&sampler);
    sampler_setup_shuffled_indices(&sampler);
    return sampler;
}

/**
 *    ██████  ███████  ██████  ██    ██ ██       █████  ██████
 *    ██   ██ ██      ██       ██    ██ ██      ██   ██ ██   ██
 *    ██████  █████   ██   ███ ██    ██ ██      ███████ ██████
 *    ██   ██ ██      ██    ██ ██    ██ ██      ██   ██ ██   ██
 *    ██   ██ ███████  ██████   ██████  ███████ ██   ██ ██   ██
 *
 *
 */

void generate_samples_regular(struct sampler* sampler) {
    i32 n = (i32)sqrt_f32((f32)sampler->num_samples);
    ASSERT(n * n == sampler->num_samples);
    f32 inv_n = 1.0f / n;
    for (i32 set = 0; set < NUM_SETS; ++set) {
        for (i32 i = 0; i < n; ++i) {
            for (i32 j = 0; j < n; ++j) {
                sampler->samples[i * n + j + set * sampler->num_samples].x = (j + 0.5f) * inv_n;
                sampler->samples[i * n + j + set * sampler->num_samples].y = (i + 0.5f) * inv_n;
            }
        }
    }
}

struct sampler sampler_create_regular(i32 num_samples) {
    void* block = malloc((sizeof(point2f) + sizeof(i32)) * num_samples * NUM_SETS);
    struct sampler sampler = {
        .samples = (point2f*)block,
        .shuffled_indices = (i32*)(sizeof(point2f) * num_samples * NUM_SETS + (u8*)block),
        .num_samples = num_samples,
        .count = 0,
        .jump = 0,
    };
    generate_samples_regular(&sampler);
    sampler_setup_shuffled_indices(&sampler);
    return sampler;
}

/**
 *         ██ ██ ████████ ████████ ███████ ██████  ███████ ██████
 *         ██ ██    ██       ██    ██      ██   ██ ██      ██   ██
 *         ██ ██    ██       ██    █████   ██████  █████   ██   ██
 *    ██   ██ ██    ██       ██    ██      ██   ██ ██      ██   ██
 *     █████  ██    ██       ██    ███████ ██   ██ ███████ ██████
 *
 *
 */

void generate_samples_jittered(struct sampler* sampler) {
    i32 n = (i32)sqrt_f32((f32)sampler->num_samples);
    ASSERT(n * n == sampler->num_samples);
    f32 inv_n = 1.0f / (f32)n;
    for (i32 set = 0; set < NUM_SETS; ++set) {
        for (i32 i = 0; i < n; ++i) {
            for (i32 j = 0; j < n; ++j) {
                sampler->samples[set * sampler->num_samples + i * n + j].x = (j + random_f32()) * inv_n;
                sampler->samples[set * sampler->num_samples + i * n + j].y = (i + random_f32()) * inv_n;
            }
        }
    }
}

struct sampler sampler_create_jittered(i32 num_samples) {
    void* block = malloc((sizeof(point2f) + sizeof(i32)) * num_samples * NUM_SETS);
    struct sampler sampler = {
        .samples = (point2f*)block,
        .shuffled_indices = (i32*)(sizeof(point2f) * num_samples * NUM_SETS + (u8*)block),
        .num_samples = num_samples,
        .count = 0,
        .jump = 0,
    };
    generate_samples_jittered(&sampler);
    sampler_setup_shuffled_indices(&sampler);
    return sampler;
}

/**
 *    ███    ███ ██    ██ ██      ████████ ██            ██ ██ ████████ ████████ ███████ ██████  ███████ ██████
 *    ████  ████ ██    ██ ██         ██    ██            ██ ██    ██       ██    ██      ██   ██ ██      ██   ██
 *    ██ ████ ██ ██    ██ ██         ██    ██ █████      ██ ██    ██       ██    █████   ██████  █████   ██   ██
 *    ██  ██  ██ ██    ██ ██         ██    ██       ██   ██ ██    ██       ██    ██      ██   ██ ██      ██   ██
 *    ██      ██  ██████  ███████    ██    ██        █████  ██    ██       ██    ███████ ██   ██ ███████ ██████
 *
 *
 */

void generate_samples_multi_jittered(struct sampler* sampler) {
    i32 n = (i32)sqrt_f32((f32)sampler->num_samples);
    ASSERT(n * n == sampler->num_samples);
    f32 inv_num_samples = 1.0f / sampler->num_samples;
    for (i32 set = 0; set < NUM_SETS; ++set) {
        for (i32 i = 0; i < n; ++i) {
            for (i32 j = 0; j < n; ++j) {
                sampler->samples[set * sampler->num_samples + i * n + j].x = (j * n + i + random_f32()) * inv_num_samples;
                sampler->samples[set * sampler->num_samples + i * n + j].y = (i * n + j + random_f32()) * inv_num_samples;
            }
        }
    }

    for (i32 set = 0; set < NUM_SETS; ++set) {
        for (i32 i = 0; i < n; ++i) {
            for (i32 j = 0; j < n; ++j) {
                i32 k = j + random_i32() % (n - j);
                f32 temp = sampler->samples[set * sampler->num_samples + i * n + j].x;
                sampler->samples[set * sampler->num_samples + i * n + j].x = sampler->samples[set * sampler->num_samples + i * n + k].x;
                sampler->samples[set * sampler->num_samples + i * n + k].x = temp;
            }
        }
    }

    for (i32 set = 0; set < NUM_SETS; ++set) {
        for (i32 i = 0; i < n; ++i) {
            for (i32 j = 0; j < n; ++j) {
                i32 k = j + random_i32() % (n - j);
                f32 temp = sampler->samples[set * sampler->num_samples + j * n + i].y;
                sampler->samples[set * sampler->num_samples + j * n + i].y = sampler->samples[set * sampler->num_samples + k * n + i].y;
                sampler->samples[set * sampler->num_samples + k * n + i].y = temp;
            }
        }
    }
}

struct sampler sampler_create_multi_jittered(i32 num_samples) {
    void* block = malloc((sizeof(point2f) + sizeof(i32)) * num_samples * NUM_SETS);
    struct sampler sampler = {
        .samples = (point2f*)block,
        .shuffled_indices = (i32*)(sizeof(point2f) * num_samples * NUM_SETS + (u8*)block),
        .num_samples = num_samples,
        .count = 0,
        .jump = 0,
    };
    generate_samples_multi_jittered(&sampler);
    sampler_setup_shuffled_indices(&sampler);
    return sampler;
}

/**
 *    ███    ██       ██████   ██████   ██████  ██   ██ ███████
 *    ████   ██       ██   ██ ██    ██ ██    ██ ██  ██  ██
 *    ██ ██  ██ █████ ██████  ██    ██ ██    ██ █████   ███████
 *    ██  ██ ██       ██   ██ ██    ██ ██    ██ ██  ██       ██
 *    ██   ████       ██   ██  ██████   ██████  ██   ██ ███████
 *
 *
 */

void generate_samples_n_rooks(struct sampler* sampler) {
    f32 inv_num_samples = 1.0f / sampler->num_samples;
    for (i32 set = 0; set < NUM_SETS; ++set) {
        for (i32 i = 0; i < sampler->num_samples; ++i) {
            sampler->samples[i + set * sampler->num_samples].x = (i + random_f32()) * inv_num_samples;
            sampler->samples[i + set * sampler->num_samples].y = (i + random_f32()) * inv_num_samples;
        }
    }

    for (i32 set = 0; set < NUM_SETS; ++set) {
        for (i32 i = 0; i < sampler->num_samples; ++i) {
            i32 k = i + random_i32() % (sampler->num_samples - i);
            f32 temp = sampler->samples[i + set * sampler->num_samples].x;
            sampler->samples[i + set * sampler->num_samples].x = sampler->samples[k + set * sampler->num_samples].x;
            sampler->samples[k + set * sampler->num_samples].x = temp;
        }
    }

    for (i32 set = 0; set < NUM_SETS; ++set) {
        for (i32 i = 0; i < sampler->num_samples; ++i) {
            i32 k = i + random_i32() % (sampler->num_samples - i);
            f32 temp = sampler->samples[i + set * sampler->num_samples].y;
            sampler->samples[i + set * sampler->num_samples].y = sampler->samples[k + set * sampler->num_samples].y;
            sampler->samples[k + set * sampler->num_samples].y = temp;
        }
    }
}

struct sampler sampler_create_n_rooks(i32 num_samples) {
    void* block = malloc((sizeof(point2f) + sizeof(i32)) * num_samples * NUM_SETS);
    struct sampler sampler = {
        .samples = (point2f*)block,
        .shuffled_indices = (i32*)(sizeof(point2f) * num_samples * NUM_SETS + (u8*)block),
        .num_samples = num_samples,
        .count = 0,
        .jump = 0,
    };
    generate_samples_n_rooks(&sampler);
    sampler_setup_shuffled_indices(&sampler);
    return sampler;
}

/**
 *    ██   ██  █████  ███    ███ ███    ███ ███████ ██████  ███████ ██      ███████ ██    ██
 *    ██   ██ ██   ██ ████  ████ ████  ████ ██      ██   ██ ██      ██      ██       ██  ██
 *    ███████ ███████ ██ ████ ██ ██ ████ ██ █████   ██████  ███████ ██      █████     ████
 *    ██   ██ ██   ██ ██  ██  ██ ██  ██  ██ ██      ██   ██      ██ ██      ██         ██
 *    ██   ██ ██   ██ ██      ██ ██      ██ ███████ ██   ██ ███████ ███████ ███████    ██
 *
 *
 */

void generate_samples_hammersley(struct sampler* sampler) {
    f32 inv_num_samples = 1.0f / sampler->num_samples;
    for (i32 set = 0; set < NUM_SETS; ++set) {
        for (i32 i = 0; i < sampler->num_samples; ++i) {
            i32 j = i;
            f32 k = 0.5f;
            f32 y = 0;
            while (j) {
                if (j & 1) {
                    y += k;
                }
                k *= 0.5f;
                j >>= 1;
            }
            sampler->samples[set * sampler->num_samples + i].x = i * inv_num_samples;
            sampler->samples[set * sampler->num_samples + i].y = y;
        }
    }
}

struct sampler sampler_create_hammersley(i32 num_samples) {
    void* block = malloc((sizeof(point2f) + sizeof(i32)) * num_samples * NUM_SETS);
    struct sampler sampler = {
        .samples = (point2f*)block,
        .shuffled_indices = (i32*)(sizeof(point2f) * num_samples * NUM_SETS + (u8*)block),
        .num_samples = num_samples,
        .count = 0,
        .jump = 0,
    };
    generate_samples_hammersley(&sampler);
    sampler_setup_shuffled_indices(&sampler);
    return sampler;
}