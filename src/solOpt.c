#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "graph.h"
#include "solOpt.h"


/**
 * @brief Construit la matrice des distances minimales (pour les sommets non reliés, donne la distance du chemin optimal les reliant)
 * @param distTab le tableau des distances initiales (sera modifié)
 * @param n la taille du tableau
*/
void Floyd_Warshall(int ** distTab, int n)
{
    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (distTab[i][j] == -1)
                {
                    distTab[i][j] = INT_MAX;
                }

                if (distTab[i][k] != -1 && distTab[k][j] != -1 && distTab[i][k] + distTab[k][j] < distTab[i][j])
                {
                    distTab[i][j] = distTab[i][k] + distTab[k][j];
                }
            }
        }
    }
}

int cycle_min_approx(int ** distTab, sommet_t ** tabSommets, int n)
{
    int somme=0, cpt=0;
    int indAct = rand()%n;
    int indTmp=0;
    int distTmp=0;
    char dep = tabSommets[indAct]->val;
    char tmp = dep;
    do
    {
        printf("Somet courant: %c\n", tmp);
        for(int i=0; i<n; i++)
        {
            if(i==0 && i!=indAct)
            {
                distTmp = distTab[indAct][i];
                indTmp = i;
            }
            if (i!=0 && i!=indAct && distTmp>distTab[indAct][i])
            {
                distTmp = distTab[indAct][i];
                indTmp = i;
            }
        }
        for(int i=0; i<n; i++)
        {
            distTab[indAct][i] = INT_MAX;
            distTab[i][indAct] = INT_MAX;
        }
        somme += distTmp;
        indAct = indTmp;
        tmp = tabSommets[indAct]->val;
        cpt++;
        print_dist_tab(distTab, &n);
    } while (tmp != dep && cpt != n);

    return somme;
    
}