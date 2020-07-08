#ifndef _MC_MENU_H_
#define _MC_MENU_H_

/************
 * Includes
 ************/ 
#include "mc-font.h"
#include "mc-obj.h"
#include "linked_list.h"

/************
 * Typedefs
 ************/ 
typedef struct{
    mcFont_t * title_font;
    mcFont_t * item_font;
    
    mcObj_t * title;
    list_t item_list;
    uint16_t v_spacing;
    uint16_t h_spacing;

    //mcTextAlign_t align;
    //mcTextMargin_t margin;
}mcObjData_menu_t;

/******************************
 * Public function prototypes
 ******************************/ 
/**
 * @brief: creates a menu object
 * @parent: parent object of the returned one.
 * @scr: screen where the returned object will be displayed.
 * @return: new text object
 */ 
mcObj_t * mcMenu_create(mcObj_t *parent, mcObj_t *scr);

/**
 * @brief: set a font to the menu object 
 * @m: menu object
 * @font: font to be used by the menu object
 */
void mcMenu_setTitleFont(mcObj_t * m, mcFont_t * font);

/**
 * @brief: set a font to the menu object 
 * @m: menu object
 * @font: font to be used by the menu object
 */
void mcMenu_setItemFont(mcObj_t * m, mcFont_t * font);

/**
 * @brief: set the string to the object
 * @m: menu object
 * @item_str: string which contains the items of the menu.
 * Each item is separated by | , for example:
 *  "Item1|Item2|Item3|Item4"
 */ 
void mcMenu_setItems(mcObj_t * m, char * item_str);

#endif
