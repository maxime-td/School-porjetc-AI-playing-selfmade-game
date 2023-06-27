#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    int y = 0, x, i = 0;
    int width, height, w_window = 800, h_window = 400, final_width = w_window/2, n_floor = 9, floo_speed = -5;
    int frame_size = 16, size_floor = w_window/n_floor;
    int frame = 0;
    SDL_bool program_on = SDL_TRUE;
    SDL_Event event;   

    SDL_DisplayMode dm;
    SDL_Renderer* renderer;

    SDL_Window *window = NULL;

    SDL_Surface* surface = IMG_Load("slime.png");
    SDL_Texture* texture = NULL;
    SDL_Rect srcrect = {0, 0, frame_size, frame_size+2}; // position and size of the part of the image to draw
    SDL_Rect dstrect = {w_window/2-50, 250, 100, 100}; // position and size of the destination on the screen


    SDL_Rect fg[n_floor+1];

    for (int i = 0; i < n_floor+1; i++){
        fg[i].x = i*size_floor;
        fg[i].y = h_window-size_floor;
        fg[i].w = size_floor;
        fg[i].h = size_floor;
    }
    

    

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
        "Animation",    // codage en utf8, donc accents possibles
        width/2-w_window/2, 0,                  // coin haut gauche en haut gauche de l'écran
        w_window, h_window,              // largeur = 400, hauteur = 300
        SDL_WINDOW_RESIZABLE);

    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    /* Conversion de l'image en un format supporté par SDL */
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL)
    {
        SDL_Log("Error: SDL_CreateTextureFromSurface - %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
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

        if(frame%10 == 0){
            for (int i = 0; i < n_floor+1; i++){
                fg[i].x += floo_speed;
            }

            SDL_SetRenderDrawColor(renderer, 0, 150, 200, 255);
            SDL_RenderClear(renderer);
            for (int i = 0; i < n_floor+1; i++){
                if(fg[i].x < -size_floor){
                    fg[i].x = fg[(i+n_floor)%(n_floor+1)].x+size_floor;
                }
                SDL_SetRenderDrawColor(renderer, 0, 255-50*(i%2), 0, 255);
                SDL_RenderFillRect(renderer, &fg[i]);
            }
            srcrect.x = (srcrect.x+frame_size)%(frame_size*18);
            SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);

            SDL_RenderPresent(renderer);
        }
        SDL_Delay(10);
        frame++;
    }

    if (window == NULL)
    {
        SDL_Log("Error : SDL window 1 creation - %s\n",
                SDL_GetError()); // échec de la création de la fenêtre
        SDL_Quit();              // On referme la SDL
        exit(EXIT_FAILURE);
    }

    SDL_GetWindowPosition(window, &x, &y);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    SDL_DestroyRenderer(renderer);
    /* et on referme tout ce qu'on a ouvert en ordre inverse de la création */
    SDL_DestroyWindow(window); // la fenêtre


    
    SDL_Quit(); // la SDL

    return 0;
}


