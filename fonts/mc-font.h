#ifndef _MC_FONT_H_
#define _MC_FONT_H_

#include <stdint.h>

typedef struct{
	uint16_t bitmap_idx;
	uint8_t y_offset;
	uint8_t w;
	uint8_t h;
}ch_info_t;

typedef struct{
	uint16_t start;
	uint16_t end;
	uint16_t ch_info_idx;
}uc_range_t;

typedef struct{
	uint8_t line_height;
	uint8_t line_spacing;
	uint8_t fix_kern;
	const uc_range_t* uc_rg;
	const uint16_t uc_rg_n;
	const ch_info_t* ch_info;
	const uint8_t* bm;
}mcFont_t;

extern mcFont_t helvetica_12;

#endif
