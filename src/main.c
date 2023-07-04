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
    
    boucle_jeu_sans_graph();
    return 0;
}