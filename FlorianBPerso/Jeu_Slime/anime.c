#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char **argv)
{
    srand(time(NULL));
    (void)argc;
    (void)argv;

    int i = 0, j=0, vitesseBoing = 10, vitesseD = 20;
    int width, w_window = 400, h_window = 300;
    int frame_sizeSlime = 20;
    int frame_sizeCoin = 138;

    SDL_bool program_on = SDL_TRUE;
    SDL_Event event;

    SDL_DisplayMode dm;
    SDL_Window
        *window = NULL;
    SDL_Renderer
        *renderer = NULL;
    SDL_Surface
        *surf = IMG_Load("Slime_Boing-5.png"),
        *coinSurf = IMG_Load("coin.png"),
        *fond = IMG_Load("fond_herbe.jpg");

    SDL_Texture *texture = NULL;
    SDL_Texture *textFond = NULL;
    SDL_Texture *textCoin = NULL;
    SDL_Rect srcrect = {0, 0, frame_sizeSlime, frame_sizeSlime}; // position and size of the part of the image to draw
    SDL_Rect dstrect = {100, 100, 100, 100};           // position and size of the destination on the screen

    SDL_Rect srcrectCoin = {0, 0, frame_sizeCoin, frame_sizeCoin}; // position and size of the part of the image to draw
    SDL_Rect dstrectCoin = {0, 0, 75, 75};           // position and size of the destination on the screen

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
    w_window = dm.w;
    h_window = dm.h;

    dstrectCoin.x = rand()%(w_window-frame_sizeCoin);
    dstrectCoin.y = rand()%(h_window-frame_sizeCoin);

    SDL_Rect srcrectFond = {0, 0, 1497, 1500};         // position and size of the part of the image to draw
    SDL_Rect dstrectFond = {0, 0, w_window, h_window}; // position and size of the destination on the screen

    /* Création de la fenêtre de gauche */
    window = SDL_CreateWindow(
        "Slime Sprite",              // codage en utf8, donc accents possibles
        width / 2 - w_window / 2, 0, // coin haut gauche en haut gauche de l'écran
        w_window, h_window,
        0); // redimensionnable

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
    textFond = SDL_CreateTextureFromSurface(renderer, fond);
    textCoin = SDL_CreateTextureFromSurface(renderer, coinSurf);
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
    SDL_RenderCopy(renderer, textFond, &srcrectFond, &dstrectFond);
    SDL_RenderCopy(renderer, textCoin, &srcrectCoin, &dstrectCoin);
    SDL_RenderPresent(renderer);
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
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_UP)
                    dstrect.y -= vitesseD;
                if (event.key.keysym.sym == SDLK_RIGHT && dstrect.x < w_window - dstrect.w)
                    dstrect.x += vitesseD;
                if (event.key.keysym.sym == SDLK_LEFT && dstrect.x > 0)
                    dstrect.x -= vitesseD;
                if (event.key.keysym.sym == SDLK_DOWN && dstrect.y < h_window - dstrect.h)
                    dstrect.y += vitesseD;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {

                    dstrect.h += 10;
                    dstrect.w += 10;
                    vitesseBoing += 1.15;
                    if (vitesseD > 1)
                    {
                        vitesseD -= 1;
                    }
                }
                if (event.button.button == SDL_BUTTON_RIGHT)
                {
                    if (dstrect.h > 15)
                    {
                        dstrect.h -= 10;
                        dstrect.w -= 10;
                        if (vitesseBoing > 1)
                        {
                            vitesseBoing /= 1.15;
                            vitesseD += 1;
                        }
                    }
                }

            default:
                break;
            }
        }

        if (i % vitesseBoing == 0)
        {
            srcrect.x = (srcrect.x + frame_sizeSlime) % (frame_sizeSlime * 4);
            i = 0;
        }
        if(j%3==0)
        {
            srcrectCoin.x = (srcrectCoin.x + frame_sizeCoin) % (frame_sizeCoin * 6);
            j = 0;
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, textFond, &srcrectFond, &dstrectFond);
            SDL_RenderCopy(renderer, textCoin, &srcrectCoin, &dstrectCoin);

        SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
        SDL_RenderPresent(renderer);
        SDL_Delay(20);

        i++;j++;
    }

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surf);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit(); // la SDL

    return 0;
}