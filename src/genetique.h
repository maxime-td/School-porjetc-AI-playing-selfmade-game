#ifndef _GENETIQUE_H_
#define _GENETIQUE_H_

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <limits.h>

#include "calculPosition.h"
#include "graph.h"
#include "bouclesJeu.h"
#include "bot.h"

#define NB_SURV 16
#define NB_HERITIER 1008
#define MATCH 63
#define N_TAB_REGLE 20
#define NB_TEST_GEN 10


typedef struct{
    int i;
    int *** cerveaux;
    int n_regle;
    int res;
    int score;
}argsMatch;

/**
 * @brief fait fusionner 2 cerveaux et produit des mutations eventuellement
 * @param cerveau1 1er cerveau
 * @param cerveau2 2eme cerveau
 * @param n_regle le nombre de regles par cerveau
 * @param cerveauFils cerveau resultant de la fusion des deux autres
*/
void mutation_gen(int ** cerveau1, int ** cerveau2, int n_regle, int ** cerveauFils);

/**
 * @brief Génère la génération suivante pour le tournoi à partir des meilleurs cerveaux de la génération précédante
 * @param survivants tableau des meilleurs cerveaux de la génération précédante
 * @param n_surv le nombre de cerveaux survivants de la génération précédante
 * @param n_heritiers le nombre de cerveaux de la nouvelle génération
 * @param n_regle le nombre de regles par cerveau
 * @return tableau de la nouvelle génération
*/
void nouv_generation(int *** survivants, int *** heritiers, int n_surv, int n_heritiers, int n_regle);

void * match(argsMatch * argsM);

int eval_gen(int ** cerveau, int n_cerv);

/**
 * @brief Lance un tournoi composé de NB_SURV matchs
 * @param cerveaux Les NB_HERITIER cerveaux de depart
 * @param n_regle nombre de regle par cerveau
 * @return un tableau de 10 meilleurs cerveaux
 */
void tournoi(int *** cerveaux, int *** best ,int n_regle);


void charge_anc_cerv(int *** cerveaux, int * n);


#endif