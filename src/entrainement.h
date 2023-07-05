#ifndef ENTRAINEMENT_H
#define ENTRAINEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <limits.h>

#include "calculPosition.h"
#include "graph.h"
#include "bouclesJeu.h"
#include "bot.h"

#define NB_TEST 50

typedef struct {
    int ** regle;
    int n_regle;
    int * res;
    int * x;
    int * y;
    int * val;
    int n_val;
}argsEval;

int **copie_line_tab(int **tab, int n, int * i, int n_val);
void* eval(argsEval * argsEv);
int **recherche_local_bot_iteration(int **regles, int n_regles, int *ordre, int *score, int n_val);

void free_line(int ** tab, int *i, int n_val);
/**
 * @brief Genère un tableau de taille n avec des entiers disposés aléatoirement
 * @param n Le nombre d'entiers
*/
int* gen_tableau_alea(int n);


int ** practice_cycle(int **regles, int n_regles, int *ordre, int *score, int eps);


#endif