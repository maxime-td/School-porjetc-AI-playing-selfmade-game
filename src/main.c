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
    srand(0*time(NULL));

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

    /*
    char file_name[20];
    int nb_tournoi = 0;
    int n_regle = 20;
    int *** res;
    int *** cerveaux = (int ***) malloc(sizeof(int **)*NB_HERITIER);
    for (int i = 0; i < NB_HERITIER; i++){
        cerveaux[i] = generate_tab_rules(n_regle);
    }

    while (1){
        printf("nb tournoi : %d\n", nb_tournoi);
        res = tournoi(cerveaux, n_regle);

        for (int i = 0; i < NB_SURV; i++)
        {
            sprintf(file_name, "RULES_GEN%d.txt", i);
            set_rules_into_file(file_name, res[i], n_regle);
        }


        cerveaux = nouv_generation(res, NB_SURV, NB_HERITIER, n_regle);
        free(res);
        nb_tournoi++;
    }
    
        */

    //boucle_jeu_sans_graph();
    return 0;
}