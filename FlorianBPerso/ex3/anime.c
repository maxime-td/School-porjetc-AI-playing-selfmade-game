#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

/************************************/
/*  exemple de création de fenêtres */
/************************************/

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    int i = 0, vitesseBoing = 10, vitesseD = 20;
    int width, w_window = 400, h_window = 300;
    int frame_size = 20;

    SDL_bool program_on = SDL_TRUE;
    SDL_Event event;

    SDL_DisplayMode dm;
    SDL_Window
        *window = NULL;
    SDL_Renderer
        *renderer = NULL;
    SDL_Surface
        *surf = IMG_Load("Slime_Boing.BMP");

    SDL_Texture *texture = NULL;
    SDL_Rect srcrect = {0, 48, frame_size, frame_size}; // position and size of the part of the image to draw
    SDL_Rect dstrect = {100, 100, 100, 100};            // position and size of the destination on the screen

    /* Initialisation de la SDL  + gestion de l'échec possible */
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Error : SDL initialisation - %s\n",
                SDL_GetError()); // l'initialisation de la SDL a échoué
        exit(EXIT_FAILURE);
    }
       
    if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
    {
        SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
        return 1;
    }
    width = dm.w;
    height = dm.h;

    /* Création de la fenêtre de gauche */
    window = SDL_CreateWindow(
        "Slime Sprite", // codage en utf8, donc accents possibles
        width/2-w_window/2, 0,                  // coin haut gauche en haut gauche de l'écran
        w_window, h_window,
        0);               // redimensionnable

    if (window == NULL)
    {
        SDL_Log("Error : SDL window 1 creation - %s\n",
                SDL_GetError()); // échec de la création de la fenêtre
        SDL_Quit();              // On referme la SDL
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    /* Conversion de l'image en un format supporté par SDL */
    texture = SDL_CreateTextureFromSurface(renderer, surf);
    if (texture == NULL)
    {
        SDL_Log("Error: SDL_CreateTextureFromSurface - %s\n", SDL_GetError());
        SDL_FreeSurface(surf);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    /* Dessin de la partie de l'image sur l'écran */
    SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
    SDL_RenderPresent(renderer);

    while (program_on)
    {
        /* Gestion des événements */
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

        SDL_RenderClear(renderer);
        srcrect.x = (srcrect.x+frame_size)%(frame_size*8);
        SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
        SDL_RenderPresent(renderer);
        SDL_Delay(300);
    }

    SDL_GetWindowPosition(window, &x, &y);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surf);
    SDL_DestroyRenderer(renderer);
    /* et on referme tout ce qu'on a ouvert en ordre inverse de la création */
    SDL_DestroyWindow(window); // la fenêtre

    /*
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
                    if (event.key.keysym.sym == SDLK_RIGHT && x < 700)
                        x += 10;
                    if (event.key.keysym.sym == SDLK_LEFT && x > 0)
                        x -= 10;
                    break;

                default:
                    break;
                }
                break;
            }
            if (y < 700)
            {
                y += 5;
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
    */
    /* et on referme tout ce qu'on a ouvert en ordre inverse de la création */


    SDL_Quit(); // la SDL

    return 0;
}