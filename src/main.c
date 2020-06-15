#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include "mc-display.h"
#include "mc-draw.h"

#include "test-draw.h"
#include "test-text.h"

SDL_Window *window;
SDL_Renderer *renderer;

void display_buffer(uint8_t *buffer);

int main(int argc, char **argv)
{
    _Bool quit = 0;
    SDL_Event event;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(SCR_WIDTH, SCR_HEIGHT, 0 , &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderPresent(renderer);

    //create and select the buffer
    printf("SCR_WIDTH*SCR_HEIGHT/8 = %u\n", SCR_WIDTH*SCR_HEIGHT/8);
    uint8_t buf[SCR_WIDTH*SCR_HEIGHT/8]={0};
    mcDraw_selectBuffer(buf);

    //display
    mc_display_t disp;
    disp.width = SCR_WIDTH;
    disp.height = SCR_HEIGHT;
    disp.buffer = buf;
    disp.displayBuffer_cb = display_buffer;

    //enables mask to ensure that all pixels are within the buffer
    mcDraw_enableMask(0,SCR_WIDTH,0, SCR_HEIGHT);

    //tests
    test_draw();
    test_text();

    //print buffer to screen
    disp.displayBuffer_cb(buf);

    while(!quit)
    {
        if(event.type == SDL_QUIT)
            quit = 1;
        /*        
        else if(event.type == SDL_KEYDOWN){
            switch (event.key.keysym.sym){
            case SDLK_UP:
                break;
            case SDLK_DOWN:
                break;
            case SDLK_RIGHT:
                break;
            case SDLK_LEFT:
                break;
            default:
                break;
            }
        }
        SDL_RenderPresent(renderer);
        */
        SDL_WaitEvent(&event);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}

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
