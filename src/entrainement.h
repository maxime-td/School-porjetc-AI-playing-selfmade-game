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

#define NB_TEST 10

typedef struct {
    int ** regle;
    int n_regle;
    int * res;
    int * x;
    int * y;
    int * val;
    int n_val;
    int * tab_rand;
    int n_rand;
}argsEval;

int **copie_line_tab(int **tab, int n, int * i, int n_val);
void* eval(argsEval * argsEv);

/**
 * @brief effectue une itteration de la recherche local soit un parcour de tableau complet
 * @param regles le tableau de regles (le cerveau)
 * @param n_regles le nombre de regles
 * @param ordre un tableau donnant l'ordre de parcour des regles
 * @param score permet de recupérer le score trouvé par le meilleur bot
 * @param n_val le nombre de valeur à modifier en même temps
 * @param tab_rand tableau de nombre aleatoire pregenere
 * @param n_rand taille du tableau
*/
int **recherche_local_bot_iteration(int **regles, int n_regles, int *ordre, int *score, int n_val, int * tab_rand, int n_rand);

void free_line(int ** tab, int *i, int n_val);
/**
 * @brief Genère un tableau de taille n avec des entiers disposés aléatoirement
 * @param n Le nombre d'entiers
*/
void gen_tableau_alea(int n, int* tab_final);


int ** practice_cycle(int **regles, int n_regles, int *score, int eps, int * tab_rand, int n_rand);

/**
 * @brief genere un tableau de taille n de nombre aleatoire venant de rand()
 * @param n taille du tableau à générer
 * @return le tableau
*/
int * gen_rand_tab(int n);


#endif