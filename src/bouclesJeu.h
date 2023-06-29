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

#define MAX_SPEED 2
#define ACCELERATION 0.005

// Structure pour les arguments de multi_start_fourmi
typedef struct coordonne_s{
    int x;
    int y;
} coordonne_t;
 
// Structure pour les arguments de multi_start_fourmi
typedef struct FourmiArgs_s{
    int** matDist;
    int n;
    int result;
} FourmiArgs;

// Structure pour les arguments de cycle_Floyd_Warshall
typedef struct {
    int** tabWarshall;
    int** tabDist;
    int n;
    int result;
} FloydWarshallArgs;

/**
 * @brief Exécute la boucle de jeu  de graphe
 * @param tab Le tableau des sommets
 * @param n Le nombre de sommets
 * @return tableau du chemin du joueur
*/
int * boucle_jeu_graphe(sommet_t** tab, int n, int * nb_noeuds_chemin);

/**
 * @brief Exécute la boucle de jeu dans l'espace
 * @param tab Le tableau des sommets
 * @param n Le nombre de sommets
 * @param chemin Tableau du chemin choisi par le joueur
*/
void boucle_jeu_espace(sommet_t** tab, int n, int * chemin, int n_chemin);

/**
 * @brief Exécute la boucle de jeu principal
 * @param tab Le tableau des sommets
 * @param n Le nombre de sommets
*/
void boucle_jeu(sommet_t ** tab, int n);
#endif