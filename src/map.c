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

int coord_sur_chemin(int pX, int pY, sommet_t **tabSom, int n, int frame_size, int largeur)
{
    int x1, x2, x3, x4, y1, y2, y3, y4;
    int delta1, delta2;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if(tabSom[i]->voisin[j]==1)
            {
                x1 = tabSom[i]->x; y1 = tabSom[i]->y - largeur;
                x2 = tabSom[i]->x + largeur; y2 = tabSom[i]->y;
                x3 = tabSom[j]->x - largeur; y3 = tabSom[j]->y;
                x4 = tabSom[j]->x; y4 = tabSom[j]->y + largeur;
                delta1 = (y3-y1)/(x3-x1);
                delta2 = (y4-y2)/(x4-x2);
            }
        }
    }
}

/*
void passage_Ast(SDL_Renderer * render, sommet_t ** tabsomm)
{

}*/