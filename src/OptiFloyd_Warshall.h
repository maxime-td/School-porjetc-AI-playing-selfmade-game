#ifndef SOLOPT_H_
#define SOLOPT_H_


/**
 * @brief Construit la matrice des distances minimales (pour les sommets non reliés, donne la distance du chemin optimal les reliant)
 * @param distTab le tableau des distances initiales (sera modifié)
 * @param n la taille du tableau
*/
void Floyd_Warshall(int ** distTab, int n);


/**
 * @brief Construit un cycle en selectionnant toujours le point le plus proche par lequel on n'est encore pas passé (distances calculé précédemment par Floyd_Warshall)
 * @param distTab le tableau des distances initiales (sera modifié)
 * @param n la taille du tableau
 * @param tabSommets le tableau des sommets
 * @param indDep l'indice dans tabSommet du point de départ
 * @return un tableau d'entier de taille n+1 décrivant le cycle optimal et sa longueur
*/
void cycle_Floyd_Warshall(int **tabWarshall, sommet_t **tabSommets, int **tabDist, int n, int indDep, int *sol);


/**
 * @brief Créé une copie d'un tableau 2D d'entier
 * @param tab le tableau des entiers à copier
 * @param n la taille du tableau
 * @return un tableau 2D d'entiers
*/
int ** copie_tab(int ** tab, int n);

/**
 * @brief Applique l'algo de cyclage ci-dessus sur chaque premier point et sors le plus efficace
 * @param distTab le tableau des distances initiales (sera modifié)
 * @param n la taille du tableau
 * @param tabSommets le tableau des sommets
 * @return le meilleur tableau decrivant le meilleur chemin pour chaque point de départ 
*/
int multi_Start_Floyd_Warshall(int ** tabWarshall, int **distTab, int n, sommet_t **tabSommet);



#endif /* SOLOPT_H_ */