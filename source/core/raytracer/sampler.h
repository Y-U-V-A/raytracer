#ifndef SAMPLER__H
#define SAMPLER__H

#include "geometry.h"

enum sampling_method {
    RANDOM,
    REGULAR,
    JITTERED,
    MULTI_JITTERED,
    N_ROOKS,
    HAMMERSLEY,
};

#define NUM_SETS 83

struct sampler {
    point2f* samples;
    point2f* disk_samples;
    point3f* hemisphere_samples;
    i32* shuffled_indices;
    i32 num_samples;
    i32 count;
    i32 jump;
};

// NOTE: these functions are not thread safe
INLINE point2f sampler_sample_unit_square(struct sampler* sampler) {
    if (sampler->count % sampler->num_samples == 0) {
        sampler->jump = (random_i32() % NUM_SETS) * sampler->num_samples;
        sampler->count = 0;
    }
    return sampler->samples[(sampler->jump) + sampler->shuffled_indices[sampler->jump + sampler->count++]];
}

INLINE point2f sampler_sample_unit_disk(struct sampler* sampler) {
    if (sampler->count % sampler->num_samples == 0) {
        sampler->jump = (random_i32() % NUM_SETS) * sampler->num_samples;
        sampler->count = 0;
    }
    return sampler->disk_samples[(sampler->jump) + sampler->shuffled_indices[sampler->jump + sampler->count++]];
}

INLINE point3f sampler_sample_unit_hemisphere(struct sampler* sampler) {
    if (sampler->count % sampler->num_samples == 0) {
        sampler->jump = (random_i32() % NUM_SETS) * sampler->num_samples;
        sampler->count = 0;
    }
    return sampler->hemisphere_samples[(sampler->jump) + sampler->shuffled_indices[sampler->jump + sampler->count++]];
}

struct sampler sampler_create(i32 num_samples, enum sampling_method);

void sampler_destroy(struct sampler*);

void sampler_map_samples_to_unit_disk(struct sampler*);

void sampler_map_samples_to_hemisphere(struct sampler*, f32 e);

#endif