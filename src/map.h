#ifndef MAP_H
#define MAP_H

#define PATH_SIZE 90

#include "graph.h"
#include <SDL2/SDL.h>
#include "affiche.h"

typedef struct {
    int x;
    int y;
} Point;

typedef struct asteroid_s{
    int x;
    int y;
    int frame;
    int angle;
} asteroid_t;

int isPointInsideRectangle(Point p, Point rect[4]);


asteroid_t* ast_Partout(sommet_t ** tab, int n, int *nAst);


float fonction_affine(float a, int x, float b);

int isInPath(int pX, int pY, sommet_t **tabSom, int n, int largeur);


/**
 * @brief Permet de savoir si un objet est dans un chemin ou s'il est en dehors de tous
 * @param pX position x de l'objet
 * @param pY position y de l'objet
 * @param tabSomm tableau de sommet
 * @param n nombre de sommet
 * @param frame_size dimension de l'objet
 * @param largeur largeur du chemin
 * @return 1 si l'objet est sur un chemin, 0 sinon
*/
int coord_sur_chemin(int pX, int pY, sommet_t **tabSom, int n, int frame_size, int largeur);

int isInPath(int pX, int pY, sommet_t **tabSom, int n, int largeur);


void calculateLineCoefficients(int x1, int y1, int x2, int y2, float *a, float *b);

float distance(Point p1, Point p2);

#endif