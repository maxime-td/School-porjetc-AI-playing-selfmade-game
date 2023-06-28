#ifndef AFFICHE_H
#define AFFICHE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "graph.h"
#include "affiche.h"
#include <math.h>

#define R_NOEUD 15

/**
 * @brief Trace un disque
 * @param renderer Le renderer où tracer le disque
 * @param center_x La coordonnée x du centre du disque
 * @param center_y La coordonnée y du centre du disque
 * @param radius Le rayon du disque
*/
void draw_disk(SDL_Renderer* renderer, int center_x, int center_y, int radius);


void draw_path(sommet_t ** tab, int n, int * path, int nPath);


/**
 * @brief Affiche un graphe à l'aide de la bibliothèque SDL.
 * @param tab Le tableau de sommets représentant le graphe.
 * @param n Le nombre de sommets dans le tableau.
*/
void init(sommet_t ** tab, int n);

/**
 * @brief Affiche un graphe à l'aide de la bibliothèque SDL.
 * @param tab Le tableau de sommets représentant le graphe.
 * @param n Le nombre de sommets dans le tableau.
*/
void affiche(sommet_t ** tab, int n, int r, int g, int b, int a, int displayPoid);

void render();

void closeSDL();

#endif // AFFICHE_H