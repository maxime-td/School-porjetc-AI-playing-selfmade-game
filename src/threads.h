
#ifndef THREADS_H
#define THREADS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "graph.h"
#include "affiche.h"
#include "fourmi.h"
#include "bouclesJeu.h"
#include "OptiFloyd_Warshall.h"

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

// Structure pour les arguments du Timer
typedef struct {
    SDL_bool * fin;
    int time;
} timerArgs;

/**
 * @brief fonction de lancement de thread pour l'algorithme des fourmis
 * @param args Une structure contenant tout les argument a donner a multi_start_fourmi
*/
void *thread_fourmi(FourmiArgs *args);

/**
 * @brief fonction de lancement de thread pour l'algorithme des floyd
 * @param args Une structure contenant tout les argument a donner a multi_Start_Floyd_Warshall
*/
void *thread_floyd(FloydWarshallArgs *args);

/**
 * @brief fonction de lancement de thread pour le timer du jeu
 * @param args Une structure contenant tout les argument necessaire au timer et à la recuperation de ce timer
*/
void *timer(timerArgs* timer);

#endif