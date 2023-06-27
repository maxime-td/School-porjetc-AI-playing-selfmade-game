#include <stdio.h>
#include <time.h>
#include "graph.h"

int main()
{
    srand(time(NULL));
    int n=0;
    sommet_t ** tab = genTabSommets(&n, 800, 800);
    printTabCoord(tab, &n);
    printf("--------------------------------------------\n");
    tabToGraph(tab, 0, n-1);
    printTabCoord(tab, &n);

    makeNewLinks(25, tab, &n);
    printTabCoord(tab, &n);
    int ** TableauDistances = distTab(tab, &n);
    printDistTab(TableauDistances, &n);

    /*
    int y = 0, x;
    int width, height, w_window = 800, h_window = 800, final_width = w_window/2;
    SDL_bool program_on = SDL_TRUE;
    SDL_Event event;   

    SDL_DisplayMode dm;
    SDL_Renderer* renderer;

    SDL_Window *window = NULL;
*/
    

    /* Initialisation de la SDL  + gestion de l'échec possible 
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Error : SDL initialisation - %s\n",
                SDL_GetError()); // l'initialisation de la SDL a échoué
        exit(EXIT_FAILURE);
    }
*/
    /* recupere la taille de l'ecran 
    if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
    {
        SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
        return 1;
    }

    width = dm.w;
    height = dm.h;
*/
    /* Création de la fenêtre de gauche 
    window = SDL_CreateWindow(
        "Goutte",    // codage en utf8, donc accents possibles
        width/2-w_window/2, 0,                  // coin haut gauche en haut gauche de l'écran
        w_window, h_window,              // largeur = 400, hauteur = 300
        SDL_WINDOW_RESIZABLE);

    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 250);
    SDL_RenderClear(renderer);
    

    if (window == NULL)
    {
        SDL_Log("Error : SDL window 1 creation - %s\n",
                SDL_GetError()); // échec de la création de la fenêtre
        SDL_Quit();              // On referme la SDL
        exit(EXIT_FAILURE);
    }

    SDL_GetWindowPosition(window, &x, &y);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 250);
    SDL_Delay(100);
    drawGraph(renderer, tab, n);
    
    SDL_RenderPresent(renderer);

    while (program_on)
    {
        /* Gestion des événements 
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    program_on = SDL_FALSE;
                    break;
                default :
                    break;
            }
        }
    }
*/
    return 0;
}