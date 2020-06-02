#ifndef _MC_DISPLAY_H_
#define _MC_DISPLAY_H_

#include <stdint.h>

#define SCR_WIDTH 128
#define SCR_HEIGHT 64
/*
#define _UINT8_T_     8
#define _UINT16_T_    16
#define _UINT32_T_    32

#define BUFFER _UINT8_T_

#if BUFFER == _UINT8_T_
    #define UINTX_T uint8_t
    #define BIT_SIZE 8
    #define LOG2_SIZE 3
    #define MASK 0x80
#elif BUFFER == _UINT16_T_
    #define UINTX_T uint16_t
    #define BIT_SIZE 16
    #define LOG2_SIZE 4
    #define MASK 0x8000
#else
    #define UINTX_T uint32_t
    #define BIT_SIZE 32
    #define LOG2_SIZE 5
    #define MASK 0x80000000
#endif
*/

typedef void(*mc_displayBuffer_cb_t)(uint8_t * buf);

typedef struct{
    uint32_t width;
    uint32_t height;
    uint8_t * buffer;
    mc_displayBuffer_cb_t displayBuffer_cb;
}mc_display_t;

#endif