#ifndef _MC_SHAPES_H_
#define _MC_SHAPES_H_

/************
 * Includes
 ************/
#include "mc-obj.h"
#include "mc-draw.h"

/*****************************
 * Public function prototypes
 *****************************/ 
/**
 * @brief: creates a pixel object
 * @parent: parent object of the returned one.
 * @scr: screen where the returned object will be displayed.
 * @return: new pixel object
 */ 
mcObj_t * mcPix_create(mcObj_t *parent, mcObj_t *scr);

/**
 * @brief: creates a line object
 * @parent: parent object of the returned one.
 * @scr: screen where the returned object will be displayed.
 * @return: new line object
 */ 
mcObj_t * mcLine_create(mcObj_t * parent, mcObj_t * scr);

/**
 * @brief: creates a rectangle object
 * @parent: parent object of the returned one.
 * @scr: screen where the returned object will be displayed.
 * @return: new rectangle object
 */ 
mcObj_t * mcRect_create(mcObj_t *parent, mcObj_t *scr);

/**
 * @brief: set inside of a rectangle
 * @rec: rectangle object
 * @shape_type: parameter which can be:
 *      - nothing: not relevant for this function, same as border
 *      - border: just the rectangle contour is drawn
 *      - filled: a filled and solid rectangle is drawn
 */ 
void mcRect_defineInside(mcObj_t * rec, mcShape_t shape_type);

#endif