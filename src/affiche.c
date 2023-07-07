#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "graph.h"
#include "affiche.h"

SDL_Renderer* renderer;
SDL_Window* window = NULL;
SDL_DisplayMode dm;


/**
 * @brief affiche un grph qui sert à faire le debugage de certaine fonction
 * @param tab Le chemin à tracer
 * @param tabSom Le tableau de sommet
 * @param n La taille du chemin
*/
void affiche_Chem(int * tab, sommet_t ** tabSom, int n) {
    for(int i=0; i<n-1; i++) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawLine(renderer, tabSom[tab[i]]->x, tabSom[tab[i]]->y, tabSom[tab[i+1]]->x, tabSom[tab[i+1]]->y); //Traçage du lien
        SDL_RenderPresent(renderer);
    }
}

/**
 * @brief Initialise l'affichage SDL
*/
void init() {
    //Initialisation de la SDL  + gestion de l'échec possible
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Error : SDL initialisation - %s\n",
            SDL_GetError()); // l'initialisation de la SDL a échoué
        exit(EXIT_FAILURE);
    }

    if (TTF_Init()!= 0) {
        SDL_Log("Error : SDL initialisation - %s\n",
            SDL_GetError()); // l'initialisation de la TTF a échoué
        exit(EXIT_FAILURE);
    }

    if (SDL_GetDesktopDisplayMode(0, &dm) != 0) {
        SDL_Log("SDL_GetDesktopDisplayMode failed: %s",
            SDL_GetError());
        exit(EXIT_FAILURE);
    }

    //Création de la fenêtre de gauche
    window = SDL_CreateWindow(
        "The travelling Spaceman", //Nom de la fenètre
        0, 0, //Coordonnées de la fenètre
        W, H, //Dimensions de la fenètre
        SDL_WINDOW_RESIZABLE); //Paramètre de la fenètre

    renderer = SDL_CreateRenderer(window, -1, 0);

    if (window == NULL) {
        SDL_Log("Error : SDL window 1 creation - %s\n",
            SDL_GetError()); // échec de la création de la fenêtre
        SDL_Quit(); // On referme la SDL
        exit(EXIT_FAILURE);
    }
}

int get_w_window(){
    return dm.w-10;
}

int get_h_window(){
    return dm.h-35;
}


/**
 * @brief Clear le fond en blanc
*/
void clear_SDL() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
}

/**
 * @brief Affiche un entier en bas à droite de l'ecran
 * @param n L'entier à afficher
*/
void draw_int(int n) {
    char Val[10];

    SDL_Rect textRect;
    TTF_Font* font;
    SDL_Surface* textSurface;
    SDL_Texture* textTexture;
    SDL_Color color = {50, 150, 0, 255};
    
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

/**
 * @brief Sert à afficher le chronometre en haut à gauche
 * @param n temps à afficher
*/
void draw_time(int n) {
    char Val[10];

    SDL_Rect textRect;
    TTF_Font* font;
    SDL_Surface* textSurface;
    SDL_Texture* textTexture;
    SDL_Color color = {200, 200, 200, 255};
    
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
            if ((i - center_x) * (i - center_x) + (j - center_y) * (j - center_y) <= radius * radius)
                SDL_RenderDrawPoint(renderer, i, j);
        }
    }
}

/**
 * @brief Dessine un graphe à l'aide d'un rendu SDL.
 * @param renderer Le rendu SDL utilisé pour afficher le graphe.
 * @param tab Le tableau de sommet du graph.
 * @param n Le nombre de sommets dans le tableau.
 * @param displayPoid booleen permetant d'afficher ou non les poids sur les arretes
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

                if (displayPoid) {
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
    char Tag[10];

    SDL_Rect textRect;
    TTF_Font* font;
    SDL_Surface* textSurface;
    SDL_Texture* textTexture;
    SDL_Color color = {50, 150, 0, 255};

    font = TTF_OpenFont("arial.ttf", R_NOEUD+10);

    textRect.y = 0;

    for (int i = 0; i < nPath ; i++){
        if (i != nPath-1)
            sprintf(Tag, "%c - ", tab[path[i]]->val);
        else
            sprintf(Tag, "%c", tab[path[i]]->val);
        
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

/**
 * @brief Affiche les asteroid avec des rotation à partir d'un tableau
 * @param tableau d'asteroid à afficher
 * @param n taille du tableau
 * @param image texture des asteroids
*/
void affichAst(asteroid_t * tab, int n, SDL_Texture * image) {
    SDL_Rect dest = {0, 0, 48, 48};
    for (int i = 0; i < n; i++) {
        dest.x = tab[i].x;
        dest.y = tab[i].y;
        draw_sprite(dest, image, tab[i].frame, 0, tab[i].angle, 48);
    }
}

/**
 * @brief Affiche l'ecran de fin
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

/**
 * @brief affiche l'ecran de fin du jeu trzvelling spaceman
 * @param time le temps de fin 
 * @param type_fin permet d'afficher soit l'ecran de mort doit l'ecran de victoire 
*/
void affiche_fin_espace(int time, int type_fin) {
    SDL_Rect textRect;
    TTF_Font* font;
    SDL_Surface* textSurface;
    SDL_Texture* textTexture;
    SDL_Color color = {200, 200, 200, 255};

    char Txt1[100];

    font = TTF_OpenFont("arial.ttf", 40);
    if (!type_fin)
        sprintf(Txt1, "Votre temps : %d s", time);
    else 
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
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
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

/**
 * @brief permet d'afficher un sprite
 * @param destination le rectangle de destination du sprite
 * @param texture la texture du sprite
 * @param x la coordoné x de la texture à partir de la quelle prendre le sprite
 * @param y la coordoné y de la texture à partir de la quelle prendre le sprite
 * @param angle l'angle de rotation du sprite
 * @param size la taille du sprite sur l'image
*/
void draw_sprite(SDL_Rect destination, SDL_Texture * texture, int x, int y, int angle, int size) {
    SDL_Rect srcrect = {size*x, size*y, size, size};
    SDL_RenderCopyEx(renderer, texture, &srcrect, &destination, angle, NULL, 0);
}

/**
 * @brief cree une texture à partir d'une surface
 * @param surface la surface
 * @return la texture cree
*/
SDL_Texture * create_texture(SDL_Surface * surface) {
    return SDL_CreateTextureFromSurface(renderer, surface);
}

/**
 * @brief Dessine un rectangle
 * @param rect Le rectangle à dessiner
*/
void draw_rect(SDL_Rect rect) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

/**
 * @brief Dessine une ligne entre deux points
 * @param point1 premier point de la ligne
 * @param point2 deuxieme point de la ligne
*/
void draw_Line(Point point1, Point point2) {
    SDL_SetRenderDrawColor(renderer, 0,255,0,0);
    SDL_RenderDrawLine(renderer, point1.x, point1.y, point2.x, point2.y);
    SDL_RenderPresent(renderer);
}

/**
 * @brief fonction de lancement de thread pour l'affichage graphique du jeu
 * @param args Une structure contenant tout les argument necessaire a l'affichage du jeu
*/
void * afficheJeu(afficheArgs * argsAff) {
    int alpha = 0; //l'aplpha des etoile de fond
    int etatAlpha = 1; //booleen disant si l'on doit augmenter ou baisser l'alpha des etoiles
    int first = 1; //booleen permetant de savoir si c le premier tour de boucle depuis la fin du jeu
    int seconde = 0; //variable permetant de stoquer le temps de fin de jeu

    int firstTour = 1;
    
    while (*(argsAff->program_on))
    {   
        //verification de fin de jeu pour stoquer le temps de fin
        
        if (*(argsAff->fin) && first) {
            first = 0;
            seconde = *(argsAff->count)/1000;
        }
        if(firstTour) printf("\n");

        firstTour = 0;
        //une refresh toute les 2 ms
        if (*(argsAff->count)%20 == 0) {
            //changement de frame pour les etoile filantes
            if (*(argsAff->count)%100 == 0) {
                argsAff->frameEF = (argsAff->frameEF + 1)%8;
                if (argsAff->frameEF == 0) {
                    argsAff->etoileFilante.x = rand()%W;
                    argsAff->etoileFilante.y = rand()%H;
                }
            }
            
            //Affichage du fond
            draw_sprite(argsAff->background, argsAff->textureBg, 1, 0, 0, 540);

            //Set de l'alpha des etoile de fond
            SDL_SetTextureAlphaMod(argsAff->textureE2, alpha);
            SDL_SetTextureAlphaMod(argsAff->textureE1, 255-alpha);

            //Affichage des etoiles de fond
            draw_sprite(argsAff->etoile, argsAff->textureE1, 0, 0, 0, 540);
            draw_sprite(argsAff->etoile, argsAff->textureE2, 0, 1, 0, 540);

            //Affichage de l'etoile filante
            draw_sprite(argsAff->etoileFilante, argsAff->textureEF, argsAff->frameEF, 0, 0, 26);
            
            if (!(*(argsAff->fin))) {
                //Affichage en jeu

                //Affichage des planetes
                for (int i = 0; i < argsAff->n; i++) {
                    argsAff->planete.x = argsAff->tab[i]->x-24;
                    argsAff->planete.y = argsAff->tab[i]->y-24;
                    draw_sprite(argsAff->planete, argsAff->textureP, argsAff->co[i].x, argsAff->co[i].y, 0, 48);
                }

                //Affichage des drapeaux
                for (int i = 0; i < argsAff->n; i++) {
                    if (argsAff->planeteVisite[i]) {
                        argsAff->flag.x = argsAff->tab[i]->x-12;
                        argsAff->flag.y = argsAff->tab[i]->y-48;
                        if (i == 0)
                            draw_sprite(argsAff->flag, argsAff->textureF, argsAff->frameFlag, 1, 0, 60);
                        else
                            draw_sprite(argsAff->flag, argsAff->textureF, argsAff->frameFlag, 0, 0, 60);
                    }
                }

                //Affichage du trou noir
                draw_sprite(argsAff->affTrouNoir, argsAff->textureTN, argsAff->frameTN, 0, 0, 48);
                if (W > 800){
                    draw_sprite(argsAff->affTrouNoir2, argsAff->textureTN2, argsAff->frameTN, 0, 0, 48);
                }
                
                

                //Affichage de la navette
                draw_sprite(argsAff->navette, argsAff->texture, argsAff->frame, 0, 0, argsAff->navette.w);

                //Frame de la nevette du trou noir et du drapeau
                if (*(argsAff->count)%200 == 0) {
                    argsAff->frame = (argsAff->frame + 1)%4;
                    argsAff->frameTN = (argsAff->frameTN + 1)%4;
                    argsAff->frameFlag = (argsAff->frameFlag + 1)%5;
                }

                //Affichage des asteroides
                affichAst(argsAff->asteroid, argsAff->n_ast, argsAff->textureP);

                argsAff->navette.x = *(argsAff->x);
                argsAff->navette.y = *(argsAff->y);

                //Affichage du temps
                draw_time(*(argsAff->count)/1000);
            }

            else //Affiche ecran de fin
                affiche_fin_espace(seconde, argsAff->type_fin);

            render(); //rendre les differents elements

            //Modification du alpha des etoiles de fond
            if (etatAlpha) {
                alpha--;
                if (0 >= alpha)
                    etatAlpha = !etatAlpha;
            }
            else {
                alpha++;
                if (255 <= alpha)
                    etatAlpha = !etatAlpha;
            }      
        }
    }

    return NULL;
}