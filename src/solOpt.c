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
                    distTab[i][j] = 9999999;
                }

                if (distTab[i][k] != -1 && distTab[k][j] != -1 && distTab[i][k] + distTab[k][j] < distTab[i][j])
                {
                    distTab[i][j] = distTab[i][k] + distTab[k][j];
                }
            }
        }
    }
}


/**
 * @brief Créé une copie d'un tableau 2D d'entier
 * @param tab le tableau des entiers à copier
 * @param n la taille du tableau
 * @return un tableau 2D d'entiers
*/
int ** copie_tab(int ** tab, int n)
{
    int ** new = malloc(n*sizeof(int *));
    for(int i=0; i<n; i++)
    {
        new[i] = malloc(n*sizeof(int));
        for(int j=0; j<n; j++)
        {
            new[i][j] = tab [i][j];
        }
    }
    return new;
}



/**
 * @brief Construit un cycle en selectionnant toujours le point le plus proche par lequel on n'est encore pas passé (distances calculé précédemment par Floyd_Warshall)
 * @param distTab le tableau des distances initiales (sera modifié)
 * @param n la taille du tableau
 * @param tabSommets le tableau des sommets
*/
int cycle_min_approx(int ** distTab, sommet_t ** tabSommets, int n)
{
    int ** copie = copie_tab(distTab, n);
    int somme=0, cpt=0;
    int indDep = rand()%n;
    int indAct = indDep;
    int indTmp=0;
    int distTmp=0;
    char dep = tabSommets[indAct]->val;
    char tmp = dep;
    do
    {
        printf("Sommet courant: %c\n", tmp);
        for(int i=0; i<n; i++)
        {
            if(i==0 && i!=indAct)
            {
                distTmp = copie[indAct][i];
                indTmp = i;
            }
            if (i!=0 && i!=indAct && distTmp>copie[indAct][i])
            {
                distTmp = copie[indAct][i];
                indTmp = i;
            }
        }
        for(int i=0; i<n; i++)
        {
            copie[indAct][i] = INT_MAX;
            copie[i][indAct] = INT_MAX;
        }
        somme += distTmp;
        indAct = indTmp;
        tmp = tabSommets[indAct]->val;
        cpt++;
        printf("Somme actualisée: %d\n", somme);
        //print_dist_tab(copie, &n);
    } while (tmp != dep && cpt != n);
    somme += distTab[indDep][indAct];


    return somme;
    
}