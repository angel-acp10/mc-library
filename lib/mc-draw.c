#include "mc-draw.h"

typedef struct{
    uint16_t xMin, xMax, yMin, yMax; // all limits are included
    _Bool on;
}mask_t;
static mask_t mask = {0,0,0,0,0};

static uint8_t * currBuffer;

/**********
 * Macros
 **********/
#define _IDX_(x,y) ( (y*SCR_WIDTH/8) + (x/8) )
#define _BIT_(x) (x-(x/8)*8)
#define _BIT_MASK_(x) ( 0x80>>_BIT_(x) )

#define _mcDraw_pixel_(x,y,color){\
    if(!mask.on || (x>=mask.xMin && x<=mask.xMax && y>=mask.yMin && y<=mask.yMax) ){\
        if( color )\
            *(currBuffer+_IDX_(x,y)) |= (_BIT_MASK_(x));\
        else\
            *(currBuffer+_IDX_(x,y)) &= ~(_BIT_MASK_(x));\
    }\
}

void mcDraw_selectBuffer(uint8_t * buffer)
{
    currBuffer = buffer;
}

void mcDraw_pixel(uint16_t x, uint16_t y, mcColor_t color)
{
    _mcDraw_pixel_(x,y,color);
}

void mcDraw_xLine(mcGeo_t * line)
{
    uint32_t x = line->x;
    uint32_t w = line->w;

    if(mask.on){
        if(line->x < mask.xMin)     
            x = mask.xMin;
        if(line->x+line->w > mask.xMax) 
            w = mask.xMax - x;
    }
    if( !mask.on || (line->y>=mask.yMin && line->y<=mask.yMax) ){
        if( line->color ){
            while(w--){
                *(currBuffer+_IDX_(x,line->y)) |= _BIT_MASK_(x);
                x++;
            }
        }else{
            while(w--){
                *(currBuffer+_IDX_(x,line->y)) &= ~_BIT_MASK_(x);
                x++;
            }
        }
    }
}

void mcDraw_yLine(mcGeo_t * line)
{
    uint32_t y = line->y;
    uint32_t h = line->h;

    if(mask.on){
        if(line->y < mask.yMin)
            y = mask.yMin;
        if(line->y+line->h > mask.yMax)
            h = mask.yMax - y;
    }

    if( !mask.on || (line->x>=mask.xMin && line->x<=mask.xMax) ){
        uint32_t idx = _IDX_(line->x, y);
        uint32_t bit_mask = _BIT_MASK_(line->x);

        if( line->color ){
            while(h--){
                *(currBuffer+idx) |= (bit_mask);
                idx += SCR_WIDTH/8; 
            }
        }else{
            while(h--){
                *(currBuffer+idx) &= ~(bit_mask); 
                idx += SCR_WIDTH/8; 
            }
        }
    }
}

void mcDraw_fRectangle(mcGeo_t * rect)
{
    mcGeo_t line = *rect;

    uint16_t x = rect->x;
    uint16_t w = rect->w;
    while(w)
    {
        mcDraw_yLine(&line);
        line.x++;
        w--;
    }
}

void mcDraw_rectangle(mcGeo_t * rect)
{
    mcGeo_t line = {rect->x, rect->y, rect->w, rect->h, rect->color};
    mcDraw_xLine(&line);
    mcDraw_yLine(&line);
    line.y += rect->h;
    mcDraw_xLine(&line);
    line.y = rect->y;
    line.x += rect->w;
    mcDraw_yLine(&line);
}

void mcDraw_enableMask(uint16_t xMin, uint16_t xMax, uint16_t yMin, uint16_t yMax)
{
    mask.xMin = xMin;
    mask.xMax = xMax;
    mask.yMin = yMin;
    mask.yMax = yMax;
    mask.on = 1;
}

void mcDraw_disableMask()
{
    mask.on = 0;
}
