#include <SDL2/SDL.h>
#include "mc-draw.h"
#include "mc-shapes.h"
#include <unistd.h>
#include <math.h>
#include <stdio.h>

#define SCR_WIDTH 1280
#define SCR_HEIGHT 640

#define PI 3.14159265358979323846

SDL_Window *window;
SDL_Renderer *renderer;

uint8_t buf[SCR_WIDTH*SCR_HEIGHT/8];
mcObj_t * scr;

void display_buffer(uint8_t *buffer);
void refresh_frame();

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

    // test pix obj
    mcObj_t * pix[SCR_WIDTH];
    for(int i = 0; i<SCR_WIDTH; i++){
        pix[i] = mcPix_create(scr,scr);
        mcObj_setPos( pix[i], random()%SCR_WIDTH, random()%SCR_HEIGHT);
        mcObj_setColor(pix[i], white);
    }
    refresh_frame();

    //test xLine (horizontal line)
    mcObj_t * xLine[SCR_HEIGHT];
    for(int y=0; y<SCR_HEIGHT; y++){
        xLine[y] = mcLine_create(scr,scr);
        mcObj_set2Points(xLine[y], 0, y, SCR_WIDTH-1, y);
        if(y%2)
            mcObj_setColor(xLine[y], white);
        else
            mcObj_setColor(xLine[y], black);
    }
    refresh_frame();

    //test yLine (vertical)
    mcObj_t * yLine[SCR_WIDTH];
    for(int x=0; x<SCR_WIDTH; x++){
        yLine[x] = mcLine_create(scr,scr);
        mcObj_set2Points(yLine[x], x, 0, x, SCR_HEIGHT-1);
        if(x%2)
            mcObj_setColor(yLine[x], white);
        else
            mcObj_setColor(yLine[x], black);
    }
    refresh_frame();

    //test xyLine (anyline) by plotting a polygon
    const int N = 5;
    mcObj_t * xyLinePol[N];
    int x_offs = SCR_WIDTH/2, y_offs = SCR_HEIGHT/2;
    int R = y_offs;
    int xpol[N], ypol[N];
    float theta = PI/2; 
    for(int n=0; n<N; n++){
        xpol[n] = R*cos(theta);
        ypol[n] = R*sin(theta);
        theta += 2*PI/N;
    }
    for(int i = 0; i<N; i++){
        int j = (i==N-1)? 0 : i+1;
        xyLinePol[i] = mcLine_create(scr,scr);
        mcObj_set2Points(xyLinePol[i], xpol[i]+x_offs, ypol[i]+y_offs, xpol[j]+x_offs, ypol[j]+y_offs);
        mcObj_selectCoord(xyLinePol[i], cartesian);
    }
    refresh_frame();

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
    sleep(2);
    mcObj_delete(scr); //delete all scr child_list but not scr
}
