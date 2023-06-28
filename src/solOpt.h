#ifndef SOLOPT_H_
#define SOLOPT_H_


#define M 276447230

/**
 * @brief Construit la matrice des distances minimales (pour les sommets non reliés, donne la distance du chemin optimal les reliant)
 * @param distTab le tableau des distances initiales (sera modifié)
 * @param n la taille du tableau
*/
void Floyd_Warshall(int ** distTab, int n);

int cycle_min_approx(int ** distTab, sommet_t ** tabSommets, int n);



#endif /* SOLOPT_H_ */