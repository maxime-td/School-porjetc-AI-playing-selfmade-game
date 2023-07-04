#ifndef SOLOPT_H_
#define SOLOPT_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "graph.h"

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

/**
 * @brief permute deux entier dans un tableau 
 * @param tab le pointeur sur le tableau d'entier dont deux indices vont etre échangé
 * @param n nombre de sommets du graph (et dimension du tableau des distances)
*/
void permute(int ** tab, int n);

/**
 * @brief calcul la taille d'un chemin passer en parametre avec la méthode de Floyd-Warshall
 * @param tabWarshall Tableau des distance de Floyd-Warshall
 * @param n nombre de sommets du graph (et dimension du tableau des distances)
 * @param tab le tableau d'entier d'ordre d'apparition des sommets (tab[0] = 1er sommet du cycle)
 * @return la taille du chemin
*/
int calcul_chemin_Floy_Warshall(int **tabWarshall, int n, int *tab);

/**
 * @brief applique la méthode du recuit simulé 
 * @param tabWarshall Tableau des distance de Floyd-Warshall
 * @param n nombre de sommets du graph (et dimension du tableau des distances)
 * @return taille chemin optimal trouvé
*/
int recuit_simule(int **tabWarshall, int n, sommet_t ** tabSom);

#endif /* SOLOPT_H_ */