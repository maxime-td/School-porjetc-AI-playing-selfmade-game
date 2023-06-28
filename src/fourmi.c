#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "fourmi.h"
#include "graph.h"

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

int tout_noeud(int * tab, int n){
    for (int i = 0; i < n; i++){
        if(!tab[i]) return 0;
    }
    return 1;
}

void affich_tab(int * tab, int n){
    for (int i = 0; i < n; i++){
        printf("%c - ", tab[i] + 'A');
    }
    printf("\n");
}

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

int path_size(int * path, int ** distMat, int n){
    int size = 0;

    for (int i = 0; i < n-1; i++){
        size += distMat[path[i]][path[i+1]];
    }
    
    return size;
}

void add_feromone(int* path, int ** probaMat, int n, int sizePath){
    for (int i = 0; i < n-1; i++){
        probaMat[path[i]][path[i+1]] -= (int) 100/(sizePath);
        if (probaMat[path[i]][path[i+1]] <= 0){
            probaMat[path[i]][path[i+1]] = 1;
        }
    }
}

void remove_feromone(int ** probaMat, int n, int facteur){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if (probaMat[i][j] != 0){
                probaMat[i][j] += facteur;
            } 
        }
    }
}