#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "graph.h"

/**
 * @brief Convertit un tableau de sommets en un graphe connexe non cyclique.
 * @param tab Le tableau de sommets à convertir.
 * @param n Le nombre de sommets dans le tableau.
 * @return Un pointeur vers un sommet du graph
 */
void tabToGraph(sommet_t ** tab, int start, int end){
    if(start < end){
        int k = rand()%(end-start)+(start+1);
        tab[start]->voisins[start+1] = 1;
        tab[start + 1]->voisins[start] = 1;
        tab[start]->n_voisin++;
        tab[start + 1]->n_voisin++;

        if(k+1 <= end){
            tab[start]->voisins[k+1] = 1; 
            tab[k+1]->voisins[start] = 1;
            tab[start]->n_voisin++;
            tab[k+1]->n_voisin++;   
        }

        tabToGraph(tab, start+1, k);
        tabToGraph(tab, k+1, end);
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
    *n = rand()%(N-3) +4;
    sommet_t ** tab = malloc((*n)*sizeof(sommet_t *));
    for(int i=0; i<*n; i++)
    {
        tab[i] = malloc(sizeof(sommet_t));
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
    printf("nombre de points: %d\n", *n);
    for(int i=0; i<(*n); i++)
    {
        printf("(%d,%d) :\n",tab[i]->x, tab[i]->y);
        for (int j = 0; j < (*n); j++)
        {
            printf("%d ", tab[i]->voisins[j]);
        }
        printf("\n");
        
    }
    printf("\n");

}


void drawGraph(SDL_Renderer* renderer, sommet_t** tab, int n) {
    char deja_trace[n];
}
