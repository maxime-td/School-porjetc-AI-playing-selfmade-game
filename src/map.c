#include <stdio.h>
#include "graph.h"
#include "affiche.h"
#include <SDL2/SDL.h>



void ast_Partout(SDL_Renderer * render)
{
    int frame_size = 54;
     SDL_Surface
        *surf = IMG_Load("../images/planetes.png");

    SDL_Texture *texture = NULL;

    SDL_Rect srcrect = {0, 0, frame_size, frame_size}; // position and size of the part of the image to draw
    SDL_Rect dstrect = {100, 100, 54, 54};           // position and size of the destination on the screen

    texture = SDL_CreateTextureFromSurface(render, surf);
    
    for(int i=0; i<W; i+=54)
    {
        dstrect.x=i;
        for(int j=0; j<H; j+=54)
        {
            dstrect.y=j;
            srcrect.x = 54*rand()%16;
            SDL_RenderCopy(render, texture, &srcrect, &dstrect);
        }
    }
    SDL_RenderPresent(render); 

    SDL_Delay(6000);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surf);
}
