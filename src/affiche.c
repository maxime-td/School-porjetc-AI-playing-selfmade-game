#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "graph.h"
#include "affiche.h"
#include <math.h>

SDL_Renderer* renderer;
SDL_Window *window = NULL;

/**
 * @brief Affiche un graphe à l'aide de la bibliothèque SDL.
 * @param tab Le tableau de sommets représentant le graphe.
 * @param n Le nombre de sommets dans le tableau.
*/
void init(sommet_t ** tab, int n){
    int y = 0, x;
    int width, height, w_window = 800, h_window = 800, final_width = w_window/2;
    SDL_bool program_on = SDL_TRUE;
    SDL_Event event;   

    SDL_DisplayMode dm;

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
        return;
    }

    width = dm.w;
    height = dm.h;

    /* Création de la fenêtre de gauche */
    window = SDL_CreateWindow(
        "Graph",    // codage en utf8, donc accents possibles
        width/2-w_window/2, 0,                  // coin haut gauche en haut gauche de l'écran
        w_window, h_window,              // largeur = 400, hauteur = 300
        SDL_WINDOW_RESIZABLE);

    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    

    if (window == NULL)
    {
        SDL_Log("Error : SDL window 1 creation - %s\n",
                SDL_GetError()); // échec de la création de la fenêtre
        SDL_Quit();              // On referme la SDL
        exit(EXIT_FAILURE);
    }

    SDL_GetWindowPosition(window, &x, &y);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Delay(100);
    draw_graph(renderer, tab, n, 1);
    
    SDL_RenderPresent(renderer);
    SDL_Delay(60000);
    //temporaire
    //int chemin[2] = {0,2};
    //int n_chemin = 2;
    //sommet_t** sous_graphe;

    //SDL_Delay(1000);
    //SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
    //sous_graphe = chemin_en_graphe(chemin, n_chemin, tab, n);
    //jusque là
    
    //draw_graph(renderer, sous_graphe, n_chemin, 0);
    //SDL_RenderPresent(renderer);
}

/**
 * @brief Trace un disque
 * @param renderer Le renderer où tracer le disque
 * @param center_x La coordonnée x du centre du disque
 * @param center_y La coordonnée y du centre du disque
 * @param radius Le rayon du disque
*/
void draw_disk(SDL_Renderer* renderer, int center_x, int center_y, int radius) {
    // Calculer les coordonnées du rectangle englobant le disque
    int x = center_x - radius;
    int y = center_y - radius;
    int width = radius * 2;
    int height = radius * 2;

    // Dessiner le disque rempli
    for (int i = x; i < x + width; i++) {
        for (int j = y; j < y + height; j++) {
            // Vérifier si le point (i, j) est à l'intérieur du cercle
            if ((i - center_x) * (i - center_x) + (j - center_y) * (j - center_y) <= radius * radius) {
                SDL_RenderDrawPoint(renderer, i, j);
            }
        }
    }
}

/**
 * @brief Dessine un graphe à l'aide d'un rendu SDL.
 * @param renderer Le rendu SDL utilisé pour afficher le graphe.
 * @param graph Le pointeur vers le graphe à dessiner.
 * @param n Le nombre de sommets dans le tableau.
 */
void draw_graph(SDL_Renderer* renderer, sommet_t** tab, int n, int displayPoid) {

    // Initialisations 
    int i, j, k; //Incréments
    int rayon = R_NOEUD; //Rayon des disques des sommets
    char nom_sommet;

    sommet_t* sommet_courant; //Sommet courant
    sommet_t* voisin_courant; //Voisin courant

    char Tag[3];
    char Poid[8];

    TTF_Font* font;
    SDL_Surface* textSurface;
    SDL_Texture* textTexture;
    SDL_Color color = {150, 250, 0, 255};

    SDL_Surface* textSurfacePoid;
    SDL_Texture* textTexturePoid;
    SDL_Color colorPoid = {50, 10, 0, 255};

    SDL_Rect textRect;
    SDL_Rect poidRect = {0, 0, 30, 30};

    int ** distTab = dist_tab(tab, &n);


    if (TTF_Init()!= 0)
    {
        SDL_Log("Error : SDL initialisation - %s\n",
                SDL_GetError()); // l'initialisation de la TTF a échoué
        exit(EXIT_FAILURE);
    }

    font = TTF_OpenFont("arial.ttf", rayon+10);

    // Parcour 
    for(i = 0; i < n; i+=1) {
        sommet_courant = tab[i];
        for(j = i; j < n; j+=1) {
            k=0;
            // Si j est voisin de i
            if(tab[i]->voisins[j] == 1) {
                voisin_courant = tab[j];
                SDL_RenderDrawLine(renderer, sommet_courant->x, sommet_courant->y, voisin_courant->x, voisin_courant->y); //Traçage du lien
                poidRect.x = (sommet_courant->x + voisin_courant->x)/2 - poidRect.w/2;
                poidRect.y = (sommet_courant->y + voisin_courant->y)/2 - poidRect.h/2;

                sprintf(Poid, "%d", (int) round(distTab[i][j]/10));

                textSurfacePoid = TTF_RenderText_Solid(font, Poid, colorPoid);
                textTexturePoid = SDL_CreateTextureFromSurface(renderer, textSurfacePoid);

                SDL_FreeSurface(textSurfacePoid);
                SDL_QueryTexture(textTexturePoid, NULL, NULL, &poidRect.w, &poidRect.h);
                //SDL_RenderFillRect(renderer, &poidRect);
                if (displayPoid){
                    SDL_RenderCopy(renderer, textTexturePoid, NULL, &poidRect);
                }
                
                SDL_DestroyTexture(textTexturePoid);
            }
        }
        draw_disk(renderer, sommet_courant->x, sommet_courant->y, rayon); //Traçage du sommet

        sprintf(Tag, "%c", tab[i]->val);

        textSurface = TTF_RenderText_Solid(font, Tag, color);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_FreeSurface(textSurface);
        SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
        textRect.x = sommet_courant->x-textRect.w/2;
        textRect.y = sommet_courant->y-textRect.h/2;

        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_DestroyTexture(textTexture);
    }

    free2DTab((void**)distTab, n);
    TTF_CloseFont(font);
    TTF_Quit();
}