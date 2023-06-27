#include <stdio.h>
#include <time.h>
#include "graph.h"
#include "affiche.h"

int main()
{
    srand(time(NULL));
    int n=0;
    sommet_t ** tab = gen_tab_sommets(&n, 800, 800);
    //printTabCoord(tab, &n);
    //printf("--------------------------------------------\n");
    tab_to_graph(tab, 0, n-1);
    //printTabCoord(tab, &n);

    make_new_links(10, tab, &n);
    //printTabCoord(tab, &n);
    int ** TableauDistances = dist_tab(tab, &n);
    print_dist_tab(TableauDistances, &n);

    affiche(tab, n);

    free2DTab((void **)TableauDistances, n);
    free2DTab((void **)tab, n);

    return 0;
}