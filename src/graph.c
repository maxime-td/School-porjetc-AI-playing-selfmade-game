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




void drawGraph(SDL_Renderer* renderer, sommet_t** graph) {

}