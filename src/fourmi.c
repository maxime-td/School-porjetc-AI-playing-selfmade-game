#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "fourmi.h"
#include "graph.h"

/**
 * Recherche le chemin optimal pour le voyageur de commerce en utilisant des fourmis artificielles.
 * @param tab Le tableau des sommets du graphe.
 * @param matDist La matrice des distances entre les sommets.
 * @param n Le nombre de sommets dans le graphe.
 * @param dep Le sommet de départ.
 * @param nPath Un pointeur vers une variable pour stocker la taille du chemin optimal.
 * @return Un tableau d'entiers représentant le chemin optimal.
 */
int * colonni_fourmi(sommet_t ** tab, int ** matDist, int n, int dep, int * nPath){
    int nCour, sizeCour;
    int * courPath;
    int nBest, sizeBest = INT_MAX;
    int * bestPath = NULL;
    int ** probaMat = (int **) malloc(sizeof(int *)*n);


    for (int i = 0; i < n; i++){
        probaMat[i] = calloc(n, sizeof(int));
        for (int j = 0; j < n; j++){
            if(matDist[i][j] > 0){
                probaMat[i][j] = 1;
            } 
        }
    }

    for (int i = 0; i < ITERATION*n; i++){
        courPath = fourmi(tab, n, &nCour, probaMat, dep);

        if (courPath != NULL){
            sizeCour = path_size(courPath, matDist, nCour);
            //printf("%d\n", sizeCour);
            remove_feromone(probaMat, n, 100);
            
            add_feromone(courPath, probaMat, n, sizeCour);

            //affich_tab_2D(probaMat, n);

            if (sizeBest > sizeCour){
                if(bestPath != NULL)
                    free(bestPath);
                bestPath = courPath;
                sizeBest = sizeCour;
                nBest    = nCour   ;
            }else{
                free(courPath);
            }
        }
    }
    
    *nPath = nBest;
    
    free2DTab((void **) probaMat, n);
    return bestPath;
}

/**
 * Fonction auxiliaire pour l'algorithme des fourmis artificielles.
 * @param tab Le tableau des sommets du graphe.
 * @param n Le nombre de sommets dans le graphe.
 * @param nPath Un pointeur vers une variable pour stocker la taille du chemin trouvé.
 * @param probaMat La matrice des probabilités de transition entre les sommets.
 * @param dep Le sommet de départ.
 * @return Un tableau d'entiers représentant le chemin trouvé.
 */
int * fourmi(sommet_t ** tab, int n, int * nPath, int ** probaMat, int dep){
    int fait[n];
    for (int i = 0; i < n; i++){
        fait[i] = 0;
    }
    
    int * chemin = (int*) malloc(sizeof(int)*MAX_PATH);
    int n_noeud = 1;
    int k;
    int voisin;
    int somme;
    int pathChoose;
    fait[dep] = 1;

    chemin[0] = dep;
    while (dep != chemin[n_noeud-1] || !tout_noeud(fait, n)){
        pathChoose = 0;
        
        somme = 0;
        for (int i = 0; i < n; i++){
            somme += probaMat[chemin[n_noeud-1]][i];
        }

        voisin = rand()%somme +1;

        somme = 0;
        for (k = 0; voisin > somme && k < n; k++){
            somme += probaMat[chemin[n_noeud-1]][k];
        }
        k--;


        fait[k] = 1;
        chemin[n_noeud] = k;
        //printf("++++\n");
        n_noeud++;

        if (n_noeud >= MAX_PATH){
            //printf("Bruh\n");
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
int tout_noeud(int * tab, int n){
    for (int i = 0; i < n; i++){
        if(!tab[i]) return 0;
    }
    return 1;
}

/**
 * Affiche un tableau d'entiers et les remplaces par des lettres (0 -> A, 1 -> B ...)
 * @param tab Le tableau d'entiers.
 * @param n Le nombre d'éléments dans le tableau.
 */
void affich_tab(int * tab, int n){
    for (int i = 0; i < n; i++){

        printf("%c (%d)- ", tab[i] + 'A', tab[i]);
    }
    printf("\n");
}

/**
 * Affiche une matrice d'entiers.
 * @param tab La matrice d'entiers.
 * @param n Le nombre de lignes et de colonnes dans la matrice.
 */
void affich_tab_2D(int ** tab, int n){
    for (int i = 0; i < n; i++){
        printf("| ");
        for (int j = 0; j < n; j++)
        {
            printf("%d |", tab[i][j]);
        }
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
int path_size(int * path, int ** distMat, int n){
    int size = 0;

    for (int i = 0; i < n-1; i++){
        size += distMat[path[i]][path[i+1]];
    }
    
    return size;
}

/**
 * Ajoute de la phéromone sur les arêtes du chemin.
 * @param path Le chemin représenté par un tableau d'entiers.
 * @param probaMat La matrice des probabilités de transition entre les sommets.
 * @param n Le nombre de sommets dans le graphe.
 * @param sizePath La taille du chemin.
 */
void add_feromone(int* path, int ** probaMat, int n, int sizePath){
    for (int i = 0; i < n-1; i++){
        probaMat[path[i]][path[i+1]] -= (int) 100/(sizePath);
        if (probaMat[path[i]][path[i+1]] <= 0){
            probaMat[path[i]][path[i+1]] = 1;
        }
    }
}

/**
 * Enlève de la phéromone sur toutes les arêtes.
 * @param probaMat La matrice des probabilités de transition entre les sommets.
 * @param n Le nombre de sommets dans le graphe.
 * @param facteur Le facteur de diminution de la phéromone.
 */
void remove_feromone(int ** probaMat, int n, int facteur){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if (probaMat[i][j] != 0){
                probaMat[i][j] += facteur;
            } 
        }
    }
}