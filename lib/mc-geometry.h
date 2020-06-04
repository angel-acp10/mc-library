#ifndef _MC_GEOMETRY_H_
#define _MC_GEOMETRY_H_

#include <stdint.h>
#include "mc-display.h"

/*************
 * Typedefs
 *************/ 
enum{
    black = 0,
    white = 1
};
typedef uint8_t mcColor_t;

typedef struct{
    int16_t x, y, w, h;
    mcColor_t color;
}mcGeo_t;

/***********************
 * Function prototypes 
 **********************/
void mc_selectBuffer(uint8_t * buffer);
void mcGeo_drawPixel(uint16_t x, uint16_t y, mcColor_t color);
void mcGeo_yLine(mcGeo_t * line);
void mcGeo_xLine(mcGeo_t * line);
void mcGeo_fRectangle(mcGeo_t * rect);
void mcGeo_rectangle(mcGeo_t * rect);
void mcGeo_enableMask(uint16_t xMin, uint16_t xMax, uint16_t yMin, uint16_t yMax);
void mcGeo_disableMask();


#endif