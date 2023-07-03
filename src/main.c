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
    sommet_t **tab = NULL;

    tab = gen_tab_sommets(&n);
    tab_to_graph(tab, 0, n - 1);
    make_new_links(10*5/n, tab, &n);
    init(tab, n);


    int **warshallDist;
    int **distMat = dist_tab(tab, &n);
    warshallDist = copie_tab(distMat, n);

    printf("%d",recuit_simule(warshallDist, n, tab));
    //boucle_jeu();
    return 0;
}