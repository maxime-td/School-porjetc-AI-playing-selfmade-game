#include <stdio.h>
#include <time.h>
#include "graph.h"
#include "affiche.h"

int main()
{
    srand(time(NULL));
    int n=0;
    sommet_t ** tab = genTabSommets(&n, 800, 800);
    //printTabCoord(tab, &n);
    //printf("--------------------------------------------\n");
    tabToGraph(tab, 0, n-1);
    //printTabCoord(tab, &n);

    makeNewLinks(5, tab, &n);
    //printTabCoord(tab, &n);
    int ** TableauDistances = distTab(tab, &n);
    printDistTab(TableauDistances, &n);

    affiche(tab, n);

    return 0;
}