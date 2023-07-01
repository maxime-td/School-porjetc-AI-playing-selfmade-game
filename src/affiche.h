#ifndef AFFICHE_H
#define AFFICHE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "graph.h"
#include "map.h"
#include <math.h>

#define R_NOEUD 15

/**
 * @brief Clear le fond en blanc
*/
void clear_SDL();

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
 * @param tab Le tableau de sommets représentant le graphe.
 * @param n Le nombre de sommets dans le tableau.
 * @param displayPoid
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
 * @param score Le score à afficher en tant que votre score
 * @param bestScore Le score à afficher en tant que meilleur score 
*/
void afficheFin(int score, int bestScore);

/**
 * @brief Affiche le graphe initial à l'aide de la bibliothèque SDL.
 * @param tab Le tableau de sommets représentant le graphe.
 * @param n Le nombre de sommets dans le tableau.
*/
void init();

/**
 * @brief Affiche un graphe à l'aide de la bibliothèque SDL.
 * @param tab Le tableau de sommets représentant le graphe.
 * @param n Le nombre de sommets dans le tableau.
*/
void affiche(sommet_t ** tab, int n, int r, int g, int b, int a, int displayPoid);

/**
 * @brief Affiche un graphe à l'aide de la bibliothèque SDL.
 * @param score Le score à afficher en tant que votre score
 * @param bestScore Le score à afficher en tant que meilleur score 
*/
void draw_int(int n);

/**
 * @brief Affiche le rendue du renderer
*/
void render();

/**
 * @brief Libere le renderer la fenêtre  et ferme SDL
*/
void closeSDL();

SDL_Texture* load_texture_from_image(SDL_Surface * texture_soucoupe);

void afficheFinEspace(int time);

/**
 * @brief C'est un secret
*/
void secret1();

SDL_Texture * create_texture(SDL_Surface * surface);

void draw_time(int n);
void draw_sprite(SDL_Rect destination, SDL_Texture * texture, int x, int y, int angle, int size);

void draw_rect(SDL_Rect rect);

void affichAst(asteroid_t * tab, int n, SDL_Texture * image);


#endif // AFFICHE_H