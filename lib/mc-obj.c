#include "mc-obj.h"

#include <stdlib.h>
#include <stdio.h>

 /* @brief: creates an object, but the data struct
  * is not malloc'd.
  * @parent: pointer to its parent obj
  * @scr: pointer to the desired screen
  * @return: a pointer to the created obj */
mcObj_t* mcObj_create(mcObj_t *parent, mcObj_t *scr)
{
    /* allocate memory for "obj" */
    mcObj_t* obj;
    obj = (mcObj_t*)malloc( sizeof(mcObj_t) );

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
    obj->align = MC_ALIGN_CENTER;
    obj->geom.alignObj = NULL;

    obj->refreshBuffer = 1;

    return obj;
}

/* @brief: aligns one object respect to another one
 * @obj: object to be aligned
 * @alignObj: reference object used to align obj
 * @align: predefined alignment
 * @x_offset: given a predefined alignment, "obj" can be moved along
 * x axis using this variable
 * @y_offset: given a predefined alignment, "obj" can be moved along
 * y axis using this variable */
void mcObj_align(mcObj_t * obj, mcObj_t * align_obj, mcAlign_t align, int16_t x_offset, int16_t y_offset)
{
    /* save the alignment configuration */
    obj->align = align;
    obj->geom.alignObj = align_obj;

    if(align_obj != NULL)
    {
        /* two pointers to the interesting structs are
        created to simplify the code */
        mcGeo_t* o = &(obj->geom);
        mcGeo_t* r = &(align_obj->geom);

        switch(align)
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

        o->x += x_offset;
        o->y += y_offset;
    }
}