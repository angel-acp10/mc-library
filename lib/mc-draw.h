#ifndef _MC_DRAW_H_
#define _MC_DRAW_H_

#include <stdint.h>
#include "mc-display.h"
#include "mc-font.h"

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
void mcDraw_selectBuffer(uint8_t * buffer);
void mcDraw_pixel(uint16_t x, uint16_t y, mcColor_t color);
void mcDraw_yLine(mcGeo_t * line);
void mcDraw_xLine(mcGeo_t * line);
void mcDraw_fRectangle(mcGeo_t * rect);
void mcDraw_rectangle(mcGeo_t * rect);
void mcDraw_enableMask(uint16_t xMin, uint16_t xMax, uint16_t yMin, uint16_t yMax);
void mcDraw_disableMask();
void mcDraw_char(uint32_t ch_idx, uint16_t x_ofs, uint16_t y_ofs, mcFont_t * f);

#endif