#ifndef _MC_OBJ_H_
#define _MC_OBJ_H_

/************
 * Includes
 ************/
#include <stdint.h>
#include <stdlib.h>
#include "linked_list.h"

/************
 * Typedefs
 ************/
enum{
    black = 0,
    white = 1
};
typedef uint8_t mcColor_t;

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
    struct mcObj * align_ref_obj;
    mcAlign_t align_type;
    int16_t x, y;
    int16_t w, h;
    mcColor_t color;
}mcGeo_t;

typedef void(*mcDraw_cb_t)(struct mcObj* obj);
typedef void(*mcDelete_cb_t)(struct mcObj* obj);

typedef struct mcObj{
    struct mcObj * scr;
    struct mcObj * parent;
    list_t child_list;

    mcGeo_t geom;

    mcDraw_cb_t drawToBuffer_cb;
    mcDelete_cb_t delete_cb;
    _Bool refreshBuffer;

    void * obj_data; // points to an object specific structure
}mcObj_t;

/**********************
 * Function prototypes
 **********************/
 /* @brief: creates an object, but the data struct
  * is not malloc'd.
  * @parent: pointer to its parent obj
  * @scr: pointer to the desired screen
  * @return: a pointer to the created obj */
mcObj_t* mcObj_create(mcObj_t *parent, mcObj_t *scr);

/*
 * @brief: Not only deletes an object from the proper screen child_list, but
 * also removes all the objects found in the object child_list.
 * @obj: pointer to the object to be deleted
 */
void mcObj_delete(mcObj_t * obj);

/* @brief: aligns one object respect to another one
 * @obj: object to be aligned
 * @alignObj: reference object used to align obj
 * @align: predefined alignment
 * @x_offset: given a predefined alignment, "obj" can be moved along
 * x axis using this variable
 * @y_offset: given a predefined alignment, "obj" can be moved along
 * y axis using this variable */
void mcObj_align(mcObj_t * obj, mcObj_t * align_obj, mcAlign_t alignment_type, int16_t x_offset, int16_t y_offset);

#endif
