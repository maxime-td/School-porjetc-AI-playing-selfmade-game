#ifndef FOURMI_H_
#define FOURMI_H_

#include "graph.h"
#include <stdio.h>

#define MAX_PATH 12*12
#define ITERATION 100

int * colonni_fourmi(sommet_t ** tab, int ** matDist, int n, int dep, int * nPath);

int * fourmi(sommet_t ** tab, int n, int * n_path, int ** probaMat, int dep);

int tout_noeud(int * tab, int n);

void affich_tab(int * tab, int n);

void affich_tab_2D(int ** tab, int n);

int path_size(int * path, int ** distMat, int n);

void remove_feromone(int ** probaMat, int n, int facteur);

void add_feromone(int* path, int ** probaMat, int n, int sizePath);

#endif /* FOURMI_H_ */