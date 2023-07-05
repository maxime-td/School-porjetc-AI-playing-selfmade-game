#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <limits.h>

#include "calculPosition.h"
#include "graph.h"
#include "bouclesJeu.h"
#include "bot.h"
#include "entrainement.h"

int eval(int ** regle, int n_regle){
    int n = 0;
    sommet_t **tab = NULL;
    int res;

    tab = gen_tab_sommets_rand(&n);  
    tab_to_graph(tab, 0, n - 1);
    make_new_links(7*5/n, tab, &n);
    boucle_jeu_espace(tab, n, NULL, 1, regle, n_regle, &res, 0);

    return res;
}

int ** recherche_local_bot_iteration(int ** regles, int n_regles, int * ordre, int * score){
    int regle_taille[N_RULE+3] = {5, 6, 3, 5, 4, 3, 3, 10};
    int x, y;
    int min;
    int sum_score;
    int best_res = 0;
    int val_best_res = 0;
    int res;

    for (int i = 0; i < n_regles*(N_RULE+3); i++){
        x = ordre[i]%(N_RULE+3);
        y = ordre[i]/(N_RULE+3);
        min = (x == N_RULE+2) ? 1 : -1;
        best_res = 0;
        for (int j = min; j < regle_taille[x] + min; j++){
            sum_score = 0;
            regles[y][x] = j;
            for (int k = 0; k < NB_TEST; i++){
                sum_score += eval(regles, n_regles);
            }

            res = sum_score/NB_TEST;

            if (res > best_res){
                val_best_res = j  ;
                best_res     = res;
            }
        }
        regles[y][x] = val_best_res;
    }

    *score = best_res;
    return regles;
}
/**
 * @brief Genère un tableau de taille n avec des entiers disposés aléatoirement
 * @param n Le nombre d'entiers
*/
int* gen_tableau_alea(int n) {
    int i, j; //Incréments
    int * tab_final = (int *) malloc(sizeof(int)*(n)); //Initialisation du tableau qu'on renvoie
    int * tab_temp = (int *) malloc(sizeof(int)*(n)); //Initialisation du tableau temporaire

    //Initialisation du tableau temporaire
    for(i = 0; i < n; i++)
        tab_temp[i] = i;

    for(i = 0; i < n; i++) {
        j = rand()%n; //On prend un j aléatoire
        while(tab_temp[j] == -1) { //Parcours de tab_temp si on a déjà pris cette valeur
            j++;
            if(j == n) //Retour au début du tableau si on l'a parcouru en entier
                j = 0;
        }
        tab_final[i] = tab_temp[j]; //On met la valeur aléatoire dans tab_final
        tab_temp[j] = -1; //Pour ne pas reprendre cette valeur
    }
    
    free(tab_temp); //On libère le tableau temporaire
    return tab_final;
}
