#ifndef INTERACTIONJOUEUR_H
#define INTERACTIONJOUEUR_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "graph.h"
#include "affiche.h"
#include "fourmi.h"

/**
 * @brief Regarde si le noeud d'index i est dans le chemin. Renvoie 1 si vrai, 0 si faux
 * @param i L'index
 * @param chemin Le tableau du chemin
*/
int est_dans_chemin(int i, int* chemin);

/**
 * @brief Exécute la boucle de jeu
 * @param tab Le tableau des sommets
 * @param n Le nombre de sommets
*/
void boucle_jeu(sommet_t** tab, int n);

#endif