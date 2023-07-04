#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "fourmi.h"
#include "graph.h"
#include "affiche.h"
#include "map.h"

/**
 * @brief génère le champ d'atéroïdes hors des chemins entre les planètes
 * @param tab le tableau des sommets
 * @param n le nombre de sommets
 * @param nAst adresse du nombre d'asteroïdes
 * @return retourne le tableau de taille nAst des astéroïdes
*/
asteroid_t *ast_Partout(sommet_t **tab, int n, int *nAst) {
    int frame_size = 48;
    int alea = 0;
    int angle = 0;
    int ecart = (rand() % 50) - 25;
    asteroid_t *tabAst = (asteroid_t *)malloc(sizeof(asteroid_t) * H * W);
    int a = 0;

    SDL_Rect srcrect = {0, 0, frame_size, frame_size}; // position and size of the part of the image to draw
    SDL_Rect dstrect = {100, 100, 42, 42};             // position and size of the destination on the screen

    for (int i = 0; i < W; i += 21 + ecart) {
        ecart = (rand() % 50) - 25;
        dstrect.x = i;
        for (int j = 0; j < H; j += 21 + ecart) {
            ecart = (rand() % 50) - 25;
            dstrect.y = j;
            alea = (rand() % 16);
            srcrect.x = alea;
            angle = rand() % 360;

            if (isInPath_Line(dstrect.x, dstrect.y, tab, n, PATH_SIZE) == 0) {
                tabAst[a].x = dstrect.x;
                tabAst[a].y = dstrect.y;
                tabAst[a].frame = srcrect.x;
                tabAst[a].angle = angle;
                a++;
            }
        }
    }
    *nAst = a;

    return tabAst;
}

/**
 * @brief calcul la distance euclidienne entre 2 points
 * @param p1 1er points 
 * @param p2 2eme points
 * @return la distance en floatant
*/
float distance(Point p1, Point p2) {
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    return sqrt(dx * dx + dy * dy);
}

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
void projetOrthogonal(int x, int y, int x1, int y1, int x2, int y2, double* x_proj, double* y_proj) {
    // Calcul des coordonnées du vecteur de la droite
    double dx = x2 - x1;
    double dy = y2 - y1;

    // Calcul des coordonnées du vecteur du point à projeter par rapport à un des points de la droite
    double dx_point = x - x1;
    double dy_point = y - y1;

    // Calcul du produit scalaire entre le vecteur de la droite et le vecteur du point à projeter
    double dot_product = dx * dx_point + dy * dy_point;

    // Calcul des coordonnées du projeté orthogonal en utilisant le produit scalaire
    *x_proj = x1 + (dot_product * dx) / (dx * dx + dy * dy);
    *y_proj = y1 + (dot_product * dy) / (dx * dx + dy * dy);
}

/**
 * @brief donne le maximum parmi 2 entier
 * @param a premier entier
 * @param b second entier
 * @return la valeur du plus grand entre a et b
*/
int max(int a, int b) {
    if(a<b){return b;}
    return a;
}

/**
 * @brief donne le minimum parmi 2 entier
 * @param a premier entier
 * @param b second entier
 * @return la valeur du plus petit entre a et b
*/
int min(int a, int b) {
    if(a>b){return b;}
    return a;
}

int isInPath_Line(int pX, int pY, sommet_t **tabSom, int n, int largeur) {
    int res = 0;
    int dist = 0;

    Point tmp;
    Point P = {pX, pY};
    Point p1;
    Point p2;
    Point projete;

    double x_proj, y_proj;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (tabSom[i]->voisins[j] == 1) {
                p1.x = tabSom[i]->x;
                p1.y = tabSom[i]->y;

                p2.x = tabSom[j]->x;
                p2.y = tabSom[j]->y;

                projetOrthogonal(pX, pY, p1.x, p1.y, p2.x, p2.y, &x_proj, &y_proj);
                projete.x = x_proj;
                projete.y = y_proj;

                dist = distance(projete, P);
                if(dist<largeur && x_proj>min(p1.x, p2.x) && x_proj<max(p1.x, p2.x)){res++;}
            }
        }
    }

    for (int i = 0; i < n; i++) {
        tmp.x = tabSom[i]->x;
        tmp.y = tabSom[i]->y;
        if (distance(tmp, P) < largeur)
            res += 1;
    }

    return res;
}

int isPointInsideRectangle(Point p, Point rect[4]) {
    int c = 0;
    if (rect[0].x < p.x && rect[1].x > p.x && rect[0].y < p.y && rect[3].y > p.y)
        c = 1;

    return c;
}

void calculateLineCoefficients(int x1, int y1, int x2, int y2, float *a, int *b) {
    if (x1 > x2) {
        // Inverser les points
        int tempX = x1;
        int tempY = y1;
        x1 = x2;
        y1 = y2;
        x2 = tempX;
        y2 = tempY;
    }

    *a = (float)(y1 - y2) / (x2 - x1);
    *b = y1 - (*a) * x1;
}