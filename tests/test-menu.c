#include "mc-menu.h"
#include "mc-draw.h"
#include "mc-text.h"
#include <SDL2/SDL.h>
#include <stdio.h>

#define SCR_WIDTH 1280
#define SCR_HEIGHT 640

SDL_Window *window;
SDL_Renderer *renderer;

uint8_t buf[SCR_WIDTH*SCR_HEIGHT/8];
mcObj_t * scr;

void display_buffer(uint8_t *buffer)
{
    uint32_t pixels = SCR_WIDTH*SCR_HEIGHT;
    uint32_t x = 0, y = 0, idx = 0;
    uint8_t bit = 0;

    while(pixels){
        if( buffer[idx] & 0x80>>bit  )
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        else
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderDrawPoint(renderer, x,y);

        x++;
        if(x >= SCR_WIDTH){
            x = 0;
            y++;
        }
        bit++;
        if(bit >= 8){
            bit = 0;
            idx++;
        }
        pixels--;
    }
    SDL_RenderPresent(renderer);
}

void refresh_frame(){
    scr->drawToBuffer_cb(scr); 
    ((mcObjData_scr_t*)scr->obj_data)->send_buf_cb(buf);
}

int main()
{
    // SDL init
    _Bool quit = 0;
    SDL_Event event;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(SCR_WIDTH, SCR_HEIGHT, 0 , &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderPresent(renderer);

    // mc lib init
    scr = mcScr_create();
    mcScr_setResolution(scr, SCR_WIDTH, SCR_HEIGHT);
    mcScr_setBuffer(scr, buf);
    mcScr_setDisplayCb(scr, display_buffer);
    mcScr_select(scr);

    // menu
    mcObj_t * menu = mcMenu_create(scr, scr);
    mcObjData_menu_t * data = (mcObjData_menu_t*)menu->obj_data;
    
    

    mcMenu_setItems(menu, "Item1|Item2|item3|item4");
    //printf("%s\n", ((mcObjData_text_t*)((mcObj_t*)data->item_list.tail->data)->obj_data)->str);

    mcMenu_setItems(menu, "I1|I2");
    //printf("%s\n", string);
    //printf("%s\n", ((mcObjData_text_t*)item->obj_data)->str );
    //char * string = ((mcObjData_text_t*)((mcObj_t*)data->item_list.head->data)->obj_data)->str;
    //printf("%s\n", ((mcObjData_text_t*)((mcObj_t*)data->item_list.tail->data)->obj_data)->str);

    //refresh_frame();
    //mcObj_delete(txt);

    // sdl close window
    while(!quit)
    {
        if(event.type == SDL_QUIT)
            quit = 1;

        SDL_WaitEvent(&event);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();


    return 0;
}