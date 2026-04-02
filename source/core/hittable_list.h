#ifndef HITTABLE_LIST__H
#define HITTABLE_LIST__H

#include "defines.h"
#include "darray.h"
#include "hittable.h"
#include "zmemory.h"

typedef struct hittable_list {
    hittable** objects; // darray of hittable pointers
} hittable_list;

bool hittable_list_hit(hittable_list* list_object, ray* r, interval r_t, hit_record* record);

INLINE hittable_list* hittable_list_create() {
    hittable_list* list = zmemory_allocate(sizeof(hittable_list));
    list->objects = darray_create(hittable*);
    return list;
}

INLINE void hittable_list_destroy(hittable_list* list) {
    darray_destroy(list->objects);
    zmemory_free(list, sizeof(hittable_list));
}

INLINE void hittable_list_add(hittable_list* list, hittable* object) {
    darray_push_back(list->objects, object);
}

#endif