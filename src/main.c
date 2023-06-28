#include <stdio.h>
#include <time.h>
#include "graph.h"
#include "affiche.h"
#include "solOpt.h"
#include "fourmi.h"
#include "interactionJoueur.h"

int main()
{
    srand(time(NULL));
    int n=0;
    sommet_t ** tab = gen_tab_sommets(&n, 800, 800);

    tab_to_graph(tab, 0, n-1);

    make_new_links(10, tab, &n);
    print_tab_coord(tab, &n);

    int ** TableauDistances = dist_tab(tab, &n);
    print_dist_tab(TableauDistances, &n);

    int n_chemin = 0;
    int * path = colonni_fourmi(tab, TableauDistances, n, rand()%n, &n_chemin);

    affich_tab(path, n_chemin);
    printf("%d : %d\n",n , path_size(path,TableauDistances , n_chemin));
    
    //affiche(tab, n);

    boucle_jeu(tab, n);

    free2DTab((void **)TableauDistances, n);
    free2DTab((void **)tab, n);
    free(path);

    return 0;
}