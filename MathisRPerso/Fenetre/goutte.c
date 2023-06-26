#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    int y = 0, x;
    int width, height, w_window = 400, h_window = 300, final_width = w_window/2;
    SDL_bool program_on = SDL_TRUE;
    SDL_Event event;   

    SDL_DisplayMode dm;
    SDL_Renderer* renderer;

    SDL_Window *window = NULL;

    

    /* Initialisation de la SDL  + gestion de l'échec possible */
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Error : SDL initialisation - %s\n",
                SDL_GetError()); // l'initialisation de la SDL a échoué
        exit(EXIT_FAILURE);
    }

    /* recupere la taille de l'ecran */
    if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
    {
        SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
        return 1;
    }

    width = dm.w;
    height = dm.h;

    /* Création de la fenêtre de gauche */
    window = SDL_CreateWindow(
        "Goutte",    // codage en utf8, donc accents possibles
        width/2-w_window/2, 0,                  // coin haut gauche en haut gauche de l'écran
        w_window, h_window,              // largeur = 400, hauteur = 300
        SDL_WINDOW_BORDERLESS);

    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 0, 100, 150, 250);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    if (window == NULL)
    {
        SDL_Log("Error : SDL window 1 creation - %s\n",
                SDL_GetError()); // échec de la création de la fenêtre
        SDL_Quit();              // On referme la SDL
        exit(EXIT_FAILURE);
    }

    SDL_GetWindowPosition(window, &x, &y);

    while (w_window > final_width)
    {
        w_window -= 10;
        h_window += 10; 
        x += 5;
        y += 10;
        SDL_SetWindowSize(window, w_window, h_window);
        SDL_SetWindowPosition(window, x, y);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }
    

    while (y < height-h_window)
    {
        y += 10;
        SDL_SetWindowPosition(window, x, y);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    while (w_window < width){
        w_window += 80;
        h_window -= 16;
        x -= 40;
        y += 16;
        SDL_SetWindowSize(window, w_window, h_window);
        SDL_SetWindowPosition(window, x, y);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    while (y < height)
    {
        y += 10;
        SDL_SetWindowPosition(window, x, y);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }
    
    /* et on referme tout ce qu'on a ouvert en ordre inverse de la création */
    SDL_DestroyWindow(window); // la fenêtre



    SDL_Quit(); // la SDL

    return 0;
}


