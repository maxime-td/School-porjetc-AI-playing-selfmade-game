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
    srand(0*time(NULL));

    int result;
    int n_regle = 20;
    int * alea   = gen_tableau_alea((n_regle-1)*(N_RULE+3));
    int ** regles = generate_tab_rules(n_regle);
        set_rules_into_file("RULES.txt", regles, n_regle);

<<<<<<< HEAD

    regles = practice_cycle(regles, n_regle, alea, &result, 15);
=======
    regles = recherche_local_bot_iteration(regles, n_regle, alea, &result, 1);
>>>>>>> 0de50fe8142b0417e2ddf546b14035e38fc543e6
    printf("%d\n", result);
    set_rules_into_file("RULES.txt", regles, n_regle);
    free2DTab((void**)regles, n_regle);
    free(alea);

    //boucle_jeu_sans_graph();
    return 0;
}