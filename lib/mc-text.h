#ifndef _MC_TEXT_H_
#define _MC_TEXT_H_

#include <stdint.h>
#include "mc-font.h"
#include "mc-obj.h"

typedef struct{
    uint16_t x, y, w;
    int16_t start_idx, end_idx;
}mcTextLine_t;

typedef struct{
    uint8_t l, r, t, b;
}mcTextMargin_t;

enum{
    MC_TEXT_ALIGN_LEFT = 0,
    MC_TEXT_ALIGN_CENTER,
    MC_TEXT_ALIGN_RIGHT
};
typedef uint8_t mcTextAlign_t;

typedef struct{
    uint16_t usr_max_width;
    mcFont_t * font;

    char * str;
    mcTextLine_t * t_line;
    uint16_t N_lines;
    mcTextAlign_t align;
    mcTextMargin_t margin;

    // callback to draw the text background
    mcShape_t back_rectangle;
}mcObjData_text_t;

mcObj_t * mcText_create(mcObj_t *parent, mcObj_t *scr);
void mcText_setFont(mcObj_t * t, mcFont_t * font);
void mcText_setWidth(mcObj_t * t, uint16_t width);
void mcText_setStr(mcObj_t * t, char * str);
void mcText_setAlign(mcObj_t * t, mcTextAlign_t txt_align);
void mcText_setMargins(mcObj_t * t, uint8_t left, uint8_t right, uint8_t top, uint8_t bottom);
void mcText_defineBackground(mcObj_t * t, mcShape_t shape_type);

#endif