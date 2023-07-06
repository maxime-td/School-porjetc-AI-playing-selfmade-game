#include <stdio.h>
#include <time.h>

#include "graph.h"
#include "affiche.h"
#include "OptiFloyd_Warshall.h"
#include "fourmi.h"
#include "bouclesJeu.h"
#include "entrainement.h"
#include "genetique.h"

int main()
{
    srand(time(NULL));
    /*
    int n_rand = 1000;
    int * rand_tab = gen_rand_tab(n_rand);

    int result;
    int n_regle = 20;
    int * alea   = gen_tableau_alea((n_regle-1)*(N_RULE+3)+3);
    int ** regles = generate_tab_rules(n_regle);

    //    set_rules_into_file("RULES.txt", regles, n_regle);

    
    regles = practice_cycle(regles, n_regle, alea, &result, 5, rand_tab, n_rand);
    printf("%d\n", result);
    set_rules_into_file("RULES.txt", regles, n_regle);
    free2DTab((void**)regles, n_regle);
    free(alea);
    */
    char file_name[25];
    int nb_tournoi = 0;
    int *** res = (int ***) malloc(NB_SURV*sizeof(int**));
    for (int i = 0; i < NB_SURV; i++){
        res[i] = (int **) malloc(sizeof(int *)*N_TAB_REGLE);
        for (int j = 0; j < N_TAB_REGLE; j++){
            res[i][j] = (int*) malloc(sizeof(int)*(N_RULE+3));
        }
        
    }
    
    int *** cerveaux = malloc(NB_HERITIER*sizeof(int**));
    for (int i = 0; i < NB_HERITIER; i++){
        cerveaux[i] = (int **) malloc(N_TAB_REGLE*sizeof(int*));
        generate_tab_rules(N_TAB_REGLE, cerveaux[i]);
    }

    while (1){
        tournoi(cerveaux, res, N_TAB_REGLE);

        for (int i = 0; i < NB_SURV; i++)
        {
            sprintf(file_name, "RULES_GEN%d.txt", i);
            set_rules_into_file(file_name, res[i], N_TAB_REGLE);
        }

        nouv_generation(res, cerveaux, NB_SURV, NB_HERITIER, N_TAB_REGLE);
        nb_tournoi++;
    }
    

    //boucle_jeu_sans_graph();
    return 0;
}