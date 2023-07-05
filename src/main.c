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
    //int * alea   = gen_tableau_alea((n_regle-1)*(N_RULE+3));
    int ** regles = generate_tab_rules(n_regle);
        set_rules_into_file("RULES.txt", regles, n_regle);


    //int * alea2   = gen_tableau_alea((n_regle-1)*(N_RULE+3));
    int ** regles2 = generate_tab_rules(n_regle);
        set_rules_into_file("RULES2.txt", regles2, n_regle);

    int ** fils = mutation_gen(regles, regles2, n_regle);
        set_rules_into_file("RULES_FILS.txt", fils, n_regle);

   // regles = practice_cycle(regles, n_regle, alea, &result, 15);
  //  printf("%d\n", result);
   // set_rules_into_file("RULES.txt", regles, n_regle);
    free2DTab((void**)regles, n_regle);
   // free(alea);

    //boucle_jeu_sans_graph();
    return 0;
}