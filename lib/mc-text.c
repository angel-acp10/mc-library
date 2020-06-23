#include "mc-text.h"
#include "mc-draw.h"
#include <stdio.h>
#include <stdint.h>

static void mcText_deleteObjData(mcObj_t * t);
static void mcText_preRender(mcObj_t * text);
static void mcDraw_text(mcObj_t * t);
static uint32_t get_unicode(char * ch, uint8_t * out_byte_n);
static _Bool get_chIdx(uint32_t unicode, uint32_t * ch_idx, mcFont_t * f);
static mcTextLine_t getLine(mcObj_t * obj, uint16_t s_idx);

/**
 * @brief: creates a text object
 */ 
mcObj_t * mcText_create(mcObj_t *parent, mcObj_t *scr)
{
    mcObj_t * text = mcObj_create(parent, scr);

    /* allocate memory for obj_data */
    text->obj_data = (mcObjData_text_t*)malloc( sizeof(mcObjData_text_t) );
    if(text->obj_data == NULL)
        return NULL;
    
    /* init obj_data */
    ((mcObjData_text_t*)text->obj_data)->font = NULL;
    ((mcObjData_text_t*)text->obj_data)->str = NULL;
    ((mcObjData_text_t*)text->obj_data)->text_line = NULL;
    ((mcObjData_text_t*)text->obj_data)->N_lines = 0;

    /* init coordinates and color */
    text->geom.x = 0;
    text->geom.y = 0;
    text->geom.w = 0;
    text->geom.h = 0;
    text->geom.color = white;

    /* callbacks */
    text->delete_obj_data_cb = mcText_deleteObjData;
    text->preRender_cb = mcText_preRender;
    text->drawToBuffer_cb = mcDraw_text;

    return text;
}

void mcText_setFont(mcObj_t * t, mcFont_t * font)
{
    ((mcObjData_text_t*)t->obj_data)->font = font;
}

void mcText_setWidth(mcObj_t * t, uint16_t width)
{
    t->geom.w = width;
}

void mcText_setStr(mcObj_t * t, char * str)
{
    uint16_t len = 0;
    while(*(str+len)){
        len++;
    }

    // the nested struct is so complex, just copy the pointer to another var
    char * ptr_cpy = ((mcObjData_text_t*)t->obj_data)->str;

    if( ptr_cpy == NULL )
        ptr_cpy = (char *)malloc( (len+1)*sizeof(char) ); //len + \0
    else
        ptr_cpy = (char *)realloc( ptr_cpy, len+1 ); //len + \0
    
    // copying characters from str to ptr_cpy 
    uint16_t i;
    for (i = 0; str[i] != '\0'; ++i)
        ptr_cpy[i] = str[i]; 
    ptr_cpy[i] = '\0'; 

    ((mcObjData_text_t*)t->obj_data)->str = ptr_cpy;
}

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
        if(!get_chIdx(uc, &ch_idx, f))
            return;

        //set the appropiate vertical offset and draw the char
        y_char = y_ofs + f->ch_info[ch_idx].y_offset;
        mcDraw_char(ch_idx, x_char, y_char, f);

        // move cursor
        x_char += f->ch_info[ch_idx].w + f->fix_kern;
        str += n_char;
    }
}

static void mcText_deleteObjData(mcObj_t * t)
{

}

static void mcText_preRender(mcObj_t * text)
{
    mcTextLine_t line = {.x=0, .y=0, .start_idx=0, .end_idx=0};
    mcObjData_text_t* data = ((mcObjData_text_t*)text->obj_data);
    
    // allocate memory for the parameters of one line
    data->text_line = (mcTextLine_t*) malloc(sizeof(mcTextLine_t));
    if(data->text_line == NULL){
        printf("malloc failed\n");
        return;
    }

    data->N_lines = 1;

    line = getLine(text, line.end_idx);
    data->text_line[0] = line;
    if(line.end_idx == -1){
        printf("no char available\n");
        return;
    }
    printf("s=%u \t e=%u\n",line.start_idx, line.end_idx);
    
    while(data->str[line.end_idx+1])
    {
        // reallocate memory for the parameters
        data->text_line = (mcTextLine_t*) realloc(data->text_line, data->N_lines+1);
        if(data->text_line == NULL){
            printf("realloc failed\n");
            return;
        }
        
        line = getLine(text, line.end_idx+1);
        data->text_line[data->N_lines] = line;
        data->N_lines++;
        printf("s=%u \t e=%u\n",line.start_idx, line.end_idx);
    }
    
}

static void mcDraw_text(mcObj_t * t)
{

}

/**
 * @brief: Given a character encoded in UTF-8, returns its unicode code point.
 * It also returns "out_byte_n" which is the number of bytes used by UTF-8 to
 * encode the given character.
 */
static uint32_t get_unicode(char * ch, uint8_t * out_byte_n)
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

/**
 * @brief: get ch_idx from a unicode code pointer in a font. 
 * Return 0 if glyph is not within the font
 */ 
static _Bool get_chIdx(uint32_t unicode, uint32_t * ch_idx, mcFont_t * f)
{
    // check if glyph is included in font & get ch_idx
    for(int i = 0; i<f->uc_rg_n; i++){
        if( unicode >= f->uc_rg[i].start  &&  unicode <= f->uc_rg[i].end){
            // ch_idx is the index to acces "ch_info" which is an array 
            // within mcFont_t
            *ch_idx = (unicode - f->uc_rg[i].start + f->uc_rg[i].ch_info_idx);
            //printf("ch_idx = %u\n", *ch_idx);
            return 1;
        }
    }
    return 0;
}

/**
 * @brief: find start and end indexes of str for each line
 */ 
static mcTextLine_t getLine(mcObj_t * obj, uint16_t s_idx)
{
    mcFont_t * f = ((mcObjData_text_t*)obj->obj_data)->font;
    char * str = ((mcObjData_text_t*)obj->obj_data)->str;
    uint16_t idx = s_idx;
    
    // don't count initial spaces
    while(str[idx] == ' '){
        idx++;
    }

    mcTextLine_t line = {.x=0, .y=0, .start_idx = idx, .end_idx = idx};
    uint32_t uc, ch_idx;
    uint8_t n_ch;
    uint16_t pix_len = 0;

    while( pix_len<=obj->geom.w && str[idx] )
    {
        // if a space is detected, set end_idx to the previous char
        if(str[idx] == ' ')
            line.end_idx = idx - 1;

        uc = get_unicode( str+idx, &n_ch );
        if(!get_chIdx(uc, &ch_idx, f)){
            line.start_idx = -1;
            line.end_idx = -1;
            return line;
        }

        pix_len += f->ch_info[ch_idx].w + f->fix_kern;
        idx += n_ch;
    }
    // if end of text, set end_idx to the previous char
    if(!str[idx])
        line.end_idx = idx - 1;

    // if the word width is higher than the line width, the word is split
    if(line.start_idx == line.end_idx)
        line.end_idx = idx - n_ch;
    
    return line;
}

