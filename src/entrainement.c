#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <limits.h>
#include <pthread.h>
#include <unistd.h>

#include "calculPosition.h"
#include "graph.h"
#include "bouclesJeu.h"
#include "bot.h"
#include "entrainement.h"

void *eval(argsEval *argsEv)
{
    int n = 0;
    sommet_t **tab = NULL;
    int res;
    int sum_score;

    int **regle_copie = copie_line_tab(argsEv->regle, argsEv->n_regle, argsEv->y, argsEv->n_val);
    for (int i = 0; i < argsEv->n_val; i++){
        regle_copie[argsEv->y[i]][argsEv->x[i]] = argsEv->val[i];
    }

    sum_score = 0;

    for (int k = 0; k < NB_TEST; k++)
    {
        tab = gen_tab_sommets_rand(&n);
        tab_to_graph(tab, 0, n - 1);
        make_new_links(7 * 5 / n, tab, &n);
        boucle_jeu_espace(tab, n, NULL, 1, regle_copie, argsEv->n_regle, &res, 0, 1);
        sum_score += res;
    }

    res = sum_score / NB_TEST;
    (*argsEv->res) = res;
    free_line(regle_copie, argsEv->y, argsEv->n_val);
    return NULL;
}

void free_line(int ** tab, int *i, int n_val){
    for (int j = 0; j < n_val; j++){
        if(tab[i[j]]){
            free(tab[i[j]]);
            tab[i[j]] = NULL;
        }
    }
    
    free(tab);
}

int **copie_line_tab(int **tab, int n, int * i, int n_val){
    int **tabCopie = (int **)malloc(sizeof(int *) * n);
    int find = 0;
    for (int j = 0; j < n; j++)
    {
        tabCopie[j] = tab[j];
        find = 0;
        for(int l = 0 ; l < n_val && !find; l++){
            if (i[l] == j){
                tabCopie[j] = (int *)malloc(sizeof(int) * (N_RULE + 3));
                for (int k = 0; k < (N_RULE + 3); k++){
                    tabCopie[j][k] = tab[i[l]][k];
                }
                find = 1;
            }
        }
    }
    return tabCopie;
}

int **recherche_local_bot_iteration(int **regles, int n_regles, int *ordre, int *score, int n_val)
{
    int regle_taille[N_RULE + 3] = {5, 6, 3, 5, 4, 3, 3, 5};
    pthread_t pthreads[6];
    argsEval argsE[6];
    int x[3], y[3], val[3];
    int min;
    int best_res = 0;
    int val_best_res[3];
    int res[6];

    for (int i = 0; i < ((n_regles-1) * (N_RULE + 3) - n_val + 1); i++)
    {
        for (int j = 0; j < n_val; j++){
            x[j] = ordre[i+j] % (N_RULE + 3);
            y[j] = ordre[i+j] / (N_RULE + 3);
        }

        best_res = 0;
        printf("%d/151\n", i);
        for (int k = 0; k < ((n_val < 3) ? 1 : regle_taille[x[2]]) ; k++){
            if (n_val == 3){
                min = (x[2] == N_RULE + 2) ? 1 : -1;
                val[2] =  k+min;
            }
            
            for (int l = 0; l < ((n_val < 2) ? 1 : regle_taille[x[1]]) ; l++){
                if (n_val == 2){
                    min = (x[1] == N_RULE + 2) ? 1 : -1;
                    val[1] =  k+min;
                }
                
                for (int j = 0; j < regle_taille[x[0]]; j++){
                    min = (x[0] == N_RULE + 2) ? 1 : -1;
                    val[0] = j+min;
                    argsE[j].n_regle = n_regles;
                    argsE[j].regle = regles;
                    argsE[j].y = y;
                    argsE[j].x = x;
                    argsE[j].val = val;
                    argsE[j].n_val = n_val;
                    argsE[j].res = &res[j];

                    //eval(&(argsE[j]));
                    pthread_create(&pthreads[j], NULL, (void *(*)(void *))eval, &(argsE[j]));
                }

                for (int j = 0; j < regle_taille[x[0]]; j++)
                {
                    pthread_join(pthreads[j], NULL);
                    if (res[j] > best_res)
                    {
                        for (int m = 0; m < n_val ; m++){
                            val_best_res[m] = argsE[j].val[m];
                        }
                        
                        best_res = res[j];
                    }
                }
            }
        }


        for (int m = 0; m < n_val ; m++){
            regles[y[m]][x[m]] = val_best_res[m];
        }
    }

    *score = best_res;
    return regles;
}
/**
 * @brief Genère un tableau de taille n avec des entiers disposés aléatoirement
 * @param n Le nombre d'entiers
 */
int *gen_tableau_alea(int n)
{
    int i, j;                                          // Incréments
    int *tab_final = (int *)malloc(sizeof(int) * (n)); // Initialisation du tableau qu'on renvoie
    int *tab_temp = (int *)malloc(sizeof(int) * (n));  // Initialisation du tableau temporaire

    // Initialisation du tableau temporaire
    for (i = 0; i < n; i++)
        tab_temp[i] = i;

    for (i = 0; i < n; i++)
    {
        j = rand() % n; // On prend un j aléatoire
        while (tab_temp[j] == -1)
        { // Parcours de tab_temp si on a déjà pris cette valeur
            j++;
            if (j == n) // Retour au début du tableau si on l'a parcouru en entier
                j = 0;
        }
        tab_final[i] = tab_temp[j]; // On met la valeur aléatoire dans tab_final
        tab_temp[j] = -1;           // Pour ne pas reprendre cette valeur
    }

    free(tab_temp); // On libère le tableau temporaire
    return tab_final;
}
