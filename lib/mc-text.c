#include "mc-text.h"
#include "mc-draw.h"
#include <stdio.h>
#include <stdint.h>

#define _MAX_(A,B) ( ((A)>(B))?(A):(B) )

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
    mcObj_t * t = mcObj_create(parent, scr);

    /* allocate memory for obj_data */
    t->obj_data = (mcObjData_text_t*)malloc( sizeof(mcObjData_text_t) );
    if(t->obj_data == NULL)
        return NULL;
    
    /* init obj_data */
    ((mcObjData_text_t*)t->obj_data)->font = NULL;
    ((mcObjData_text_t*)t->obj_data)->str = NULL;
    ((mcObjData_text_t*)t->obj_data)->t_line = NULL;
    ((mcObjData_text_t*)t->obj_data)->N_lines = 0;
    ((mcObjData_text_t*)t->obj_data)->align = MC_TEXT_ALIGN_LEFT;
    ((mcObjData_text_t*)t->obj_data)->usr_max_width = scr->geom.w;

    /* init coordinates and color */
    t->geom.x = 0;
    t->geom.y = 0;
    t->geom.w = 0;
    t->geom.h = 0;
    t->geom.color = white;

    /* callbacks */
    t->delete_obj_data_cb = mcText_deleteObjData;
    t->preRender_cb = mcText_preRender;
    t->drawToBuffer_cb = mcDraw_text;

    t->prerender_flg = 1;
    
    return t;
}

void mcText_setFont(mcObj_t * t, mcFont_t * font)
{
    ((mcObjData_text_t*)t->obj_data)->font = font;
    t->prerender_flg = 1;
}

void mcText_setWidth(mcObj_t * t, uint16_t width)
{
    ((mcObjData_text_t*)t->obj_data)->usr_max_width = width;
    t->prerender_flg = 1;
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
        ptr_cpy = (char *)realloc( ptr_cpy, (len+1)*sizeof(char) ); //len + \0
    
    // copying characters from str to ptr_cpy 
    uint16_t i;
    for (i = 0; str[i] != '\0'; ++i)
        ptr_cpy[i] = str[i]; 
    ptr_cpy[i] = '\0'; 

    ((mcObjData_text_t*)t->obj_data)->str = ptr_cpy;
    t->prerender_flg = 1;
}

void mcText_setAlign(mcObj_t * t, mcTextAlign_t txt_align)
{
    ((mcObjData_text_t*)t->obj_data)->align = txt_align;
    t->prerender_flg = 1;
}

static void mcText_deleteObjData(mcObj_t * t)
{
    free( ((mcObjData_text_t*)t->obj_data)->str );
    free( ((mcObjData_text_t*)t->obj_data)->t_line );
    free( ((mcObjData_text_t*)t->obj_data) );
}

static void mcText_preRender(mcObj_t * t)
{
    mcTextLine_t line = {.x=0, .y=0, .start_idx=0, .end_idx=0};
    mcObjData_text_t* data = ((mcObjData_text_t*)t->obj_data);
    mcFont_t * f = ((mcObjData_text_t*)t->obj_data)->font;
    
    // allocate memory for the parameters of one line
    data->t_line = (mcTextLine_t*) malloc(sizeof(mcTextLine_t));
    if(data->t_line == NULL)
        return;

    data->N_lines = 1;

    line = getLine(t, line.end_idx);
    data->t_line[0] = line;
    if(line.end_idx == -1){
        printf("no char available\n");
        return;
    }
    
    while(data->str[line.end_idx+1])
    {
        // reallocate memory for the parameters
        data->t_line = (mcTextLine_t*) realloc(data->t_line, (data->N_lines+1)*sizeof(mcTextLine_t));
        if(data->t_line == NULL){
            return;
        }
        
        line = getLine(t, line.end_idx+1);
        data->t_line[data->N_lines] = line;
        data->N_lines++;
    }

    for(uint16_t i = 0; i<data->N_lines; i++){
        printf("L=%u \t s=%u \t e=%u\n", i, data->t_line[i].start_idx, data->t_line[i].end_idx);
    }

    // compute maximum width and height of the entire paragraph
    t->geom.w = 0;
    for(uint16_t l = 0; l<data->N_lines; l++)
        t->geom.w  = _MAX_( t->geom.w , data->t_line[l].w );
    
    t->geom.h = (f->line_height * data->N_lines) + (f->line_spacing*(data->N_lines+1));
    
    // in case the object is aligned, compute the new x and y coordinates
    mcObj_align_prerender(t);

    //compute x and y of each line
    switch (data->align)
    {
    case MC_TEXT_ALIGN_CENTER:
        data->t_line[0].x = (t->geom.w - data->t_line[0].w)/2 + t->geom.x;
        data->t_line[0].y = t->geom.y + f->line_spacing;
        for(uint16_t l = 1; l<data->N_lines; l++){
            data->t_line[l].x = (t->geom.w - data->t_line[l].w)/2 + t->geom.x;
            data->t_line[l].y = data->t_line[l-1].y + f->line_height + f->line_spacing;
        }
        break;
    
    case MC_TEXT_ALIGN_RIGHT:
        data->t_line[0].x = (t->geom.w - data->t_line[0].w) + t->geom.x;
        data->t_line[0].y = t->geom.y + f->line_spacing;
        for(uint16_t l = 1; l<data->N_lines; l++){
            data->t_line[l].x = (t->geom.w - data->t_line[l].w) + t->geom.x;
            data->t_line[l].y = data->t_line[l-1].y + f->line_height + f->line_spacing;
        }
        break;

    case MC_TEXT_ALIGN_LEFT:
    default:
        data->t_line[0].x = t->geom.x;
        data->t_line[0].y = t->geom.y + f->line_spacing;
        for(uint16_t l = 1; l<data->N_lines; l++){
            data->t_line[l].x = t->geom.x;
            data->t_line[l].y = data->t_line[l-1].y + f->line_height + f->line_spacing;
        }
        break;
    }

    t->prerender_flg = 0;
}

static void mcDraw_text(mcObj_t * t)
{
    mcObjData_text_t * data = ((mcObjData_text_t*)t->obj_data);
    mcFont_t * f = ((mcObjData_text_t*)t->obj_data)->font;
    char * str = ((mcObjData_text_t*)t->obj_data)->str;

    int16_t idx;
    uint32_t uc, ch_idx;
    uint8_t n_char; 

    uint16_t x_char, y_char;

    for(uint16_t l=0; l<data->N_lines; l++)
    {
        idx = data->t_line[l].start_idx;

        x_char = data->t_line[l].x;
        
        while(idx <= data->t_line[l].end_idx)
        {
            // get unicode code point and its number of used bytes "n_char"
            uc = get_unicode(str+idx, &n_char);
            printf("uc_code_point = %u\n", uc);

            // check if glyph is included in font & get ch_idx
            if(!get_chIdx(uc, &ch_idx, f))
                return;

            //set the appropiate vertical offset and draw the char
            y_char = data->t_line[l].y + f->ch_info[ch_idx].y_offset;
            mcDraw_char(ch_idx, x_char, y_char, f);

            // move cursor
            x_char += f->ch_info[ch_idx].w + f->fix_kern;
            idx += n_char;
        }
    }
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
    mcObjData_text_t* data = ((mcObjData_text_t*)obj->obj_data);
    mcFont_t * f = ((mcObjData_text_t*)obj->obj_data)->font;
    uint16_t idx = s_idx;
    
    // don't count initial spaces
    while(data->str[idx] == ' '){
        idx++;
    }

    mcTextLine_t line = {.x=0, .y=0, .w=0, .start_idx = idx, .end_idx = idx};
    uint32_t uc, ch_idx;
    uint8_t n_ch;
    uint16_t pix_len = 0;

    while( pix_len<=data->usr_max_width && data->str[idx] )
    {
        // if a space is detected, set end_idx to the previous char
        if(data->str[idx] == ' '){
            line.end_idx = idx - 1;
            line.w = pix_len - f->fix_kern;
        }
        uc = get_unicode( (data->str)+idx, &n_ch );
        if(!get_chIdx(uc, &ch_idx, f)){
            line.start_idx = -1;
            line.end_idx = -1;
            return line;
        }

        pix_len += f->ch_info[ch_idx].w + f->fix_kern;
        idx += n_ch;
    }
    // if end of text, set end_idx to the previous char
    if(!data->str[idx]){
        line.end_idx = idx - 1;
        line.w = pix_len - f->fix_kern;
    }
    // if the word width is higher than the line width, the word is split
    if(line.start_idx == line.end_idx){
        line.end_idx = idx - n_ch;
        line.w = pix_len - f->fix_kern;
    }
    
    return line;
}

