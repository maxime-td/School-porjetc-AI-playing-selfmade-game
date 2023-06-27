#include <stdio.h>
#include <time.h>
#include "graph.h"

int main()
{
    srand(time(NULL));
    int n=0;
    sommet_t ** tab = genTabSommets(&n, 800, 800);
    printTabCoord(tab, &n);
    printf("--------------------------------------------\n");
    tabToGraph(tab, 0, n-1);
    printTabCoord(tab, &n);

    makeNewLinks(25, tab, &n);
    printTabCoord(tab, &n);
    int ** TableauDistances = distTab(tab, &n);
    printDistTab(TableauDistances, &n);
    return 0;
}