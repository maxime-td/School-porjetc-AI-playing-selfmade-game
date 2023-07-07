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
    int mutationRate = (rand()%3)+2;

    int mutationLi, mutationCol;
    int mutagene; 

    int i;

    for(i=0; i<tirage; i++)
    {
        for (int j = 0; j < N_RULE+3; j++)
        {
            cerveauFils[i][j] = cerveau1[i][j];
        }
    }
    for(i=tirage; i<n_regle; i++)
    {
        for (int j = 0; j < N_RULE+3; j++){
            cerveauFils[i][j] = cerveau2[i][j];
        }
    }

    for(i=0; i<mutationRate; i++)
    {
        mutationCol = rand()%(N_RULE+3);
        mutationLi = rand()%(n_regle);
        while(mutationLi==(n_regle-1) && mutationCol<N_RULE)
            mutationCol = (rand()%3)+N_RULE;

        mutagene = rand()%(regle_taille[mutationCol])-1;
        if(mutationCol == 7)
            mutagene+=2;
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
void nouv_generation(int *** survivants, int *** heritiers, int n_surv, int n_heritiers, int n_regle)
{
    int rand_cerv1, rand_cerv2, i;
    //int *** heritiers = malloc(n_heritiers*sizeof(int **));
    for(i=0; i<n_surv; i++)
    {
        heritiers[i] = survivants[i];
    }
    for(i=n_surv; i<n_heritiers-n_surv; i++)
    {                   
        rand_cerv1 = rand()%n_surv;
        rand_cerv2 = rand()%n_surv;
        //heritiers[i] = malloc(n_regle*sizeof(int*));
        mutation_gen(survivants[rand_cerv1], survivants[rand_cerv2], n_regle, heritiers[i]);
    }
    //return heritiers;
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
    int best = argsM->i;
    int best_score = 0, score = 0;
    for (int i = argsM->i; i < MATCH+argsM->i; i++){
       // printf("NUM MATCH : %d\n", i);

        score = eval_gen(argsM->cerveaux[i], argsM->n_regle);
        if (score > best_score){
            best = i;
            best_score = score;
        }
    }
    argsM->res = best;
    argsM->score = best_score;
    return NULL;
}


/**
 * @brief Lance un tournoi composé de NB_SURV matchs
 * @param cerveaux Les NB_HERITIER cerveaux de depart
 * @param n_regle nombre de regle par cerveau
 * @return un tableau de 10 meilleurs cerveaux
 */
void tournoi(int *** cerveaux, int *** best ,int n_regle){
    pthread_t matchs[NB_SURV];
    argsMatch argsMs[NB_SURV];
    int is_best = 0;
    int k = 0;
    for (int t = 0; t < NB_SURV; t++){
        printf("Start : %d\n", t);
        argsMs[t].cerveaux = cerveaux;
        argsMs[t].i = t*MATCH;
        argsMs[t].n_regle = n_regle;
        pthread_create(&matchs[t], NULL, (void *(*)(void *))match, &argsMs[t]);     
    }
    
    for (int i = 0; i < NB_SURV; i++){
        pthread_join(matchs[i], NULL);
        printf("Stop : %d\n", i);
    }

    int sum = 0;
    for (int i = 0; i < NB_SURV; i++)
    {
        sum += argsMs[i].score;
        printf("score %d : %d\n", i, argsMs[i].score);
        if (argsMs[i].score > 1000){
            set_rules_into_file("GOOD_RULES.txt", cerveaux[argsMs->res], N_TAB_REGLE);
        }
        
    }
    
    printf("Moyenne : %d\n", sum/NB_SURV);

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
        }
    }
}

void charge_anc_cerv(int *** cerveaux, int * n)
{
    char name[25];
    for(int i=0; i<NB_SURV; i++)
    {
        cerveaux[i] = (int**) malloc(sizeof(int*)*(N_TAB_REGLE));
        sprintf(name, "RULES_GEN%d.txt", i);
        get_rule_from_file(name, n, cerveaux[i]);
    }

}
