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

    /* Initialisations */
    char tab_deja_trace[n]; //tableau des sommets déjà tracés
    int bool_deja_trace; //Booléeen si un sommet à déjà été tracé ou non. 0 = Faux, 1 = Vrai
    int i, j, k; //Incréments
    sommet_t* sommet_courant; //Sommet courant
    sommet_t* voisin_courant; //Voisin courant

    /* Initialisation tableau deja_trace */
    for(i = 0; i < n; i+=1) {
        deja_trace[i] = NULL;
    }

    /* Parcour */
    for(i = 0; i < n; i+=1) {
        sommet_courant = *tab[i];
        //[tracer sommet]
        //[ajouter sommet_courant a tab_deja_trace]
        
        for(j = 0; j < sommet_courant.n_voisins; j+=1) {
            voisin_courant = sommet_courant.voisins[j];

            /* Vérification si déjà tracé ou pas */
            bool_deja_trace = 0;
            while(deja_trace[k] != NULL) {
                if(deja_trace[k] == voisin_courant.val)
                    bool_deja_trace = 1;
                k += 1;
            }

            if(bool_deja_trace == 0) {
                //[tracer lien]
                //[ajouter voisin_courant à tab_deja_trace]
            }
        }
    }
}