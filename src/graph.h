#ifndef GRAPH_H_
#define GRAPH_H_
#define MAX_VOISIN 16

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
 * @return Un pointeur vers le graphe créé.
 */
sommet_t * tabToGraph(sommet_t ** tab, int n);

/**
 * @brief Dessine le graphe sur l'écran.
 * @param graph Le graphe à dessiner.
 */
void drawGraph(sommet_t * graph);

#endif /* GRAPH_H_ */