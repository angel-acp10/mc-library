#ifndef _MC_TEXT_H_
#define _MC_TEXT_H_

/************
 * Includes
 ************/ 
#include <stdint.h>
#include "mc-font.h"
#include "mc-obj.h"

/************
 * Typedefs
 ************/ 
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

/******************************
 * Public function prototypes
 ******************************/ 
/**
 * @brief: creates a text object
 * @parent: parent object of the returned one.
 * @scr: screen where the returned object will be displayed.
 * @return: new text object
 */ 
mcObj_t * mcText_create(mcObj_t *parent, mcObj_t *scr);

/**
 * @brief: set a font to the text object 
 * @t: text object
 * @font: font to be used by the text object
 */
void mcText_setFont(mcObj_t * t, mcFont_t * font);

/**
 * @brief: set the text width. Actually, this is not the maximum width 
 * of the text object, which will depend on the text to be shown.
 * @t: text object
 * @width: maximum width of the text object. The final object width 
 * will be the maximum length among all text lines.
 */
void mcText_setWidth(mcObj_t * t, uint16_t width);

/**
 * @brief: set the string to the object
 * @t: text object
 * @str: string to be displayed
 */ 
void mcText_setStr(mcObj_t * t, char * str);

/**
 * @brief: set the text alignment.
 * @t: text object
 * @txt_align: parameter which defines the text alignment
 */
void mcText_setAlign(mcObj_t * t, mcTextAlign_t txt_align);

/**
 * @brief: set text margins
 * @left: left text margin in px
 * @right: right text margin in px
 * @top: top text margin in px
 * @bottom: bottom text margin in px
 */
void mcText_setMargins(mcObj_t * t, uint8_t left, uint8_t right, uint8_t top, uint8_t bottom);

/**
 * @brief: sets a rectangle behind the text
 * @t: text object
 * @shape_type: defines the rectangle appearance. 
 * The possible parameters are listed below:
 *      - nothing: no rectangle is drawn
 *      - border: just the rectangle contour is drawn
 *      - filled: a filled and solid rectangle is drawn
 */ 
void mcText_defineBackground(mcObj_t * t, mcShape_t shape_type);

#endif
