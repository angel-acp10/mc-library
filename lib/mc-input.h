#ifndef _MC_INPUT_H_
#define _MC_INPUT_H_

/************
 * Includes
 ************/ 
#include <stdint.h>
#include "mc-obj.h"

/************
 * Typedefs
 ************/ 
enum{
    NONE=0,
    RISING,
    FALLING
};
typedef uint8_t mcEdge_flg;

enum{
    RELEASED=0,
    PRESSED
};
typedef uint8_t mcKey_state_t;

enum{
    ENTER=0,
    UP,
    DOWN
};
typedef uint8_t mcKey_name_t;

typedef struct{
    // input data
    _Bool flg;
    _Bool state;
    _Bool def; // state when the button is not pressed

    // output data 
    mcKey_name_t key_name;
    mcKey_state_t key_state;

    // object to be controlled
    //mcObj_t * ctrlObj;
}mcInput_t;

/****************************
 * Public funtion prototypes
 ****************************/ 
//void mcInput_init(mcInput_t * i, mcKey_name_t key_name, mcObj_t * ctrlObj);
void mcInput_defState(mcInput_t * i, _Bool def_state);
void mcInput_process(mcInput_t * i);

#endif
