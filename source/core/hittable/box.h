#ifndef BOX__H
#define BOX__H

#include "hittable.h"

/**
 * @brief box is a derived struct from base hittable which contains bvh which has all the sides
 *
 */

typedef struct box {
    hittable base;
    quad* front;
    quad* back;
    quad* left;
    quad* right;
    quad* top;
    quad* bottom;
    hittable** darray;
    hittable* bvh;
} box;

/**
 * @brief
 *
 * @param box_object pointer to starting addr of box{hittable{base},...}
 * @param r_in ray
 * @param r_t ray interval
 * @param out_record details like point, t,normal,u,v, are written into record
 * @return bool
 */
INLINE bool box_hit(hittable* box_object, ray* r_in, interval r_t, hit_record* out_record) {
    box* b = (box*)box_object;
    if (b->bvh->hit(b->bvh, r_in, r_t, out_record)) {
        return true;
    }
    return false;
}

/**
 * @brief box_create will create box based on the diagnoal points provided
 * it will create box with all sides included into a bvh
 *
 * @param point1 diagnoal point of box
 * @param point2 opposite diagnoal point of point1
 * @param mat type of box material
 * @return box*
 */
box* box_create(point3 point1, point3 point2, material* mat);

/**
 * @brief box_create will create box based on the diagnoal points provided and materials
 * it will create box with all sides included into a bvh
 *
 * @param point1 diagnoal point of box
 * @param point2 opposite diagnoal point of point2
 * @param front front face material
 * @param back back face material
 * @param left left face material
 * @param right right face material
 * @param top top face material
 * @param bottom bottom face material
 * @return box*
 */
box* box_create_different_material(point3 point1, point3 point2,
                                   material* front, material* back,
                                   material* left, material* right,
                                   material* top, material* bottom);

/**
 * @brief box_destroy free's the memory allocated
 *
 * @param box_object pointer to starting addr of the box
 */
void box_destroy(box* box_object);

#endif