#include <SDL2/SDL.h>
#include <stdio.h>

/************************************/
/*  exemple de création de fenêtres */
/************************************/

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    int continuer = 1, direction = 0;
    SDL_Window
        *window = NULL;
    SDL_Renderer
        *renderer = NULL;

    int x = 20, y = 20, width = 100, heigh = 100;
    SDL_Rect carre = {x, y, width, heigh};

    SDL_Event event;

    /* Initialisation de la SDL  + gestion de l'échec possible */
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Error : SDL initialisation - %s\n",
                SDL_GetError()); // l'initialisation de la SDL a échoué
        exit(EXIT_FAILURE);
    }

    /* Création de la fenêtre de gauche */
    window = SDL_CreateWindow(
        "X-TREM GRAVITY",        // codage en utf8, donc accents possibles
        650, 200,              // coin haut gauche en haut gauche de l'écran
        800, 800,              // largeur = 400, hauteur = 300
        0); // redimensionnable
    if (window == NULL)
    {
        SDL_Log("Error : SDL window 1 creation - %s\n",
                SDL_GetError()); // échec de la création de la fenêtre
        SDL_Quit();              // On referme la SDL
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);

    while (continuer)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                continuer = 0;
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_UP)
                    y -= 20;
                if (event.key.keysym.sym == SDLK_RIGHT && x<700)
                    x += 10;
                if (event.key.keysym.sym == SDLK_LEFT && x>0)
                    x -= 10;
                break;

            default:
                break;
            }
            break;
        }
        if (y<700)
            {
                y+=5;
                SDL_Delay(18);
            }
        carre.x = x;
        carre.y = y;

        SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &carre);
        SDL_RenderPresent(renderer);
    }
    SDL_RenderClear(renderer);
    SDL_Delay(20);
    SDL_RenderPresent(renderer);

    /* et on referme tout ce qu'on a ouvert en ordre inverse de la création */
    SDL_DestroyWindow(window);

    SDL_Quit(); // la SDL

    return 0;
}