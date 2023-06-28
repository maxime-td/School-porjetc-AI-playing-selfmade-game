#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "graph.h"
#include "OptiFloyd_Warshall.h"

/**
 * @brief Construit la matrice des distances minimales (pour les sommets non reliés, donne la distance du chemin optimal les reliant)
 * @param distTab le tableau des distances initiales (sera modifié)
 * @param n la taille du tableau
 */
void Floyd_Warshall(int **distTab, int n)
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
int **copie_tab(int **tab, int n)
{
    int **new = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        new[i] = malloc(n * sizeof(int));
        for (int j = 0; j < n; j++)
        {
            new[i][j] = tab[i][j];
        }
    }
    return new;
}

/**
 * @brief Construit un cycle en selectionnant toujours le point le plus proche par lequel on n'est encore pas passé (distances calculé précédemment par Floyd_Warshall)
 * @param distTab le tableau des distances initiales (sera modifié)
 * @param n la taille du tableau
 * @param tabSommets le tableau des sommets
 * @param indDep l'indice dans tabSommet du point de départ
 * @return un tableau d'entier de taille n+1 décrivant le cycle optimal et sa longueur

int * cycle_Floyd_Warshall(int ** distTab, sommet_t ** tabSommets, int n, int indDep)
{
    int * retour = malloc((n+1)*sizeof(int));

    int ** copie = copie_tab(distTab, n);
    int distR=0, cpt=0;
    int indAct = indDep;
    int indTmp=0;
    int distTmp=999999;
    char dep = tabSommets[indAct]->val;
    char tmp = dep;
    do
    {
        //printf("CPT : %d\n", cpt);
        retour[indAct] = cpt;
        for(int i=0; i<n; i++)
        {
            if(i==0 && i==indAct)
            {
                distTmp = 999999;
            }
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

        //print_dist_tab(copie, &n);
        for(int i=0; i<n; i++)
        {
            copie[indAct][i] = INT_MAX;
            copie[i][indAct] = INT_MAX;
        }
        indAct = indTmp;
        tmp = tabSommets[indAct]->val;
        cpt++;
    } while (tmp != dep && cpt < n+1);
    free2DTab((void **)copie, n);
    distR = distTab[indDep][indAct];


    return retour;
}
*/

/**
 * @brief Construit un cycle en selectionnant toujours le point le plus proche par lequel on n'est encore pas passé (distances calculé précédemment par Floyd_Warshall)
 * @param distTab le tableau des distances initiales (sera modifié)
 * @param n la taille du tableau
 * @param tabSommets le tableau des sommets
 * @param indDep l'indice dans tabSommet du point de départ
 * @return un tableau d'entier de taille n+1 décrivant le cycle optimal et sa longueur
 */
void cycle_Floyd_Warshall(int ** tabWarshall, sommet_t **tabSommets, int ** tabDist ,int n, int indDep, int * sol)
{
    char vDep = tabSommets[indDep]->val;
    char vAct = vDep;

    int indAct = indDep;
    int indNext;

    int compteur = 0;
    int DISTANCE;
    int minDistVoisin = 0;

    int * tabValide = malloc(n*sizeof(int));
    for(int k=0; k<n; k++)
    {
        tabValide[k]=0;
    }
    int nbValide=0;

    while (nbValide != n)
    {
        nbValide+=1;
        tabValide[indAct]=1;
        indNext=-1;
        for(int i=0; i<n; i++)
        {
            if(i!=indAct && tabDist[indAct][i]>0 && tabDist[indAct][i]<minDistVoisin && tabValide[i]==0)
            {
                indNext = i;
                minDistVoisin = tabDist[indAct][i];
            }
        }
        if(indNext==-1)
        {
            for(int j=0; j<n; j++)
            {
                if(j!=indAct && tabWarshall[indAct][j]<minDistVoisin && tabValide[j]==0)
                {
                    indNext = j;
                    minDistVoisin = tabWarshall[indAct][j];
                }
            }
        }
        if(indNext==-1)
        {
            break;
        }
        DISTANCE += minDistVoisin;
    }
    DISTANCE += tabWarshall[vAct][vDep];
    *sol = DISTANCE;
}

/**
 * @brief Applique l'algo de cyclage ci-dessus sur chaque premier point et sors le plus efficace
 * @param distTab le tableau des distances initiales (sera modifié)
 * @param n la taille du tableau
 * @param tabSommets le tableau des sommets
 * @return le meilleur tableau decrivant le meilleur chemin pour chaque point de départ
 */
int *multi_Start_Floyd_Warshall(int **distTab, int n, sommet_t **tabSommet)
{
    int **AllTab = malloc(n * sizeof(int *));
    int *meilleurTab = malloc((n + 1) * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        AllTab[i] = malloc(n * sizeof(int));
    }
    meilleurTab = AllTab[0];
    for (int i = 1; i < n; i++)
    {
        if (AllTab[i][n] < meilleurTab[n])
        {
            meilleurTab = AllTab[0];
        }
    }
    free2DTab((void **)AllTab, n);
    return meilleurTab;
}