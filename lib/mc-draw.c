/*
 *  This file contains all these functions which directly write to buffer
 */ 

/***********
 * Includes 
 ***********/ 
#include "mc-draw.h"

/*******************
 * Private typedefs
 *******************/ 
typedef struct{
    uint16_t xMin, xMax, yMin, yMax; // all limits are included
    _Bool on;
}mask_t;

/********************
 * Private variables
 ********************/ 
static mask_t mask = {0,0,0,0,0};
static uint8_t * currBuffer;

/**********
 * Macros
 **********/
/* Macros related to the buffer acces */
#define _IDX_(x,y) ( (y*SCR_WIDTH/8) + (x/8) )
#define _BIT_(x) (x-(x/8)*8)
#define _BIT_MASK_(x) ( 0x80>>_BIT_(x) )

/* Draws a single pixel */
#define _mcDraw_pixel_(x,y,color){\
    if(!mask.on || ((x)>=mask.xMin && (x)<=mask.xMax && (y)>=mask.yMin && (y)<=mask.yMax) ){\
        if( color )\
            *(currBuffer+_IDX_((x),(y))) |= (_BIT_MASK_((x)));\
        else\
            *(currBuffer+_IDX_((x),(y))) &= ~(_BIT_MASK_((x)));\
    }\
}

/********************
 * Public functions
 ********************/ 
/*
 * @brief: select a buffer to draw
 */ 
void mcDraw_selectBuffer(uint8_t * buffer)
{
    currBuffer = buffer;
}

/*
 * @brief: draw a single pixel
 */ 
void mcDraw_pixel(uint16_t x, uint16_t y, mcColor_t color)
{
    _mcDraw_pixel_(x,y,color);
}

/*
 * @brief: draw an horizontal line
 */ 
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

/*
 * @brief: draw a vertical line
 */ 
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

/*
 * @brief: draw a filled rectangle
 */ 
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

/*
 * @brief: draw an empty rectangle
 */ 
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

/*
 * @brief: when enabled, only pixels within it will be drawn
 */ 
void mcDraw_enableMask(uint16_t xMin, uint16_t xMax, uint16_t yMin, uint16_t yMax)
{
    mask.xMin = xMin;
    mask.xMax = xMax;
    mask.yMin = yMin;
    mask.yMax = yMax;
    mask.on = 1;
}

/*
 * @brief: disbale mask. Pixels are drawn faster when it is disbaled, 
 * but is dangerous to write outside the buffer dimmensions
 */ 
void mcDraw_disableMask()
{
    mask.on = 0;
}

/*
 * @biref: Given a "ch_idx" draws the corresponding character with the offset
 * x_ofs and y_ofs and font f. This function is here due to optimization purposes, 
 * but should in "mc-text.c" module.
 */ 
void mcDraw_char(uint32_t ch_idx, uint16_t x_ofs, uint16_t y_ofs, mcFont_t * f)
{
    uint32_t bm_idx;
    printf("ch_idx = %u\n", ch_idx);

    // using ch_idx, get bitmap index
    bm_idx = f->ch_info[ch_idx].bitmap_idx;
    printf("bm_idx = %u\n", bm_idx);

    // draw the glyph
    uint32_t px = f->ch_info[ch_idx].w * f->ch_info[ch_idx].h;
    uint16_t x = 0, y = 0;
    uint8_t bit = 0;
    while(px)
    {
        // set/reset pixel
        if(f->bm[bm_idx] & 0x80>>bit){   
            _mcDraw_pixel_(x+x_ofs, y+y_ofs, white);
        }else{        
            _mcDraw_pixel_(x+x_ofs, y+y_ofs, black);
        }
        
        x++;
        if(x >= f->ch_info[ch_idx].w){ // end line check
            x = 0;
            bit = 0;
            bm_idx++;
            y++;
        }else{ // end byte check
            bit++;
            if(bit >= 8){
                bit = 0;
                bm_idx++;
            }
        }
        px--;
    }
}
