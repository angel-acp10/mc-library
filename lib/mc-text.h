#ifndef _MC_TEXT_H_
#define _MC_TEXT_H_

#include <stdint.h>
#include "mc-font.h"

void drawString(char * str, uint16_t x_ofs, uint16_t y_ofs, mcFont_t * f);
uint32_t get_unicode(char * ch, uint8_t * out_byte_n);

#endif