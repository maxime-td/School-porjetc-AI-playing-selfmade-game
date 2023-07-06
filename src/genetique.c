#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <limits.h>
#include <pthread.h>

#include "calculPosition.h"
#include "graph.h"
#include "bouclesJeu.h"
#include "bot.h"
#include "entrainement.h"

/**
 * @brief fait fusionner 2 cerveaux et produit des mutations eventuellement
 * @param cerveau1 1er cerveau
 * @param cerveau2 2eme cerveau
 * @param n_regle le nombre de regles par cerveau
 * @param cerveauFils cerveau resultant de la fusion des deux autres
*/
void mutation_gen(int ** cerveau1, int ** cerveau2, int n_regle, int ** cerveauFils)
{
    int regle_taille[N_RULE + 3] = {5, 6, 3, 5, 4, 3, 3, 5};
    int tirage = rand()%20;
    int mutationRate = rand()%3;

    int mutationLi, mutationCol;
    int mutagene; 

    int i;

    for(i=0; i<tirage; i++)
    {
        cerveauFils[i] = cerveau1[i];

    }
    for(i=tirage; i<n_regle; i++)
    {
        cerveauFils[i] = cerveau2[i];
    }

    for(i=0; i<mutationRate; i++)
    {
        mutationCol = rand()%(N_RULE+3);
        mutationLi = rand()%(n_regle);
        if(mutationLi==n_regle && mutationCol<5)
            mutationCol = (rand()%3)+N_RULE;

        mutagene = rand()%(regle_taille[mutationCol]);
        cerveauFils[mutationLi][mutationCol] = mutagene;
    }
}

/**
 * @brief Génère la génération suivante pour le tournoi à partir des meilleurs cerveaux de la génération précédante
 * @param survivants tableau des meilleurs cerveaux de la génération précédante
 * @param n_surv le nombre de cerveaux survivants de la génération précédante
 * @param n_heritiers le nombre de cerveaux de la nouvelle génération
 * @param n_regle le nombre de regles par cerveau
 * @return tableau de la nouvelle génération
*/
int *** nouv_generation(int *** survivants, int n_surv, int n_heritiers, int n_regle)
{
    int rand_cerv1, rand_cerv2, i;
    int *** heritiers = malloc(n_heritiers*sizeof(int **));
    for(i=0; i<n_surv; i++)
    {
        heritiers[i] = survivants[i];
    }
    for(i=n_surv; i<n_heritiers-n_surv; i++)
    {                   
        rand_cerv1 = rand()%n_surv;
        rand_cerv2 = rand()%n_surv;
        heritiers[i] = malloc(n_regle*sizeof(int*));
        mutation_gen(survivants[rand_cerv1], survivants[rand_cerv2], n_regle, heritiers[i]);
    }
    return heritiers;
}

/**
 * @brief fait tourner les 1000 cerveaux sur différents threads par groupes de 100, 
*/
void tournoi(int *** cerveaux)
{
    
}
