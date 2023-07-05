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
 * @return le cerveau fils
*/
int ** mutation_gen(int ** cerveau1, int ** cerveau2, int n_regle)
{
    int regle_taille[N_RULE + 3] = {5, 6, 3, 5, 4, 3, 3, 5};
    int tirage = rand()%20;
    int mutationRate = rand()%3;
    int mutation = rand()%(N_RULE*(n_regle-1)+3);

    int ligne;
    int col;
    int mutagene; 

    int i;
    int ** fils = malloc(n_regle*sizeof(int *));
    for(i=0; i<tirage; i++)
    {
        fils[i] = cerveau1[i];
    }
    for(i=tirage; i<n_regle; i++)
    {
        fils[i] = cerveau2[i];
    }
    for(i=0; i<mutationRate; i++)
    {
        if(mutation>N_RULE*(n_regle-1))
            mutation += 5;
        ligne = (N_RULE*n_regle)%mutation;
        col = (N_RULE*n_regle)/mutation;

        mutagene = rand()%(regle_taille[col]);

        fils[ligne][col] = mutagene;
    }

    return fils;
}


void tournoi(int *** cerveaux){
    
}
