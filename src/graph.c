#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "graph.h"

/**
 * @brief Convertit un tableau de sommets en un graphe.
 * @param tab Le tableau de sommets à convertir.
 * @param n Le nombre de sommets dans le tableau.
 * @return Un pointeur vers un sommet du graph
 */
sommet_t * tabToGraph(sommet_t ** tab, int n){
    if(tab != NULL){
        srand(time(NULL));
        int cut = rand()%(n-1);
        tab[0].voisins[0] = tabToGraph(tab+1, cut);
        tab[0].n_voisin++;
        if(cut != 0){
            tab[0].voisins[1] = tabToGraph(tab+cut, cut-n);
            tab[0].n_voisin++;
        }
        return tab[0];
    }
}


/**
 * @brief Créée un tableau de points avec des coordonnées aléatoires
 * @param n le pointeur sur le nombre de points à générer
 * @param width la borne sur l'axe des abscisses
 * @param height la borne sur l axe des ordonnées
 * @return un pointeur sur un tableau de sommets
 */

sommet_t ** genTabSommets(int * n, int width, int height)
{
    *n = rand()%(N-2) +4;
    sommet_t ** tab = malloc((*n)*sizeof(sommet_t *));
    for(int i=0; i<*n-1; i++)
    {
        tab[i]->x = rand()%(width);
        tab[i]->y = rand()%(height);
    }
    return tab;
}

/**
 * @brief Affiche le tableau des coordonnées des points
 * @param tab le pointeur sur le tableau de points
 * @param n le pointeur sur le nombre de sommets du tableau
*/
void printTabCoord(sommet_t ** tab, int * n)
{
    for(int i=0; i<(*n)-1; i++)
    {
        printf("(%d,%d) - ",tab[i]->x, tab[i]->y);
    }
}