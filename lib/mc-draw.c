/*
 *  This file contains the screen object management, and all low level functions 
 *  which directly write to buffer
 */ 

/***********
 * Includes 
 ***********/ 
#include "mc-draw.h"
#include <stdio.h>
#include <string.h>

/*******************
 * Private typedefs
 *******************/ 
typedef struct{
    uint16_t xMin, xMax, yMin, yMax; // all limits are included
}mask_t;

typedef struct{
    mask_t mask;
    uint8_t * buf;
    uint32_t w;
    uint32_t h;
}currScr_t;

/*******************************
 *  Static function prototypes
 *******************************/
static void mcDraw_renderFrameBuf(mcObj_t * scr);

/********************
 * Private variables
 ********************/ 
static currScr_t c_scr;

/**********
 * Macros
 **********/
/* Macros related to the buffer acces */
#define _IDX_(x,y) ( ((y)*(c_scr.w)/8) + (x/8) )
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
    scr->obj_data = (mcObjData_scr_t*)malloc( sizeof(mcObjData_scr_t) );
    if(scr->obj_data == NULL)
        return NULL;
    
    /* set callbacks */
    ((mcObjData_scr_t*)scr->obj_data)->buffer = NULL;
    ((mcObjData_scr_t*)scr->obj_data)->send_buf_cb = NULL;
    scr->drawToBuffer_cb = mcDraw_renderFrameBuf;

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
void mcDraw_pixel(mcObj_t * p)
{
    _mcDraw_pixel_(p->geom.x, p->geom.y, p->geom.color);
}

/*
 * @brief: draw an horizontal line
 */ 
void mcDraw_xLine(mcObj_t * l)
{
    uint16_t x = l->geom.x;
    uint16_t w = l->geom.w;

    if(l->geom.x < c_scr.mask.xMin)     
        x = c_scr.mask.xMin;
    if(l->geom.x+l->geom.w > c_scr.mask.xMax) 
        w = c_scr.mask.xMax - x;
    
    if( l->geom.y>=c_scr.mask.yMin && l->geom.y<=c_scr.mask.yMax ){
        if( l->geom.color ){
            while(w--){
                *(c_scr.buf+_IDX_(x,l->geom.y)) |= _BIT_MASK_(x);
                x++;
            }
        }else{
            while(w--){
                *(c_scr.buf+_IDX_(x,l->geom.y)) &= ~_BIT_MASK_(x);
                x++;
            }
        }
    }
}

/*
 * @brief: draw a vertical line
 */ 
void mcDraw_yLine(mcObj_t * l)
{
    uint16_t y = l->geom.y;
    uint16_t h = l->geom.h;

    if(l->geom.y < c_scr.mask.yMin)
        y = c_scr.mask.yMin;
    if(l->geom.y+l->geom.h > c_scr.mask.yMax)
        h = c_scr.mask.yMax - y;
    
    if( l->geom.x>=c_scr.mask.xMin && l->geom.x<=c_scr.mask.xMax ){
        uint32_t idx = _IDX_(l->geom.x, y);
        uint16_t bit_mask = _BIT_MASK_(l->geom.x);

        if( l->geom.color ){
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
 * @brief: draws any line, including non horizontal or vertical 
 * @Bresenham's line algorithm: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
*/
void mcDraw_xyLine(mcObj_t * l)
{
    int16_t x0 = l->geom.x;
    int16_t x1 = l->geom.x + l->geom.w;
    int16_t y0 = l->geom.y;
    int16_t y1 = l->geom.y + l->geom.h;

    int16_t dx =  abs(x1-x0);
    int16_t sx = x0<x1 ? 1 : -1;
    int16_t dy = -abs(y1-y0);
    int16_t sy = y0<y1 ? 1 : -1;
    int16_t err = dx+dy;  /* error value e_xy */

    int16_t e2;

    while (1){/* loop */
        _mcDraw_pixel_(x0,y0, l->geom.color);

        if (x0==x1 && y0==y1)
            break;

        e2 = 2*err;

        if (e2 >= dy){ /* e_xy+e_x > 0 */
            err += dy;
            x0 += sx;
        }

        if (e2 <= dx){ /* e_xy+e_y < 0 */
            err += dx;
            y0 += sy;
        }
    }
}

/*
 * @brief: draw a filled rectangle
 */ 
void mcDraw_fRectangle(mcObj_t * r)
{
    mcObj_t line = *r;

    uint16_t x = r->geom.x;
    uint16_t w = r->geom.w;
    while(w)
    {
        mcDraw_yLine(&line);
        line.geom.x++;
        w--;
    }
}

/*
 * @brief: draw an empty rectangle
 */ 
void mcDraw_rectangle(mcObj_t * r)
{
    mcObj_t line = *r;

    mcDraw_xLine(&line);
    mcDraw_yLine(&line);
    line.geom.y += r->geom.h-1;
    mcDraw_xLine(&line);
    line.geom.y = r->geom.y;
    line.geom.x += r->geom.w-1;
    mcDraw_yLine(&line);
}

/*
 * @brief: Given a "ch_idx" draws the corresponding character with the offset
 * x_ofs and y_ofs and font f. This function is here due to optimization purposes, 
 * but should in "mc-text.c" module.
 */ 
void mcDraw_char(mcObj_t* text, uint32_t ch_idx, uint16_t x_ofs, uint16_t y_ofs)
{
    mcFont_t * f = ((mcObjData_text_t*)text->obj_data)->font;
    mcColor_t color = text->geom.color;
    
    uint32_t bm_idx;
    //printf("ch_idx = %u\n", ch_idx);

    // using ch_idx, get bitmap index
    bm_idx = f->ch_info[ch_idx].bitmap_idx;
    //printf("bm_idx = %u\n", bm_idx);

    // draw the glyph
    uint32_t px = f->ch_info[ch_idx].w * f->ch_info[ch_idx].h;
    uint16_t x = 0, y = 0;
    uint8_t bit = 0;
    while(px)
    {
        // set/reset pixel
        if(f->bm[bm_idx] & 0x80>>bit){   
            _mcDraw_pixel_(x+x_ofs, y+y_ofs, color);
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

/*****************************
 * Static funtion definitions 
 *****************************/
static void mcDraw_renderFrameBuf(mcObj_t * scr)
{
    /* clear the entire buffer */
    uint8_t * buf = ((mcObjData_scr_t*)scr->obj_data)->buffer;
    for(uint32_t i = 0; i<1280*640/8; i++)
        *(buf + i) = 0;

    /* draw each object in scr child_list */
    node_t * node_ptr = scr->child_list.head;
    while(node_ptr){
        mcObj_t * obj_ptr = ((mcObj_t *)node_ptr->data);
        if(obj_ptr->prerender_flg)
            obj_ptr->preRender_cb(obj_ptr);
        obj_ptr->drawToBuffer_cb(obj_ptr);

        node_ptr = node_ptr->next;
    }
}
