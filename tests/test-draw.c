#include <SDL2/SDL.h>
#include "mc-draw.h"
#include "mc-obj.h"

#define SCR_WIDTH 1280
#define SCR_HEIGHT 640

SDL_Window *window;
SDL_Renderer *renderer;

void display_buffer(uint8_t *buffer);

int main()
{
    _Bool quit = 0;
    SDL_Event event;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(SCR_WIDTH, SCR_HEIGHT, 0 , &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderPresent(renderer);
    ///////////////////////////////////////////////////////////////////////////

    //create and select the buffer
    uint8_t buf[SCR_WIDTH*SCR_HEIGHT/8]={0};

    mcObj_t * scr = mcScr_create();
    mcScr_setResolution(scr, SCR_WIDTH, SCR_HEIGHT);
    mcScr_setBuffer(scr, buf);
    mcScr_setDisplayCb(scr, display_buffer);
    mcScr_select(scr);

    //filled rectangle
    mcGeo_t fRec;
    fRec.x = 1200;//10-60
    fRec.y = 600;//10-30
    fRec.w = 50;
    fRec.h = 20;
    fRec.color = white;
    mcDraw_fRectangle(&fRec);

    //rectangle
    mcGeo_t rec;
    rec.x = 110;
    rec.y = 20;
    rec.w = 10;
    rec.h = 40;
    rec.color = white;
    mcDraw_rectangle(&rec);

    //hLine
    mcGeo_t hLine;
    hLine.x = 10;
    hLine.y = 40;
    hLine.w = 60;
    hLine.color = white;
    mcDraw_xLine(&hLine);

    //vLine
    mcGeo_t vLine;
    vLine.x = 100;
    vLine.y = 5;
    vLine.h = 25;
    vLine.color = white;
    mcDraw_yLine(&vLine);

    //draw a single pixel
    mcDraw_pixel(100, 60, white);


    //print buffer to screen
    ((mcObjData_scr_t*)scr->obj_data)->send_buf_cb(buf);

    ///////////////////////////////////////////////////////////////////////////
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

