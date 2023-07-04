#include <stdio.h>

#include "threads.h"
#include "bouclesJeu.h"
#include "affiche.h"

/**
 * @brief fonction de lancement de thread pour l'algorithme des fourmis
 * @param args Une structure contenant tout les argument a donner a multi_start_fourmi
*/
void *thread_fourmi(FourmiArgs *args)
{
    args->result = multi_start_fourmi(args->matDist, args->n);
    return NULL;
}

/**
 * @brief fonction de lancement de thread pour l'algorithme des floyd
 * @param args Une structure contenant tout les argument a donner a multi_Start_Floyd_Warshall
*/
void *thread_floyd(FloydWarshallArgs *args)
{
    args->result = multi_Start_Floyd_Warshall(args->tabWarshall, args->tabDist, args->n);
    return NULL;
}

/**
 * @brief fonction de lancement de thread pour le timer du jeu
 * @param args Une structure contenant tout les argument necessaire au timer et à la recuperation de ce timer
*/
void *timer(timerArgs* timer){
    while((*timer->fin)){
        SDL_Delay(1);
        (timer->time)++;
    }
    return NULL;
}