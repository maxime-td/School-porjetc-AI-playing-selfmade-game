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
sommet_t * tabToGraph(sommet_t ** tab, int n){
    if(tab != NULL){
        srand(time(NULL));
        int cut = rand()%(n-1);
        tab[0]->voisins[tab[0]->n_voisin] = tabToGraph(tab+1, cut);
        tab[1]->voisins[tab[1]->n_voisin] = tab[0];
        tab[0]->n_voisin++;
        tab[1]->n_voisin++;
        if(cut != 0){
            tab[0]->voisins[tab[0]->n_voisin] = tabToGraph(tab+cut, cut-n);
            tab[cut]->voisins[tab[cut]->n_voisin] = tab[0];
            tab[0]->n_voisin++;
            tab[cut]->n_voisin++;
        }
        return tab[0];
    }
}




void drawGraph(SDL_Renderer* renderer, sommet_t** tab, int n) {
    char deja_trace[n];

}