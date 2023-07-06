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
#include "genetique.h"

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
        if(mutationLi==(n_regle-1) && mutationCol<5)
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

int eval_gen(int ** cerveau, int n_cerv)
{
    int n = 0;
    sommet_t **tab = NULL;
    int res;
    int sum_score;
    sum_score = 0;

    for (int k = 0; k < NB_TEST_GEN; k++)
    {
        
        tab = gen_tab_sommets_rand(&n, 1, NULL, 0, 0);
        tab_to_graph(tab, 0, n - 1, 1, NULL, 0, 0);
        make_new_links(7 * 5 / n, tab, &n, 1, NULL, 0, 0);
        boucle_jeu_espace(tab, n, NULL, 1, cerveau, n_cerv, &res, 0, 1, 1, NULL, 0);
        sum_score += res;
    }

    res = sum_score / NB_TEST_GEN;
    return res;
}

void * match(argsMatch * argsM){
    int best = 0;
    int best_score = 0, score = 0;
    for (int i = argsM->i; i < MATCH+argsM->i; i++){
        
        score = eval_gen(argsM->cerveaux[i], argsM->n_regle);
        if (score > best_score){
            best = i;
            best_score = score;
        }
    }
    argsM->res = best;
    return NULL;
}


/**
 * @brief Lance un tournoi composé de NB_SURV matchs
 * @param cerveaux Les NB_HERITIER cerveaux de depart
 * @param n_regle nombre de regle par cerveau
 * @return un tableau de 10 meilleurs cerveaux
 */
int *** tournoi(int *** cerveaux, int n_regle){
    pthread_t matchs[10];
    argsMatch argsMs[10];
    int is_best = 0;
    int *** best = NULL;
    int k = 0;
    for (int i = 0; i < NB_SURV; i++){
        printf("Start : %d\n", i);
        argsMs[i].cerveaux = cerveaux;
        argsMs[i].i = i*MATCH;
        argsMs[i].n_regle = n_regle;
        pthread_create(&matchs[i], NULL, (void *(*)(void *))match, &argsMs[i]);       
    }
    
    for (int i = 0; i < NB_SURV; i++){
        pthread_join(matchs[i], NULL);
        printf("Stop : %d\n", i);
    }

    best = (int ***) malloc(sizeof(int**)*NB_SURV);
    for (int i = 0; i < NB_HERITIER; i++){
        is_best = 0;
        for (int j = 0; j < NB_SURV; j++){
            if(i == argsMs[j].res){
                is_best = 1;
            }
        }
        
        if(is_best){
            best[k] = cerveaux[i];
            k++;
        }else{
            free2DTab((void**) cerveaux[i], n_regle);
            
        }
    }
    free(cerveaux);
    return best;
}
