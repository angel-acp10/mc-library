#include <SDL2/SDL.h>
#include "libtest.h"

#define SCR_WIDTH 640
#define SCR_HEIGHT 320

SDL_Window *window;
SDL_Renderer *renderer;

int main(int argc, char **argv)
{
    testFunction();
    _Bool quit = 0;
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_CreateWindowAndRenderer(SCR_WIDTH, SCR_HEIGHT, 0 , &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderPresent(renderer);

    while(!quit)
    {
        if(event.type == SDL_QUIT)
            quit = 1;
        else if(event.type == SDL_KEYDOWN){
            SDL_SetRenderDrawColor(renderer, 0,0,0,0);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            switch (event.key.keysym.sym){
            case SDLK_UP:
                SDL_RenderDrawPoint(renderer, SCR_WIDTH/2, 0);
                break;
            case SDLK_DOWN:
                SDL_RenderDrawPoint(renderer, SCR_WIDTH/2, SCR_HEIGHT-1);
                break;

            case SDLK_RIGHT:
                SDL_RenderDrawPoint(renderer, SCR_WIDTH-1, SCR_HEIGHT/2);
                break;

            case SDLK_LEFT:
                SDL_RenderDrawPoint(renderer, 0, SCR_HEIGHT/2);
                break;
            
            default:
                break;
            }
            SDL_RenderPresent(renderer);
        }
        SDL_RenderPresent(renderer);
        SDL_WaitEvent(&event);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}