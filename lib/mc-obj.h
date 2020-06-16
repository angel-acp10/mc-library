#ifndef _MC_OBJ_H_
#define _MC_OBJ_H_

/************
 * Includes
 ************/
#include <stdint.h>
#include "linked_list.h"

/************
 * Typedefs
 ************/
enum {
    MC_ALIGN_CENTER = 0,
    MC_ALIGN_IN_TOP_LEFT,
    MC_ALIGN_IN_TOP_MID,
    MC_ALIGN_IN_TOP_RIGHT,
    MC_ALIGN_IN_BOTTOM_LEFT,
    MC_ALIGN_IN_BOTTOM_MID,
    MC_ALIGN_IN_BOTTOM_RIGHT,
    MC_ALIGN_IN_LEFT_MID,
    MC_ALIGN_IN_RIGHT_MID,
    MC_ALIGN_OUT_TOP_LEFT,
    MC_ALIGN_OUT_TOP_MID,
    MC_ALIGN_OUT_TOP_RIGHT,
    MC_ALIGN_OUT_BOTTOM_LEFT,
    MC_ALIGN_OUT_BOTTOM_MID,
    MC_ALIGN_OUT_BOTTOM_RIGHT,
    MC_ALIGN_OUT_LEFT_TOP,
    MC_ALIGN_OUT_LEFT_MID,
    MC_ALIGN_OUT_LEFT_BOTTOM,
    MC_ALIGN_OUT_RIGHT_TOP,
    MC_ALIGN_OUT_RIGHT_MID,
    MC_ALIGN_OUT_RIGHT_BOTTOM,
};
typedef uint8_t mcAlign_t;

typedef struct{
    struct mcObj * alignObj;
    int16_t x, y;
    int16_t w, h;
}mcGeo_t;

typedef void(*mcDraw_cb_t)(struct mcObj* obj);
typedef void(*mcDelete_cb_t)(struct mcObj* obj);

typedef struct mcObj{
    struct mcObj * scr;
    struct mcObj * parent;
    list_t child_list;

    mcGeo_t geom;
    mcAlign_t align;

    mcDraw_cb_t drawToBuffer_cb;
    mcDelete_cb_t delete_cb;
    _Bool refreshBuffer;

    void * data; // points to an object specific structure
}mcObj_t;

 /* @brief: creates an object, but the data struct
  * is not malloc'd.
  * @parent: pointer to its parent obj
  * @scr: pointer to the desired screen
  * @return: a pointer to the created obj */
mcObj_t* mcObj_create(mcObj_t *parent, mcObj_t *scr);

/* @brief: aligns one object respect to another one
 * @obj: object to be aligned
 * @alignObj: reference object used to align obj
 * @align: predefined alignment
 * @x_offset: given a predefined alignment, "obj" can be moved along
 * x axis using this variable
 * @y_offset: given a predefined alignment, "obj" can be moved along
 * y axis using this variable */
void mcObj_align(mcObj_t * obj, mcObj_t * align_obj, mcAlign_t align, int16_t x_offset, int16_t y_offset);



#endif