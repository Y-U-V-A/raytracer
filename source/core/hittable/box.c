#include "box.h"
#include "darray.h"
#include "zmemory.h"
#include "bvh.h"
#include "logger.h"

////////////////////////////////////
//  __                            //
// /  |                           //
// $$ |____    ______   __    __  //
// $$      \  /      \ /  \  /  | //
// $$$$$$$  |/$$$$$$  |$$  \/$$/  //
// $$ |  $$ |$$ |  $$ | $$  $$<   //
// $$ |__$$ |$$ \__$$ | /$$$$  \  //
// $$    $$/ $$    $$/ /$$/ $$  | //
// $$$$$$$/   $$$$$$/  $$/   $$/  //
//                                //
////////////////////////////////////

box* box_create(point3 point1, point3 point2, material* mat) {

    if (DOUBLE_COMPARE(point1.x, point2.x) || DOUBLE_COMPARE(point1.y, point2.y) || DOUBLE_COMPARE(point1.z, point2.z)) {
        LOGE("box_create : invalid params");
        return 0;
    }

    aabb bounds = aabb_create(point1, point2);
    vec3 dx = (vec3){bounds.x_range.max - bounds.x_range.min, 0, 0};
    vec3 dy = (vec3){0, bounds.y_range.max - bounds.y_range.min, 0};
    vec3 dz = (vec3){0, 0, bounds.z_range.max - bounds.z_range.min};

    box* temp = zmemory_allocate(sizeof(box));
    temp->base.box = bounds;
    temp->base.hit = box_hit;
    temp->front = zmemory_allocate(sizeof(quad));
    temp->back = zmemory_allocate(sizeof(quad));
    temp->left = zmemory_allocate(sizeof(quad));
    temp->right = zmemory_allocate(sizeof(quad));
    temp->top = zmemory_allocate(sizeof(quad));
    temp->bottom = zmemory_allocate(sizeof(quad));
    temp->darray = darray_create(hittable*);

    /// get the faces of box
    *(temp->front) = quad_create((point3){bounds.x_range.min, bounds.y_range.min, bounds.z_range.max}, dx, dy, mat);
    *(temp->back) = quad_create((point3){bounds.x_range.min, bounds.y_range.min, bounds.z_range.min}, dx, dy, mat);
    *(temp->left) = quad_create((point3){bounds.x_range.max, bounds.y_range.min, bounds.z_range.min}, dz, dy, mat);
    *(temp->right) = quad_create((point3){bounds.x_range.min, bounds.y_range.min, bounds.z_range.min}, dz, dy, mat);
    *(temp->top) = quad_create((point3){bounds.x_range.min, bounds.y_range.max, bounds.z_range.min}, dz, dx, mat);
    *(temp->bottom) = quad_create((point3){bounds.x_range.min, bounds.y_range.min, bounds.z_range.min}, dz, dx, mat);

    temp->darray = _darray_push_back(temp->darray, &temp->front);
    temp->darray = _darray_push_back(temp->darray, &temp->back);
    temp->darray = _darray_push_back(temp->darray, &temp->left);
    temp->darray = _darray_push_back(temp->darray, &temp->right);
    temp->darray = _darray_push_back(temp->darray, &temp->top);
    temp->darray = _darray_push_back(temp->darray, &temp->bottom);

    temp->bvh = bvh_create(temp->darray);

    return temp;
}

box* box_create_different_material(point3 point1, point3 point2,
                                   material* front, material* back,
                                   material* left, material* right,
                                   material* top, material* bottom) {

    if (DOUBLE_COMPARE(point1.x, point2.x) || DOUBLE_COMPARE(point1.y, point2.y) || DOUBLE_COMPARE(point1.z, point2.z) ||
        front == 0 || left == 0 || top == 0 || back == 0 || right == 0 || bottom == 0) {
        LOGE("box_create : invalid params");
        return 0;
    }

    aabb bounds = aabb_create(point1, point2);
    vec3 dx = (vec3){bounds.x_range.max - bounds.x_range.min, 0, 0};
    vec3 dy = (vec3){0, bounds.y_range.max - bounds.y_range.min, 0};
    vec3 dz = (vec3){0, 0, bounds.z_range.max - bounds.z_range.min};

    box* temp = zmemory_allocate(sizeof(box));
    temp->base.box = bounds;
    temp->base.hit = box_hit;
    temp->front = zmemory_allocate(sizeof(quad));
    temp->back = zmemory_allocate(sizeof(quad));
    temp->left = zmemory_allocate(sizeof(quad));
    temp->right = zmemory_allocate(sizeof(quad));
    temp->top = zmemory_allocate(sizeof(quad));
    temp->bottom = zmemory_allocate(sizeof(quad));
    temp->darray = darray_create(hittable*);

    /// get the faces of box
    *(temp->front) = quad_create((point3){bounds.x_range.min, bounds.y_range.min, bounds.z_range.max}, dx, dy, front);
    *(temp->back) = quad_create((point3){bounds.x_range.min, bounds.y_range.min, bounds.z_range.min}, dx, dy, back);
    *(temp->left) = quad_create((point3){bounds.x_range.max, bounds.y_range.min, bounds.z_range.min}, dz, dy, left);
    *(temp->right) = quad_create((point3){bounds.x_range.min, bounds.y_range.min, bounds.z_range.min}, dz, dy, right);
    *(temp->top) = quad_create((point3){bounds.x_range.min, bounds.y_range.max, bounds.z_range.min}, dz, dx, top);
    *(temp->bottom) = quad_create((point3){bounds.x_range.min, bounds.y_range.min, bounds.z_range.min}, dz, dx, bottom);

    temp->darray = _darray_push_back(temp->darray, &temp->front);
    temp->darray = _darray_push_back(temp->darray, &temp->back);
    temp->darray = _darray_push_back(temp->darray, &temp->left);
    temp->darray = _darray_push_back(temp->darray, &temp->right);
    temp->darray = _darray_push_back(temp->darray, &temp->top);
    temp->darray = _darray_push_back(temp->darray, &temp->bottom);

    temp->bvh = bvh_create(temp->darray);

    return temp;
}

void box_destroy(box* box_object) {
    if (box_object == 0) {
        LOGE("box_destroy: invalid params");
    }
    bvh_destroy(box_object->bvh);
    darray_destroy(box_object->darray);
    zmemory_free(box_object->front, sizeof(quad));
    zmemory_free(box_object->back, sizeof(quad));
    zmemory_free(box_object->left, sizeof(quad));
    zmemory_free(box_object->right, sizeof(quad));
    zmemory_free(box_object->top, sizeof(quad));
    zmemory_free(box_object->bottom, sizeof(quad));
    zmemory_free(box_object, sizeof(box));
}