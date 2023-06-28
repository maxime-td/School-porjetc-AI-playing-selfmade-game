#ifndef FOURMI_H_
#define FOURMI_H_
#define MAX_PATH 12*6
#include "graph.h"
#include <stdio.h>

int * fourmi(sommet_t ** tab, int n, int * n_path, int ** probaMat);

int tout_noeud(int * tab, int n);

void affichTab(int * tab, int n);


#endif /* FOURMI_H_ */