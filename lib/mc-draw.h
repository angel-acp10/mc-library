#ifndef _MC_DRAW_H_
#define _MC_DRAW_H_

#include <stdint.h>
#include "mc-font.h"
#include "mc-obj.h"

/*************
 * Typedefs
 *************/ 
typedef void(*mcCb_sendBuf_t)(uint8_t * buf);

typedef struct{
    uint8_t * buffer;
    mcCb_sendBuf_t send_buf_cb;
    mcColor_t def_color;
}mcObjData_scr_t;

/***********************
 * Function prototypes 
 **********************/
mcObj_t * mcScr_create();
void mcScr_setDisplayCb(mcObj_t * scr, mcCb_sendBuf_t send_buffer_cb);
void mcScr_setResolution(mcObj_t * scr, uint16_t width, uint16_t height);
void mcScr_setBuffer(mcObj_t * scr, uint8_t * buffer);
void mcScr_select(mcObj_t * scr);
void mcDraw_enableMask(uint16_t xMin, uint16_t xMax, uint16_t yMin, uint16_t yMax);
void mcDraw_disableMask();

void mcDraw_pixel(uint16_t x, uint16_t y, mcColor_t color);
void mcDraw_yLine(mcGeo_t * line);
void mcDraw_xLine(mcGeo_t * line);
void mcDraw_fRectangle(mcGeo_t * rect);
void mcDraw_rectangle(mcGeo_t * rect);
void mcDraw_char(uint32_t ch_idx, uint16_t x_ofs, uint16_t y_ofs, mcFont_t * f);

#endif