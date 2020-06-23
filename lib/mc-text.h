#ifndef _MC_TEXT_H_
#define _MC_TEXT_H_

#include <stdint.h>
#include "mc-font.h"
#include "mc-obj.h"

typedef struct{
    uint16_t x, y;
    int16_t start_idx, end_idx;
}mcTextLine_t;

typedef struct{
    mcFont_t * font;

    char * str;
    mcTextLine_t * text_line;
    uint8_t N_lines;
}mcObjData_text_t;

mcObj_t * mcText_create(mcObj_t *parent, mcObj_t *scr);
void mcText_setFont(mcObj_t * t, mcFont_t * font);
void mcText_setWidth(mcObj_t * t, uint16_t width);
void mcText_setStr(mcObj_t * t, char * str);
void drawString(char * str, uint16_t x_ofs, uint16_t y_ofs, mcFont_t * f);

#endif