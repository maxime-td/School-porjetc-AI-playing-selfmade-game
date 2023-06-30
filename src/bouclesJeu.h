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

#define MAX_SPEED 0.001
#define ACCELERATION 0.0000000025

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

// Structure pour les arguments de cycle_Floyd_Warshall
typedef struct {
    int * fin;
    int time;
} timerArgs;

typedef struct afficheArgs_s{
    int * count;
    int frame;
    int frameEF;
    int frameFlag;
    SDL_Rect etoile;
    SDL_Rect etoileFilante;
    SDL_Rect background;
    SDL_Rect navette;
    SDL_Rect planete;
    SDL_Rect flag;
    asteroid_t * asteroid;
    SDL_Texture * texture;
    SDL_Texture * textureBg;
    SDL_Texture * textureE1;
    SDL_Texture * textureE2;
    SDL_Texture * textureEF;
    SDL_Texture * textureP;
    SDL_Texture * textureF;
    int * fin;
    int * planeteVisite;
    sommet_t ** sous_graphe;
    sommet_t ** tab;
    int n_sous_graphe;
    int n;
    int n_ast;
    SDL_bool * program_on;
    int * chemin;
    coordonne_t * co;
    float * x;
    float * y;
}afficheArgs;

/**
 * @brief Exécute la boucle de jeu  de graphe
 * @param tab Le tableau des sommets
 * @param n Le nombre de sommets
 * @return tableau du chemin du joueur
*/
int * boucle_jeu_graphe(sommet_t** tab, int n, int * nb_noeuds_chemin, int *fin);

/**
 * @brief Exécute la boucle de jeu dans l'espace
 * @param tab Le tableau des sommets
 * @param n Le nombre de sommets
 * @param chemin Tableau du chemin choisi par le joueur
*/
void boucle_jeu_espace(sommet_t** tab, int n, int * chemin, int n_chemin, int* close);

/**
 * @brief Exécute la boucle de jeu principal
 * @param tab Le tableau des sommets
 * @param n Le nombre de sommets
*/
void boucle_jeu();
#endif