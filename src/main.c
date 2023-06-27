#include <stdio.h>
#include "graph.h"

int main()
{
    int n=0;
    sommet_t ** tab = genTabSommets(&n, 800, 800);
    printTabCoord(tab, &n);

    return 0;
}