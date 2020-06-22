#include "mc-obj.h"

#include <stdio.h>

#define _TEST_OBJ_ 0

 /**
  *  @brief: creates an object, but the data struct
  * is not malloc'd.
  * @parent: pointer to its parent obj
  * @scr: pointer to the desired screen
  * @return: a pointer to the created obj */
mcObj_t* mcObj_create(mcObj_t *parent, mcObj_t *scr)
{
    /* allocate memory for "obj" */
    mcObj_t* obj;
    obj = (mcObj_t*)malloc( sizeof(mcObj_t) );
    if(obj == NULL)
        return NULL;

    if(parent==NULL && scr==NULL){ //screen obj
        obj->parent = NULL;
        obj->scr = NULL;

    }else if(parent==scr){ //parent object and screen are the same
        obj->parent = scr;
        obj->scr = scr;

        ll_addBack(&(scr->child_list), obj);    /* store the obj in screen child list */

    }else{// parent object and screen are different
        obj->parent = parent;
        obj->scr = scr;

        ll_addBack(&(parent->child_list), obj); /* store obj in the parent child list */
        ll_addBack(&(scr->child_list), obj);    /* store obj in the screen child list */
    }

    /* create an empty child list for the current object */
    obj->child_list = ll_create();

    /* define the default geometry */
    obj->geom.x = 0;
    obj->geom.y = 0;
    obj->geom.w = 0;
    obj->geom.h = 0;
    obj->geom.coord_type = screen;
    obj->geom.align.pos = MC_ALIGN_CENTER;
    obj->geom.align.ref_obj = NULL;

    /* by default, obj_data is null */
    obj->obj_data = NULL;

    /* initialize callback to null */
    obj->delete_obj_data_cb = NULL;
    obj->drawToBuffer_cb = NULL;

    return obj;
}

/**
 * @brief: Not only deletes an object from the proper screen child_list, but
 * also removes all the objects found in the object child_list.
 * @obj: pointer to the object to be deleted
 */
void mcObj_delete(mcObj_t * obj)
{
    while(obj->child_list.size)
    {
        #if _TEST_OBJ_==1
        printf("o = %p Has children, removing its children first.\n", obj);
        #endif
        mcObj_delete((mcObj_t *)obj->child_list.head->data);
    }

    if(obj->parent == NULL && obj->scr==NULL){ 
        #if _TEST_OBJ_==1
        printf("o = %p is this is a screen object, which is not allowed to be removed\n", obj);
        #endif
        //this is a screen object, which is not allowed to be removed
    }
    else if(obj->parent == obj->scr){
        #if _TEST_OBJ_==1
        printf("o = %p is now removed\t[case: obj->parent == obj->scr]\n", obj);
        #endif
        //this is a normal object, but its screen is also its parent,
        //so the object will only be removed from scr child_list
        ll_searchData_deleteItem(&obj->scr->child_list, obj);
        
        free(obj);
        obj = NULL;
    }
    else{
        #if _TEST_OBJ_==1
        printf("o = %p is now removed\t[case: obj->parent != obj->scr]\n", obj);
        #endif 
        //this is a normal object, whose screen and parent objects are
        //different. The object must be deleted from scr and parent 
        //child_lists

        //remove the object from the screen list
        ll_searchData_deleteItem(&obj->scr->child_list, obj);

        //remove it from the parent screen child_list
        ll_searchData_deleteItem(&obj->parent->child_list, obj);

        free(obj);
        obj = NULL;
    }
}


/* @brief: set the alignment parameters to align one object to another one
 * @obj: object to be aligned
 * @align_obj: reference object used to align obj
 * @alignment_type: predefined alignment
 * @x_offset: given a predefined alignment, "obj" can be moved along
 * x axis using this variable
 * @y_offset: given a predefined alignment, "obj" can be moved along
 * y axis using this variable */
void mcObj_align(mcObj_t * obj, mcObj_t * align_obj, mcRelPos_t alignment_type,
    int16_t x_offset, int16_t y_offset)
{
    obj->geom.align.ref_obj = align_obj;
    obj->geom.align.pos = alignment_type;
    obj->geom.align.x_offs = x_offset;
    obj->geom.align.y_offs = y_offset;

    obj->prerender_flg = 1;
}

/* @brief: aligns one object to another one (called by obj->preRender_cb)
 * @obj: object to be aligned with obj->geom.align.ref_obj 
 */ 
void mcObj_align_prerender(mcObj_t * obj)
{
    if(obj->geom.align.ref_obj != NULL)
    {
        // pointers to the interesting structs are created to simplify the code 
        mcGeo_t* o = &(obj->geom);
        mcGeo_t* r = &(obj->geom.align.ref_obj->geom);

        switch(obj->geom.align.pos)
        {
            case MC_ALIGN_CENTER:
                o->x = r->x + r->w/2 - o->w/2;
                o->y = r->y + r->h/2 - o->h/2;
                break;
            case MC_ALIGN_IN_TOP_LEFT:
                o->x = r->x;
                o->y = r->y;
                break;
            case MC_ALIGN_IN_TOP_MID:
                o->x = r->x + r->w/2 - o->w/2;
                o->y = r->y;
                break;
            case MC_ALIGN_IN_TOP_RIGHT:
                o->x = r->x + r->w - o->w;
                o->y = r->y;
                break;
            case MC_ALIGN_IN_BOTTOM_LEFT:
                o->x = r->x;
                o->y = r->y + r->h - o->h;
                break;
            case MC_ALIGN_IN_BOTTOM_MID:
                o->x = r->x + r->w/2 - o->w/2;
                o->y = r->y + r->h - o->h;
                break;
            case MC_ALIGN_IN_BOTTOM_RIGHT:
                o->x = r->x + r->w - o->w;
                o->y = r->y + r->h - o->h;
                break;
            case MC_ALIGN_IN_LEFT_MID:
                o->x = r->x;
                o->y = r->y + r->h/2 - o->h/2;
                break;
            case MC_ALIGN_IN_RIGHT_MID:
                o->x = r->x + r->w - o->w;
                o->y = r->y + r->h/2 - o->h/2;
                break;
            case MC_ALIGN_OUT_TOP_LEFT:
                o->x = r->x;
                o->y = r->y - o->h;
                break;
            case MC_ALIGN_OUT_TOP_MID:
                o->x = r->x + r->w/2 - o->w/2;
                o->y = r->y - o->h;
                break;
            case MC_ALIGN_OUT_TOP_RIGHT:
                o->x = r->x + r->w - o->w;
                o->y = r->y - o->h;
                break;
            case MC_ALIGN_OUT_BOTTOM_LEFT:
                o->x = r->x;
                o->y = r->y + r->h;
                break;
            case MC_ALIGN_OUT_BOTTOM_MID:
                o->x = r->x + r->w/2 - o->w/2;
                o->y = r->y + r->h;
                break;
            case MC_ALIGN_OUT_BOTTOM_RIGHT:
                o->x = r->x + r->w - o->w;
                o->y = r->y + r->h;
                break;
            case MC_ALIGN_OUT_LEFT_TOP:
                o->x = r->x - o->w;
                o->y = r->y;
                break;
            case MC_ALIGN_OUT_LEFT_MID:
                o->x = r->x - o->w;
                o->y = r->y + r->h/2 - o->h/2;
                break;
            case MC_ALIGN_OUT_LEFT_BOTTOM:
                o->x = r->x - o->w;
                o->y = r->y + r->h - o->h;
                break;
            case MC_ALIGN_OUT_RIGHT_TOP:
                o->x = r->x + r->w;
                o->y = r->y;
                break;
            case MC_ALIGN_OUT_RIGHT_MID:
                o->x = r->x + r->w;
                o->y = r->y + r->h/2 - o->h/2;
                break;
            case MC_ALIGN_OUT_RIGHT_BOTTOM:
            default:
                o->x = r->x + r->w;
                o->y = r->y + r->h - o->h;
                break;
        }
        
        o->x += obj->geom.align.x_offs;

        if(obj->geom.coord_type == cartesian)
            o->y -= obj->geom.align.y_offs;
        else
            o->y += obj->geom.align.y_offs;
    }
}

/**
 * @brief: set top left x,y coordinates of an object
 * @param: x: x coordinate
 * @param: y: y coordinate
 */
void mcObj_setPos(mcObj_t * obj, uint16_t x, uint16_t y)
{
    obj->geom.x = x;
    obj->geom.y = y;

    obj->prerender_flg = 1;
}

/**
 * @brief: set width and height of an object
 * @param: width
 * @param: height
 */
void mcObj_setDim(mcObj_t * obj, int16_t width, int16_t height)
{
    obj->geom.w = width;
    obj->geom.h = height;

    obj->prerender_flg = 1;
}

/**
 * @brief: define an object position and dimmensions using two points
 * @param: point0: x0, y0
 * @param: point1: x1, y1
 */ 
void mcObj_set2Points(mcObj_t * obj, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    /* they are simply stored as x,y,w,h, then in the pre render callback,
     data will be managed for each object type */
    obj->geom.x = x0;
    obj->geom.y = y0;
    obj->geom.w = x1-x0;
    obj->geom.h = y1-y0;
    obj->prerender_flg = 1;
}

/**
 * @brief: set color of an object
 * @param: color
 */
void mcObj_setColor(mcObj_t * obj, mcColor_t color)
{
    obj->geom.color = color;
}

/**
 * @brief: select default screen coordinate or change to cartesian ones
 */ 
void mcObj_selectCoord(mcObj_t * obj, mcCoord_t coord_type)
{
    obj->geom.coord_type = coord_type;
    obj->prerender_flg = 1;
}
