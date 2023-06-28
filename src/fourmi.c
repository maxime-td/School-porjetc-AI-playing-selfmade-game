#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "fourmi.h"
#include "graph.h"

int * colonni_fourmi(sommet_t ** tab, int ** matDist, int n, int dep){
    int probaMat[n][n];
    int nCour, sizeCour;
    int * courPath;
    int nBest, sizeBest = INT_MAX;
    int * bestPath;


    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            probaMat[i][j] = 0;
            if(matDist[i][j] > 0){
                probaMat[i][j] = 1;
            } 
        }
    }
    
    courChemin = fourmi(tab, n, &nCour, probaMat);

    sizeCour = path_size(courChemin, nCour, n);
    affich_tab(courChemin, nCour);
    printf("size : %d\n", sizeCour);
}


int * fourmi(sommet_t ** tab, int n, int * nPath, int ** probaMat, int dep){
    int fait[n];
    for (int i = 0; i < n; i++){
        fait[i] = 0;
    }
    
    int * chemin = (int*) malloc(sizeof(int)*MAX_PATH);
    int n_noeud = 1;
    int voisin;
    int pathChoose;
    fait[dep] = 1;

    chemin[0] = dep;
    while (0 != chemin[n_noeud-1] || !tout_noeud(fait, n)){
        pathChoose = 0;
        voisin = rand()%(n-1);
        while (!pathChoose){
            if (probaMat[chemin[n_noeud-1]][voisin] != 0 && rand()%(probaMat[chemin[n_noeud-1]][voisin]) == 1){
                pathChoose = 1;
            }else{
                voisin = (voisin+1)%(n);
            }
       }

        fait[voisin] = 1;
        chemin[n_noeud] = voisin;
        
        n_noeud++;

        if (n_noeud >= MAX_PATH){
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
        printf("%d - ", tab[i]);
    }
    printf("\n");
}

int path_size(int * path, int ** distMat, int n){
    int size = 0;

    for (int i = 0; i < n-1; i++){
        size += distMat[path[i]][path[i+1]];
    }
    
    return size;
}