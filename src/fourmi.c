#include <stdio.h>
#include <stdlib.h>
#include "fourmi.h"
#include "graph.h"

int * colonni_fourmi(sommet_t ** tab, int ** matDist, int n){
    int probaMat[n][n];
}


int * fourmi(sommet_t ** tab, int n, int * n_path, int ** probaMat){
    int fait[n];
    for (int i = 0; i < n; i++){
        fait[i] = 0;
    }
    
    int * chemin = (int*) malloc(sizeof(int)*MAX_PATH);
    int n_noeud = 1;
    int voisin;
    int pathChoose;
    fait[0] = 1;

    chemin[0] = 0;
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
    *n_path = n_noeud;
    return chemin;
}

int tout_noeud(int * tab, int n){
    for (int i = 0; i < n; i++){
        if(!tab[i]) return 0;
    }
    return 1;
}

void affichTab(int * tab, int n){
    for (int i = 0; i < n; i++){
        printf("%d - ", tab[i]);
    }
    printf("\n");
}