#ifndef GRAPH_H_
#define GRAPH_H_
#define MAX_VOISIN 32

#include <SDL2/SDL.h>

/**
 * @struct sommet_s
 * Structure représentant un sommet dans un graphe.
 */
typedef struct sommet_s{
    struct sommet_s * voisins[MAX_VOISIN];  /**< Tableau des voisins du sommet */
    int n_voisin;                /**< Nombre de voisins du sommet */
    char val;
    int x;                       /**< Coordonnée x du sommet */
    int y;                       /**< Coordonnée y du sommet */
}sommet_t;

/**
 * @brief Convertit un tableau de sommets en un graphe connexe non cyclique.
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
 * @param n Le nombre de sommets dans le tableau.
 */
void drawGraph(SDL_Renderer* renderer, sommet_t** tab, int n);

#endif /* GRAPH_H_ */