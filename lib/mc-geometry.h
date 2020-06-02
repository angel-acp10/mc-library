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
void mcGeo_yLine(mcGeo_t * line, uint8_t * buf);
void mcGeo_xLine(mcGeo_t * line, uint8_t * buf);
void mcGeo_fRectangle(mcGeo_t * rect, uint8_t * buf);
void mcGeo_rectangle(mcGeo_t * rect, uint8_t * buf);

/**********
 * Macros
 **********/
#define _IDX_(x,y) ( (y*SCR_WIDTH/8) + (x/8) )
#define _BIT_(x) (x-(x/8)*8)
#define _BIT_MASK_(x) ( 0x80>>_BIT_(x) )

#define _mcGeo_drawPixel_(x,y,color,buf){\
    if( color )\
        *(buf+_IDX_(x,y)) |= (_BIT_MASK_(x));\
    else\
        *(buf+_IDX_(x,y)) &= ~(_BIT_MASK_(x));\
}

#endif