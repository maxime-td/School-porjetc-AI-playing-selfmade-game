#include <stdio.h>
#include <time.h>
#include "graph.h"

int main()
{
    srand(time(NULL));
    int n=0;
    sommet_t ** tab = genTabSommets(&n, 800, 800);
    printTabCoord(tab, &n);

    return 0;
}