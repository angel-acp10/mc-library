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

enum{
    nothing = 0, //nothing
    border,    //just border
    filled       //border + inside
};
typedef uint8_t mcShape_t;

enum{
    screen = 0,
    cartesian
};
typedef uint8_t mcCoord_t;

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
typedef uint8_t mcRelPos_t;

typedef struct{
    struct mcObj * ref_obj;
    mcRelPos_t pos;
    int16_t x_offs, y_offs;
}mcAlign_t;

typedef struct{
    mcAlign_t align;
    uint16_t x, y;
    int16_t w, h;
    mcCoord_t coord_type;
    mcColor_t color;
}mcGeo_t;


typedef void(*mcCb_drawBuffer_t)(struct mcObj* obj);
typedef void(*mcCb_deleteObjData_t)(struct mcObj* obj);
typedef void(*mcCb_preRender_t)(struct mcObj* obj);

typedef struct mcObj{
    struct mcObj * scr;
    struct mcObj * parent;

    mcGeo_t geom;

    //object specific data
    void * obj_data; // points to an object specific structure
    mcCb_deleteObjData_t delete_obj_data_cb;

    // child_list
    list_t child_list;

    //update pre-render
    _Bool prerender_flg;
    mcCb_preRender_t preRender_cb;
    
    //buffer
    mcCb_drawBuffer_t drawToBuffer_cb;

}mcObj_t;

/**********************
 * Function prototypes
 **********************/
  /**
  *  @brief: creates an object, but the data struct
  * is not malloc'd.
  * @parent: pointer to its parent obj
  * @scr: pointer to the desired screen
  * @return: a pointer to the created obj */
mcObj_t* mcObj_create(mcObj_t *parent, mcObj_t *scr);

/**
 * @brief: Not only deletes an object from the proper screen child_list, but
 * also removes all the objects found in the object child_list.
 * @obj: pointer to the object to be deleted
 */
void mcObj_delete(mcObj_t * obj);

/* @brief: set the alignment parameters to align one object to another one
 * @obj: object to be aligned
 * @align_obj: reference object used to align obj
 * @alignment_type: predefined alignment
 * @x_offset: given a predefined alignment, "obj" can be moved along
 * x axis using this variable
 * @y_offset: given a predefined alignment, "obj" can be moved along
 * y axis using this variable */
void mcObj_align(mcObj_t * obj, mcObj_t * align_obj, mcRelPos_t alignment_type,
    int16_t x_offset, int16_t y_offset);

/* @brief: aligns one object to another one (called by obj->preRender_cb)
 * @obj: object to be aligned with obj->geom.align.ref_obj 
 */ 
void mcObj_align_prerender(mcObj_t * obj);

/**
 * @brief: set top left x,y coordinates of an object
 * @param: x: x coordinate
 * @param: y: y coordinate
 */
void mcObj_setPos(mcObj_t * obj, uint16_t x, uint16_t y);

/**
 * @brief: set width and height of an object
 * @param: width
 * @param: height
 */
void mcObj_setDim(mcObj_t * obj, int16_t width, int16_t height);

/**
 * @brief: define an object position and dimmensions using two points
 * @param: point0: x0, y0
 * @param: point1: x1, y1
 */ 
void mcObj_set2Points(mcObj_t * obj, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

/**
 * @brief: set color of an object
 * @param: color
 */
void mcObj_setColor(mcObj_t * obj, mcColor_t color);

/**
 * @brief: select default screen coordinate or change to cartesian ones
 */ 
void mcObj_selectCoord(mcObj_t * obj, mcCoord_t coord_type);

#endif
