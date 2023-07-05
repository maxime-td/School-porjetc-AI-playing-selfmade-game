#include <stdio.h>
#include <time.h>

#include "graph.h"
#include "affiche.h"
#include "OptiFloyd_Warshall.h"
#include "fourmi.h"
#include "bouclesJeu.h"
#include "entrainement.h"

int main()
{
    srand(time(NULL));

    int n_rand = 1000;
    int * rand_tab = gen_rand_tab(n_rand);

    int result;
    int n_regle = 20;
    int * alea   = gen_tableau_alea((n_regle-1)*(N_RULE+3)+3);
    int ** regles = generate_tab_rules(n_regle);
        set_rules_into_file("RULES.txt", regles, n_regle);


    regles = practice_cycle(regles, n_regle, alea, &result, 5, rand_tab, n_rand);
    printf("%d\n", result);
    set_rules_into_file("RULES.txt", regles, n_regle);
    free2DTab((void**)regles, n_regle);
    free(alea);

    //boucle_jeu_sans_graph();
    return 0;
}