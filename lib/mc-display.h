#ifndef _MC_DISPLAY_H_
#define _MC_DISPLAY_H_

/************
 * Includes
 ************/ 
#include <stdint.h>

#define SCR_WIDTH 1280
#define SCR_HEIGHT 640

typedef void(*mc_displayBuffer_cb_t)(uint8_t * buf);

typedef struct{
    uint32_t width;
    uint32_t height;
    uint8_t * buffer;
    mc_displayBuffer_cb_t displayBuffer_cb;
}mc_display_t;

#endif