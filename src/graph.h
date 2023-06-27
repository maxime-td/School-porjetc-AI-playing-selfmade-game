#ifndef GRAPH_H_
#define GRAPH_H_
#define MAX_VOISIN 32

#include <SDL2/SDL.h>

#define N 12 // NOMBRE_MAX_SOMMET 

/**
 * @struct sommet_s
 * Structure représentant un sommet dans un graphe.
 */
typedef struct sommet_s{
    struct sommet_s ** voisins;  /**< Tableau des voisins du sommet */
    int n_voisin;                /**< Nombre de voisins du sommet */
    int x;                       /**< Coordonnée x du sommet */
    int y;                       /**< Coordonnée y du sommet */
}sommet_t;

/**
 * @brief Convertit un tableau de sommets en un graphe.
 * @param tab Le tableau de sommets à convertir.
 * @param n Le nombre de sommets dans le tableau.
 * @return Un pointeur vers un sommet du graph
 */
sommet_t * tabToGraph(sommet_t ** tab, int n);


/**
 * Dessine un graphe à l'aide d'un rendu SDL.
 *
 * @param renderer Le rendu SDL utilisé pour afficher le graphe.
 * @param graph Le pointeur vers le graphe à dessiner.
 */
void drawGraph(SDL_Renderer* renderer, sommet_t * graph);

#endif /* GRAPH_H_ */