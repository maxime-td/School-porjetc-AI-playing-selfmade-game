#ifndef GRAPH_H_
#define GRAPH_H_

#define N  12// NOMBRE_MAX_SOMMET 
#define N_MIN  5 // NOMBRE_MIN_SOMMET 
#define R 300 //Rayon cercle repartition
#define W get_w_window()
#define H get_h_window()
 
typedef struct {
    int x;
    int y;
} Point;

/**
 * @struct sommet_s
 * Structure représentant un sommet dans un graphe.
 */
typedef struct sommet_s{
    int voisins[N];  /**< Tableau des voisins du sommet */
    int n_voisin;                /**< Nombre de voisins du sommet */
    char val;
    int x;                       /**< Coordonnée x du sommet */
    int y;                       /**< Coordonnée y du sommet */
}sommet_t;

/**
 * @struct segmment_s
 * Structure représentant une arrete d'un graphe.
 */
typedef struct segmment_s{
    Point p1;
    Point p2;
}segmment_t;


/**
 * Libère la mémoire allouée pour un tableau à deux dimensions.
 *
 * @param tab Le tableau à libérer.
 * @param n   Le nombre de lignes du tableau.
 */
void free2DTab(void ** tab, int n);

/**
 * @brief Convertit un tableau de sommets en un graphe connexe non cyclique.
 * @param tab Le tableau de sommets à convertir.
 * @param n Le nombre de sommets dans le tableau.
 * @return Un pointeur vers un sommet du graph
 */
void tab_to_graph(sommet_t ** tab, int start, int end, int use_rand, int * tab_rand, int i_rand, int n_rand);

/**
 * @brief Créée un tableau de points avec des coordonnées aléatoires
 * @param n le pointeur sur le nombre de points à générer
 * @return un pointeur sur un tableau de sommets
 */
sommet_t ** gen_tab_sommets_cercle(int * n);

/**
 * @brief Créée un tableau de points avec des coordonnées aléatoires
 * @param n le pointeur sur le nombre de points à générer
 * @return un pointeur sur un tableau de sommets
 */
sommet_t ** gen_tab_sommets_rand(int * n, int use_rand, int * tab_rand, int n_rand, int i_rand);

/**
 * @brief Créée des liasons aléatoires entre les sommets
 * @param p probabilité de création de liaison pour chaque couple de point.
 * @param tab le pointeur sur le tableau des sommets
 * @param n le pointeur sur le ombre de sommets
 */
void make_new_links(int p, sommet_t ** tab, int * n, int use_rand, int * tab_rand, int n_rand, int i_rand);

/**
 * @brief Affiche le tableau des coordonnées des points
 * @param tab le pointeur sur le tableau de points
 * @param n le pointeur sur le nombre de sommets du tableau
*/
void print_tab_coord(sommet_t ** tab, int * n);

/**
 * @brief Calcul la distance entre 2 sommets
 * @param a pointeur sur un sommet
 * @param b pointeur sur un sommet
 * @return La distance
 */
int calcul_distance(sommet_t * a, sommet_t * b);

/**
 * @brief Créée un tableau de distance entre les points, -1 si non lié, > 0 sinon.
 * @param tab pointeur sur le tableau des sommets
 * @param n le pointeur sur le ombre de sommets
 * @return Le pointeur sur le tableau 2D des distances.
 */
int ** dist_tab(sommet_t ** tab, int * n);

/**
 * @brief Affiche le tableau des distances
 * @param distTab le tableau des distances
 * @param n le pointeur sur le ombre de sommets
 */
void print_dist_tab(int ** distTab, int * n);

/**
 * @brief Transforme un chemin en sous-graphe pour le tracé avec draw_graph
 * @param chemin Le chemin
 * @param n_chemin Le nombre de noeud dans le chemin
 * @param tab Le graphe initial
 * @param n Le nombre de noeuds dans le graphe initial
*/
sommet_t** chemin_en_graphe(int * chemin, int n_chemin, sommet_t** tab, int n, int* n_sous_graph);

segmment_t * gen_tab_seg(sommet_t ** tab, int n, int * n_seg);

#endif /* GRAPH_H_ */