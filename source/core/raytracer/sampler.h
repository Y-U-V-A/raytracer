#ifndef SAMPLER__H
#define SAMPLER__H

#include "defines.h"
#include "geometry.h"

#define NUM_SETS 83

struct sampler {
    point2f* samples;
    i32* shuffled_indices;
    i32 num_samples;
    i32 count;
    i32 jump;
};

INLINE point2f sampler_sample_unit_square(struct sampler* sampler) {
    // not shuffled
    // return sampler->samples[(sampler->count++) % (sampler->num_samples * NUM_SETS)];

    // shuffled sets
    // if (sampler->count % sampler->num_samples == 0) {
    //     sampler->jump = (random_i32() % NUM_SETS) * sampler->num_samples;
    //     sampler->count = 0;
    // }
    // return sampler->samples[(sampler->jump + sampler->count++)];

    // shuffled sets and shuffled points
    if (sampler->count % sampler->num_samples == 0) {
        sampler->jump = (random_i32() % NUM_SETS) * sampler->num_samples;
        sampler->count = 0;
    }
    return sampler->samples[(sampler->jump) + sampler->shuffled_indices[sampler->jump + sampler->count++]];
}

void sampler_destroy(struct sampler* sampler);

struct sampler sampler_create_random(i32 num_samples);

struct sampler sampler_create_regular(i32 num_samples);

struct sampler sampler_create_jittered(i32 num_samples);

struct sampler sampler_create_multi_jittered(i32 num_samples);

struct sampler sampler_create_n_rooks(i32 num_samples);

struct sampler sampler_create_hammersley(i32 num_samples);

#endif