#ifndef FOURMI_H_
#define FOURMI_H_

#include "graph.h"
#include <stdio.h>

#define MAX_PATH 12*12
#define ITERATION 100

/**
 * Recherche le chemin optimal pour le voyageur de commerce en utilisant des fourmis artificielles.
 * @param tab Le tableau des sommets du graphe.
 * @param matDist La matrice des distances entre les sommets.
 * @param n Le nombre de sommets dans le graphe.
 * @param dep Le sommet de départ.
 * @param nPath Un pointeur vers une variable pour stocker la taille du chemin optimal.
 * @return Un tableau d'entiers représentant le chemin optimal.
 */
int * colonni_fourmi(sommet_t ** tab, int ** matDist, int n, int dep, int * nPath);

/**
 * Fonction auxiliaire pour l'algorithme des fourmis artificielles.
 * @param tab Le tableau des sommets du graphe.
 * @param n Le nombre de sommets dans le graphe.
 * @param nPath Un pointeur vers une variable pour stocker la taille du chemin trouvé.
 * @param probaMat La matrice des probabilités de transition entre les sommets.
 * @param dep Le sommet de départ.
 * @return Un tableau d'entiers représentant le chemin trouvé.
 */
int * fourmi(sommet_t ** tab, int n, int * nPath, int ** probaMat, int dep);

/**
 * Vérifie si toutes les valeur d'un tableau sont à 1.
 * @param tab Le tableau des sommets visités.
 * @param n Le nombre de sommets dans le graphe.
 * @return 1 si tous les sommets ont été visités, 0 sinon.
 */
int tout_noeud(int * tab, int n);

/**
 * Affiche un tableau d'entiers et les remplaces par des lettres (0 -> A, 1 -> B ...)
 * @param tab Le tableau d'entiers.
 * @param n Le nombre d'éléments dans le tableau.
 */
void affich_tab(int * tab, int n);

/**
 * Affiche une matrice d'entiers.
 * @param tab La matrice d'entiers.
 * @param n Le nombre de lignes et de colonnes dans la matrice.
 */
void affich_tab_2D(int ** tab, int n);

/**
 * Calcule la taille d'un chemin en utilisant la matrice des distances.
 * @param path Le chemin représenté par un tableau d'entiers.
 * @param distMat La matrice des distances entre les sommets.
 * @param n Le nombre de sommets dans le graphe.
 * @return La taille du chemin.
 */
int path_size(int * path, int ** distMat, int n);

/**
 * Ajoute de la phéromone sur les arêtes du chemin.
 * @param path Le chemin représenté par un tableau d'entiers.
 * @param probaMat La matrice des probabilités de transition entre les sommets.
 * @param n Le nombre de sommets dans le graphe.
 * @param sizePath La taille du chemin.
 */
void add_feromone(int* path, int ** probaMat, int n, int sizePath);

/**
 * Enlève de la phéromone sur toutes les arêtes.
 * @param probaMat La matrice des probabilités de transition entre les sommets.
 * @param n Le nombre de sommets dans le graphe.
 * @param facteur Le facteur de diminution de la phéromone.
 */
void remove_feromone(int ** probaMat, int n, int facteur);

#endif /* FOURMI_H_ */