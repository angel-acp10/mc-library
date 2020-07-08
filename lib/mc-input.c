#include "mc-input.h"

/****************************
 * Public funtion prototypes
 ****************************/ 
/*
void mcInput_init(mcInput_t * i, mcKey_name_t key_name, mcObj_t * ctrlObj)
{
    i->key_name = key_name;
    i->key_state = RELEASED;
    i->ctrlObj = ctrlObj;

    i->def = 0; 
    i->state = 0;
    i->flg = 0;
}
*/
void mcInput_defState(mcInput_t * i, _Bool def_state)
{
    i->def = def_state;
}

void mcInput_process(mcInput_t * i)
{
    if(i->flg){
        i->flg = 0;

        /* translate the raw input into PRESSED or RELEASED 
            depending on the default state "i->def" of the button */
        if(i->def) 
            i->key_state = (i->state)? RELEASED : PRESSED;
        else 
            i->key_state = (i->state)? PRESSED : RELEASED;

        /* callback to perform the changes in the object */
        //i->ctrlObj->keyInput_cb(i->ctrlObj, i->key_name, i->key_state);
    }
}