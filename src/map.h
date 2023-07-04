#ifndef MAP_H
#define MAP_H

#define PATH_SIZE 75

#include <SDL2/SDL.h>

#include "graph.h"
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

/**
 * @brief génère le champ d'atéroïdes hors des chemins entre les planètes
 * @param tab le tableau des sommets
 * @param n le nombre de sommets
 * @param nAst adresse du nombre d'asteroïdes
 * @return retourne le tableau de taille nAst des astéroïdes
*/
asteroid_t* ast_Partout(sommet_t ** tab, int n, int *nAst);


/**
 * @brief permet de savoir si un objet est situé entre 2 planètes connectées en se servant du projeté orthogonal
 * @param pX coord X de l'objet
 * @param pY coord Y de l'objet
 * @param tabSom le tableau des sommets (planètes)
 * @param n le nombre de sommets (planètes)
 * @param largeur la largeur du chemin souhaité entre les 2 planètes
 * @return - 0 si l'objet n'est pas sur le chemin
 *         - 1 si l'objet est sur le chemin
*/
int isInPath_Line(int pX, int pY, sommet_t **tabSom, int n, int largeur) ;


/**
 * @brief calcul le projeté orthogonal d'un point sur une droite
 * @param x la coord X du point a projeter
 * @param y la coord Y du point a projeter
 * @param x1 la xoord X d'un point de la droite
 * @param y1 la coord Y d'un point de la droite
 * @param x2 la xoord X d'un deuxieme point de la droite
 * @param y2 la coord Y d'un deuxieme point de la droite
 * @param x_proj l'adresse de la coord X du projeté du point sur la droite
 * @param y_proj l'adresse de la coord X du projeté du point sur la droite
*/
void projetOrthogonal(int x, int y, int x1, int y1, int x2, int y2, double* x_proj, double* y_proj);


/**
 * @brief donne le minimum parmi 2 entier
 * @param a premier entier
 * @param b second entier
 * @return la valeur du plus petit entre a et b
*/
int min(int a, int b) ;

/**
 * @brief donne le maximum parmi 2 entier
 * @param a premier entier
 * @param b second entier
 * @return la valeur du plus grand entre a et b
*/
int max(int a, int b);


/**
 * @brief calcul la distance euclidienne entre 2 points
 * @param p1 1er points 
 * @param p2 2eme points
 * @return la distance en floatant
*/
float distance(Point p1, Point p2);

#endif