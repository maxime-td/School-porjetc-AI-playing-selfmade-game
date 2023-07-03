#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "graph.h"
#include "affiche.h"
#include <math.h>
#include <string.h>


SDL_Renderer* renderer;
SDL_Window* window = NULL;

void affiche_Chem(int * tab, sommet_t ** tabSom, int n)
{
    for(int i=0; i<n-1; i++)
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawLine(renderer, tabSom[tab[i]]->x, tabSom[tab[i]]->y, tabSom[tab[i+1]]->x, tabSom[tab[i+1]]->y); //Traçage du lien
        SDL_RenderPresent(renderer);
    }
}


/**
 * @brief Affiche un graphe à l'aide de la bibliothèque SDL.
 * @param tab Le tableau de sommets représentant le graphe.
 * @param n Le nombre de sommets dans le tableau.
*/
void init() {
    int w_window = W, h_window = H;

    SDL_DisplayMode dm;  

    //Initialisation de la SDL  + gestion de l'échec possible
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Error : SDL initialisation - %s\n",
                SDL_GetError()); // l'initialisation de la SDL a échoué
        exit(EXIT_FAILURE);
    }

    if (TTF_Init()!= 0){
        SDL_Log("Error : SDL initialisation - %s\n",
                SDL_GetError()); // l'initialisation de la TTF a échoué
        exit(EXIT_FAILURE);
    }

    if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
    {
        SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    //Création de la fenêtre de gauche
    window = SDL_CreateWindow(
        "The travelling Spaceman", //Nom de la fenètre
        dm.w/2-w_window/2, 0, //Coordonnées de la fenètre
        w_window, h_window, //Dimensions de la fenètre
        SDL_WINDOW_RESIZABLE); //Paramètre de la fenètre

    renderer = SDL_CreateRenderer(window, -1, 0);

    if (window == NULL)
    {
        SDL_Log("Error : SDL window 1 creation - %s\n",
                SDL_GetError()); // échec de la création de la fenêtre
        SDL_Quit();              // On referme la SDL
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Clear le fond en blanc
*/
void clear_SDL(){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
}

/**
 * @brief Affiche un graphe à l'aide de la bibliothèque SDL.
 * @param score Le score à afficher en tant que votre score
 * @param bestScore Le score à afficher en tant que meilleur score 
*/
void draw_int(int n) {
    SDL_Rect textRect;
    TTF_Font* font;
    SDL_Surface* textSurface;
    SDL_Texture* textTexture;
    SDL_Color color = {50, 150, 0, 255};
    char Val[10];

    font = TTF_OpenFont("arial.ttf", 60);

    sprintf(Val, "%d", n);
    
    textSurface = TTF_RenderText_Solid(font, Val, color);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_FreeSurface(textSurface);
    SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
    textRect.x = W-textRect.w;
    textRect.y = H-textRect.h;

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
    
    TTF_CloseFont(font);
}

void draw_time(int n) {
    SDL_Rect textRect;
    TTF_Font* font;
    SDL_Surface* textSurface;
    SDL_Texture* textTexture;
    SDL_Color color = {200, 200, 200, 255};
    char Val[10];
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);

    font = TTF_OpenFont("arial.ttf", 60);

    sprintf(Val, "%02d", n);
    
    textSurface = TTF_RenderText_Solid(font, Val, color);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_FreeSurface(textSurface);
    SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
    textRect.x = 0;
    textRect.y = 0;

    SDL_RenderFillRect(renderer, &textRect);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
    
    TTF_CloseFont(font);
}

/**
 * @brief Trace un disque
 * @param center_x La coordonnée x du centre du disque
 * @param center_y La coordonnée y du centre du disque
 * @param radius Le rayon du disque
*/
void draw_disk(int center_x, int center_y, int radius) {
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
    int i, j; //Incréments
    int rayon = R_NOEUD; //Rayon des disques des sommets

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

    font = TTF_OpenFont("arial.ttf", rayon+10);

    // Parcour 
    for(i = 0; i < n; i+=1) {
        sommet_courant = tab[i];
        for(j = i; j < n; j+=1) {
            // Si j est voisin de i
            if(tab[i]->voisins[j] == 1) {
                voisin_courant = tab[j];
                SDL_RenderDrawLine(renderer, sommet_courant->x, sommet_courant->y, voisin_courant->x, voisin_courant->y); //Traçage du lien
                poidRect.x = (sommet_courant->x + voisin_courant->x)/2 - poidRect.w/2;
                poidRect.y = (sommet_courant->y + voisin_courant->y)/2 - poidRect.h/2;

                sprintf(Poid, "%d", (int) round(distTab[i][j]));

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
        draw_disk(sommet_courant->x, sommet_courant->y, rayon); //Traçage du sommet

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
}

/**
 * @brief Ecrit le chemin parcouru en haut à droite.
 * @param tab Le tableau de sommet.
 * @param path Le tableau du chemin (les index des noeuds).
 * @param nPath La taille du tableau path
 */
void draw_path(sommet_t ** tab, int * path, int nPath) {
    SDL_Rect textRect;
    TTF_Font* font;
    SDL_Surface* textSurface;
    SDL_Texture* textTexture;
    SDL_Color color = {50, 150, 0, 255};
    char Tag[10];

    font = TTF_OpenFont("arial.ttf", R_NOEUD+10);

    textRect.y = 0;
    for (int i = 0; i < nPath ; i++){
        if (i != nPath-1){
            sprintf(Tag, "%c - ", tab[path[i]]->val);
        }else{
            sprintf(Tag, "%c", tab[path[i]]->val);
        }
        
        textSurface = TTF_RenderText_Solid(font, Tag, color);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_FreeSurface(textSurface);
        SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
        textRect.x = i*(R_NOEUD-2)*3;
        textRect.y = ((int)(textRect.x/W))*textRect.h;
        textRect.x = textRect.x%W;

        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_DestroyTexture(textTexture);
    }

    TTF_CloseFont(font);
}

/**
 * @brief Affiche un graphe à l'aide de la bibliothèque SDL.
 * @param tab Le tableau de sommets représentant le graphe.
 * @param n Le nombre de sommets dans le tableau.
*/
void affiche(sommet_t ** tab, int n, int r, int g, int b, int a, int displayPoid) {
    SDL_SetRenderDrawColor(renderer, r, g , b, a);
    draw_graph(renderer, tab, n, displayPoid);
}

void affichAst(asteroid_t * tab, int n, SDL_Texture * image){
    SDL_Rect dest = {0, 0, 48, 48};
    for (int i = 0; i < n; i++){
        dest.x = tab[i].x;
        dest.y = tab[i].y;
        draw_sprite(dest, image, tab[i].frame, 0, tab[i].angle, 48);
    }
}

/**
 * @brief Affiche un graphe à l'aide de la bibliothèque SDL.
 * @param score Le score à afficher en tant que votre score
 * @param bestScore Le score à afficher en tant que meilleur score 
*/
void afficheFin(int score, int bestScore) {
    SDL_Rect textRect;
    TTF_Font* font;
    SDL_Surface* textSurface;
    SDL_Texture* textTexture;
    SDL_Color color = {50, 150, 0, 255};
    char Txt1[100];
    char Txt2[100];

    font = TTF_OpenFont("arial.ttf", 40);

    sprintf(Txt1, "Votre score : %d", score);
    sprintf(Txt2, "Meilleur score possible : %d", bestScore);

    textSurface = TTF_RenderText_Solid(font, Txt1, color);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_FreeSurface(textSurface);
    SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
    textRect.x = W/2-textRect.w;
    textRect.y = H/2-textRect.h;

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);

    textSurface = TTF_RenderText_Solid(font, Txt2, color);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_FreeSurface(textSurface);
    SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
    textRect.y = (H/2-textRect.h)+50;

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
    
    TTF_CloseFont(font);
}

void affiche_fin_espace(int time, int type_fin) {
    if(type_fin)
    {
        SDL_Rect textRect;
        TTF_Font* font;
        SDL_Surface* textSurface;
        SDL_Texture* textTexture;
        SDL_Color color = {200, 200, 200, 255};
        char Txt1[100];

        font = TTF_OpenFont("arial.ttf", 40);

        sprintf(Txt1, "Vous etes mort");

        textSurface = TTF_RenderText_Solid(font, Txt1, color);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_FreeSurface(textSurface);
        SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
        textRect.x = W/2-textRect.w/2;
        textRect.y = H/2-textRect.h/2;

        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_DestroyTexture(textTexture);

        TTF_CloseFont(font);
    }
    else
    {
        SDL_Rect textRect;
        TTF_Font* font;
        SDL_Surface* textSurface;
        SDL_Texture* textTexture;
        SDL_Color color = {200, 200, 200, 255};
        char Txt1[100];

        font = TTF_OpenFont("arial.ttf", 40);

        sprintf(Txt1, "Votre temps : %d s", time);

        textSurface = TTF_RenderText_Solid(font, Txt1, color);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_FreeSurface(textSurface);
        SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
        textRect.x = W/2-textRect.w/2;
        textRect.y = H/2-textRect.h/2;

        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_DestroyTexture(textTexture);

        TTF_CloseFont(font);
    }
}

/**
 * @brief Affiche le rendue du renderer
*/
void render() {
    SDL_RenderPresent(renderer);
}

/**
 * @brief Libere le renderer la fenêtre  et ferme SDL
*/
void closeSDL() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

/**
 * @brief C'est un secret
*/
void secret1() {
    //Initialisation
    SDL_Texture* texture_secrete = NULL;
    SDL_Surface* surface = IMG_Load("images/texture_secrete.png");

    texture_secrete = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect source = {0}, destination = {50, 50, 200, 200};
    SDL_QueryTexture(texture_secrete, NULL, NULL, &source.w, &source.h);

    SDL_RenderCopy(renderer, texture_secrete, &source, &destination);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture_secrete);
}

SDL_Texture * create_texture(SDL_Surface * surface){
    return SDL_CreateTextureFromSurface(renderer, surface);
}

//Partie sur l'animation de la soucoupe volante

void draw_sprite(SDL_Rect destination, SDL_Texture * texture, int x, int y, int angle, int size){
    SDL_Rect srcrect = {size*x, size*y, size, size};
    SDL_RenderCopyEx(renderer, texture, &srcrect, &destination, angle, NULL, 0);
}

void draw_rect(SDL_Rect rect){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void draw_Line(Point point1, Point point2)
{
    SDL_SetRenderDrawColor(renderer, 0,255,0,0);
    SDL_RenderDrawLine(renderer, point1.x, point1.y, point2.x, point2.y);
    SDL_RenderPresent(renderer);
}