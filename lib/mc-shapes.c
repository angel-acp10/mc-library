/***********
 * Includes
 ***********/ 
#include "mc-shapes.h"

/**********
 * Macros
 **********/
#define _MIN_(A,B) (A<B ? A : B)
#define _MAX_(A,B) (A>B ? A : B)

//-------
// Pixel
//-------
/*
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
    pix->drawToBuffer_cb = mcDraw_pixel;

    return pix;
}

/*
 * @brief: set pixel coordinates
 */ 
void mcPix_setCoord(mcObj_t * pix, uint16_t x, uint16_t y, mcCoord_t coord_type)
{
    pix->geom.x = x;

    if(coord_type == cartesian)
        pix->geom.y = (pix->scr->geom.h-1) - y;

    else //default screen coordinates
        pix->geom.y = y;
}

/*
 * @brief: set pixel color
 */ 
void mcPix_setColor(mcObj_t * pix, mcColor_t color)
{
    pix->geom.color = color;
}

//-------
// Line
//-------
/*
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
    line->drawToBuffer_cb = mcDraw_xyLine;

    return line;
}

/*
 * @brief: set line coordinates
 */ 
void mcLine_setCoord(mcObj_t * line, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, mcCoord_t coord_type)
{
    if(coord_type == cartesian){
        y0 = (line->scr->geom.h-1) - y0;
        y1 = (line->scr->geom.h-1) - y1;
    }

    if(y0 == y1){ // horizontal line
        line->geom.x = _MIN_(x0,x1);
        line->geom.w = _MAX_(x0,x1) - _MIN_(x0,x1);
        line->geom.y = y0;
        line->geom.h = 0;
        line->drawToBuffer_cb = mcDraw_xLine;

    }else if(x0 == x1){ // vertical line
        line->geom.x = x0;
        line->geom.w = 0;
        line->geom.y = _MIN_(y0,y1);
        line->geom.h = _MAX_(y0,y1) - _MIN_(y0,y1);
        line->drawToBuffer_cb = mcDraw_yLine;

    }else{
        line->geom.x = x0;
        line->geom.w = x1 - x0;
        line->geom.y = y0;
        line->geom.h = y1 - y0; 
        line->drawToBuffer_cb = mcDraw_xyLine;
    }
}

/*
 * @brief: set line color
 */ 
void mcLine_setColor(mcObj_t * line, mcColor_t color)
{
    line->geom.color = color;
}

//------------
// Rectangle 
//------------
/*
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
    rec->drawToBuffer_cb = mcDraw_rectangle;

    return rec;
}

/*
 * @brief: set rectangle coordinates
 */ 
void mcRect_setCoord(mcObj_t * rec, uint16_t x, uint16_t y, uint16_t w, uint16_t h, mcCoord_t coord_type)
{
    rec->geom.x = x;
    rec->geom.w = w;
    rec->geom.h = h;

    if(coord_type == cartesian)
        rec->geom.y = (rec->scr->geom.h-1) - y - h;

    else //default screen coordinates
        rec->geom.y = y;
}

/*
 * @brief: set rectangle color
 */ 
void mcRect_setColor(mcObj_t * rec, mcColor_t color, mcShape_t shape_type)
{
    rec->geom.color = color;

    // rectangle can be filled or empty. It is drawn different for each case
    if(shape_type == filled)
        rec->drawToBuffer_cb = mcDraw_fRectangle;
    else
        rec->drawToBuffer_cb = mcDraw_rectangle;
}
