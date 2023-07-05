#ifndef MAP_H
#define MAP_H

#define PATH_SIZE 75

#include <SDL2/SDL.h>

#include "graph.h"
#include "affiche.h"

typedef struct asteroid_s{
    int x;
    int y;
    int frame;
    int angle;
} asteroid_t;

/**
 * @brief génère des astéroïdes et verifie s'ils sont sur un chemin entre 2 planètes
 * @param tab tableau des sommets (planètes)
 * @param n nombre de sommets (planètes)
 * @param segs le tableau des segments (liens entre planètes)
 * @param n_seg le nombre de segments (planètes voisines)
 * @param nAst le pointeur sur le nombre d astéroïdes a actualiser
 * @return le tableau des astéroïdes
 */
asteroid_t *ast_Partout(sommet_t **tab, int n, segmment_t *segs, int n_seg, int *nAst);

/**
 * @brief cacul la distance entre 2 points
 * @param p1 1er point
 * @param p2 2ème point
 * @return la distance entre les deux
*/
float distance(Point p1, Point p2);

/**
 * @brief calcul le projeté orthogonal d'un point sur une droite définie par 2 autres points
 * @param x la coord X du point à projeter
 * @param y la coord Y du point à projeter
 * @param x1 la coord X d'un des points de la droite
 * @param y1 la coord Y d'un des points de la droite
 * @param x2 la coord X d'un des points de la droite
 * @param y2 la coord Y d'un des points de la droite
 * @param x_proj le pointeur sur la coord X du point qui est le projeté
 * @param y_proj le pointeur sur la coord Y du point qui est le projeté
*/
void projetOrthogonal(int x, int y, int x1, int y1, int x2, int y2, double *x_proj, double *y_proj);

/**
 * @brief calcul le max entre 2 entiers
 * @param a entier 1
 * @param b entier 2
 * @return le plus grand entre a et b
*/
int max(int a, int b);

/**
 * @brief calcul le min entre 2 entiers
 * @param a entier 1
 * @param b entier 2
 * @return le plus petit entre a et b
*/
int min(int a, int b);

/**
 * @brief Permet de savoir si un point est sur un chemin ou non
 * @param pX la coord X de l'objet à tester
 * @param pY la coord Y de l'objet à tester
 * @param tabSom le tableau des sommets (planètes)
 * @param n le nombre de sommets (planètes)
 * @param tabSeg le tableau des segments (des chemins) 
 * @param n_seg le nombre de segments (chemins)
 * @param largeur la largeur d'un chemin
 * @return - 0 si l'objet n'est sur aucun chemin, - 1 s'il est sur un chemin
*/
int isInPath_Line(int pX, int pY, sommet_t ** tabSom, int n, segmment_t *tabSeg, int n_seg, int largeur);

#endif