#ifndef _ENTRAINEMENT_H_
#define _ENTRAINEMENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <limits.h>

#include "calculPosition.h"
#include "graph.h"
#include "bouclesJeu.h"
#include "bot.h"

#define NB_TEST 100

int eval(int ** regle, int n_regle);
int ** recherche_local_bot_iteration(int ** regles, int n_regles, int * ordre, int * score);


/**
 * @brief Genère un tableau de taille n avec des entiers disposés aléatoirement
 * @param n Le nombre d'entiers
*/
int* gen_tableau_alea(int n);

#endif