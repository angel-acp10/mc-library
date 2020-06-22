/***********
 * Includes
 ***********/ 
#include "mc-shapes.h"
#include <stdio.h>

/**********
 * Macros
 **********/
#define _MIN_(A,B) ((A)<(B) ? (A) : (B))
#define _MAX_(A,B) ((A)>(B) ? (A) : (B))
#define _ABS_(A)   ((A)<0 ? -(A) : (A))

/******************************
 * Static function prototypes
 ******************************/
static void mcPix_preRender(mcObj_t * p);
static void mcLine_preRender(mcObj_t * l);
static void mcRect_preRender(mcObj_t * r);

/*****************************
 * Public funtion definitions
 *****************************/
/**
 * @brief: creates a pixel object
 */ 
mcObj_t * mcPix_create(mcObj_t *parent, mcObj_t *scr)
{
    mcObj_t * pix = mcObj_create(parent, scr);

    /* obj_data is empty */
    pix->obj_data = NULL;

    /* init coordinates and color */
    pix->geom.x = 0;
    pix->geom.y = 0;
    pix->geom.color = white;

    /* callbacks */
    pix->delete_obj_data_cb = NULL;
    pix->preRender_cb = mcPix_preRender;
    pix->drawToBuffer_cb = mcDraw_pixel;

    return pix;
}

/**
 * @brief: creates a line object
 */ 
mcObj_t * mcLine_create(mcObj_t * parent, mcObj_t * scr)
{
    mcObj_t * line = mcObj_create(parent, scr);

    /* obj_data is empty */
    line->obj_data = NULL;

    /* init coordinates and color */
    line->geom.x = 0;
    line->geom.y = 0;
    line->geom.w = 8;
    line->geom.h = 8;
    line->geom.color = white;

    /* callbacks */
    line->delete_obj_data_cb = NULL;
    line->preRender_cb = mcLine_preRender;
    line->drawToBuffer_cb = mcDraw_xyLine;

    return line;
}

/**
 * @brief: creates a rectangle object
 */ 
mcObj_t * mcRect_create(mcObj_t *parent, mcObj_t *scr)
{
    mcObj_t * rec = mcObj_create(parent, scr);

    /* obj_data is empty */
    rec->obj_data = NULL;

    /* init coordinates and color */
    rec->geom.x = 0;
    rec->geom.y = 0;
    rec->geom.w = 8;
    rec->geom.h = 8;
    rec->geom.color = white;

    /* callbacks */
    rec->delete_obj_data_cb = NULL;
    rec->preRender_cb = mcRect_preRender;
    rec->drawToBuffer_cb = mcDraw_rectangle;

    return rec;
}

/**
 * @brief: set inside of a rectangle
 */ 
void mcRect_defineInside(mcObj_t * rec, mcShape_t shape_type)
{
    // rectangle can be filled or empty. It is drawn different for each case
    if(shape_type == filled)
        rec->drawToBuffer_cb = mcDraw_fRectangle;
    else
        rec->drawToBuffer_cb = mcDraw_rectangle;
}

/******************************
 * Static funtion definitions
 ******************************/ 
/**
 * @brief: pixel prerender function
 */ 
static void mcPix_preRender(mcObj_t * p)
{
    if(p->geom.coord_type == cartesian)
        p->geom.y = (p->scr->geom.h-1) - p->geom.y;

    p->geom.h = 1;
    p->geom.w = 1;

    mcObj_align_prerender(p);
    p->prerender_flg = 0;
}

/**
 * @brief: line prerender function
 */ 
static void mcLine_preRender(mcObj_t * l)
{
    if(l->geom.coord_type == cartesian){
        l->geom.y = (l->scr->geom.h-1) - l->geom.y;
        l->geom.h = -l->geom.h;
    }
    
    if(l->geom.h == 0){ // horizontal line
        int16_t w = l->geom.w;
        l->geom.w = _ABS_(w);
        l->geom.x = _MIN_(l->geom.x, l->geom.x+w);
        l->drawToBuffer_cb = mcDraw_xLine;

    }else if(l->geom.w == 0){ // vertical line
        int16_t h = l->geom.h;
        l->geom.h = _ABS_(h);
        l->geom.y = _MIN_(l->geom.y, l->geom.y+h);
        l->drawToBuffer_cb = mcDraw_yLine;

    }else{
        l->drawToBuffer_cb = mcDraw_xyLine;
    }

    

    mcObj_align_prerender(l);
    l->prerender_flg = 0;
}

/**
 * @brief: rectangle prerender function
 */ 
static void mcRect_preRender(mcObj_t * r)
{
    if(r->geom.coord_type == cartesian){
        r->geom.y = (r->scr->geom.h-1) - r->geom.y -_ABS_(r->geom.h);
        r->geom.h = -r->geom.h;
    }

    int16_t w = r->geom.w;
    r->geom.w = _ABS_(w);
    r->geom.x = _MIN_(r->geom.x, r->geom.x+w);

    int16_t h = r->geom.h;
    r->geom.h = _ABS_(h);
    r->geom.y = _MIN_(r->geom.y, r->geom.y+h);

    r->geom.w = w;
    r->geom.h = h;

    mcObj_align_prerender(r);
    r->prerender_flg = 0;
}
