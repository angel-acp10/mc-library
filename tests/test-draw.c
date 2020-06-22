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

void pixel_test();
void xLine_test();
void yLine_test();
void xyLine_test();
void rectangle_test();
void chess_table_test();

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
    sleep(1);
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

    pixel_test();
    refresh_frame();
    mcObj_delete(scr); //delete all scr child_list but not scr

    xLine_test();
    refresh_frame();
    mcObj_delete(scr); //delete all scr child_list but not scr

    yLine_test();
    refresh_frame();
    mcObj_delete(scr); //delete all scr child_list but not scr

    xyLine_test();
    refresh_frame();
    mcObj_delete(scr); //delete all scr child_list but not scr

    rectangle_test();
    refresh_frame();
    mcObj_delete(scr); //delete all scr child_list but not scr

    chess_table_test();
    refresh_frame();
    mcObj_delete(scr); //delete all scr child_list but not scr

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

void pixel_test()
{
    // test pix obj
    mcObj_t * pix[SCR_WIDTH];
    for(int i = 0; i<SCR_WIDTH; i++){
        pix[i] = mcPix_create(scr,scr);
        mcObj_setPos( pix[i], random()%SCR_WIDTH, random()%SCR_HEIGHT);
        mcObj_setColor(pix[i], white);
    }
}

void xLine_test()
{
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
}

void yLine_test()
{
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
}

void xyLine_test()
{
    //test xyLine (anyline) by plotting a polygon
    const int N = 5;
    mcObj_t * xyLinePol[N];
    int x_offs = SCR_WIDTH/2, y_offs = SCR_HEIGHT/2;
    int L = y_offs;
    int xpol[N], ypol[N];
    float theta = PI/2; 
    for(int n=0; n<N; n++){
        xpol[n] = L*cos(theta);
        ypol[n] = L*sin(theta);
        theta += 2*PI/N;
    }
    for(int i = 0; i<N; i++){
        int j = (i==N-1)? 0 : i+1;
        xyLinePol[i] = mcLine_create(scr,scr);
        mcObj_set2Points(xyLinePol[i], xpol[i]+x_offs, ypol[i]+y_offs, xpol[j]+x_offs, ypol[j]+y_offs);
        mcObj_selectCoord(xyLinePol[i], cartesian);
    }
}

void rectangle_test()
{
    // test empty rectangle and alignment
    mcObj_t * R = mcRect_create(scr,scr);
    mcObj_setDim(R, 100, 100);
    mcObj_align(R, scr, MC_ALIGN_CENTER, 0, 0);

    mcObj_t * RT = mcRect_create(R,scr);
    mcObj_setDim(RT, 100, 100);
    mcObj_align(RT, R, MC_ALIGN_OUT_TOP_MID, 0, -100);

    mcObj_t * RB = mcRect_create(R,scr);
    mcObj_setDim(RB, 100, 100);
    mcObj_align(RB, R, MC_ALIGN_OUT_BOTTOM_MID, 0, 100);

    mcObj_t * RL = mcRect_create(R,scr);
    mcObj_setDim(RL, 100, 100);
    mcObj_align(RL, R, MC_ALIGN_OUT_LEFT_MID, -100, 0);

    mcObj_t * RR = mcRect_create(R,scr);
    mcObj_setDim(RR, 100, 100);
    mcObj_align(RR, R, MC_ALIGN_OUT_RIGHT_MID, 100, 0);

    // test filled rectnagle and alignment
    mcObj_t * rt = mcRect_create(R,scr);
    mcObj_setDim(rt, 30, 30);
    mcObj_align(rt, R, MC_ALIGN_IN_TOP_LEFT, 0, 0);
    mcRect_defineInside(rt, filled);

    mcObj_t * rb = mcRect_create(R,scr);
    mcObj_setDim(rb, 30, 30);
    mcObj_align(rb, R, MC_ALIGN_IN_BOTTOM_LEFT, 0, 0);
    mcRect_defineInside(rb, filled);

    mcObj_t * rl = mcRect_create(R,scr);
    mcObj_setDim(rl, 30, 30);
    mcObj_align(rl, R, MC_ALIGN_IN_TOP_RIGHT, 0, 0);
    mcRect_defineInside(rl, filled);

    mcObj_t * rr = mcRect_create(R,scr);
    mcObj_setDim(rr, 30, 30);
    mcObj_align(rr, R, MC_ALIGN_IN_BOTTOM_RIGHT, 0, 0);
    mcRect_defineInside(rr, filled);
}

void chess_table_test()
{
    const int N = 64, col = 8;
    mcObj_t * sq[N];
    const int a = SCR_HEIGHT/8;
    _Bool fill = 1;

    mcObj_t * outbox = mcRect_create(scr,scr);
    mcObj_setDim(outbox, SCR_HEIGHT, SCR_HEIGHT);
    mcObj_align(outbox,scr,MC_ALIGN_CENTER,0,0);
    mcObj_setColor(outbox, black);

    for(int i=0; i<N; i++){
        if(i==0){
            sq[i] = mcRect_create(outbox, scr);
            mcObj_align(sq[i], outbox, MC_ALIGN_IN_TOP_LEFT, 0, 0);
        
        }else if( i%col == 0 ){
            sq[i] = mcRect_create(sq[i-1], scr);
            mcObj_align(sq[i], sq[i-col], MC_ALIGN_OUT_BOTTOM_MID, 0, 0);
            fill = !fill;
        }else{
            sq[i] = mcRect_create(sq[i-1], scr);
            mcObj_align(sq[i], sq[i-1], MC_ALIGN_OUT_RIGHT_MID, 0, 0);
        }
        mcObj_setDim(sq[i], a, a);
        mcRect_defineInside(sq[i], filled);
        mcObj_setColor(sq[i], fill);
        fill = !fill;
    }
}

