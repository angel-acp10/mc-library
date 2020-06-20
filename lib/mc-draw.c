/*
 *  This file contains the screen object management, and all low level functions 
 *  which directly write to buffer
 */ 

/***********
 * Includes 
 ***********/ 
#include "mc-draw.h"
#include <stdio.h>

/*******************
 * Private typedefs
 *******************/ 
typedef struct{
    uint16_t xMin, xMax, yMin, yMax; // all limits are included
    _Bool on;
}mask_t;

typedef struct{
    mask_t mask;
    uint8_t * buf;
    uint16_t w;
    uint16_t h;
}currScr_t;

/********************
 * Private variables
 ********************/ 
static currScr_t c_scr;

/**********
 * Macros
 **********/
/* Macros related to the buffer acces */
#define _IDX_(x,y) ( (y*c_scr.w/8) + (x/8) )
#define _BIT_(x) (x-(x/8)*8)
#define _BIT_MASK_(x) ( 0x80>>_BIT_(x) )

/* Draws a single pixel */
#define _mcDraw_pixel_(x,y,color){\
    if( (x)>=c_scr.mask.xMin && (x)<=c_scr.mask.xMax && (y)>=c_scr.mask.yMin && (y)<=c_scr.mask.yMax ){\
        if( color )\
            *(c_scr.buf+_IDX_((x),(y))) |= (_BIT_MASK_((x)));\
        else\
            *(c_scr.buf+_IDX_((x),(y))) &= ~(_BIT_MASK_((x)));\
    }\
}

/********************
 * Public functions
 ********************/ 
/*
 * @brief: creates a screen object
 */ 
mcObj_t * mcScr_create()
{
    mcObj_t * scr = mcObj_create(NULL, NULL);

    /* allocate memory for "data" */
    mcObjData_scr_t * scr_data;
    scr_data = (mcObjData_scr_t*)malloc( sizeof(mcObjData_scr_t) );
    if(scr == NULL)
        return NULL;
    
    scr_data->buffer = NULL;
    scr_data->send_buf_cb = NULL;
    scr->obj_data = scr_data;

    return scr;
}

/*
 * @brief: set a callback so the buffer is sent to the hardware
 */ 
void mcScr_setDisplayCb(mcObj_t * scr, mcCb_sendBuf_t send_buffer_cb)
{
   ((mcObjData_scr_t*)scr->obj_data)->send_buf_cb = send_buffer_cb;
}

/*
 * @brief: set resolution into a screen object
 */ 
void mcScr_setResolution(mcObj_t * scr, uint16_t width, uint16_t height)
{
    scr->geom.w = width;
    scr->geom.h = height;
}

/*
 * @brief: set a buffer into a screen object
 */  
void mcScr_setBuffer(mcObj_t * scr, uint8_t * buffer)
{
    ((mcObjData_scr_t*)scr->obj_data)->buffer = buffer;
}

/*
 * @brief: select a screen to draw in
 */ 
void mcScr_select(mcObj_t * scr)
{
    c_scr.w = scr->geom.w;
    c_scr.h = scr->geom.h;

    mcDraw_disableMask();
    c_scr.buf = ((mcObjData_scr_t*)scr->obj_data)->buffer;
}

/*
 * @brief: when enabled, only pixels within it will be drawn
 * It must always be called after mcScr_select, because mcScr_select
 * will disable the mask
 */ 
void mcDraw_enableMask(uint16_t xMin, uint16_t xMax, uint16_t yMin, uint16_t yMax)
{
    c_scr.mask.xMin = xMin;
    c_scr.mask.xMax = xMax;
    c_scr.mask.yMin = yMin;
    c_scr.mask.yMax = yMax;
}

/*
 * @brief: disbale mask. Pixels are drawn faster when it is disbaled, 
 * but is dangerous to write outside the buffer dimmensions
 */ 
void mcDraw_disableMask()
{
    c_scr.mask.xMin = 0;
    c_scr.mask.xMax = c_scr.w-1;
    c_scr.mask.yMin = 0;
    c_scr.mask.yMax = c_scr.h-1;
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

    if(line->x < c_scr.mask.xMin)     
        x = c_scr.mask.xMin;
    if(line->x+line->w > c_scr.mask.xMax) 
        w = c_scr.mask.xMax - x;
    
    if( line->y>=c_scr.mask.yMin && line->y<=c_scr.mask.yMax ){
        if( line->color ){
            while(w--){
                *(c_scr.buf+_IDX_(x,line->y)) |= _BIT_MASK_(x);
                x++;
            }
        }else{
            while(w--){
                *(c_scr.buf+_IDX_(x,line->y)) &= ~_BIT_MASK_(x);
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

    if(line->y < c_scr.mask.yMin)
        y = c_scr.mask.yMin;
    if(line->y+line->h > c_scr.mask.yMax)
        h = c_scr.mask.yMax - y;
    
    if( line->x>=c_scr.mask.xMin && line->x<=c_scr.mask.xMax ){
        uint32_t idx = _IDX_(line->x, y);
        uint32_t bit_mask = _BIT_MASK_(line->x);

        if( line->color ){
            while(h--){
                *(c_scr.buf+idx) |= (bit_mask);
                idx += c_scr.w/8; 
            }
        }else{
            while(h--){
                *(c_scr.buf+idx) &= ~(bit_mask); 
                idx += c_scr.w/8; 
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
    mcGeo_t line;
    line.x = rect->x;
    line.y = rect->y;
    line.w = rect->w;
    line.h = rect->h;
    line.color = rect->color;

    mcDraw_xLine(&line);
    mcDraw_yLine(&line);
    line.y += rect->h;
    mcDraw_xLine(&line);
    line.y = rect->y;
    line.x += rect->w;
    mcDraw_yLine(&line);
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
