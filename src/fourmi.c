#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "fourmi.h"
#include "graph.h"
#include "OptiFloyd_Warshall.h"

/**
 * Lance la recherche par fourmi de meilleur chemin depuis tout les sommets du graph et garde le meilleur resultat.
 * @param matDist La matrice des distances entre les sommets.
 * @param n Le nombre de sommets dans le graphe.
 * @return Un entier representant la distance minimal trouvé.
 */
int multi_start_fourmi(int ** matDist, int n) {
    int best = INT_MAX;
    int cour = 0;
    int nPath;

    for (int i = 0; i < n; i++) {
        int * path = colonni_fourmi(matDist, n, i, &nPath);
        cour = path_size(path, matDist, nPath);
        free(path);
        if(cour < best)
            best = cour;
    }
    return best;
}

/**
 * Recherche le chemin optimal pour le voyageur de commerce en utilisant des fourmis artificielles.
 * @param matDist La matrice des distances entre les sommets.
 * @param n Le nombre de sommets dans le graphe.
 * @param dep Le sommet de départ.
 * @param nPath Un pointeur vers une variable pour stocker la taille du chemin optimal.
 * @return Un tableau d'entiers représentant le chemin optimal.
 */
int * colonni_fourmi(int ** matDist, int n, int dep, int * nPath) {
    int nCour, sizeCour;
    int * courPath;
    int nBest = 0, sizeBest = INT_MAX;
    int * bestPath = NULL;
    int ** probaMatCopy;
    int ** probaMat = getProbaMat(matDist, n);

    for (int i = 0; i < ITERATION*n; i++) {
        
        probaMatCopy = copie_tab(probaMat, n);

        for (int j = 0; j < ITERATION; j++) {
            courPath = fourmi(n, &nCour, probaMat, dep);
            if (courPath != NULL) {
                sizeCour = path_size(courPath, matDist, nCour);
                remove_feromone(probaMatCopy, n, 100);
                add_feromone(courPath, probaMatCopy, nCour, sizeCour);

                if (sizeBest > sizeCour) {
                    if(bestPath != NULL)
                        free(bestPath);
                    bestPath = courPath;
                    sizeBest = sizeCour;
                    nBest    = nCour   ;
                }
                else
                    free(courPath);
            }
        }
        
        free2DTab((void **) probaMat, n);
        probaMat = probaMatCopy;
    }
    
    *nPath = nBest;
    
    free2DTab((void **) probaMat, n);

    return bestPath;
}


int ** getProbaMat(int ** matDist, int n) {
    int ** probaMat = (int **) malloc(sizeof(int *)*n);
    
    for (int i = 0; i < n; i++) {
        probaMat[i] = calloc(n, sizeof(int));
        for (int j = 0; j < n; j++) {
            if(matDist[i][j] > 0)
                probaMat[i][j] = 10000/matDist[i][j];
        }
    }

    return probaMat;
}

/**
 * Fonction auxiliaire pour l'algorithme des fourmis artificielles.
 * @param n Le nombre de sommets dans le graphe.
 * @param nPath Un pointeur vers une variable pour stocker la taille du chemin trouvé.
 * @param probaMat La matrice des probabilités de transition entre les sommets.
 * @param dep Le sommet de départ.
 * @return Un tableau d'entiers représentant le chemin trouvé.
 */
int * fourmi(int n, int * nPath, int ** probaMat, int dep) {
    int fait[n];
    for (int i = 0; i < n; i++)
        fait[i] = 0;
    
    int * chemin = (int*) malloc(sizeof(int)*MAX_PATH);
    int n_noeud = 1;
    int k;
    int voisin;
    int somme;
    fait[dep] = 1;

    chemin[0] = dep;
    while (dep != chemin[n_noeud-1] || !tout_noeud(fait, n)) {
        somme = 0;
        for (int i = 0; i < n; i++)
            somme += probaMat[chemin[n_noeud-1]][i];

        voisin = rand()%somme +1;

        somme = 0;
        for (k = 0; voisin > somme && k < n; k++)
            somme += probaMat[chemin[n_noeud-1]][k];
        k--;

        fait[k] = 1;
        chemin[n_noeud] = k;
        n_noeud++;

        if (n_noeud >= MAX_PATH) {
            free(chemin);
            return NULL;
        }
    }
    *nPath = n_noeud;

    return chemin;
}

/**
 * Vérifie si toutes les valeur d'un tableau sont à 1.
 * @param tab Le tableau des sommets visités.
 * @param n Le nombre de sommets dans le graphe.
 * @return 1 si tous les sommets ont été visités, 0 sinon.
 */
int tout_noeud(int * tab, int n) {
    for (int i = 0; i < n; i++)
        if(!tab[i]) return 0;
    return 1;
}

/**
 * Affiche un tableau d'entiers et les remplaces par des lettres (0 -> A, 1 -> B ...)
 * @param tab Le tableau d'entiers.
 * @param n Le nombre d'éléments dans le tableau.
 */
void affich_tab(int * tab, int n) {
    for (int i = 0; i < n; i++)
        printf("%c (%d)- ", tab[i] + 'A', tab[i]);
    printf("\n");
}

/**
 * Affiche une matrice d'entiers.
 * @param tab La matrice d'entiers.
 * @param n Le nombre de lignes et de colonnes dans la matrice.
 */
void affich_tab_2D(int ** tab, int n) {
    for (int i = 0; i < n; i++) {
        printf("| ");
        for (int j = 0; j < n; j++)
            printf("%d |", tab[i][j]);
        printf("\n");
    }
}

/**
 * Calcule la taille d'un chemin en utilisant la matrice des distances.
 * @param path Le chemin représenté par un tableau d'entiers.
 * @param distMat La matrice des distances entre les sommets.
 * @param n Le nombre de sommets dans le graphe.
 * @return La taille du chemin.
 */
int path_size(int * path, int ** distMat, int n) {
    int size = 0;

    for (int i = 0; i < n-1; i++)
        size += distMat[path[i]][path[i+1]];
    
    return size;
}

/**
 * Ajoute de la phéromone sur les arêtes du chemin.
 * @param path Le chemin représenté par un tableau d'entiers.
 * @param probaMat La matrice des probabilités de transition entre les sommets.
 * @param n Le nombre de sommets dans le graphe.
 * @param sizePath La taille du chemin.
 */
void add_feromone(int* path, int ** probaMat, int n, int sizePath) {
    for (int i = 0; i < n-1; i++) {
        probaMat[path[i]][path[i+1]] += (int) 100000.0/(sizePath);
        if (probaMat[path[i]][path[i+1]] <= 0)
            probaMat[path[i]][path[i+1]] = 1;
    }
}

/**
 * Enlève de la phéromone sur toutes les arêtes.
 * @param probaMat La matrice des probabilités de transition entre les sommets.
 * @param n Le nombre de sommets dans le graphe.
 * @param facteur Le facteur de diminution de la phéromone.
 */
void remove_feromone(int ** probaMat, int n, int facteur) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (probaMat[i][j] != 0)
                probaMat[i][j] -= facteur;
        }
    }
}