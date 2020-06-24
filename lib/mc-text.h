#ifndef _MC_TEXT_H_
#define _MC_TEXT_H_

#include <stdint.h>
#include "mc-font.h"
#include "mc-obj.h"

typedef struct{
    uint16_t x, y, w;
    int16_t start_idx, end_idx;
}mcTextLine_t;

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
}mcObjData_text_t;

mcObj_t * mcText_create(mcObj_t *parent, mcObj_t *scr);
void mcText_setFont(mcObj_t * t, mcFont_t * font);
void mcText_setWidth(mcObj_t * t, uint16_t width);
void mcText_setStr(mcObj_t * t, char * str);
void mcText_setAlign(mcObj_t * t, mcTextAlign_t txt_align);

#endif