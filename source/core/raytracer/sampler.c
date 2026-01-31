#include "sampler.h"
#include "memory.h"
#include "logger.h"

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

void sampler_map_samples_to_unit_disk(struct sampler* sampler) {
    for (i32 i = 0; i < sampler->num_samples * NUM_SETS; ++i) {
        point2f point = {
            .x = 2.0f * sampler->samples[i].x - 1.0f,
            .y = 2.0f * sampler->samples[i].y - 1.0f,
        };
        f32 radius;
        f32 phi;
        if (point.x > -point.y) {
            if (point.x > point.y) {
                radius = point.x;
                phi = point.y / point.x;
            } else {
                radius = point.y;
                phi = 2 - point.x / point.y;
            }
        } else {
            if (point.x < point.y) {
                radius = -point.x;
                phi = 4 + point.y / point.x;
            } else {
                radius = -point.y;
                if (point.y != 0.0f) {
                    phi = 6 - point.x / point.y;
                } else {
                    phi = 0.0f;
                }
            }
        }
        phi *= PI_OVER_4;
        sampler->disk_samples[i].x = radius * cos_f32(phi);
        sampler->disk_samples[i].y = radius * sin_f32(phi);
    }
}

// TODO:derive this formula
void sampler_map_samples_to_hemisphere(struct sampler* sampler, f32 e) {
    for (i32 i = 0; i < sampler->num_samples * NUM_SETS; ++i) {
        f32 cos_phi = cos_f32(2.0f * PI * sampler->samples[i].x);
        f32 sin_phi = sin_f32(2.0f * PI * sampler->samples[i].x);
        f32 cos_theta = pow_f32((1.0f - sampler->samples[i].y), 1.0f / (e + 1.0f));
        f32 sin_theta = sqrt_f32(1.0f - cos_theta * cos_theta);
        sampler->hemisphere_samples[i].x = sin_theta * cos_phi;
        sampler->hemisphere_samples[i].y = sin_theta * sin_phi;
        sampler->hemisphere_samples[i].z = cos_phi;
    }
}

struct sampler sampler_create(i32 num_samples, enum sampling_method method) {
    void* block = malloc((sizeof(point2f) + sizeof(point2f) + sizeof(point3f) + sizeof(i32)) * num_samples * NUM_SETS);
    struct sampler sampler = {
        .samples = (point2f*)block,
        .disk_samples = (point2f*)(sizeof(point2f) * num_samples * NUM_SETS + (u8*)block),
        .hemisphere_samples = (point3f*)((sizeof(point2f) + sizeof(point2f)) * num_samples * NUM_SETS + (u8*)block),
        .shuffled_indices = (i32*)((sizeof(point2f) + sizeof(point2f) + sizeof(point3f)) * num_samples * NUM_SETS + (u8*)block),
        .num_samples = num_samples,
        .count = 0,
        .jump = 0,
    };
    switch (method) {
    case RANDOM:
        generate_samples_random(&sampler);
        break;
    case REGULAR:
        generate_samples_regular(&sampler);
        break;
    case JITTERED:
        generate_samples_jittered(&sampler);
        break;
    case MULTI_JITTERED:
        generate_samples_multi_jittered(&sampler);
        break;
    case N_ROOKS:
        generate_samples_n_rooks(&sampler);
        break;
    case HAMMERSLEY:
        generate_samples_hammersley(&sampler);
        break;
    default:
        LOGE("sampler_create: invalid sampling_method");
        ASSERT(0);
    }
    sampler_setup_shuffled_indices(&sampler);
    return sampler;
}

void sampler_destroy(struct sampler* sampler) {
    free(sampler->samples, (sizeof(point2f) + sizeof(point2f) + sizeof(point3f) + sizeof(i32)) * sampler->num_samples * NUM_SETS);
}
