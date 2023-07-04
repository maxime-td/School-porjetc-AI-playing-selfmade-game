#ifndef AFFICHE_H
#define AFFICHE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "graph.h"
#include "map.h"
#include "threads.h"

#define R_NOEUD 15

SDL_Texture* create_texture(SDL_Surface * surface);

void draw_rect(SDL_Rect rect);

void draw_Line(Point point1, Point point2);

// Structure pour les arguments de affiche
typedef struct afficheArgs_s{
    int * count;
    int frame;
    int frameEF;
    int frameFlag;
    int frameTN;
    int type_fin;
    SDL_Rect etoile;
    SDL_Rect etoileFilante;
    SDL_Rect background;
    SDL_Rect navette;
    SDL_Rect planete;
    SDL_Rect flag;
    SDL_Rect affTrouNoir;
    SDL_Rect affTrouNoir2;
    asteroid_t * asteroid;
    SDL_Texture * texture;
    SDL_Texture * textureBg;
    SDL_Texture * textureE1;
    SDL_Texture * textureE2;
    SDL_Texture * textureEF;
    SDL_Texture * textureP;
    SDL_Texture * textureF;
    SDL_Texture * textureTN;
    SDL_Texture * textureTN2;
    int * fin;
    int * planeteVisite;
    sommet_t ** tab;
    int n;
    int n_ast;
    SDL_bool * program_on;
    Point * co;
    float * x;
    float * y;
}afficheArgs;


/**
 * @brief affiche un grph qui sert à faire le debugage de certaine fonction
 * @param tab Le chemin à tracer
 * @param tabSom Le tableau de sommet
 * @param n La taille du chemin
*/
void affiche_Chem(int * tab, sommet_t ** tabSom, int n);

/**
 * @brief Initialise l'affichage SDL
*/
void init();

int get_w_window();
int get_h_window();

/**
 * @brief Clear le fond en blanc
*/
void clear_SDL();

/**
 * @brief Affiche un entier en bas à droite de l'ecran
 * @param n L'entier à afficher
*/
void draw_int(int n);

/**
 * @brief Sert à afficher le chronometre en haut à gauche
 * @param n temps à afficher
*/
void draw_time(int n);

/**
 * @brief Trace un disque.
 * @param renderer Le renderer où tracer le disque.
 * @param center_x La coordonnée x du centre du disque.
 * @param center_y La coordonnée y du centre du disque.
 * @param radius Le rayon du disque.
*/
void draw_disk(int center_x, int center_y, int radius);

/**
 * @brief Dessine un graphe à l'aide d'un rendu SDL.
 * @param renderer Le rendu SDL utilisé pour afficher le graphe.
 * @param tab Le tableau de sommet du graph.
 * @param n Le nombre de sommets dans le tableau.
 * @param displayPoid booleen permetant d'afficher ou non les poids sur les arretes
 */
void draw_graph(SDL_Renderer* renderer, sommet_t** tab, int n, int displayPoid);

/**
 * @brief Ecrit le chemin parcouru en haut à droite.
 * @param tab Le tableau de sommet.
 * @param path Le tableau du chemin (les index des noeuds).
 * @param nPath La taille du tableau path
 */
void draw_path(sommet_t ** tab, int * path, int nPath);

/**
 * @brief Affiche un graphe à l'aide de la bibliothèque SDL.
 * @param tab Le tableau de sommets représentant le graphe.
 * @param n Le nombre de sommets dans le tableau.
*/
void affiche(sommet_t ** tab, int n, int r, int g, int b, int a, int displayPoid);

/**
 * @brief Affiche les asteroid avec des rotation à partir d'un tableau
 * @param tableau d'asteroid à afficher
 * @param n taille du tableau
 * @param image texture des asteroids
*/
void affichAst(asteroid_t * tab, int n, SDL_Texture * image);

/**
 * @brief Affiche l'ecran de fin
 * @param score Le score à afficher en tant que votre score
 * @param bestScore Le score à afficher en tant que meilleur score 
*/
void afficheFin(int score, int bestScore);

/**
 * @brief affiche l'ecran de fin du jeu trzvelling spaceman
 * @param time le temps de fin 
 * @param type_fin permet d'afficher soit l'ecran de mort doit l'ecran de victoire 
*/
void affiche_fin_espace(int time, int type_fin);

/**
 * @brief Affiche le rendue du renderer
*/
void render();

/**
 * @brief Libere le renderer la fenêtre  et ferme SDL
*/
void closeSDL();

/**
 * @brief C'est un secret
*/
void secret1();

/**
 * @brief permet d'afficher un sprite
 * @param destination le rectangle de destination du sprite
 * @param texture la texture du sprite
 * @param x la coordoné x de la texture à partir de la quelle prendre le sprite
 * @param y la coordoné y de la texture à partir de la quelle prendre le sprite
 * @param angle l'angle de rotation du sprite
 * @param size la taille du sprite sur l'image
*/
void draw_sprite(SDL_Rect destination, SDL_Texture * texture, int x, int y, int angle, int size);

/**
 * @brief fonction de lancement de thread pour l'affichage graphique du jeu
 * @param args Une structure contenant tout les argument necessaire a l'affichage du jeu
*/
void * afficheJeu(afficheArgs * argsAff);

#endif // AFFICHE_H