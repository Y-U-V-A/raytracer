#include "hittable_list.h"

bool hittable_list_hit(hittable_list* list, ray* r, interval r_t, hit_record* record) {
    hit_record temp_record;
    bool hit_anything = false;
    f64 closest_so_far = r_t.max;
    i32 size = darray_length(list->objects);
    for (i32 i = 0; i < size; i++) {
        if (list->objects[i]->hit(list->objects[i], r, (interval){r_t.min, closest_so_far}, &temp_record)) {
            hit_anything = true;
            closest_so_far = temp_record.t;
            *record = temp_record;
        }
    }
    return hit_anything;
}