#include <stdio.h>
#include "graph.h"
#include "affiche.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void ast_Partout(SDL_Renderer *render)
{
    int frame_size = 48;
    SDL_Surface
        *surf = IMG_Load("./images/planetes.png");

    SDL_Texture *texture = NULL;

    int alea = 0;
    int angle = 0;
    int ecart = (rand() % 20) - 10;

    SDL_Rect srcrect = {0, 0, frame_size, frame_size}; // position and size of the part of the image to draw
    SDL_Rect dstrect = {100, 100, 54, 54};             // position and size of the destination on the screen

    texture = SDL_CreateTextureFromSurface(render, surf);

    for (int i = 0; i < W; i += frame_size + ecart)
    {
        ecart = (rand() % 20) - 10;
        dstrect.x = i;
        for (int j = 0; j < H; j += frame_size + ecart)
        {
            ecart = (rand() % 20) - 10;
            dstrect.y = j;
            alea = (rand() % 16) * 48;
            printf("%d\n", alea);
            srcrect.x = alea;
            angle = rand() % 360;
            SDL_RenderCopyEx(render, texture, &srcrect, &dstrect, angle, NULL, 0);
        }
    }
    SDL_RenderPresent(render);

    // SDL_Delay(3000);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surf);
}

int coord_sur_chemin(int x, int y, sommet_t **tabSom, int n, int frame_size, int largeur)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if(tabSom[i]->voisin[j]==1)
            {
                
            }
        }
    }
}

/*
void passage_Ast(SDL_Renderer * render, sommet_t ** tabsomm)
{

}*/