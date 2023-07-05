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

    int result;
    int n_regle = 20;
    int * alea   = gen_tableau_alea((n_regle-1)*(N_RULE+3));
    int ** regles = generate_tab_rules(n_regle);

    regles = recherche_local_bot_iteration(regles, n_regle, alea, &result);
    printf("%d\n", result);

    free2DTab((void**)regles, n_regle);
    free(alea);

    //boucle_jeu_sans_graph();
    return 0;
}