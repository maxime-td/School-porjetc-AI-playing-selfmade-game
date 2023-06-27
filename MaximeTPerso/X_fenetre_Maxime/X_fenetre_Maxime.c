 #include <SDL2/SDL.h>
#include <stdio.h>

/************************************/
/*  exemple de création de fenêtres */
/************************************/

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    int sourire, clin_oeil;

    SDL_Window 
    *win1 = NULL,
    *win2 = NULL,
    *win3 = NULL,
    *win4 = NULL;

    /* Initialisation de la SDL + gestion de l'échec possible */
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Error : SDL initialisation - %s\n", 
            SDL_GetError());                // l'initialisation de la SDL a échoué 
        exit(EXIT_FAILURE);
    }

    /* Tête */
    win1 = SDL_CreateWindow("Carl", 400, 75, 500, 700, 0); //Création fenêtre 1

    if (win1 == NULL) {                             //Vérification fenêtre 1
        SDL_Log("Error : SDL window 1 creation - %s\n", 
            SDL_GetError());
        SDL_Quit();   
        exit(EXIT_FAILURE);
    }

    /* Oeil gauche */
    win2 = SDL_CreateWindow("", 500, 200, 100, 100, 0); //Création fenêtre 2

    if (win2 == NULL) {                             //Vérification fenêtre 2
        SDL_Log("Error : SDL window 2 creation - %s\n", 
            SDL_GetError());
            SDL_DestroyWindow(win1);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    /* Oeil droit */
    win3 = SDL_CreateWindow("", 700, 200, 100, 100, 0); //Création fenêtre 3

    if (win3 == NULL) {                             //Vérification fenêtre 3
        SDL_Log("Error : SDL window 3 creation - %s\n", 
            SDL_GetError());
            SDL_DestroyWindow(win2);
            SDL_DestroyWindow(win1);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    /* Bouche */
    win4 = SDL_CreateWindow("", 600, 600, 100, 50, 0); //Création fenêtre 4

    if (win4 == NULL) {                             //Vérification fenêtre 4
        SDL_Log("Error : SDL window 4 creation - %s\n", 
            SDL_GetError());
            SDL_DestroyWindow(win3);
            SDL_DestroyWindow(win2);
            SDL_DestroyWindow(win1);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    /* Sourire */
    SDL_Delay(2000);
    for(sourire = 0; sourire < 150; sourire+=15) {
        SDL_SetWindowPosition(win4, 600-sourire, 600);
        SDL_SetWindowSize(win4, 100+2*sourire, 50);
    }
    SDL_SetWindowPosition(win4, 450, 650);

    /* Clin d'oeil */
    SDL_Delay(500);
    for(clin_oeil = 0; clin_oeil < 50; clin_oeil+=5) {
        SDL_SetWindowPosition(win2, 500, 200+clin_oeil);
        SDL_SetWindowSize(win2, 100, 100-clin_oeil);
    }

    SDL_Delay(500);

    for(clin_oeil = 0; clin_oeil < 50; clin_oeil+=5) {
        SDL_SetWindowPosition(win2, 500, 250-clin_oeil);
        SDL_SetWindowSize(win2, 100, 2*clin_oeil);
    }

    SDL_Delay(4000);

    SDL_DestroyWindow(win4);
    SDL_DestroyWindow(win3);
    SDL_DestroyWindow(win2);
    SDL_DestroyWindow(win1);    

    SDL_Quit();

    return 0;
    }