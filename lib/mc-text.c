#include "mc-text.h"
#include "mc-draw.h"
#include <stdio.h>
#include <stdint.h>

/*
 * @biref: Draws string "str" with the offset x_ofs and y_ofs and font f
 */ 
void drawString(char * str, uint16_t x_ofs, uint16_t y_ofs, mcFont_t * f)
{
    uint32_t uc, ch_idx;
    uint8_t n_char; 

    uint16_t x_char = x_ofs;
    uint16_t y_char;
    while(*str){
        // get unicode code point and its number of used bytes "n_char"
        uc = get_unicode(str, &n_char);
        printf("uc_code_point = %u\n", uc);

        // check if glyph is included in font & get ch_idx
        _Bool exist_glyph = 0;
        for(int i = 0; i<f->uc_rg_n; i++){
            if( uc >= f->uc_rg[i].start  &&  uc <= f->uc_rg[i].end){
                // ch_idx is the index to acces "ch_info" which is an array 
                // within mcFont_t
                ch_idx = (uc - f->uc_rg[i].start + f->uc_rg[i].ch_info_idx);
                exist_glyph = 1;
                printf("ch_idx = %u\n", ch_idx);
            }
        }
        if(!exist_glyph)
            return;

        //set the appropiate vertical offset and draw the char
        y_char = y_ofs + f->ch_info[ch_idx].y_offset;
        mcDraw_char(ch_idx, x_char, y_char, f);

        // move cursor
        x_char += f->ch_info[ch_idx].w + f->fix_kern;
        str += n_char;
    }
}



/*
 * @brief: Given a character encoded in UTF-8, returns its unicode code point.
 * It also returns "out_byte_n" which is the number of bytes used by UTF-8 to
 * encode the given character.
 */
uint32_t get_unicode(char * ch, uint8_t * out_byte_n)
{
    uint32_t a = 0;
    uint8_t *ptr = (uint8_t*)ch;

    if ((*ptr & 0xF8) == 0xF0){ // 11110xxx
        a |= (*ptr & 0b00000011);
        a = a<<6;
        ptr++;
        a |= (*ptr & 0b00111111);
        a = a<<6;
        ptr++;
        a |= (*ptr & 0b00111111);
        a = a<<6;
        ptr++;
        a |= (*ptr & 0b00111111);
        *out_byte_n = 4;
        return a;
    }
    else if ((*ptr & 0xF0) == 0xE0){ // 1110xxxx
        a |= (*ptr & 0b00001111);
        a = a<<6;
        ptr++;
        a |= (*ptr & 0b00111111);
        a = a<<6;
        ptr++;
        a |= (*ptr & 0b00111111);
        *out_byte_n = 3;
        return a;
    }
    else if ((*ptr & 0xE0) == 0xC0){ // 110xxxxx
        a |= ( *ptr & 0b00111111 );
        ptr++;
        a = a<<6;
        a |= (*ptr & 0b00111111);
        *out_byte_n = 2;
        return a;
    }
    else{
        *out_byte_n = 1;
        return (uint32_t)*ch;
    }
}

