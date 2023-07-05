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

#define NB_TEST 10

typedef struct {
    int ** regle;
    int n_regle;
    int * res;
    int x;
    int y;
    int val;
}argsEval;

int ** copie_1_line_tab(int ** tab, int n, int i);
void* eval(argsEval * argsEv);
int ** recherche_local_bot_iteration(int ** regles, int n_regles, int * ordre, int * score);


/**
 * @brief Genère un tableau de taille n avec des entiers disposés aléatoirement
 * @param n Le nombre d'entiers
*/
int* gen_tableau_alea(int n);

#endif