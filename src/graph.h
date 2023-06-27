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
 * @brief Créée un tableau de points avec des coordonnées aléatoires
 * @param n le pointeur sur le nombre de points à générer
 * @param width la borne sur l'axe des abscisses
 * @param height la borne sur l axe des ordonnées
 * @return un pointeur sur un tableau de sommets
 */
sommet_t ** genTabSommets(int * n, int width, int height);


/**
 * @brief Affiche le tableau des coordonnées des points
 * @param tab le pointeur sur le tableau de points
 * @param n le pointeur sur le nombre de sommets du tableau
*/
void printTabCoord(sommet_t ** tab, int * n);



/**
 * Dessine un graphe à l'aide d'un rendu SDL.
 *
 * @param renderer Le rendu SDL utilisé pour afficher le graphe.
 * @param graph Le pointeur vers le graphe à dessiner.
 * @param n Le nombre de sommets dans le tableau.
 */
void drawGraph(SDL_Renderer* renderer, sommet_t** tab, int n);

#endif /* GRAPH_H_ */