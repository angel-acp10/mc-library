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
//-------
// Pixel
//-------
/*
 * @brief: creates a pixel object
 */ 
mcObj_t * mcPix_create(mcObj_t *parent, mcObj_t *scr);

/*
 * @brief: set pixel coordinates
 */ 
void mcPix_setCoord(mcObj_t * pix, uint16_t x, uint16_t y, mcCoord_t coord_type);

/*
 * @brief: set pixel color
 */ 
void mcPix_setColor(mcObj_t * pix, mcColor_t color);

//-------
// Line
//-------
/*
 * @brief: creates a line object
 */ 
mcObj_t * mcLine_create(mcObj_t * parent, mcObj_t * scr);

/*
 * @brief: set line coordinates
 */ 
void mcLine_setCoord(mcObj_t * line, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, mcCoord_t coord_type);

/*
 * @brief: set line color
 */ 
void mcLine_setColor(mcObj_t * line, mcColor_t color);

//------------
// Rectangle 
//------------
/*
 * @brief: creates a rectangle object
 */ 
mcObj_t * mcRect_create(mcObj_t *parent, mcObj_t *scr);

/*
 * @brief: set rectangle coordinates
 */ 
void mcRect_setCoord(mcObj_t * rec, uint16_t x, uint16_t y, uint16_t w, uint16_t h, mcCoord_t coord_type);

/*
 * @brief: set rectangle color
 */ 
void mcRect_setColor(mcObj_t * rec, mcColor_t color, mcShape_t shape_type);

#endif