#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "fourmi.h"
#include "graph.h"
#include "affiche.h"
#include "map.h"

/**
 * @brief génère des astéroïdes et verifie s'ils sont sur un chemin entre 2 planètes
 * @param tab tableau des sommets (planètes)
 * @param n nombre de sommets (planètes)
 * @param segs le tableau des segments (liens entre planètes)
 * @param n_seg le nombre de segments (planètes voisines)
 * @param nAst le pointeur sur le nombre d astéroïdes a actualiser
 * @return le tableau des astéroïdes
 */
asteroid_t *ast_Partout(sommet_t **tab, int n, segmment_t *segs, int n_seg, int *nAst)
{
    int frame_size = 48;
    int alea = 0;
    int angle = 0;
    int ecart = (rand() % 50) - 25;
    asteroid_t *tabAst = (asteroid_t *)malloc(sizeof(asteroid_t) * H * W);
    int a = 0;

    SDL_Rect srcrect = {0, 0, frame_size, frame_size}; // position and size of the part of the image to draw
    SDL_Rect dstrect = {100, 100, 42, 42};             // position and size of the destination on the screen

    for (int i = 0; i < W; i += 21 + ecart)
    {
        ecart = (rand() % 50) - 25;
        dstrect.x = i;
        for (int j = 0; j < H; j += 21 + ecart)
        {
            ecart = (rand() % 50) - 25;
            dstrect.y = j;
            alea = (rand() % 16);
            srcrect.x = alea;
            angle = rand() % 360;

            if (isInPath_Line(dstrect.x, dstrect.y, tab, n, segs, n_seg, PATH_SIZE) == 0)
            {
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
 * @brief cacul la distance entre 2 points
 * @param p1 1er point
 * @param p2 2ème point
 * @return la distance entre les deux
*/
float distance(Point p1, Point p2)
{
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    return sqrt(dx * dx + dy * dy);
}

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
void projetOrthogonal(int x, int y, int x1, int y1, int x2, int y2, double *x_proj, double *y_proj)
{

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
 * @brief calcul le max entre 2 entiers
 * @param a entier 1
 * @param b entier 2
 * @return le plus grand entre a et b
*/
int max(int a, int b)
{
    if (a < b)
    {
        return b;
    }
    return a;
}

/**
 * @brief calcul le min entre 2 entiers
 * @param a entier 1
 * @param b entier 2
 * @return le plus petit entre a et b
*/
int min(int a, int b)
{
    if (a > b)
    {
        return b;
    }
    return a;
}

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
int isInPath_Line(int pX, int pY, sommet_t ** tabSom, int n, segmment_t *tabSeg, int n_seg, int largeur){
    int res = 0;
    int dist = 0;

    Point tmp;
    Point P = {pX, pY};
    Point projete;

    double x_proj, y_proj;

    for (int i = 0; i < n_seg && res == 0; i++)
    {
        projetOrthogonal(pX, pY, tabSeg[i].p1.x, tabSeg[i].p1.y, tabSeg[i].p2.x, tabSeg[i].p2.y, &x_proj, &y_proj);
        projete.x = x_proj;
        projete.y = y_proj;

        dist = distance(projete, P);
        if (dist < largeur && x_proj > min(tabSeg[i].p1.x, tabSeg[i].p2.x) && x_proj < max(tabSeg[i].p1.x, tabSeg[i].p2.x))
        {
            res++;
        }
    }

    for (int i = 0; i < n && res == 0; i++)
    {
        tmp.x = tabSom[i]->x;
        tmp.y = tabSom[i]->y;
        if (distance(tmp, P) < largeur)
            res += 1;
    }

    return res;
}
