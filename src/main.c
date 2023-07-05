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

    int result;
    int n_regle = 20;
    int * alea   = gen_tableau_alea((n_regle-1)*(N_RULE+3)+3);
    int ** regles = generate_tab_rules(n_regle);
    //    set_rules_into_file("RULES.txt", regles, n_regle);


    int *** tournoi = malloc(10*sizeof(int **));
    for(int i=0; i<NB_SURV; i++)
        tournoi[i] = generate_tab_rules(n_regle);
    
    tournoi = nouv_generation(tournoi, NB_SURV, NB_HERITIER, n_regle);


    //regles = practice_cycle(regles, n_regle, alea, &result, 5);
    //printf("%d\n", result);

    //set_rules_into_file("RULES.txt", regles, n_regle);
    free2DTab((void**)regles, n_regle);
    free(alea);

    //boucle_jeu_sans_graph();
    return 0;
}