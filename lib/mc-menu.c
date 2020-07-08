#include "mc-menu.h"
#include "mc-text.h"
#include <stdio.h>

/*****************************
 * Static function prototypes
 *****************************/ 
static void mcMenu_deleteObjData(mcObj_t * m);
static void mcMenu_preRender(mcObj_t * m);
static void mcDraw_menu(mcObj_t * m);

/******************************
 * Public function definitions
 ******************************/ 
/**
 * @brief: creates a menu object
 * @parent: parent object of the returned one.
 * @scr: screen where the returned object will be displayed.
 * @return: new text object
 */ 
mcObj_t * mcMenu_create(mcObj_t *parent, mcObj_t *scr)
{
    mcObj_t * m = mcObj_create(parent, scr);

    /* allocate memory for obj_data */
    m->obj_data = (mcObjData_menu_t*)malloc( sizeof(mcObjData_menu_t) );
    if(m->obj_data == NULL)
        return NULL;
    
    /* init obj_data */
    ((mcObjData_menu_t*)m->obj_data)->title_font = NULL;
    ((mcObjData_menu_t*)m->obj_data)->item_font = NULL;
    ((mcObjData_menu_t*)m->obj_data)->item_list = ll_create();
    ((mcObjData_menu_t*)m->obj_data)->v_spacing = 2;
    ((mcObjData_menu_t*)m->obj_data)->h_spacing = 2;
    ((mcObjData_menu_t*)m->obj_data)->title = mcText_create(NULL, scr);

    /* init coordinates and color */
    m->geom.x = 0;
    m->geom.y = 0;
    m->geom.w = 100;
    m->geom.h = 50;
    m->geom.color = white;

    /* callbacks */
    m->delete_obj_data_cb = mcMenu_deleteObjData;
    m->preRender_cb = mcMenu_preRender;
    m->drawToBuffer_cb = mcDraw_menu;

    m->prerender_flg = 1;
    
    return m;
}

/**
 * @brief: set a string as a title
 * @m: menu object
 * @str: string to be used as title
 */
void mcMenu_setTitle(mcObj_t * m, char * str)
{
    mcObj_t * title = ((mcObjData_menu_t*)m->obj_data)->title;
    mcText_setStr(title, str);
}

/**
 * @brief: set a font to the menu object 
 * @m: menu object
 * @font: font to be used by the menu object
 */
void mcMenu_setTitleFont(mcObj_t * m, mcFont_t * font)
{
    ((mcObjData_menu_t*)m->obj_data)->title_font = font;
    m->prerender_flg = 1;
}

/**
 * @brief: set a font to the menu object 
 * @m: menu object
 * @font: font to be used by the menu object
 */
void mcMenu_setItemFont(mcObj_t * m, mcFont_t * font)
{
    ((mcObjData_menu_t*)m->obj_data)->item_font = font;
    m->prerender_flg = 1;
}

/**
 * @brief: set the string to the object
 * @m: menu object
 * @item_str: string which contains the items of the menu.
 * Each item is separated by | , for example:
 *  "Item1|Item2|Item3|Item4"
 */ 
void mcMenu_setItems(mcObj_t * m, char * item_str)
{
    mcObjData_menu_t * data = ((mcObjData_menu_t *)m->obj_data);

    // delete all items
    while( data->item_list.size ){
        mcObj_t * it = (mcObj_t*)data->item_list.head->data; //item

        //delete object from item_list
        ll_deleteFront( &(data->item_list) );

        // delete the entire object
        mcObj_delete(it);
    }

    // identify each item and save it in the linked list
    mcObj_t * item;
    char s[50];
    uint8_t i = 0;
    while(*item_str)
    {
        if(*item_str == '|'){ // each item is separated by '|'
            s[i] = '\0';
            i = 0;
            item = mcText_create(NULL, m->scr);
            mcText_setStr(item, s);
            //printf("%s\n", ((mcObjData_text_t*)item->obj_data)->str );
            ll_addBack( &(data->item_list), item );
            //printf("%s\n", ((mcObjData_text_t*)((mcObj_t*)data->item_list.tail->data)->obj_data)->str);
        }
        else{
            s[i] = *item_str;
            i++;
        }
        item_str++;
    }
    s[i] = '\0';
    item = mcText_create(NULL, m->scr);
    mcText_setStr(item, s);
    ll_addBack( &(data->item_list), item );
    //printf("%s\n", ((mcObjData_text_t*)item->obj_data)->str );
    m->prerender_flg = 1;

}

/*****************************
 * Static function prototypes
 *****************************/ 
static void mcMenu_deleteObjData(mcObj_t * m)
{
    mcObjData_menu_t * data = ((mcObjData_menu_t *)m->obj_data);

    // delete all items
    while( data->item_list.size ){
        mcObj_t * it = (mcObj_t*)data->item_list.head->data; //item

        //delete object from item_list
        ll_deleteFront( &(data->item_list) );

        // delete the entire object
        mcObj_delete(it);
    }

    free( ((mcObjData_text_t*)m->obj_data) );
}

static void mcMenu_preRender(mcObj_t * m)
{
    //update font

}

static void mcDraw_menu(mcObj_t * m)
{

}

