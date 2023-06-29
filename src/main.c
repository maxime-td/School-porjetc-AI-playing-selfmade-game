#include <stdio.h>
#include <time.h>
#include "graph.h"
#include "affiche.h"
#include "OptiFloyd_Warshall.h"
#include "fourmi.h"
#include "bouclesJeu.h"

int main()
{
    srand(time(NULL));
    int n = 0;
    sommet_t **tab = gen_tab_sommets(&n);

    tab_to_graph(tab, 0, n - 1);

    make_new_links(10, tab, &n);

    boucle_jeu(tab, n);
    return 0;
}