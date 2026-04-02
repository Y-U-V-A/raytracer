
#include "bvh.h"
#include "zmemory.h"
#include "logger.h"
#include "darray.h"

/////////////////////////////////////
//  __                   __        //
// /  |                 /  |       //
// $$ |____   __     __ $$ |____   //
// $$      \ /  \   /  |$$      \  //
// $$$$$$$  |$$  \ /$$/ $$$$$$$  | //
// $$ |  $$ | $$  /$$/  $$ |  $$ | //
// $$ |__$$ |  $$ $$/   $$ |  $$ | //
// $$    $$/    $$$/    $$ |  $$ | //
// $$$$$$$/      $/     $$/   $$/  //
//                                 //
/////////////////////////////////////

typedef struct bvh {
    hittable base;
    hittable* left;
    hittable* right;
    bool is_leaf;
} bvh;
// global
hittable* create_bvh(hittable** darray, i32 start, i32 end);
void destroy_bvh(hittable* node);

hittable* bvh_create(hittable** darray) {
    if (darray == 0) {
        LOGE("bvh_create: invalid params ");
        return 0;
    }

    return create_bvh(darray, 0, darray_length(darray));
}

void bvh_destroy(hittable* bvh) {
    destroy_bvh(bvh);
}

bool bvh_hit(hittable* bvh_object, ray* r_in, interval r_t, hit_record* record) {
    if (!aabb_hit(&bvh_object->box, r_in, r_t)) {
        return false;
    }
    bvh* volume = (bvh*)bvh_object;
    bool hit_left = 0;
    bool hit_right = 0;
    if (volume->left != 0) {
        hit_left = volume->left->hit(volume->left, r_in, r_t, record);
    }
    if (volume->right != 0) {
        hit_right = volume->right->hit(volume->right, r_in, (interval){r_t.min, (hit_left ? record->t : r_t.max)}, record);
    }
    return hit_left || hit_right;
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

int cmp_x_axis(const void* a, const void* b) {
    const hittable* aa = *(const hittable**)a; // Dereference the pointer
    const hittable* bb = *(const hittable**)b; // Dereference the pointer
    if (aa->box.x_range.min < bb->box.x_range.min)
        return -1;
    if (aa->box.x_range.min > bb->box.x_range.min)
        return 1;
    return 0;
}
int cmp_y_axis(const void* a, const void* b) {
    const hittable* aa = *(const hittable**)a; // Dereference the pointer
    const hittable* bb = *(const hittable**)b; // Dereference the pointer
    if (aa->box.y_range.min < bb->box.y_range.min)
        return -1;
    if (aa->box.y_range.min > bb->box.y_range.min)
        return 1;
    return 0;
}
int cmp_z_axis(const void* a, const void* b) {
    const hittable* aa = *(const hittable**)a; // Dereference the pointer
    const hittable* bb = *(const hittable**)b; // Dereference the pointer
    if (aa->box.z_range.min < bb->box.z_range.min)
        return -1;
    if (aa->box.z_range.min > bb->box.z_range.min)
        return 1;
    return 0;
}

hittable* create_bvh(hittable** darray, i32 start, i32 end) {

    bvh* temp = zmemory_allocate(sizeof(bvh));
    temp->base.hit = bvh_hit;
    temp->base.box = aabb_create_empty();
    temp->left = 0;
    temp->right = 0;
    temp->is_leaf = 0;
    for (i32 i = start; i < end; ++i) {
        temp->base.box = aabb_merge(darray[i]->box, temp->base.box);
    }

    i32 longest_axis = aabb_longest_axis(&temp->base.box);
    qsort_cmp cmp = (longest_axis == 0 ? cmp_x_axis : ((longest_axis == 1) ? cmp_y_axis : cmp_z_axis));
    i32 object_span = end - start;

    if (object_span == 1) {
        temp->left = darray[start];
        temp->is_leaf = 1;
    } else if (object_span == 2) {
        temp->left = darray[start];
        temp->right = darray[start + 1];
        temp->is_leaf = 1;
    } else {
        quick_sort(darray + start, object_span, sizeof(hittable*), cmp);
        i32 mid = start + object_span / 2;
        temp->left = create_bvh(darray, start, mid);
        temp->right = create_bvh(darray, mid, end);
    }
    return (hittable*)temp;
}

void destroy_bvh(hittable* node) {
    if (node == 0)
        return;
    bvh* temp = (bvh*)node;
    if (!temp->is_leaf) {
        destroy_bvh(temp->left);
        destroy_bvh(temp->right);
    }
    zmemory_free(temp, sizeof(bvh));
}
