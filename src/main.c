#include <stdio.h>
#include <time.h>
#include "graph.h"
#include "affiche.h"
#include "OptiFloyd_Warshall.h"
#include "fourmi.h"
#include "interactionJoueur.h"

int main()
{
    srand(time(NULL));
    int n = 0;
    sommet_t **tab = gen_tab_sommets(&n);

    tab_to_graph(tab, 0, n - 1);

    make_new_links(10, tab, &n);
    print_tab_coord(tab, &n);

    int ** TableauDistances = dist_tab(tab, &n);
    int ** tabWarshall = copie_tab(TableauDistances, n);
    Floyd_Warshall(tabWarshall, n);
    print_dist_tab(TableauDistances, &n);

    int res;
    res = multi_Start_Floyd_Warshall(tabWarshall, TableauDistances, n, tab);
    printf("\nTaille Floyd_Warshall VOISIN: %d\n\n", res);

    res = recuit_simule(tabWarshall, n);
    printf("\nTaille Floyd_Warshall RECUIT: %d\n\n", res);

    int n_chemin = 0;
    int *path = colonni_fourmi(TableauDistances, n, rand() % n, &n_chemin);

    affich_tab(path, n_chemin);
    printf("%d : %d\n",n , path_size(path,TableauDistances , n_chemin));

    boucle_jeu(tab, n);

    free(path);
    return 0;
}