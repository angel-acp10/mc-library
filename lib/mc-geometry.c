#include "mc-geometry.h"
#include <stdio.h>


void mcGeo_xLine(mcGeo_t * line, uint8_t * buf)
{
    uint32_t x = line->x;
    uint32_t w = line->w;
    while(w)
    {
        if( line->color )
            *(buf+_IDX_(x,line->y)) |= _BIT_MASK_(x);
        else
            *(buf+_IDX_(x,line->y)) &= ~_BIT_MASK_(x);

        x++;
        w--;
    }
}

void mcGeo_yLine(mcGeo_t * line, uint8_t * buf)
{
    uint32_t idx = _IDX_(line->x, line->y);
    uint32_t bit_mask = _BIT_MASK_(line->x);
    uint32_t h = line->h;
    while(h)
    {
        if( line->color ){
            *(buf+idx) |= (bit_mask);
        }else
            *(buf+idx) &= ~(bit_mask); 

        idx += SCR_WIDTH/8; 
        h--;
    }
}

void mcGeo_fRectangle(mcGeo_t * rect, uint8_t * buf)
{
    mcGeo_t line = *rect;

    uint16_t x = rect->x;
    uint16_t w = rect->w;
    while(w)
    {
        mcGeo_yLine(&line, buf);
        line.x++;
        w--;
    }
}

void mcGeo_rectangle(mcGeo_t * rect, uint8_t * buf)
{
    mcGeo_t line = {rect->x, rect->y, rect->w, rect->h, rect->color};
    mcGeo_xLine(&line, buf);
    mcGeo_yLine(&line, buf);
    line.y += rect->h;
    mcGeo_xLine(&line, buf);
    line.y = rect->y;
    line.x += rect->w;
    mcGeo_yLine(&line, buf);
}

