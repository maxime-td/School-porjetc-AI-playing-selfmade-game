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

void *eval(argsEval *argsEv)
{
    int n = 0;
    sommet_t **tab = NULL;
    int res;
    int sum_score;

    int **regle_copie = copie_1_line_tab(argsEv->regle, argsEv->n_regle, argsEv->y);
    regle_copie[argsEv->y][argsEv->x] = argsEv->val;

    sum_score = 0;

    for (int k = 0; k < NB_TEST; k++)
    {
        //Mettre ça en threas
        tab = gen_tab_sommets_rand(&n);
        tab_to_graph(tab, 0, n - 1);
        make_new_links(7 * 5 / n, tab, &n);
        boucle_jeu_espace(tab, n, NULL, 1, regle_copie, argsEv->n_regle, &res, 0);
        sum_score += res;
    }

    res = sum_score / NB_TEST;
    (*argsEv->res) = res;
    free(regle_copie[argsEv->y]);
    free(regle_copie);
    return NULL;
}

int **copie_1_line_tab(int **tab, int n, int i)
{
    int **tabCopie = (int **)malloc(sizeof(int *) * n);
    for (int j = 0; j < n; j++)
    {
        if (j != i)
        {
            tabCopie[j] = tab[j];
        }
        else
        {
            tabCopie[j] = (int *)malloc(sizeof(int) * (N_RULE + 3));
            for (int k = 0; k < (N_RULE + 3); k++)
            {
                tabCopie[j][k] = tab[i][k];
            }
        }
    }
    return tabCopie;
}

int **recherche_local_bot_iteration(int **regles, int n_regles, int *ordre, int *score)
{
    int regle_taille[N_RULE + 3] = {5, 6, 3, 5, 4, 3, 3, 5};
    pthread_t pthreads[6];
    argsEval argsE[6];
    int x, y;
    int min;
    int best_res = 0;
    int val_best_res = 0;
    int res[6];

    for (int i = 0; i < (n_regles - 1) * (N_RULE + 3); i++)
    {
        x = ordre[i] % (N_RULE + 3);
        y = ordre[i] / (N_RULE + 3);
        min = (x == N_RULE + 2) ? 1 : -1;
        best_res = 0;
        printf("%d/160\n", i);

        for (int j = 0; j < regle_taille[x]; j++)
        {
            argsE[j].n_regle = n_regles;
            argsE[j].regle = regles;
            argsE[j].y = y;
            argsE[j].x = x;
            argsE[j].val = j + min;
            argsE[j].res = &res[j];
            pthread_create(&pthreads[j], NULL, (void *(*)(void *))eval, &(argsE[j]));
        }

        for (int j = 0; j < regle_taille[x]; j++)
        {
            pthread_join(pthreads[j], NULL);
            if (res[j] > best_res)
            {
                val_best_res = j + min;
                best_res = res[j];
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
