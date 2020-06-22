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
 */ 
mcObj_t * mcPix_create(mcObj_t *parent, mcObj_t *scr);

/**
 * @brief: creates a line object
 */ 
mcObj_t * mcLine_create(mcObj_t * parent, mcObj_t * scr);

/**
 * @brief: creates a rectangle object
 */ 
mcObj_t * mcRect_create(mcObj_t *parent, mcObj_t *scr);

/**
 * @brief: set inside of a rectangle
 */ 
void mcRect_defineInside(mcObj_t * rec, mcShape_t shape_type);

#endif