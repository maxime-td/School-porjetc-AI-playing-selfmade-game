#ifndef SOLOPT_H_
#define SOLOPT_H_

#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include <math.h>

/**
 * @brief Construit la matrice des distances minimales (pour les sommets non reliés, donne la distance du chemin optimal les reliant)
 * @param distTab le tableau des distances initiales (sera modifié)
 * @param n la taille du tableau
*/
void Floyd_Warshall(int ** distTab, int n);



/**
 * @brief Applique l'algo de cyclage ci-dessus sur chaque premier point et sors le plus efficace
 * @param tabWarshall Tableau des distances de Floy-Warshall
 * @param distTab le tableau des distances initiales (sera modifié)
 * @param n la taille du tableau
 * @param tabSommets le tableau des sommets
 * @return la longueur du chemin optimal
 */
void cycle_Floyd_Warshall(int **tabWarshall, int **tabDist, int n, int indDep, int *sol);


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
 * @return le meilleur tableau decrivant le meilleur chemin pour chaque point de départ 
*/
int multi_Start_Floyd_Warshall(int ** tabWarshall, int **distTab, int n);


void permute(int ** tab, int n);


int recuit_simule(int ** tabWarshall, int n);




#endif /* SOLOPT_H_ */