#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{
    srand(time(NULL));
    (void)argc;
    (void)argv;
    int y = 0, x, y_fruit, x_fruit;
    int square_size = 50;
    int width, height, w_window = square_size*15, h_window = square_size*15, final_width = w_window/2;
    int speed = square_size, speed_x = 0, speed_y = 0, nbSquare = 3;
    SDL_bool program_on = SDL_TRUE;
    SDL_Event event;   

    SDL_DisplayMode dm;
    SDL_Renderer* renderer;

    SDL_Window *window = NULL;

    x_fruit = rand()%(w_window/square_size);
    y_fruit = rand()%(h_window/square_size);

    SDL_Rect fruit = {x_fruit*square_size+5, y_fruit*square_size+5, 40, 40};
    SDL_Rect squares[w_window*h_window/square_size];

    /* Initialisation de la SDL  + gestion de l'échec possible */
    if (SDL_Init(SDL_INIT_VIDEO)!= 0)
    {
        SDL_Log("Error : SDL initialisation - %s\n",
                SDL_GetError()); // l'initialisation de la SDL a échoué
        exit(EXIT_FAILURE);
    }

    /* recupere la taille de l'ecran */
    if (SDL_GetDesktopDisplayMode(0, &dm)!= 0)
    {
        SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
        return 1;
    }

    width = dm.w;
    height = dm.h;

    /* Création de la fenêtre de jeu */
    window = SDL_CreateWindow(
        "Jeu Snake",    // codage en utf8, donc accents possibles
        width/2-w_window/2, height/2-h_window/2,                  // coin haut gauche en haut gauche de l'écran
        w_window, h_window,              // largeur = 400, hauteur = 300
        SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    /* Création des carrés */
    
    for (int i = 0; i < nbSquare; i++){
        squares[i].x = 0;
        squares[i].y = i*square_size;
        squares[i].w = square_size;
        squares[i].h = square_size;
    }
    
    
    speed_x = speed;
    speed_y = 0;    

    /* Boucle principale du jeu */
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
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_z:
                            speed_x = 0;
                            speed_y = -speed;
                            break;
                        case SDLK_q:
                            speed_x = -speed;
                            speed_y = 0;
                            break;
                        case SDLK_s:
                            speed_x = 0;
                            speed_y = speed;
                            break;
                        case SDLK_d:
                            speed_x = speed;
                            speed_y = 0;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }

        /* Déplacement des carrés */

        if (squares[0].x+speed_x < fruit.x + fruit.w &&
            squares[0].x+speed_x + squares[0].w > fruit.x &&
            squares[0].y+speed_y < fruit.y + fruit.h &&
            squares[0].y+speed_y + squares[0].h > fruit.y)
        {
            nbSquare++;
            squares[nbSquare-1].h = square_size;
            squares[nbSquare-1].w = square_size;

            x_fruit = rand()%(w_window/square_size);
            y_fruit = rand()%(h_window/square_size);
            fruit.x = x_fruit*square_size+5;
            fruit.y = y_fruit*square_size+5;
        }

        for (int i = nbSquare-1; i > 0; i--){
            squares[i].x = squares[i-1].x;
            squares[i].y = squares[i-1].y;

        }

        squares[0].x += speed_x;
        squares[0].y += speed_y;

        /* Vérification des collisions avec les bords de la fenêtre */
        
        if (squares[0].x < 0)
        {
            squares[0].x = w_window-square_size;
        }
        else if (squares[0].x > w_window - squares[0].w)
        {
            squares[0].x = 0;
        }

        if (squares[0].y < 0)
        {
            squares[0].y = h_window - squares[0].h;
        }
        else if (squares[0].y > h_window - squares[0].h)
        {
            squares[0].y = 0;
        }
        
        /* Vérification des collisions avec les carrés */
        for (int i = 1; i < nbSquare; i++){
            if (squares[0].x < squares[i].x + squares[i].w &&
                squares[0].x + squares[0].w > squares[i].x &&
                squares[0].y < squares[i].y + squares[i].h &&
                squares[0].y + squares[0].h > squares[i].y)
            {
                program_on = SDL_FALSE;
            }
        }

        /* Dessin des carrés */
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &fruit);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        for (int i = 0; i < nbSquare; i++){
            SDL_RenderFillRect(renderer, &squares[i]);
        }
        
        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }

    /* Fermeture de la SDL */
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}