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

#define NB_SURV 10
#define NB_HERITIER 1000

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
int *** nouv_generation(int *** survivants, int n_surv, int n_heritiers, int n_regle);


#endif